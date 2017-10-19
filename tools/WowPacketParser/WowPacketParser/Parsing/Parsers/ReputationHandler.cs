using System;
using WowPacketParser.Enums;
using WowPacketParser.Misc;

namespace WowPacketParser.Parsing.Parsers
{
    public static class ReputationHandler
    {
        [Parser(Opcode.CMSG_RESET_FACTION_CHEAT)]
        public static void HandleResetFactionCheat(Packet packet)
        {
            packet.ReadUInt32("Faction Id");
        }

        [Parser(Opcode.SMSG_INITIALIZE_FACTIONS)]
        public static void HandleInitializeFactions(Packet packet)
        {
            var count = packet.ReadInt32("Count");
            for (var i = 0; i < count; i++)
            {
                packet.ReadEnum<FactionFlag>("Faction Flags", TypeCode.Byte, i);
                packet.ReadEnum<ReputationRank>("Faction Standing", TypeCode.UInt32, i);
            }
        }

        [Parser(Opcode.SMSG_SET_FACTION_VISIBLE)]
        [Parser(Opcode.CMSG_SET_WATCHED_FACTION)]
        [Parser(26423)]
        public static void HandleSetFactionMisc(Packet packet)
        {
            packet.ReadUInt32("Faction Id");
        }

        [Parser(Opcode.SMSG_SET_FORCED_REACTIONS)]
        public static void HandleForcedReactions(Packet packet)
        {
            var counter = packet.ReadBits("Factions", 6);

            for (var i = 0; i < counter; i++)
            {
                packet.ReadUInt32("Faction Id");
                packet.ReadUInt32("Reputation Rank");
            }
        }

        [Parser(Opcode.SMSG_SET_FACTION_STANDING)]
        public static void HandleSetFactionStanding(Packet packet)
        {
            packet.ReadUInt32("Unk UInt32");
            packet.ReadUInt32("Unk UInt32");

            packet.ReadBit("Increased");

            var count = packet.ReadBits("Counter", 21);

            for (int i = 0; i < count; ++i)
            {
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
            }
        }

        [Parser(Opcode.CMSG_SET_FACTION_INACTIVE)]
        public static void HandleSetFactionInactive(Packet packet)
        {
            packet.ReadUInt32("Faction Id");
            packet.ReadBoolean("Inactive");
        }

        [Parser(Opcode.CMSG_SET_FACTION_ATWAR)]
        public static void HandleSetFactionAtWar(Packet packet)
        {
            packet.ReadUInt32("Faction Id");
            packet.ReadBoolean("At War");
        }
    }
}
