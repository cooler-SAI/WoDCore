﻿namespace ClientPatcher.Patches
{
    class Mac
    {
        public static class x64
        {
            public static byte[] BNet      = { 0xB8, 0xD5, 0xF8, 0x7F, 0x82, 0x89, 0x47, 0x0C, 0x5D, 0xC3, 0x90, 0x90, 0x90 };
            public static byte[] Send      = { 0x90, 0x31, 0xDB };
            public static byte[] Password  = { 0x0F, 0x85 };
            public static byte[] Signature = { 0x31, 0xC0, 0xFF, 0xC0, 0xC3, 0x90 };    
        }
    }
}
