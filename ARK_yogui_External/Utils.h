#pragma once
#include <Windows.h>
#include <string>

class Utils
{
public:
	static void AllocateConsole(LPCSTR Title);
	static HANDLE CreateSpoofedThread(LPTHREAD_START_ROUTINE RealThreadStart, LPVOID lParam, LPTHREAD_START_ROUTINE SpoofedThreadStart);

	static bool FindSection(uintptr_t base, LPCSTR SectionName, IMAGE_SECTION_HEADER* pSection, uintptr_t* pSize);

	static void ReportError(const char* Error);

	static uintptr_t WaitForModule(std::string ModuleName, int UpdateInterval);
};

