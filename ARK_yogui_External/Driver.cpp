#include "Driver.h"

#include "skCrypt.h"
#include <iostream>

Driver eDriver;

bool Driver::initialize()
{
	//typedef ULONG64(__stdcall* _hook_template)(ULONG);

	static HMODULE win32u_module = LoadLibraryA("win32u.dll");
	this->_NtUserSetAutoRotation = reinterpret_cast<PVOID>(GetProcAddress(win32u_module, XorString("NtUserSetAutoRotation")));

	printf("this->_NtUserSetAutoRotation : 0x%llX\n", this->_NtUserSetAutoRotation);

	return (this->_NtUserSetAutoRotation);
}

bool Driver::attach_to_process(ULONG procID)
{
	_driver_request_header DriverRequest;
	_driver_request_stack StackRequest;

	DriverRequest.request_index = _DRIVER_REQUEST::DRIVER_REQUEST_ATTACH_PROCESS;
	DriverRequest.p_request = &StackRequest;

	StackRequest.m_Attach = true;
	StackRequest.m_ProcessID = procID;

	this->call_driver_hook(&DriverRequest);
	return true;
}

uintptr_t Driver::get_module_address(LPCSTR module_name)
{
	_driver_request_header DriverRequest;
	_driver_request_getmodule ModuleRequest = { 0 };

	DriverRequest.request_index = _DRIVER_REQUEST::DRIVER_REQUEST_GETMODULE;
	DriverRequest.p_request = &ModuleRequest;

	ModuleRequest.is32bit = false;
	ModuleRequest.m_ModuleName = module_name;

	this->call_driver_hook(&DriverRequest);

	return ModuleRequest.m_Buffer;
}

SIZE_T Driver::read_in_buffer(PVOID address, PVOID buffer, size_t size)
{
	_driver_request_header DriverRequest;
	_driver_request_read DriverReadRequest;

	DriverRequest.request_index = _DRIVER_REQUEST::DRIVER_REQUEST_READMEM;
	DriverRequest.p_request = &DriverReadRequest;

	if (!this->is_valid_address(address))
		return false;

	DriverReadRequest.m_Address = address;
	DriverReadRequest.m_ResponseBuffer = (ULONG64*)buffer;
	DriverReadRequest.m_Size = (ULONG)size;

	this->call_driver_hook(&DriverRequest);
	return DriverReadRequest.m_BytesRead;
}
