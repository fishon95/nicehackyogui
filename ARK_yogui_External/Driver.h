#pragma once
#include <Windows.h>
#include "..\\privated_driver\\drv_header.h"

class Driver;
extern Driver eDriver;

class Driver
{
private:
	PVOID _NtUserSetAutoRotation = nullptr;
private:
	void __forceinline call_driver_hook(void* Buffer);
public:
	bool initialize();
	bool attach_to_process(ULONG procID);

	uintptr_t get_module_address(LPCSTR module_name);

	template <typename T>
	T read_mem(PVOID address);

	SIZE_T read_in_buffer(PVOID address, PVOID buffer, size_t size);

	template <typename T>
	bool write_mem(PVOID address, const T& value);

	bool __forceinline is_valid_address(void* address);
};

#pragma section(".text")
__declspec(allocate(".text")) const unsigned char mov_r13_rcx_stub[] = { 0x49, 0x8B, 0xC5, 0x4C, 0x8B, 0xE9, 0xC3 };
void Driver::call_driver_hook(void* Buffer)
{
	typedef ULONG64(__stdcall* _hook_template)(ULONG);
	static _hook_template hook_temp = reinterpret_cast<_hook_template>(this->_NtUserSetAutoRotation);

	typedef PVOID(__stdcall* _stub_template)(PVOID);
	static _stub_template set_r13 = reinterpret_cast<_stub_template>((PVOID)mov_r13_rcx_stub);

	PVOID prev_r13 = set_r13(Buffer);
	hook_temp(DRIVER_IDENTIFICATION);
	set_r13(prev_r13);
}

bool Driver::is_valid_address(void* address)
{
	if (address == nullptr)
		return false;	//							0xFFFFFFFFFFFFFFFF
	if ((uintptr_t)address > 0x7FFFFFFFFFFF)
		return false;
	else
		return true;
}

template<typename T>
inline T Driver::read_mem(PVOID address)
{
	_driver_request_header DriverRequest;
	_driver_request_read ReadRequest;

	DriverRequest.request_index = _DRIVER_REQUEST::DRIVER_REQUEST_READMEM;
	DriverRequest.p_request = &ReadRequest;

	if (!this->is_valid_address(address))
		return *(T*)&ReadRequest.m_ResponseBuffer;

	T Buffer;
	ReadRequest.m_Address = address;
	ReadRequest.m_ResponseBuffer = reinterpret_cast<ULONG64*>(&Buffer);
	ReadRequest.m_Size = sizeof(T);

	this->call_driver_hook(&DriverRequest);
	return Buffer;
}


template<typename T>
inline bool Driver::write_mem(PVOID address, const T& value)
{
	_driver_request_header DriverRequest;
	_driver_request_write WriteRequest;

	DriverRequest.request_index = _DRIVER_REQUEST::DRIVER_REQUEST_WRITEMEM;
	DriverRequest.p_request = &WriteRequest;

	if (!this->is_valid_address(address))
		return false;

	T Buffer = value;
	WriteRequest.m_Address = address;
	WriteRequest.m_Value = reinterpret_cast<ULONG64*>(&Buffer);
	WriteRequest.m_Size = sizeof(T);

	this->call_driver_hook(&DriverRequest);

	return true;
}

