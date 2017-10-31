////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
Name: cheat_commandscript
%Complete: 100
Comment: All cheat related commands
Category: commandscripts
EndScriptData */

#include "ScriptMgr.h"
#include "ObjectMgr.h"
#include "Chat.h"

class cheat_commandscript: public CommandScript
{
    public:
        cheat_commandscript() : CommandScript("cheat_commandscript") { }

        ChatCommand* GetCommands() const
        {

            static ChatCommand cheatCommandTable[] =
            {
                { "god",        SEC_GAMEMASTER, false, &HandleGodModeCheatCommand,      "", NULL },
                { "casttime",   SEC_GAMEMASTER, false, &HandleCasttimeCheatCommand,     "", NULL },
                { "cooldown",   SEC_GAMEMASTER, false, &HandleCoolDownCheatCommand,     "", NULL },
                { "power",      SEC_GAMEMASTER, false, &HandlePowerCheatCommand,        "", NULL },
                { "waterwalk",  SEC_GAMEMASTER, false, &HandleWaterWalkCheatCommand,    "", NULL },
                { "status",     SEC_GAMEMASTER, false, &HandleCheatStatusCommand,       "", NULL },
                { "taxi",       SEC_GAMEMASTER, false, &HandleTaxiCheatCommand,         "", NULL },
                { "explore",    SEC_GAMEMASTER, false, &HandleExploreCheatCommand,      "", NULL },
                { "all",        SEC_GAMEMASTER, false, &HandleAllSpellCheatCommand,     "", NULL },
                { "nodr",       SEC_GAMEMASTER, false, &HandleNodrCheatCommand,         "", NULL },
                { NULL,         0,              false, NULL,                            "", NULL }

            };

            static ChatCommand commandTable[] =
            {
                { "cheat", SEC_GAMEMASTER, false, NULL, "", cheatCommandTable },
                { NULL, 0, false, NULL, "", NULL }
            };
            return commandTable;
        }

        static bool HandleGodModeCheatCommand(ChatHandler* handler, const char* args)
        {
            if (!handler->GetSession() && !handler->GetSession()->GetPlayer())
                return false;

            std::string argstr = (char*)args;

            if (!*args)
                argstr = (handler->GetSession()->GetPlayer()->GetCommandStatus(CHEAT_GOD)) ? "off" : "on";

            if (argstr == "off")
            {
                handler->GetSession()->GetPlayer()->SetCommandStatusOff(CHEAT_GOD);
                handler->SendSysMessage("Godmode is OFF. You can take damage.");
                return true;
            }
            else if (argstr == "on")
            {
                handler->GetSession()->GetPlayer()->SetCommandStatusOn(CHEAT_GOD);
                handler->SendSysMessage("Godmode is ON. You won't take damage.");
                return true;
            }

            return false;
        }

        static bool HandleNodrCheatCommand(ChatHandler* p_Handler, const char* p_Args)
        {
            if (!p_Handler->GetSession())
                return false;

            Player* l_Player = p_Handler->GetSession()->GetPlayer();
            if (!l_Player)
                return false;

            std::string l_Argstr = (char*)p_Args;

            if (!*p_Args)
                l_Argstr = (p_Handler->GetSession()->GetPlayer()->GetCommandStatus(CHEAT_NO_DR)) ? "off" : "on";

            if (l_Argstr == "off")
            {
                l_Player->SetCommandStatusOff(CHEAT_NO_DR);
                p_Handler->SendSysMessage("NoDR mode is OFF. You will have DR.");
                return true;
            }
            else if (l_Argstr == "on")
            {
                l_Player->SetCommandStatusOn(CHEAT_NO_DR);
                p_Handler->SendSysMessage("NoDR mode is ON. You won't have DR.");
                return true;
            }

            return false;
        }

        static bool HandleCasttimeCheatCommand(ChatHandler* handler, const char* args)
        {
            if (!handler->GetSession() && !handler->GetSession()->GetPlayer())
                return false;

            std::string argstr = (char*)args;

            if (!*args)
                argstr = (handler->GetSession()->GetPlayer()->GetCommandStatus(CHEAT_CASTTIME)) ? "off" : "on";

            if (argstr == "off")
            {
                handler->GetSession()->GetPlayer()->SetCommandStatusOff(CHEAT_CASTTIME);
                handler->SendSysMessage("CastTime Cheat is OFF. Your spells will have a casttime.");
                return true;
            }
            else if (argstr == "on")
            {
                handler->GetSession()->GetPlayer()->SetCommandStatusOn(CHEAT_CASTTIME);
                handler->SendSysMessage("CastTime Cheat is ON. Your spells won't have a casttime.");
                return true;
            }

            return false;
        }

        static bool HandleCoolDownCheatCommand(ChatHandler* handler, const char* args)
        {
            if (!handler->GetSession() && !handler->GetSession()->GetPlayer())
                return false;

            std::string argstr = (char*)args;

            if (!*args)
                argstr = (handler->GetSession()->GetPlayer()->GetCommandStatus(CHEAT_COOLDOWN)) ? "off" : "on";

            if (argstr == "off")
            {
                handler->GetSession()->GetPlayer()->SetCommandStatusOff(CHEAT_COOLDOWN);
                handler->SendSysMessage("Cooldown Cheat is OFF. You are on the global cooldown.");
                return true;
            }
            else if (argstr == "on")
            {
                handler->GetSession()->GetPlayer()->SetCommandStatusOn(CHEAT_COOLDOWN);
                handler->SendSysMessage("Cooldown Cheat is ON. You are not on the global cooldown.");
                return true;
            }

            return false;
        }

        static bool HandlePowerCheatCommand(ChatHandler* handler, const char* args)
        {
            if (!handler->GetSession() && !handler->GetSession()->GetPlayer())
                return false;

            std::string argstr = (char*)args;

            if (!*args)
                argstr = (handler->GetSession()->GetPlayer()->GetCommandStatus(CHEAT_POWER)) ? "off" : "on";

            if (argstr == "off")
            {
                handler->GetSession()->GetPlayer()->SetCommandStatusOff(CHEAT_POWER);
                handler->SendSysMessage("Power Cheat is OFF. You need mana/rage/energy to use spells.");
                return true;
            }
            else if (argstr == "on")
            {
                handler->GetSession()->GetPlayer()->SetCommandStatusOn(CHEAT_POWER);
                handler->SendSysMessage("Power Cheat is ON. You don't need mana/rage/energy to use spells.");
                return true;
            }

            return false;
        }

        static bool HandleCheatStatusCommand(ChatHandler* handler, const char* /*args*/)
        {
            Player* player = handler->GetSession()->GetPlayer();
            const char* enabled = "enabled";
            const char* disabled = "disabled";
            handler->SendSysMessage(LANG_COMMAND_CHEAT_STATUS);
            handler->PSendSysMessage(LANG_COMMAND_CHEAT_GOD, player->GetCommandStatus(CHEAT_GOD) ? enabled : disabled);
            handler->PSendSysMessage(LANG_COMMAND_CHEAT_CD, player->GetCommandStatus(CHEAT_COOLDOWN) ? enabled : disabled);
            handler->PSendSysMessage(LANG_COMMAND_CHEAT_CT, player->GetCommandStatus(CHEAT_CASTTIME) ? enabled : disabled);
            handler->PSendSysMessage(LANG_COMMAND_CHEAT_POWER, player->GetCommandStatus(CHEAT_POWER) ? enabled : disabled);
            handler->PSendSysMessage(LANG_COMMAND_CHEAT_WW, player->GetCommandStatus(CHEAT_WATERWALK) ? enabled : disabled);
            return true;
        }

        static bool HandleWaterWalkCheatCommand(ChatHandler* handler, const char* args)
        {
            if (!handler->GetSession() && !handler->GetSession()->GetPlayer())
                return false;

            std::string argstr = (char*)args;

            if (!*args)
            {
                argstr = (handler->GetSession()->GetPlayer()->GetCommandStatus(CHEAT_WATERWALK)) ? "off" : "on";
                if (handler->GetSession()->GetPlayer()->GetCommandStatus(CHEAT_WATERWALK))
                    argstr = "off";
                else
                    argstr = "on";
            }

            if (argstr == "off")
            {
                handler->GetSession()->GetPlayer()->SetCommandStatusOff(CHEAT_WATERWALK);
                handler->GetSession()->GetPlayer()->SetWaterWalking(false);      // OFF
                handler->SendSysMessage("Waterwalking is OFF. You can't walk on water.");
                return true;
            }
            else if (argstr == "on")
            {
                handler->GetSession()->GetPlayer()->SetCommandStatusOn(CHEAT_WATERWALK);
                handler->GetSession()->GetPlayer()->SetWaterWalking(true);       // ON
                handler->SendSysMessage("Waterwalking is ON. You can walk on water.");
                return true;
            }

            return false;
        }

        static bool HandleTaxiCheatCommand(ChatHandler* handler, const char* args)
        {
            if (!*args)
            {
                handler->SendSysMessage(LANG_USE_BOL);
                handler->SetSentErrorMessage(true);
                return false;
            }

            std::string argstr = (char*)args;

            Player* chr = handler->getSelectedPlayer();

            if (!chr)
                chr = handler->GetSession()->GetPlayer();
            else if (handler->HasLowerSecurity(chr, 0)) // check online security
                return false;

            if (argstr == "on")
            {
                chr->SetTaxiCheater(true);
                handler->PSendSysMessage(LANG_YOU_GIVE_TAXIS, handler->GetNameLink(chr).c_str());
                if (handler->needReportToTarget(chr))
                    ChatHandler(chr).PSendSysMessage(LANG_YOURS_TAXIS_ADDED, handler->GetNameLink().c_str());
                return true;
            }

            if (argstr == "off")
            {
                chr->SetTaxiCheater(false);
                handler->PSendSysMessage(LANG_YOU_REMOVE_TAXIS, handler->GetNameLink(chr).c_str());
                if (handler->needReportToTarget(chr))
                    ChatHandler(chr).PSendSysMessage(LANG_YOURS_TAXIS_REMOVED, handler->GetNameLink().c_str());

                return true;
            }

            handler->SendSysMessage(LANG_USE_BOL);
            handler->SetSentErrorMessage(true);
            return false;
        }

        static bool HandleExploreCheatCommand(ChatHandler* handler, const char *args)
        {
            if (!*args)
                return false;

            int flag = atoi((char*)args);

            Player* chr = handler->getSelectedPlayer();
            if (chr == NULL)
            {
                handler->SendSysMessage(LANG_NO_CHAR_SELECTED);
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (flag != 0)
            {
                handler->PSendSysMessage(LANG_YOU_SET_EXPLORE_ALL, handler->GetNameLink(chr).c_str());
                if (handler->needReportToTarget(chr))
                    ChatHandler(chr).PSendSysMessage(LANG_YOURS_EXPLORE_SET_ALL, handler->GetNameLink().c_str());
            }
            else
            {
                handler->PSendSysMessage(LANG_YOU_SET_EXPLORE_NOTHING, handler->GetNameLink(chr).c_str());
                if (handler->needReportToTarget(chr))
                    ChatHandler(chr).PSendSysMessage(LANG_YOURS_EXPLORE_SET_NOTHING, handler->GetNameLink().c_str());
            }

            for (uint32 i = 0; i < PLAYER_EXPLORED_ZONES_SIZE; ++i)
            {
                if (flag != 0)
                    handler->GetSession()->GetPlayer()->SetFlag(PLAYER_FIELD_EXPLORED_ZONES+i, 0xFFFFFFFF);
                else
                    handler->GetSession()->GetPlayer()->SetFlag(PLAYER_FIELD_EXPLORED_ZONES+i, 0);
            }

            return true;
        }

        static bool HandleAllSpellCheatCommand(ChatHandler* handler, const char* args)
        {
            if (!handler->GetSession() && !handler->GetSession()->GetPlayer())
                return false;

            std::string argstr = (char*)args;

            if (!*args)
                argstr = (handler->GetSession()->GetPlayer()->GetCommandStatus(CHEAT_ALL_SPELLS)) ? "off" : "on";

            if (argstr == "off")
            {
                handler->GetSession()->GetPlayer()->SetCommandStatusOff(CHEAT_CASTTIME);
                handler->GetSession()->GetPlayer()->SetCommandStatusOff(CHEAT_COOLDOWN);
                handler->GetSession()->GetPlayer()->SetCommandStatusOff(CHEAT_POWER);
                handler->SendSysMessage("All spell cheats are OFF. Your spells will have a casttime, a cooldown and a power cost.");
                return true;
            }
            else if (argstr == "on")
            {
                handler->GetSession()->GetPlayer()->SetCommandStatusOn(CHEAT_CASTTIME);
                handler->GetSession()->GetPlayer()->SetCommandStatusOn(CHEAT_COOLDOWN);
                handler->GetSession()->GetPlayer()->SetCommandStatusOn(CHEAT_POWER);
                handler->SendSysMessage("All spell cheats are ON. Your spells won't have a casttime, a cooldown and a power cost.");
                return true;
            }

            return false;
        }
};

#ifndef __clang_analyzer__
void AddSC_cheat_commandscript()
{
    new cheat_commandscript();
}
#endif
