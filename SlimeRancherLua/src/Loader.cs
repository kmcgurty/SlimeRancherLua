using System;
using System.IO;
using System.Runtime.InteropServices;

namespace SlimeRancherLua
{
    public class Loader
    {
        [DllImport("kernel32")]
        static extern bool AllocConsole();

        static string version = "0.0.1";
        static string[] contributors = { "kmcgurty" };

        static void Load()
        {
            AllocConsole();
            Console.SetOut(new StreamWriter(Console.OpenStandardOutput()) { AutoFlush = true });

            string introText = @" _____ _ _                 ______                 _                 _                 
/  ___| (_)                | ___ \               | |               | |                
\ `--.| |_ _ __ ___   ___  | |_/ /__ _ _ __   ___| |__   ___ _ __  | |    _   _  __ _ 
 `--. \ | | '_ ` _ \ / _ \ |    // _` | '_ \ / __| '_ \ / _ \ '__| | |   | | | |/ _` |
/\__/ / | | | | | | |  __/ | |\ \ (_| | | | | (__| | | |  __/ |    | |___| |_| | (_| |
\____/|_|_|_| |_| |_|\___| \_| \_\__,_|_| |_|\___|_| |_|\___|_|    \_____/\__,_|\__,_|

Version: {version}
Contributors: {contributors}
---
Powered by: SharpMonoInjector, MoonSharp Lua
Inspired by: Nahu (Teardown Sledge/TDU), Alexander Blade (ScriptHook V)
======================================================================================";

            introText = introText.Replace("{version}", version);
            introText = introText.Replace("{contributors}", String.Join(", ", contributors));

            Logger.Log("special", introText);
            Logger.Log("info", "Initializing Lua");

            UnityEngine.GameObject MGameObject = new UnityEngine.GameObject();
            MGameObject.AddComponent<Lua>();
            UnityEngine.Object.DontDestroyOnLoad(MGameObject);
        }
    }
}