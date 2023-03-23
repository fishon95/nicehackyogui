#include <stdio.h>
#include <intrin.h>

#include "NvidiaOverlay.h"
//#include "..\Security\Security.h"

#include "..\ImGui\\imgui.h"
#include "..\ImGui\\imgui_internal.h"
#include "..\ImGui\\imgui_impl_dx9.h"
#include "..\ImGui\\imgui_impl_win32.h"

#include "..\\HackMain.h"

#include "..\\VMProtect\\VMProtectSDK.h"

#include "..\\Menu.h"

#include "..\\ESP.h"
//
//#include "..\\ESP.h"
//#include "..\\MiscVisuals.h"
//#include "..\\Radar.h"
//#include "..\\KeyAssigner.h"

//Externs
NvidiaOverlay eNvidiaOverlay;
WNDPROC NvidiaOverlay::oWndProc = nullptr;

NvidiaRenderer eRenderer;

const char * NvidiaOverlay::GetOverlayName()
{
	return XorString("NVIDIA GeForce Overlay");
}

bool NvidiaOverlay::PrepareOverlay(HWND OverlayHandle)
{
	/*const MARGINS margin = { -1. -1, -1, -1 };
	SetLayeredWindowAttributes(eNvidiaOverlay.m_hOverlayHandle, 0, RGB(0, 0, 0), LWA_COLORKEY);
	SetLayeredWindowAttributes(eNvidiaOverlay.m_hOverlayHandle, RGB(0, 0, 0), 180, LWA_ALPHA);
	DwmExtendFrameIntoClientArea(eNvidiaOverlay.m_hOverlayHandle, &margin);
	ShowWindow(eNvidiaOverlay.m_hOverlayHandle, SW_SHOW);*/

	VMProtectBeginMutation("Olay.Prepeare()");

	//This could get me banned :(
	SetWindowLongA(OverlayHandle, GWL_EXSTYLE, (int)GetWindowLong(OverlayHandle, GWL_EXSTYLE) | WS_EX_TRANSPARENT);

	if (SetLayeredWindowAttributes(OverlayHandle, 0, RGB(0, 0, 0), LWA_COLORKEY) == 0)
	{
		//printf("[-] Failed SettingWindowAttributes LWA_COLORKEY\n");
		return false;
	}
	//Removed otherwise we have to use transparent flag
	if (SetLayeredWindowAttributes(eNvidiaOverlay.m_hOverlayHandle, RGB(0, 0, 0), 255, LWA_ALPHA) == 0)
	{
		//printf("[-] Failed SettingWindowAttributes LWA_ALPHA\n");
		return false;
	}

	static const MARGINS margin = { -1, -1, -1, -1};
	if (FAILED(DwmExtendFrameIntoClientArea(OverlayHandle, &margin)))
	{
		//printf("[-] Failed DwmExtendFrameIntoClientArea\n");
		return false;
	}


	//ShowWindow(OverlayHandle, SW_SHOW);
	//printf("[+] Prepeared\n");
	VMProtectEnd();
	return true;
}

bool NvidiaOverlay::GetTargetHandle(HWND & hwnd)
{
	hwnd = FindWindowA(NULL, XorString("ARK: Survival Evolved"));
	//hwnd = FindWindowA(NULL, "*Untitled - Notepad");

	return hwnd != NULL ? true : false;
}

IMGUI_IMPL_API LRESULT  ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT NvidiaOverlay::hWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//if (uMsg == WM_KEYDOWN)
	//{
	//	if(KeyAssigner::GetInstance())
	//		KeyAssigner::GetInstance()->WndProcUpdate(uMsg, wParam);
	//	//printf("[+] KeyPress -> %i\n", (int)wParam);
	//}
	//else if (uMsg == WM_SETCURSOR && eHackMain.mouse_cursor_locked)
	//{
	//	//printf("[+] Cursor message handled\n");
	//	SetClassLongPtr(hwnd, GCLP_HCURSOR, NULL);
	//	SetCursor(NULL);
	//}
	if (eMenu.m_IsOpen)
	{
		//if(!KeyAssigner::GetInstance()->m_IsWaiting)
		ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
		//return 0l;
	}
	//return DefWindowProc(hwnd, uMsg, wParam, lParam);
	return oWndProc(hwnd, uMsg, wParam, lParam);
}

bool NvidiaOverlay::GetOverlayHandle(HWND & hwnd)
{
	//eRenderer.DbgPrint("[?] name -> '%s'\n", this->GetOverlayName());

	hwnd = FindWindowA(XorString("CEF-OSC-WIDGET"), this->GetOverlayName());

	return hwnd != NULL ? true : false;
}

bool NvidiaOverlay::HookWndProc()
{
	//\x40\x53\x55\x56\x57\x41\x56\x41 xxxxxxxx

	aWndProc = (LPVOID)GetWindowLongPtrW(this->m_hOverlayHandle, GWLP_WNDPROC);

	if (MH_Initialize() != MH_OK)
		return false;

	if (MH_CreateHook(aWndProc, hWindowProc, ((LPVOID*)&oWndProc)) != MH_OK)
		return false;
	
	if (MH_EnableHook(aWndProc) != MH_OK)
		return false;

	//system("pause");

	//oWndProc = (WNDPROC)SetWindowLongPtr(eNvidiaOverlay.m_hOverlayHandle, GWLP_WNDPROC, (LONG_PTR)hWindowProc);

	return true;
}

NvidiaOverlay::~NvidiaOverlay()
{
	if (m_Initialized)
	{
		MH_DisableHook(aWndProc);
		MH_Uninitialize();
	}
}

bool NvidiaOverlay::Initialize()
{
	VMProtectBeginMutation("Olay.init()");

	if (!GetOverlayHandle(m_hOverlayHandle))
	{
		eRenderer.DbgPrint(XorString("[-] Failed fetching o hanlde\n"));
		return false;
	}

	if (!HookWndProc())
	{
		eRenderer.DbgPrint(XorString("[-] Failed hooking input\n"));
		return false;
	}

	//printf("[?] Overlay located at -> 0x%lX\n", m_hOverlayHandle);

	RECT WindowRect;
	if (GetWindowRect(m_hOverlayHandle, &WindowRect) == 0)
	{
		eRenderer.DbgPrint(XorString("[-] Failed fetching windows res -> 0x%lx\n"), GetLastError());
		return false;
	}

	eRenderer.DbgPrint(XorString("[?] w -> %i h -> %i\n"), WindowRect.right, WindowRect.bottom);

	m_Width = WindowRect.right;
	m_Height = WindowRect.bottom;


	if (!eRenderer.Initialize(m_hOverlayHandle))
		return false;		

	m_Initialized = eMenu.Initialize();

	VMProtectEnd();

	return m_Initialized;
}

void NvidiaRenderer::RenderingThread()
{

	while (eRenderer.m_ShouldRender)
	{
		if(eRenderer.m_Attached) // this is to avoid using topmost thingy
			SetWindowPos(eNvidiaOverlay.m_TargetHandle, eNvidiaOverlay.m_hOverlayHandle, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

		//if (!eMenu.m_IsOpen)
		//	eRenderer.cut_cursor_hole();

		eRenderer.Render();
		Sleep(eRenderer.m_UpdateInterval);
	}
	eRenderer.m_ShouldRender = true;
	return;
}

void NvidiaRenderer::RenderDebugText()
{
	if (m_DebugIndex <= 0)
		return;

	if (!this->m_DrawInitialized)
		return;

	const int x = 1;
	const int y = 1;
	RECT rect;
	SetRect(&rect, x - 1, y, x - 1, y);
	pFont[1]->DrawTextA(NULL, m_DebugInfo.c_str(), -1, &rect, DT_LEFT | DT_NOCLIP, BLACK(255));
	SetRect(&rect, x + 1, y, x + 1, y);
	pFont[1]->DrawTextA(NULL, m_DebugInfo.c_str(), -1, &rect, DT_LEFT | DT_NOCLIP, BLACK(255));
	SetRect(&rect, x, y - 1, x, y - 1);
	pFont[1]->DrawTextA(NULL, m_DebugInfo.c_str(), -1, &rect, DT_LEFT | DT_NOCLIP, BLACK(255));
	SetRect(&rect, x, y + 1, x, y + 1);
	pFont[1]->DrawTextA(NULL, m_DebugInfo.c_str(), -1, &rect, DT_LEFT | DT_NOCLIP, BLACK(255));
	pFont[1]->DrawTextA(NULL, m_DebugInfo.c_str(), -1, &rect, DT_LEFT | DT_NOCLIP, WHITE(255));
}



NvidiaRenderer::~NvidiaRenderer()
{
	m_Line->Release();
	ExitRenderThread();
}

bool NvidiaRenderer::Initialize(HWND OverlayHandle)
{
	/*HRESULT Ret;
	if (FAILED( m_pObject = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		printf("[-] DDD failed\n");
		return false;
	}*/

	VMProtectBeginMutation("Renderer.Init");

	m_pObject = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pObject == nullptr)
	{
		eRenderer.DbgPrint(XorString("[-] DDD failed\n"));
		return false;
	}

	//D3DPRESENT_PARAMETERS m_d3dpp;
	RtlZeroMemory(&m_d3dpp, sizeof(m_d3dpp));
	m_d3dpp.Windowed = TRUE;
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_d3dpp.hDeviceWindow = OverlayHandle;
	m_d3dpp.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	m_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	m_d3dpp.BackBufferWidth = eNvidiaOverlay.m_Width;
	m_d3dpp.BackBufferHeight = eNvidiaOverlay.m_Height;
	m_d3dpp.EnableAutoDepthStencil = TRUE;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	//Vsync
	//Params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	//Params.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	
	//Ret = m_pObject->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, OverlayHandle, D3DCREATE_HARDWARE_VERTEXPROCESSING, &Params, 0, &m_pDevice);
	HRESULT Ret = m_pObject->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, OverlayHandle, D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_d3dpp, &m_pDevice);
	if (FAILED(Ret))
	{
		eRenderer.DbgPrint(XorString("[-] Deviso failed -> 0x%lX\n"), Ret);
		return false;
	}
	
	//printf("[+] Directx9 Device -> 0x%llX\n",m_pDevice);

	VMProtectEnd();

	return true;
}

bool NvidiaRenderer::Attach()
{
	VMProtectBeginMutation("NvidiaRender.Attach");
	if (eNvidiaOverlay.m_TargetHandle == NULL)
		eNvidiaOverlay.GetTargetHandle(eNvidiaOverlay.m_TargetHandle);


	if (eNvidiaOverlay.m_TargetHandle == NULL)
		return false;

	RECT buffer = {NULL};
	if (GetWindowRect(eNvidiaOverlay.m_TargetHandle, &buffer) == 0)
		return false;

	//printf("[?] %i, %i\n", buffer.right, buffer.bottom);

	m_AttachedOffset.x = buffer.left;
	m_Width = buffer.right;
	m_AttachedOffset.y = buffer.top;
	m_Height = buffer.bottom;
	DWORD dwStyle = GetWindowLongA(eNvidiaOverlay.m_TargetHandle, GWL_STYLE);
	if (dwStyle & WS_BORDER)
	{
		m_AttachedOffset.y += 23;
		m_Height -= 23;
	}

	m_Attached = true;

	VMProtectEnd();

	return m_Attached;
}

int Animation = 0;
bool NvidiaRenderer::CreateRenderThread()
{
	m_hRenderThread = Utils::CreateSpoofedThread((LPTHREAD_START_ROUTINE)RenderingThread, NULL, (LPTHREAD_START_ROUTINE)((uintptr_t)GetModuleHandle(XorString("NVIDIA Share.exe")) + rand() % 0x7F000 + 0x1200));

	if (m_hRenderThread != NULL)
		return true;
	else
		return false;
}
bool NvidiaRenderer::ExitRenderThread()
{
	if (m_hRenderThread == NULL)
		return true;
	m_ShouldRender = false;
	//Utils::WaitForEvent(m_ShouldRender, true); // Thread will reset it to true before exiting
	TerminateThread(m_hRenderThread,1);
	CloseHandle(m_hRenderThread);
	m_hRenderThread = NULL;
	return true;
}
bool NvidiaRenderer::UpdateOffsets()
{
	if (m_Attached == false)
		return false;
	RECT buffer = { NULL };
	if (GetWindowRect(eNvidiaOverlay.m_TargetHandle, &buffer) == 0)
	{
		m_Attached = false;
		return false;
	}

	m_AttachedOffset.x = buffer.left;
	m_Width = buffer.right;
	m_AttachedOffset.y = buffer.top;
	m_Height = buffer.bottom;
	DWORD dwStyle = GetWindowLong(eNvidiaOverlay.m_TargetHandle, GWL_STYLE);
	if (dwStyle & WS_BORDER)
	{
		m_AttachedOffset.y += 23;
		m_Height -= 23;
	}

	return m_Attached;
}
//IDirect3DTexture9 * pText = nullptr; LPD3DXSPRITE pSprite = nullptr;
void DrawTexture(int x, int y, LPDIRECT3DTEXTURE9 dTexture, LPD3DXSPRITE sprite)
{
	D3DXVECTOR3 buffer = { (float)x, (float)y, 0.0f };

	sprite->Begin(D3DXSPRITE_ALPHABLEND);
	sprite->Draw(dTexture, NULL, NULL, &buffer, 0xFFFFFFFF);
	sprite->End();
	return;
}

void NvidiaRenderer::Render()
{
	if (!eNvidiaOverlay.m_Initialized)
		return;

	m_pDevice->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);
	m_pDevice->BeginScene();

	VMProtectBeginMutation("NvidiaRenderer.Render");

	if (!m_DrawInitialized)
	{
		eRenderer.UpdateAntiAliasing(true);
		eRenderer.AddFont(XorString("Candara"), FONT_ESP_SIZE, false, false);//Bahnschrift
		eRenderer.AddFont(XorString("Candara"), 10, false, false);
		
		//MiscVisuals::GetInstance()->Initialize();

		//Sprite
		//if (!SUCCEEDED(D3DXCreateSprite(m_pDevice, &pWarningIconSprite)))
		//{
			//("Failed creating sprite!\n");
			//system("pause");
		//}

		this->DbgClear();
		m_DrawInitialized = true;
	}

	eRenderer.BeginScene();
	////{
		//eRenderer.FilledBox(0, 0, 1920, 1080, WHITE(50));
	eRenderer.Text(0, 16, WHITE(255), lefted, 0, true, "YoGui hook");
	////
		eESP.Render();
	////
	////	eMiscVisuals.Render();
	////
	////	this->RenderDebugText();
	////}
	eRenderer.EndScene();
	//
	eMenu.BeginImGuiScene();
	//{
	//	eRadar.Render();
		eMenu.Render();
	//}
	eMenu.EndImGuiScene();

	VMProtectEnd();

	m_pDevice->EndScene();
	HRESULT result =  m_pDevice->Present(0, 0, 0, 0);

	// Handle loss of D3D9 device
	if (result == D3DERR_DEVICELOST && m_pDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		eMenu.ResetDevice();
}

void NvidiaRenderer::SetFocusOnWindow(HWND Window)
{
	HWND hCurrentWindow = NULL;
	do 
	{
		hCurrentWindow = GetForegroundWindow();

		if (hCurrentWindow == Window)
			break;

		PostMessage(hCurrentWindow, WM_SYSCOMMAND, SC_MINIMIZE, 0);

	} while (hCurrentWindow != NULL);
}

void NvidiaRenderer::cut_cursor_hole(bool under_cursor)
{
	const int CLIP_SIZE = 4;

	static bool initialized = false;

	static RECT rect1;
	static HRGN hRgn;


	if (!initialized)
	{
		SetCursor(NULL);

		RECT window_rect;
		GetClientRect(eNvidiaOverlay.m_hOverlayHandle, &window_rect);

		m_oWidth = window_rect.right;
		m_oHeight = window_rect.bottom;

		rect1 = { 0,0,m_oWidth,m_oHeight };
		hRgn = CreateRectRgnIndirect(&rect1);
		initialized = true;
	}

	static POINT cursor_pos;
	GetCursorPos(&cursor_pos);

	RECT rect;

	if(under_cursor)
		rect = { cursor_pos.x - CLIP_SIZE,cursor_pos.y - CLIP_SIZE,cursor_pos.x + CLIP_SIZE,cursor_pos.y + CLIP_SIZE };
	else
		rect = { 0, m_oHeight, 15, m_oHeight - 15 };

	HRGN hRgn2 = CreateRectRgnIndirect(&rect);

	HRGN combined_Rgn = CreateRectRgnIndirect(&rect1);
	CombineRgn(combined_Rgn, hRgn, hRgn2, RGN_XOR);

	SetWindowRgn(eNvidiaOverlay.m_hOverlayHandle, combined_Rgn, TRUE);

	DeleteObject(hRgn2);
	DeleteObject(combined_Rgn);
}

void NvidiaRenderer::DbgPrint(const char * string, ...)
{
	if (std::string(string).length() > 700)
		return;

	
	va_list args;
	char cBuffer[700];

	va_start(args, string);
	vsprintf_s(cBuffer, string, args);
	va_end(args);

	m_DebugInfo += cBuffer;

	m_DebugIndex++;

}

void NvidiaRenderer::DbgClear()
{
	m_DebugIndex = 0;
	m_DebugInfo = "";
}

bool NvidiaRenderer::DrawRect(const int & x, const int & y, const int & width, const int & height, const DWORD & Color)
{
	D3DRECT BarRect = { this->m_AttachedOffset.x + x,this->m_AttachedOffset.y + y, (this->m_AttachedOffset.x + x) + width, (this->m_AttachedOffset.y + y) + height };

	m_pDevice->Clear(1, &BarRect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, Color, 0, 0);

	return true;
}

void NvidiaRenderer::BeginScene()
{
	if(m_pDevice)
		m_pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
}

void NvidiaRenderer::EndScene()
{
	//m_pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
}

void NvidiaRenderer::UpdateAntiAliasing(const bool & status)
{
	m_pDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ANTIALIASEDLINEENABLE, status);
	m_pDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_MULTISAMPLEANTIALIAS, status);
}

void NvidiaRenderer::AddFont(const char * Caption, float size, bool bold, bool italic)
{
	D3DXCreateFont(m_pDevice, size, 0, (bold) ? FW_BOLD : FW_NORMAL, 1, (italic) ? 1 : 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, Caption, &pFont[m_FontNr]);
	m_FontNr++;
}

void NvidiaRenderer::Text(int x, int y, DWORD color, text_alignment alignment, int font, bool outlined, const char* string, ...)
{

	if (font >= MAX_FONTS)
		return;

	if (pFont[font] == nullptr)
		return;

	va_list args;
	char cBuffer[256];

	va_start(args, string);
	vsprintf_s(cBuffer, string, args);
	va_end(args);

	if (eRenderer.m_Attached)
	{
		x += eRenderer.m_AttachedOffset.x;
		y += eRenderer.m_AttachedOffset.y;
	}
	
	int OriginalAlpha = (color >> 24) & 0xFF;

	RECT rect;
	switch (alignment)
	{
	case lefted:
		if (outlined)
		{
			SetRect(&rect, x - 1, y, x - 1, y);
			pFont[font]->DrawTextA(NULL, cBuffer, -1, &rect, DT_LEFT | DT_NOCLIP, BLACK(OriginalAlpha));
			SetRect(&rect, x + 1, y, x + 1, y);											
			pFont[font]->DrawTextA(NULL, cBuffer, -1, &rect, DT_LEFT | DT_NOCLIP, BLACK(OriginalAlpha));
			SetRect(&rect, x, y - 1, x, y - 1);											
			pFont[font]->DrawTextA(NULL, cBuffer, -1, &rect, DT_LEFT | DT_NOCLIP, BLACK(OriginalAlpha));
			SetRect(&rect, x, y + 1, x, y + 1);											
			pFont[font]->DrawTextA(NULL, cBuffer, -1, &rect, DT_LEFT | DT_NOCLIP, BLACK(OriginalAlpha));
		}
		SetRect(&rect, x, y, x, y);
		pFont[font]->DrawTextA(NULL, cBuffer, -1, &rect, DT_LEFT | DT_NOCLIP, color);
		break;
	case centered:
		if (outlined)
		{
			/*SetRect(&rect, x - 1, y, x - 1, y);
			pFont[font]->DrawTextA(NULL, cBuffer, -1, &rect, DT_CENTER | DT_NOCLIP, BLACK(OriginalAlpha));
			SetRect(&rect, x + 1, y, x + 1, y);
			pFont[font]->DrawTextA(NULL, cBuffer, -1, &rect, DT_CENTER | DT_NOCLIP, BLACK(OriginalAlpha));
			SetRect(&rect, x, y - 1, x, y - 1);
			pFont[font]->DrawTextA(NULL, cBuffer, -1, &rect, DT_CENTER | DT_NOCLIP, BLACK(OriginalAlpha));
			SetRect(&rect, x, y + 1, x, y + 1);
			pFont[font]->DrawTextA(NULL, cBuffer, -1, &rect, DT_CENTER | DT_NOCLIP, BLACK(OriginalAlpha));*/
			SetRect(&rect, x - 1, y - 1, x, y);
			pFont[font]->DrawTextA(NULL, cBuffer, -1, &rect, DT_CENTER | DT_NOCLIP, BLACK(OriginalAlpha));
			SetRect(&rect, x + 1, y + 1, x, y);
			pFont[font]->DrawTextA(NULL, cBuffer, -1, &rect, DT_CENTER | DT_NOCLIP, BLACK(OriginalAlpha));
			SetRect(&rect, x, y, x, y);
			pFont[font]->DrawTextA(NULL, cBuffer, -1, &rect, DT_CENTER | DT_NOCLIP, BLACK(OriginalAlpha));
			
		}
		SetRect(&rect, x, y, x, y);
		pFont[font]->DrawTextA(NULL, cBuffer, -1, &rect, DT_CENTER | DT_NOCLIP, color);
		break;
	case righted:
		if (outlined)
		{
			SetRect(&rect, x - 1, y, x - 1, y);
			pFont[font]->DrawTextA(NULL, cBuffer, -1, &rect, DT_RIGHT | DT_NOCLIP, BLACK(OriginalAlpha));
			SetRect(&rect, x + 1, y, x + 1, y);
			pFont[font]->DrawTextA(NULL, cBuffer, -1, &rect, DT_RIGHT | DT_NOCLIP, BLACK(OriginalAlpha));
			SetRect(&rect, x, y - 1, x, y - 1);
			pFont[font]->DrawTextA(NULL, cBuffer, -1, &rect, DT_RIGHT | DT_NOCLIP, BLACK(OriginalAlpha));
			SetRect(&rect, x, y + 1, x, y + 1);
			pFont[font]->DrawTextA(NULL, cBuffer, -1, &rect, DT_RIGHT | DT_NOCLIP, BLACK(OriginalAlpha));
		}
		SetRect(&rect, x, y, x, y);
		pFont[font]->DrawTextA(NULL, cBuffer, -1, &rect, DT_RIGHT | DT_NOCLIP, color);
		break;
	default:
		break;
	}

}
void NvidiaRenderer::TextUnicode(int x, int y, DWORD color, text_alignment alignment, int font, bool outlined, wchar_t * string, ...)
{
	va_list args;
	wchar_t cBuffer[256];

	if (!pFont[font])
		return ;

	va_start(args, string);
	vswprintf_s(cBuffer, string, args);
	va_end(args);

	if (eRenderer.m_Attached)
	{
		x += eRenderer.m_AttachedOffset.x;
		y += eRenderer.m_AttachedOffset.y;
	}

	RECT rect;
	switch (alignment)
	{
	case lefted:
		if (outlined)
		{
			SetRect(&rect, x - 1, y, x - 1, y);
			pFont[font]->DrawTextW(NULL, cBuffer, -1, &rect, DT_LEFT | DT_NOCLIP, BLACK(255));
			SetRect(&rect, x + 1, y, x + 1, y);
			pFont[font]->DrawTextW(NULL, cBuffer, -1, &rect, DT_LEFT | DT_NOCLIP, BLACK(255));
			SetRect(&rect, x, y - 1, x, y - 1);
			pFont[font]->DrawTextW(NULL, cBuffer, -1, &rect, DT_LEFT | DT_NOCLIP, BLACK(255));
			SetRect(&rect, x, y + 1, x, y + 1);
			pFont[font]->DrawTextW(NULL, cBuffer, -1, &rect, DT_LEFT | DT_NOCLIP, BLACK(255));
		}
		SetRect(&rect, x, y, x, y);
		pFont[font]->DrawTextW(NULL, cBuffer, -1, &rect, DT_LEFT | DT_NOCLIP, color);
		break;
	case centered:
		if (outlined)
		{
			SetRect(&rect, x - 1, y, x - 1, y);
			pFont[font]->DrawTextW(NULL, cBuffer, -1, &rect, DT_CENTER | DT_NOCLIP, BLACK(255));
			SetRect(&rect, x + 1, y, x + 1, y);
			pFont[font]->DrawTextW(NULL, cBuffer, -1, &rect, DT_CENTER | DT_NOCLIP, BLACK(255));
			SetRect(&rect, x, y - 1, x, y - 1);
			pFont[font]->DrawTextW(NULL, cBuffer, -1, &rect, DT_CENTER | DT_NOCLIP, BLACK(255));
			SetRect(&rect, x, y + 1, x, y + 1);
			pFont[font]->DrawTextW(NULL, cBuffer, -1, &rect, DT_CENTER | DT_NOCLIP, BLACK(255));
		}
		SetRect(&rect, x, y, x, y);
		pFont[font]->DrawTextW(NULL, cBuffer, -1, &rect, DT_CENTER | DT_NOCLIP, color);
		break;
	case righted:
		if (outlined)
		{
			SetRect(&rect, x - 1, y, x - 1, y);
			pFont[font]->DrawTextW(NULL, cBuffer, -1, &rect, DT_RIGHT | DT_NOCLIP, BLACK(255));
			SetRect(&rect, x + 1, y, x + 1, y);
			pFont[font]->DrawTextW(NULL, cBuffer, -1, &rect, DT_RIGHT | DT_NOCLIP, BLACK(255));
			SetRect(&rect, x, y - 1, x, y - 1);
			pFont[font]->DrawTextW(NULL, cBuffer, -1, &rect, DT_RIGHT | DT_NOCLIP, BLACK(255));
			SetRect(&rect, x, y + 1, x, y + 1);
			pFont[font]->DrawTextW(NULL, cBuffer, -1, &rect, DT_RIGHT | DT_NOCLIP, BLACK(255));
		}
		SetRect(&rect, x, y, x, y);
		pFont[font]->DrawTextW(NULL, cBuffer, -1, &rect, DT_RIGHT | DT_NOCLIP, color);
		break;
	default:
		break;
	}
}
int NvidiaRenderer::StringWidth(ID3DXFont* font, char* string) {
	RECT pRect = RECT();
	font->DrawText(NULL, string, strlen(string), &pRect, DT_CALCRECT, D3DCOLOR_RGBA(0, 0, 0, 0));
	return pRect.right - pRect.left;
}
void NvidiaRenderer::Line(int x, int y,int x2, int y2, const float & width, const DWORD & color) {
	if (eRenderer.m_Attached)
	{
		x += eRenderer.m_AttachedOffset.x;
		y += eRenderer.m_AttachedOffset.y;
		x2 += eRenderer.m_AttachedOffset.x;
		y2 += eRenderer.m_AttachedOffset.y;
	}
	SD3DVertex pVertex[2] = { { x, y, 0.0f, width, color }, { x2, y2, 0.0f, width, color } };
	m_pDevice->SetTexture(0, NULL);
	m_pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, &pVertex, sizeof(SD3DVertex));
}
void NvidiaRenderer::ThickLine(int x, int y, int x2, int y2, const float & width, const DWORD & color, const bool & AntiAlias)
{
	if(m_Line == nullptr)
		D3DXCreateLine(m_pDevice, &m_Line);

	if (eRenderer.m_Attached)
	{
		x += eRenderer.m_AttachedOffset.x;
		y += eRenderer.m_AttachedOffset.y;
		x2 += eRenderer.m_AttachedOffset.x;
		y2 += eRenderer.m_AttachedOffset.y;
	}

	D3DXVECTOR2 line[] = { D3DXVECTOR2(x, y), D3DXVECTOR2(x2, y2) };
	m_Line->SetWidth(width);
	if (AntiAlias)
		m_Line->SetAntialias(AntiAlias);
	m_Line->Begin();
	m_Line->Draw(line, 2, color);
	m_Line->End();
	//m_Line->Release();
}
void NvidiaRenderer::FilledBox(int x, int y, int width, int height, DWORD color) {
	if (x < 0 || y < 0)
		return;
	if (eRenderer.m_Attached)
	{
		x += eRenderer.m_AttachedOffset.x;
		y += eRenderer.m_AttachedOffset.y;
	}
	SD3DVertex pVertex[4] = { { x, y + height, 0.0f, 1.0f, color }, { x, y, 0.0f, 1.0f, color }, { x + width, y + height, 0.0f, 1.0f, color }, { x + width, y, 0.0f, 1.0f, color } };

	m_pDevice->SetTexture(0, NULL);
	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, pVertex, sizeof(SD3DVertex));
}
void NvidiaRenderer::FilledBoxOutlined(int x, int y, int width, int height, int thickness, DWORD color, DWORD outlinecolor) {

	this->BorderedBox(x, y, width, height, thickness, outlinecolor);
	this->FilledBox(x + thickness, y + thickness, width - (thickness * 2), height - (thickness * 2), color);
}
void NvidiaRenderer::BorderedBox(int x, int y, int width, int height, int thickness, DWORD color) {
	
	if (x < 0 || y < 0)
		return;
	//int offset = thickness / 2;

	this->FilledBox(x, y, width, thickness, color);
	this->FilledBox(x, y, thickness, height, color);
	this->FilledBox(x + width - thickness, y, thickness, height, color);
	this->FilledBox(x, y + height - thickness, width, thickness, color);
}
void NvidiaRenderer::CornerBox(int x, int y, const int & width, const int & height, const int & linesize, const int & thickness, const DWORD & color)
{
	if (linesize == NULL)
		return;

	int LineSize = width / linesize;

	//Top Left Corner
	eRenderer.Line(x, y, x + LineSize, y, thickness, color); // -
	eRenderer.Line(x, y, x, y + LineSize, thickness, color); // |
	//Top Right Corner
	eRenderer.Line(x + width - LineSize, y, x + width, y, thickness, color); // -
	eRenderer.Line(x + width, y, x + width, y + LineSize, thickness, color); // |
	//Bottom Left Corner
	eRenderer.Line(x, y + height, x + LineSize, y + height, thickness, color); // -
	eRenderer.Line(x, y + height, x, y + height - LineSize, thickness, color); // |
	//Bottom Right Corner
	eRenderer.Line(x + width - LineSize, y + height, x + width, y + height, thickness, color); // -
	eRenderer.Line(x + width, y + height, x + width, y  + height - LineSize, thickness, color); // |

}
void NvidiaRenderer::CornerBoxOutlined(int x, int y,int & width, int & height, const int & linesize, const int & thickness, const DWORD & color, const DWORD & outlinecolor)
{
	this->CornerBox(x - 1, y - 1, width + 2, height + 2, linesize, 1, outlinecolor);
	this->CornerBox(x + 1, y + 1, width - 2, height - 2, linesize, 1, outlinecolor);
	this->CornerBox(x, y, width, height, linesize, thickness, color);
}
void NvidiaRenderer::BorderedBoxOutlined(int x, int y, const int & width, const int & height, const int & thickness, const DWORD & color, const DWORD & outlinecolor) {
	
	this->BorderedBox(x-1, y-1, width+2, height+2, thickness, outlinecolor);
	this->BorderedBox(x+1, y+1, width-2, height-2, thickness, outlinecolor);
	this->BorderedBox(x, y, width, height, thickness, color);

	//this->BorderedBox(x, y, width, height, thickness, outlinecolor);
	//this->BorderedBox(x + thickness, y + thickness, width - (thickness * 2), height - (thickness * 2), thickness, color);
	//this->BorderedBox(x + (thickness * 2), y + (thickness * 2), width - (thickness * 4), height - (thickness * 4), thickness, outlinecolor);
}
void NvidiaRenderer::GradientBox(int x, int y, int width, int height, DWORD color, DWORD color2, bool vertical) {
	if (eRenderer.m_Attached)
	{
		x += eRenderer.m_AttachedOffset.x;
		y += eRenderer.m_AttachedOffset.y;
	}
	SD3DVertex pVertex[4] = { { x, y, 0.0f, 1.0f, color }, { x + width, y, 0.0f, 1.0f, vertical ? color : color2 }, { x, y + height, 0.0f, 1.0f, vertical ? color2 : color }, { x + width, y + height, 0.0f, 1.0f, color2 } };
	//pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	m_pDevice->SetTexture(0, NULL);
	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, pVertex, sizeof(SD3DVertex));
}
void NvidiaRenderer::GradientBoxOutlined(int x, int y, int width, int height, int thickness, DWORD color, DWORD color2, DWORD outlinecolor, bool vertical) {

	this->BorderedBox(x, y, width, height, thickness, outlinecolor);
	this->GradientBox(x + thickness, y + thickness, width - (thickness * 2), height - (thickness * 2), color, color2, vertical);
}
void NvidiaRenderer::Circle(int x, int y, int radius, int points, DWORD color) {
	if (eRenderer.m_Attached)
	{
		x += eRenderer.m_AttachedOffset.x;
		y += eRenderer.m_AttachedOffset.y;
	}
	SD3DVertex* pVertex = new SD3DVertex[points + 1];
	for (int i = 0; i <= points; i++) pVertex[i] = { x + radius * (float)cos(D3DX_PI * (i / (points / 2.0f))), y - radius * (float)sin(D3DX_PI * (i / (points / 2.0f))), 0.0f, 1.0f, color };
	//pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	m_pDevice->SetTexture(0, NULL);
	m_pDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, points, pVertex, sizeof(SD3DVertex));
	delete[] pVertex;
}
void NvidiaRenderer::FilledCircle(int x, int y, int radius, int points, DWORD color) {
	if (eRenderer.m_Attached)
	{
		x += eRenderer.m_AttachedOffset.x;
		y += eRenderer.m_AttachedOffset.y;
	}
	SD3DVertex* pVertex = new SD3DVertex[points + 1];
	for (int i = 0; i <= points; i++) pVertex[i] = { x + radius * (float)cos(D3DX_PI * (i / (points / 2.0f))), y + radius * (float)sin(D3DX_PI * (i / (points / 2.0f))), 0.0f, 1.0f, color };
	//pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	m_pDevice->SetTexture(0, NULL);
	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, points, pVertex, sizeof(SD3DVertex));
	delete[] pVertex;
}
bool NvidiaRenderer::LoadTexture(char * Path, IDirect3DTexture9 ** texture)
{
	if (m_pDevice == nullptr)
		return false;
	if (D3DXCreateTextureFromFile(m_pDevice, Path, texture) == D3D_OK)
		return true;
	else
		return false;
}

bool NvidiaRenderer::LoadTextureFromMemory(const BYTE * pFile, DWORD FileSize, IDirect3DTexture9 ** texture)
{
	if (m_pDevice == nullptr)
		return false;
	if (D3DXCreateTextureFromFileInMemory(m_pDevice, pFile, FileSize, texture) == D3D_OK)
		return true;
	else
		return false;
}

Vec2 NvidiaRenderer::GetTextRect(int Font, const char * Text, ...)
{
	if (m_FontNr < Font)
		return Vec2(0, 0);

	if(!pFont[Font])
		return Vec2(0, 0);

	va_list args;
	char cBuffer[256];

	va_start(args, Text);
	vsprintf_s(cBuffer, Text, args);
	va_end(args);

	RECT rcRect = { 0,0,0,0 };
	pFont[Font]->DrawText(NULL, cBuffer, strlen(cBuffer), &rcRect, DT_CALCRECT,
		D3DCOLOR_XRGB(0, 0, 0));

	return Vec2(rcRect.right - rcRect.left, rcRect.bottom - rcRect.top);
}

