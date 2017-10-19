﻿using System;
using System.Collections.Generic;
using WowPacketParser.Enums;
using WowPacketParser.Misc;
using WowPacketParser.Parsing;
using WowPacketParser.Store;
using WowPacketParser.Store.Objects;
using CoreObjects = WowPacketParser.Store.Objects;

namespace WowPacketParserModule.V5_3_0_16981.Parsers
{
    public static class ActionBarHandler
    {
        [Parser(Opcode.SMSG_ACTION_BUTTONS)]
        public static void HandleActionButtons(Packet packet)
        {
            const int buttonCount = 132;

            var startAction = new StartAction { Actions = new List<CoreObjects.Action>(buttonCount) };

            var buttons = new byte[buttonCount][];

            for (var i = 0; i < buttonCount; i++)
            {
                buttons[i] = new byte[8];
                buttons[i][7] = packet.ReadBit();
            }

            for (var i = 0; i < buttonCount; i++)
                buttons[i][2] = packet.ReadBit();
            for (var i = 0; i < buttonCount; i++)
                buttons[i][1] = packet.ReadBit();
            for (var i = 0; i < buttonCount; i++)
                buttons[i][6] = packet.ReadBit();
            for (var i = 0; i < buttonCount; i++)
                buttons[i][3] = packet.ReadBit();
            for (var i = 0; i < buttonCount; i++)
                buttons[i][4] = packet.ReadBit();
            for (var i = 0; i < buttonCount; i++)
                buttons[i][5] = packet.ReadBit();
            for (var i = 0; i < buttonCount; i++)
                buttons[i][0] = packet.ReadBit();

            for (var i = 0; i < buttonCount; i++)
                packet.ReadXORByte(buttons[i], 3);
            for (var i = 0; i < buttonCount; i++)
                packet.ReadXORByte(buttons[i], 1);
            for (var i = 0; i < buttonCount; i++)
                packet.ReadXORByte(buttons[i], 4);
            for (var i = 0; i < buttonCount; i++)
                packet.ReadXORByte(buttons[i], 5);
            for (var i = 0; i < buttonCount; i++)
                packet.ReadXORByte(buttons[i], 6);
            for (var i = 0; i < buttonCount; i++)
                packet.ReadXORByte(buttons[i], 2);
            for (var i = 0; i < buttonCount; i++)
                packet.ReadXORByte(buttons[i], 7);
            for (var i = 0; i < buttonCount; i++)
                packet.ReadXORByte(buttons[i], 0);

            packet.ReadByte("Action Bar State");

            for (int i = 0; i < buttonCount; i++)
            {
                var actionId = BitConverter.ToInt32(buttons[i], 0);

                if (actionId == 0)
                    continue;

                var action = new CoreObjects.Action
                {
                    Button = (uint)i,
                    Id = (uint)actionId,
                    Type = 0 // removed in MoP
                };

                packet.WriteLine("Action " + i + ": " + action.Id);
                startAction.Actions.Add(action);
            }

            WoWObject character;
            if (Storage.Objects.TryGetValue(SessionHandler.LoginGuid, out character))
            {
                var player = character as Player;
                if (player != null && player.FirstLogin)
                    Storage.StartActions.Add(new Tuple<Race, Class>(player.Race, player.Class), startAction, packet.TimeSpan);
            }
        }
    }
}
