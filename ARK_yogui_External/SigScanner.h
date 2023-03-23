#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>

#include "Driver.h"


struct _SDKSignature
{
	const char* pattern;
	std::string mask;
	uintptr_t* dst;

};

class SigScanner;
extern SigScanner eSigScanner;

class SigScanner
{
private:
	std::vector<_SDKSignature> m_SignatureList;
	//Region
	bool m_IsRegionSelected = false;
	uintptr_t m_BaseModule = NULL;
	uintptr_t m_BaseRegion = NULL;
	uintptr_t m_BaseRegionSize = NULL;
public:
	static uintptr_t FindPattern(void* base, unsigned int size, const char* pattern, const char* mask);
	static uintptr_t FindPatternEx(void* base, DWORD size, const char* pattern, const char* mask);
	static uintptr_t ResolveRelativeAddress(uintptr_t Instruction, DWORD OffsetOffset, DWORD InstructionSize);

	bool SelectRegion(uintptr_t base, LPCSTR region);
	bool AddSignature(const _SDKSignature& Signature);

	bool LocateSignatures();
public:
	static SigScanner* GetInstance() { return &eSigScanner; }
};

