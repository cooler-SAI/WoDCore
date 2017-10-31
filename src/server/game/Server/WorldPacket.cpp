////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include <zlib.h>
#include "WorldPacket.h"
#include "World.h"

std::mutex gPacketProfilerMutex;
std::map<uint32, uint32> gPacketProfilerData;

//! Compresses packet in place
void WorldPacket::Compress(z_stream* compressionStream)
{
    uint16 uncompressedOpcode = GetOpcode();
    if (uncompressedOpcode & COMPRESSED_OPCODE_MASK)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Packet with opcode 0x%04X is already compressed!", uncompressedOpcode);
        return;
    }

    uint16 opcode = Opcodes(uncompressedOpcode | COMPRESSED_OPCODE_MASK);
    uint32 size = wpos();
    uint32 destsize = compressBound(size);

    std::vector<uint8> storage(destsize);

    _compressionStream = compressionStream;
    Compress(static_cast<void*>(&storage[0]), &destsize, static_cast<const void*>(contents()), size);
    if (destsize == 0)
        return;

    clear();
    reserve(destsize + sizeof(uint32));
    *this << uint32(size);
    append(&storage[0], destsize);
    SetOpcode(opcode);

    sLog->outInfo(LOG_FILTER_NETWORKIO, "Successfully compressed opcode %u (len %u) to %u (len %u)", uncompressedOpcode, size, opcode, destsize);
}

//! Compresses another packet and stores it in self (source left intact)
void WorldPacket::Compress(z_stream* compressionStream, WorldPacket const* source)
{
    ASSERT(source != this);

    uint16 uncompressedOpcode = source->GetOpcode();
    if (uncompressedOpcode & COMPRESSED_OPCODE_MASK)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Packet with opcode 0x%04X is already compressed!", uncompressedOpcode);
        return;
    }

    uint16 opcode = Opcodes(uncompressedOpcode | COMPRESSED_OPCODE_MASK);
    uint32 size = source->size();
    uint32 destsize = compressBound(size);

    size_t sizePos = 0;
    resize(destsize + sizeof(uint32));

    _compressionStream = compressionStream;
    Compress(static_cast<void*>(&_storage[0] + sizeof(uint32)), &destsize, static_cast<const void*>(source->contents()), size);
    if (destsize == 0)
        return;

    put<uint32>(sizePos, size);
    resize(destsize + sizeof(uint32));

    SetOpcode(opcode);

    sLog->outInfo(LOG_FILTER_NETWORKIO, "Successfully compressed opcode %u (len %u) to %u (len %u)", uncompressedOpcode, size, opcode, destsize);
}

void WorldPacket::Compress(void* dst, uint32 *dst_size, const void* src, int src_size)
{
    _compressionStream->next_out = (Bytef*)dst;
    _compressionStream->avail_out = *dst_size;
    _compressionStream->next_in = (Bytef*)src;
    _compressionStream->avail_in = (uInt)src_size;

    int32 z_res = deflate(_compressionStream, Z_SYNC_FLUSH);
    if (z_res != Z_OK)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Can't compress packet (zlib: deflate) Error code: %i (%s, msg: %s)", z_res, zError(z_res), _compressionStream->msg);
        *dst_size = 0;
        return;
    }

    if (_compressionStream->avail_in != 0)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Can't compress packet (zlib: deflate not greedy)");
        *dst_size = 0;
        return;
    }

    *dst_size -= _compressionStream->avail_out;
}

void WorldPacket::OnSend()
{
    if (m_opcode != UNKNOWN_OPCODE && _storage.size() > m_BaseSize)
    {
        gPacketProfilerMutex.lock();
        gPacketProfilerData[m_opcode] = std::max((uint32)_storage.size(), (uint32)gPacketProfilerData[m_opcode]);
        gPacketProfilerMutex.unlock();
    }
}
