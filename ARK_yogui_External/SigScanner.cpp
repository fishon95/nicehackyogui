#include "SigScanner.h"
#include "Utils.h"

SigScanner eSigScanner;

uintptr_t SigScanner::FindPattern(void* base, unsigned int size, const char* pattern, const char* mask)
{
	size_t patternLength = strlen(mask);

	for (uintptr_t i = 0; i < size - patternLength; i++)
	{
		bool found = true;
		for (uintptr_t j = 0; j < patternLength; j++)
		{
			if (mask[j] != '?' && pattern[j] != *(char*)((char*)base + i + j))
			{
				found = false;
				break; // yeah that's right, stop iterating when pattern is bad.  Looking at you fleep...
			}
		}

		if (found)
		{
			return (uintptr_t)base + i;
		}
	}
	return 0;
}

uintptr_t SigScanner::FindPatternEx(void* base, DWORD size, const char* pattern, const char* mask)
{
	//if (!eDriver.m_IsAttached)
		//return NULL;

	uintptr_t start = (uintptr_t)base;
	uintptr_t end = start + size;

	uintptr_t currentChunk = start;
	SIZE_T bytesRead;

	while (currentChunk < end)
	{
		//make data accessible to ReadProcessMemory
			//DWORD oldprotect;
			//VirtualProtectEx(m_hProc, (void*)currentChunk, 4096, PROCESS_VM_READ, &oldprotect);

		//Copy chunk of external memory into local storage
		BYTE buffer[4096] = { 0 };
		bytesRead = eDriver.read_in_buffer((PVOID)currentChunk, buffer, 4096);

		////if readprocessmemory failed, return
		if (bytesRead == 0)
		{
			return 0;
		}

		//Find pattern in local buffer, if pattern is found return address of matching data
		uintptr_t InternalAddress = FindPattern((char*)&buffer, bytesRead, pattern, mask);

		//if Find Pattern returned an address
		if (InternalAddress != 0)
		{
			//convert internal offset to external address and return
			uintptr_t offsetFromBuffer = InternalAddress - (uintptr_t)&buffer;
			return currentChunk + offsetFromBuffer;
		}

		//pattern not found in this chunk
		else
		{
			//advance to next chunk
			currentChunk = currentChunk + bytesRead;
		}
	}
	return 0;
}

uintptr_t SigScanner::ResolveRelativeAddress(uintptr_t Instruction, DWORD OffsetOffset, DWORD InstructionSize)
{
	LONG RipOffset = eDriver.read_mem<LONG>((PVOID)(Instruction + OffsetOffset));
	uintptr_t ResolveAddr = Instruction + InstructionSize + RipOffset;

	return ResolveAddr;
}

bool SigScanner::SelectRegion(uintptr_t base, LPCSTR region)
{
	IMAGE_SECTION_HEADER TextSection; uintptr_t TextSectionSize = NULL;

	if (!Utils::FindSection(base, region, &TextSection, &TextSectionSize))
		return false;

	this->m_BaseModule = base;

	this->m_BaseRegion = base + TextSection.VirtualAddress;
	this->m_BaseRegionSize = TextSectionSize;

	printf("[sig_scanner] region selected region(0x%llX) size(0x%lX)\n", this->m_BaseRegion, this->m_BaseRegionSize);

	m_IsRegionSelected = true;

	m_SignatureList.clear();

	return true;
}

bool SigScanner::AddSignature(const _SDKSignature& Signature)
{
	if (!m_IsRegionSelected)
		return false;

	m_SignatureList.push_back(Signature);

	return true;
}

bool SigScanner::LocateSignatures()
{
	if (!m_IsRegionSelected)
		return false;

	if (m_SignatureList.size() < 1)
		return false;

	for (auto& signature : m_SignatureList)
	{
		if (!signature.dst)
			continue;

		*signature.dst = this->FindPatternEx((PVOID)this->m_BaseRegion, this->m_BaseRegionSize, signature.pattern, signature.mask.c_str());

		//printf("[sig_scanner] located sig : 0x%llX\n", *signature.dst);
		if (!(*signature.dst))
		{
			m_SignatureList.clear();
			return false;
		}
	}

	m_SignatureList.clear();
	return true;
}
