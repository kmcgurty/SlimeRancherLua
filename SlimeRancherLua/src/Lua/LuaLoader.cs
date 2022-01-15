using System;
using System.IO;
using UnityEngine;
using MoonSharp.Interpreter;



namespace SlimeRancherLua
{
    public class Lua : UnityEngine.MonoBehaviour
    {


        private void Start()
        {
            try
            {
                string code = @"
                function init()
                    Log(""We are running inside lua!"")
                end
                ";

                Script script = new Script();
                script.Globals["Log"] = (Action<string>)Logger.LuaLog;
                script.DoString(code);
                script.Call(script.Globals["init"]);
            }
            catch (ScriptRuntimeException e)
            {
                Console.WriteLine("Doh! An error occured! {0}", e.DecoratedMessage);
            }
        }
        private void Update()
        {

        }
        private void LateUpdate()
        {

        }
        private void OnGUI()
        {
            //i++;
            GUI.contentColor = Color.black;
            GUI.Label(new Rect(10, 10, 200, 40), "We can draw shit on the screen!) - ");
        }

        private void FixedUpdate()
        {

        }
    }
}
