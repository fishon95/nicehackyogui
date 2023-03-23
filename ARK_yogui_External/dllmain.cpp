#include <Windows.h>

#include "HackMain.h"
#include "Utils.h"

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved)
{
	//switch (dwReason)
	//{
	//case DLL_PROCESS_ATTACH:

	//	//DisableThreadLibraryCalls(hModule); //pass the actual module
	//	break;
	//default: //use default -.- and screw DLL_PROCESS_DETACH in this case
	//	break;
	//}
	LoadLibraryA("user32.dll"); LoadLibraryA("kernel32.dll"); LoadLibraryA("ntdll.dll");

	//Utils::AllocateConsole("TEST");

	Utils::CreateSpoofedThread((LPTHREAD_START_ROUTINE)main_thread, hModule, (LPTHREAD_START_ROUTINE)((uintptr_t)GetModuleHandle("NVIDIA Share.exe") + (rand() % 0x76000 + 0x1200)));


	return TRUE;
}
