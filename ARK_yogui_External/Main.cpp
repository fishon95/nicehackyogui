#include "HackMain.h"

#include "NvidiaOverlay/NvidiaOverlay.h"

#include "Driver.h"

#include <thread>
#include <chrono>

#include "Menu.h"
#include "Aimbot.h"
#include "Misc.h"

int cheat_feature_thread()
{
	while (eRenderer.m_Attached) // CHANGE THIS
	{
		eAimbot.Work();
		eMisc.Work();

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	return 1;
}

int main_thread()
{
	Beep(500, 500);
	if (!eNvidiaOverlay.Initialize())
	{
		Utils::ReportError(XorString("[-] Rendered target failed to initialize!\n"));
		
		return 1;
	}
	//printf("eNvidiaOverlay.PrepareOverlay\n");
	//system("pause");

	if (!eNvidiaOverlay.PrepareOverlay(eNvidiaOverlay.m_hOverlayHandle))
	{
		Utils::ReportError(XorString("[-] Renderer failed to prepeare!\n"));
		return 1;
	}
	//printf("CreateRenderThread\n");
	//system("pause");
	if (!eRenderer.CreateRenderThread())
	{
		Utils::ReportError(XorString("[-] Failed creating Render thread!\n"));
		return 1;
	}
	//printf("eDriver.initialize\n");
	//system("pause");
	if (!eDriver.initialize())
	{
		Utils::ReportError(XorString("[-] Driver failed to initialize!\n"));
		return 1;
	}
	//printf("eDriver.initialize\n");
	//system("pause");
	eNvidiaOverlay.m_TargetHandle = NULL;
	while (!eNvidiaOverlay.m_TargetHandle)
	{
		static int counter = 0;
		if (counter >= 720)
			break;
		eNvidiaOverlay.m_TargetHandle = FindWindowA(XorString("UnrealWindow"), XorString("ARK: Survival Evolved"));
		//eNvidiaOverlay.m_TargetHandle = FindWindowA(NULL, XorString("Untitled - Notepad"));
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		counter++;
	}

	//printf("eNvidiaOverlay.m_TargetHandle\n");
	//system("pause");
	if (!eNvidiaOverlay.m_TargetHandle)
	{
		Utils::ReportError(XorString("[-] Failed finding game window!\n"));
		return 1;
	}

	GetWindowThreadProcessId(eNvidiaOverlay.m_TargetHandle, &eHackMain.proc_id);
	//system("pause");
	if (eHackMain.proc_id == NULL)
	{
		Utils::ReportError(XorString("[-] Process not found!\n"));
		return 1;
	}
	std::this_thread::sleep_for(std::chrono::seconds(10));

	if (!eRenderer.Attach())
	{
		Utils::ReportError(XorString("[-] Renderer failed to attach to target window!\n"));
		return 1;
	}
	if (!eDriver.attach_to_process(eHackMain.proc_id))
	{
		Utils::ReportError(XorString("[-] Reciever failed to attach to game!\n"));
		return 1;
	}

	Globals::Modules::ShooterGame = Utils::WaitForModule((char*)XorString("ShooterGame.exe"), 100);

	if (!Globals::Modules::ShooterGame)
	{
		Utils::ReportError(XorString("[-] Failed locating modules...!\n"));
		return 1;
	}

	//printf("Globals::Modules::ShooterGame : 0x%llX\n", Globals::Modules::ShooterGame);

	std::this_thread::sleep_for(std::chrono::seconds(2));

	if (!SDK::Functions::InitializeOffsets())
	{
		Utils::ReportError(XorString("[-] SDK failed initializing...!\n"));
		return 1;
	}

	if (!eHackMain.Initialize())
	{
		Utils::ReportError(XorString("[-] HackMain failed initializing...!\n"));
		return 1;
	}

	Utils::CreateSpoofedThread((LPTHREAD_START_ROUTINE)cheat_feature_thread, nullptr, (LPTHREAD_START_ROUTINE)((uintptr_t)GetModuleHandle("NVIDIA Share.exe") + (rand() % 0x76000 + 0x1200)));


	while (eRenderer.m_Attached) // CHANGE THIS
	{
		if ((GetAsyncKeyState(VK_END) & 0x8000))
			break;

#if(USE_GAMEBAR)
		eDXWindow.Work();
		//eMiscVisuals.UpdateMiscWindows();
#else
		eMenu.Work();
#endif

		eHackMain.UpdateInformation();
		eRenderer.UpdateOffsets();
		
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}


	eHackMain.m_ActorItterator.Free();
	eRenderer.ExitRenderThread();

	ExitProcess(0);
	return 1;
}