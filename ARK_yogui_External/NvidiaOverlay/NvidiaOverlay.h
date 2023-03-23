#pragma once
#include <Windows.h>
#include <iostream>
//#include "..\Main.h" 
#include "..\skCrypt.h"
#include "..\DirectXIncludes.h"
#include "..\Utils.h"
//#include "Menu.h"
#include "Security.h"

#include "..\MinHook\MinHook.h"
#pragma comment(lib, "libMinHook-x64-v141-mtd.lib")


#define MAX_FONTS 12

class NvidiaOverlay;
extern NvidiaOverlay eNvidiaOverlay;

class NvidiaRenderer;
extern NvidiaRenderer eRenderer;

class NvidiaOverlay
{
private:
	//const int OVERLAY_NAME_COUNT = 3;
	//const char * eOVERLAY_NAME[3] = {"NVJEKC", "GeGpteh", "Ovfsnc|"};
public:
//Window Information
	bool m_Initialized = false;
	HWND m_hOverlayHandle = NULL;
	HWND m_TargetHandle = NULL;
	int m_Width = NULL;
	int m_Height = NULL;

	LPVOID aWndProc = nullptr; // Windows Proc original function
	static WNDPROC oWndProc;	// After the hook original
private:
	static LRESULT CALLBACK hWindowProc(_In_ HWND   hwnd, _In_ UINT   uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
	const char * GetOverlayName();
	bool GetOverlayHandle(HWND & hwnd);
	bool HookWndProc();
public:
	~NvidiaOverlay();
	NvidiaOverlay * GetInstance() { return &eNvidiaOverlay; }
	bool Initialize();
	bool PrepareOverlay(HWND OverlayHandle);	//Call before rendering

	bool GetTargetHandle(HWND & hwnd);
};


enum text_alignment { lefted, centered, righted };

enum FontIDS : const int
{
	FontIDS_MainFont = 0,
	FontIDS_MainFontSmall = 1
};

struct SD3DVertex {
	float x, y, z, rhw;
	DWORD colour;
};

#define FONT_ESP_SIZE 14

class NvidiaRenderer
{
private:
//DirectX
	IDirect3D9* m_pObject = nullptr;
	IDirect3DDevice9 * m_pDevice = nullptr;
	ID3DXLine *m_Line = nullptr;
//DirectX For Rendering
	bool m_DrawInitialized = false;
	std::string m_DebugInfo = "";
	int m_DebugIndex = 0;
	int m_FontNr = 0;
	ID3DXFont *pFont[MAX_FONTS];
//Others
	POINT m_AttachedOffset = { 0, 0 };
//Render Thread
	HANDLE m_hRenderThread = NULL;
	bool m_ShouldRender = true;
	int m_UpdateInterval = 1;
private:
	static void RenderingThread();
	void RenderDebugText();
public:
	bool m_Attached = false;
	int m_Width, m_Height;

	int m_oWidth, m_oHeight;

	ID3DXSprite * pWarningIconSprite = nullptr;
	D3DPRESENT_PARAMETERS m_d3dpp;
public:
	~NvidiaRenderer();
	bool Initialize(HWND OverlayHandle);
	bool Attach();

	inline IDirect3DDevice9* GetDevice() { return m_pDevice; }

// Starts and Stops rendering
	bool CreateRenderThread();
	bool ExitRenderThread();
//Update attached window position
	bool UpdateOffsets();

	void Render();

	void SetFocusOnWindow(HWND Window);

	void cut_cursor_hole(bool under_cursor);

//Debugging
	void DbgPrint(const char * string, ...);
	void DbgClear();

//Rendering
	void BeginScene();
	void EndScene();
	void UpdateAntiAliasing(const bool & status);
	void AddFont(const char* Caption, float size, bool bold, bool italic);
	void Text(int x, int y, DWORD color, text_alignment alignment, int font, bool outlined, const char* string, ...);
	void TextUnicode(int x, int y, DWORD color, text_alignment alignment, int font, bool outlined, wchar_t* string, ...);
	int StringWidth(ID3DXFont* font, char* string);
	void Line(int x,int y,int x2, int y2, const float & width, const DWORD & color);
	void ThickLine(int x, int y, int x2, int y2, const float & width, const DWORD & color, const bool & AntiAlias);
	void FilledBox(int x, int y, int width, int height, DWORD color);
	void FilledBoxOutlined(int x, int y, int width, int height, int thickness, DWORD color, DWORD outlinecolor);
	void BorderedBox(int x, int y, int width, int height, int thickness, DWORD color);
	void CornerBox(int  x, int  y, const int & width, const int & height, const int & linesize, const int & thickness, const DWORD & color);
	void CornerBoxOutlined(int x, int y, int & width, int & height, const int & linesize, const int & thickness, const DWORD & color, const DWORD & outlinecolor);
	void BorderedBoxOutlined(int x, int y, const int & width,const int & height,const int & thickness,const DWORD & color,const DWORD & outlinecolor);
	void GradientBox(int x, int y, int width, int height, DWORD color, DWORD color2, bool vertical);
	void GradientBoxOutlined(int x, int y, int width, int height, int thickness, DWORD color, DWORD color2, DWORD outlinecolor, bool vertical);
	void Circle(int x, int y, int radius, int points, DWORD color);
	void FilledCircle(int x, int y, int radius, int points, DWORD color);
	bool LoadTexture(char * Path, IDirect3DTexture9 ** texture);
	bool LoadTextureFromMemory(const BYTE * pFile, DWORD FileSize, IDirect3DTexture9 ** texture);
	int FrameRate();
	Vec2 GetTextRect(int Font, const char * Text, ...);

	//void Begin();
	//void End();

	bool DrawRect(const int & x, const int & y, const int & width, const int & height, const DWORD & Color);
};
