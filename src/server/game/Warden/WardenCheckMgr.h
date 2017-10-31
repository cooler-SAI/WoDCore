////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _WARDENCHECKMGR_H
#define _WARDENCHECKMGR_H

#include "Common.h"
#include "Cryptography/BigNumber.h"

enum WardenActions
{
    WARDEN_ACTION_LOG,
    WARDEN_ACTION_KICK,
    WARDEN_ACTION_BAN
};

struct WardenCheck
{
    uint8 Type;
    BigNumber Data;
    uint32 Address;                                         // PROC_CHECK, MEM_CHECK, PAGE_CHECK
    uint8 Length;                                           // PROC_CHECK, MEM_CHECK, PAGE_CHECK
    std::string Str;                                        // LUA, MPQ, DRIVER
    std::string Comment;
    uint16 CheckId;
    enum WardenActions Action;
};

struct WardenCheckResult
{
    BigNumber Result;                                       // MEM_CHECK
};

class WardenCheckMgr
{
    friend class ACE_Singleton<WardenCheckMgr, ACE_Null_Mutex>;
    WardenCheckMgr();
    ~WardenCheckMgr();

    public:
        // We have a linear key without any gaps, so we use vector for fast access
        typedef std::vector<WardenCheck*> CheckContainer;
        typedef std::map<uint32, WardenCheckResult*> CheckResultContainer;

        WardenCheck* GetWardenDataById(uint16 Id);
        WardenCheckResult* GetWardenResultById(uint16 Id);

        std::vector<uint16> MemChecksIdPool;
        std::vector<uint16> OtherChecksIdPool;

        void LoadWardenChecks();
        void LoadWardenOverrides();

        ACE_RW_Mutex _checkStoreLock;

    private:
        CheckContainer CheckStore;
        CheckResultContainer CheckResultStore;
};

#define sWardenCheckMgr ACE_Singleton<WardenCheckMgr, ACE_Null_Mutex>::instance()

#endif
