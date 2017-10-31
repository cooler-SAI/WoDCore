////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
Name: bf_commandscript
%Complete: 100
Comment: All bf related commands
Category: commandscripts
EndScriptData */

#include "ScriptMgr.h"
#include "Chat.h"
#include "BattlefieldMgr.h"

class bf_commandscript: public CommandScript
{
public:
    bf_commandscript() : CommandScript("bf_commandscript") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand battlefieldcommandTable[] =
        {
            { "start",          SEC_ADMINISTRATOR,  false, &HandleBattlefieldStart,            "", NULL },
            { "stop",           SEC_ADMINISTRATOR,  false, &HandleBattlefieldEnd,              "", NULL },
            { "switch",         SEC_ADMINISTRATOR,  false, &HandleBattlefieldSwitch,           "", NULL },
            { "timer",          SEC_ADMINISTRATOR,  false, &HandleBattlefieldTimer,            "", NULL },
            { "enable",         SEC_ADMINISTRATOR,  false, &HandleBattlefieldEnable,           "", NULL },
            { NULL,             0,                  false, NULL,                               "", NULL }
        };
        static ChatCommand commandTable[] =
        {
            { "bf",             SEC_ADMINISTRATOR,  false, NULL,            "", battlefieldcommandTable },
            { NULL,             0,                  false, NULL,                               "", NULL }
        };
        return commandTable;
    }

    static bool HandleBattlefieldStart(ChatHandler* handler, const char* args)
    {
        uint32 battleid = 0;
        char* battleid_str = strtok((char*)args, " ");
        if (!battleid_str)
            return false;

        battleid = atoi(battleid_str);

        Battlefield* bf = sBattlefieldMgr->GetBattlefieldByBattleId(battleid);

        if (!bf)
            return false;

        bf->StartBattle();

        if (battleid == 1)
            handler->SendGlobalGMSysMessage("Wintergrasp (Command start used)");

        return true;
    }

    static bool HandleBattlefieldEnd(ChatHandler* handler, const char* args)
    {
        uint32 battleid = 0;
        char* battleid_str = strtok((char*)args, " ");
        if (!battleid_str)
            return false;

        battleid = atoi(battleid_str);

        Battlefield* bf = sBattlefieldMgr->GetBattlefieldByBattleId(battleid);

        if (!bf)
            return false;

        bf->EndBattle(true);

        if (battleid == 1)
            handler->SendGlobalGMSysMessage("Wintergrasp (Command stop used)");

        return true;
    }

    static bool HandleBattlefieldEnable(ChatHandler* handler, const char* args)
    {
        uint32 battleid = 0;
        char* battleid_str = strtok((char*)args, " ");
        if (!battleid_str)
            return false;

        battleid = atoi(battleid_str);

        Battlefield* bf = sBattlefieldMgr->GetBattlefieldByBattleId(battleid);

        if (!bf)
            return false;

        if (bf->IsEnabled())
        {
            bf->ToggleBattlefield(false);
            if (battleid == 1)
                handler->SendGlobalGMSysMessage("Wintergrasp is disabled");
        }
        else
        {
            bf->ToggleBattlefield(true);
            if (battleid == 1)
                handler->SendGlobalGMSysMessage("Wintergrasp is enabled");
        }

        return true;
    }

    static bool HandleBattlefieldSwitch(ChatHandler* handler, const char* args)
    {
        uint32 battleid = 0;
        char* battleid_str = strtok((char*)args, " ");
        if (!battleid_str)
            return false;

        battleid = atoi(battleid_str);

        Battlefield* bf = sBattlefieldMgr->GetBattlefieldByBattleId(battleid);

        if (!bf)
            return false;

        bf->EndBattle(false);
        if (battleid == 1)
            handler->SendGlobalGMSysMessage("Wintergrasp (Command switch used)");

        return true;
    }

    static bool HandleBattlefieldTimer(ChatHandler* handler, const char* args)
    {
        uint32 battleid = 0;
        uint32 time = 0;
        char* battleid_str = strtok((char*)args, " ");
        if (!battleid_str)
            return false;
        char* time_str = strtok(NULL, " ");
        if (!time_str)
            return false;

        battleid = atoi(battleid_str);

        time = atoi(time_str);

        Battlefield* bf = sBattlefieldMgr->GetBattlefieldByBattleId(battleid);

        if (!bf)
            return false;

        bf->SetTimer(time * IN_MILLISECONDS);
        bf->SendInitWorldStatesToAll();
        if (battleid == 1)
            handler->SendGlobalGMSysMessage("Wintergrasp (Command timer used)");

        return true;
    }
};

#ifndef __clang_analyzer__
void AddSC_bf_commandscript()
{
    new bf_commandscript();
}
#endif
