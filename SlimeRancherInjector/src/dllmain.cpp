// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "winhttp_proxy.h"
#include <iostream>
#include <chrono>
#include <thread>
#include "INIReader.h"

std::string modDLLName;
std::string injectedAddr;

std::string exec(std::string command) {
    char buffer[128];
    std::string result = "";

    // Open pipe to file
    FILE* pipe = _popen(command.c_str(), "r");
    if (!pipe) {
        return "popen failed!";
    }

    // read till end of process:
    while (!feof(pipe)) {

        // use buffer to read and add to result
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }

    _pclose(pipe);
    return result;
}

void inject(std::string dllToInject, std::string namespaceToInject) {
    std::string injectCMD = ".\\injector\\smi.exe inject -p SlimeRancher -a " + dllToInject + " -n " + namespaceToInject + " -c Loader -m Load";
    
    std::cout << injectCMD << std::endl;
    
    std::string output = exec(injectCMD);

    injectedAddr = output.substr(output.find("0x"), output.length() - 1);

    std::cout << "Injected Address: " << injectedAddr << std::endl;
}

//not used
void eject(std::string namespaceToEject) {
    if (injectedAddr == "")
        return;

    std::string ejectCMD = ".\\injector\\smi.exe eject -p SlimeRancher -a " + injectedAddr + " -n " + namespaceToEject + " -c Loader -m Unload";
    std::string output = exec(ejectCMD);

    std::cout << "Eject output: " << output << std::endl;
    injectedAddr = "";
}

void init() {
    std::string iniName = ".\\LuaConfig.ini";

    INIReader reader(iniName);

    bool shouldAlloc = reader.GetBoolean("SlimeRancherInjector", "allocConsole", true);

    if (shouldAlloc) {
        //allocate console for debugging
        AllocConsole();
        FILE* ignore = freopen("CONOUT$", "w", stdout);
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    }

    std::thread t([iniName]() {
        INIReader reader(iniName);

        if (reader.ParseError() < 0) {
            std::cout << "Can't load 'MPConfig.ini', aborting injection. Error no. " << reader.ParseError() << std::endl;
            return;
        }

        int sleepTime = reader.GetInteger("SlimeRancherInjector", "sleepTime", -1);

        if (sleepTime == -1) {
            std::cout << "[SlimeRancherInjector] sleepTime in config does not exist. Aborting injection." << std::endl;
            return;
        }

        std::cout << "Sleeping injector for " << sleepTime << " second(s) while the game loads..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(sleepTime));
        
        std::string dllToInject = reader.Get("SlimeRancherInjector", "dllToInject", "UNKNOWN");

        if (dllToInject == "UNKNOWN") {
            std::cout << "[SlimeRancherInjector] dllToInject in config does not exist. Aborting injection." << std::endl; \
            return;
        }

        std::string namespaceToInject = reader.Get("SlimeRancherInjector", "namespaceToInject", "UNKNOWN");

        if (namespaceToInject == "UNKNOWN") {
            std::cout << "[SlimeRancherInjector] namespaceToInject in config does not exist. Aborting injection." << std::endl;
            return;
        }


        std::cout << "Injecting mod..." << std::endl;
        inject(dllToInject, namespaceToInject);
    });

    t.detach();
}


BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        init();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

//#include <stdlib.h>
//#include <map>
//#include <vector>

//LPCWSTR dll_path = L"MonoBleedingEdge\\EmbedRuntime\\mono-2.0-bdwgc.dll";

//void dothehack() {
    //This is all a failed attempt to inject c# directly before I gave up.
    //Some of this is still useful but I don't know nearly enough to keep this going. Keeping it here in case anything changes
    // 
    ////load mono into memory dll
    //HMODULE monoHandle = LoadLibraryW(dll_path);
    //std::cout << "Mono handle pointer: " << std::hex << monoHandle << std::endl;

    //std::map<LPCSTR, INT_PTR> monoFunctions = {
    //    {"mono_get_root_domain", NULL},
    //    {"mono_thread_attach", NULL},
    //    {"mono_image_open_from_data", NULL},
    //    {"mono_assembly_load_from_full", NULL},
    //    {"mono_assembly_get_image", NULL},
    //    {"mono_class_from_name", NULL},
    //    {"mono_class_get_method_from_name", NULL},
    //    {"mono_runtime_invoke", NULL},
    //    {"mono_assembly_close", NULL}
    //};

    ////load mono function addresses into monoFunctions
    //for (const auto& function : monoFunctions) {
    //    INT_PTR address = (INT_PTR)GetProcAddress(monoHandle, function.first);
    //    monoFunctions.at(function.first) = address;
    //}

    //FreeLibrary(monoHandle);

    ////print out addresses for debugging
    //for (const auto& function : monoFunctions) {
    //    std::cout << function.first << " at " << std::hex << function.second << std::endl;
    //}

    //INT_PTR addr = monoFunctions.at("mono_get_root_domain");


    //char code[] = "\x48\xC7\xC0";
    ////{
    //    //0x48, 0xC7, 0xC0, addr    //mov rax [address]
    ////};

    //std::cout << std::hex << code <<std::endl;

    ////monodomain domain = funct();

    ////INT_PTR test = asm_func(addr);
    ////std::cout << "int_ptr for mono_get_root_domain is: " << test << std::endl;


//}