#include "Utils.h"
#include <iostream>
#include "Driver.h"

#pragma warning(disable:4996)

void Utils::AllocateConsole(LPCSTR Title)
{
	AllocConsole();
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);
}

HANDLE Utils::CreateSpoofedThread(LPTHREAD_START_ROUTINE RealThreadStart, LPVOID lParam, LPTHREAD_START_ROUTINE SpoofedThreadStart)
{
	HANDLE hThread = CreateThread(0, 0, SpoofedThreadStart, lParam, CREATE_SUSPENDED, 0);

	if (hThread == NULL)
		return NULL;

	//printf("[+] (%i)Thread created with handle 0x%lX\n",GetThreadId(hThread), hThread);

	CONTEXT ThreadContext;
	RtlZeroMemory(&ThreadContext, sizeof(ThreadContext));
	ThreadContext.ContextFlags = CONTEXT_FULL;
	if (!GetThreadContext(hThread, &ThreadContext))
	{
		CloseHandle(hThread);
		return NULL;
	}

	//printf("[+] Got thread context: RCX -> 0x%llX\n", ThreadContext.Rcx);

	ThreadContext.ContextFlags = CONTEXT_FULL;
	ThreadContext.Rcx = (DWORD64)RealThreadStart;
	if (!SetThreadContext(hThread, &ThreadContext))
	{
		CloseHandle(hThread);
		return NULL;
	}

	//printf("[+] Set thread context!\n");

	if (ResumeThread(hThread) < 0)
	{
		CloseHandle(hThread);
		return NULL;
	}

	//printf("[+] Resumed thread!\n");

	return hThread;
}

bool Utils::FindSection(uintptr_t base, LPCSTR SectionName, IMAGE_SECTION_HEADER* pSection, uintptr_t* pSize)
{
	auto DosHeader = eDriver.read_mem<IMAGE_DOS_HEADER>((void*)base);
	auto NtHeader = eDriver.read_mem<IMAGE_NT_HEADERS64>((void*)(base + DosHeader.e_lfanew));

	for (int i = 0; i < NtHeader.FileHeader.NumberOfSections; i++)
	{
		auto CurrentSection = eDriver.read_mem<IMAGE_SECTION_HEADER>((void*)(base + DosHeader.e_lfanew + sizeof(IMAGE_NT_HEADERS64) + (i * 40)));

		if (strcmp((char*)CurrentSection.Name, SectionName) == 0)
		{
			if (pSection)
			{
				*pSection = CurrentSection;
			}
			if (pSize)
			{
				auto NextSection = eDriver.read_mem<IMAGE_SECTION_HEADER>((void*)(base + DosHeader.e_lfanew + sizeof(IMAGE_NT_HEADERS64) + ((i + 1) * 40)));
				*pSize = NextSection.VirtualAddress - CurrentSection.VirtualAddress;
			}
			return true;
		}

	}
	return false;
}

void Utils::ReportError(const char* Error)
{
	printf("Error : %s\n", Error);
	system("pause");
	MessageBoxA(NULL, Error, "Critical Error!", MB_OK);
}

uintptr_t Utils::WaitForModule(std::string ModuleName, int UpdateInterval)
{
	const int MaxTries = 10;
	int Tries = 0;
	uintptr_t Buffer = NULL;
	do
	{
		if (Tries >= MaxTries)
			return NULL;
		Buffer = eDriver.get_module_address(ModuleName.c_str());
		Sleep(UpdateInterval);
		Tries++;
	} while (Buffer == NULL);
	return Buffer;
}
