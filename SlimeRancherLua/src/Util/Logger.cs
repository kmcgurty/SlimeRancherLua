using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SlimeRancherLua
{
    internal class Logger
    {
        public static void Log(string type, string message)
        {
            type = type.ToLower();
            switch (type)
            {
                case "special":
                    Console.WriteLine(message.ToString());
                    break;
                case "info":
                    Console.WriteLine($"[{type}] " + message);
                    break;
                case "warn":
                    Console.WriteLine($"[{type}] " + message);
                    break;
                case "error":
                    Console.WriteLine($"[{type}] " + message);
                    break;
            }
        }

        public static void LuaLog(string message)
        {
            Console.WriteLine("Lua script says: " + message);
        }
    }
}
