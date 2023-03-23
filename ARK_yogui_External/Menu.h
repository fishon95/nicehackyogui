#pragma once
#include "DirectXIncludes.h"

//ImGui Includes
#include "ImGui//imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui//imgui_impl_dx9.h"
#include "ImGui//imgui_impl_win32.h"

namespace Features
{
	struct sAimbotFeatures
	{
		struct sAimbotMainSettings
		{
			float FieldOfView = 90.0f;
			float Smooth = 1.0f;

			bool LockTarget = false;
			
			bool DrawFOV = false;

			DWORD Color_FOV = WHITE(255);

			int AimKey = 70; // key F
		};

		struct sAimbotPlayerSettings
		{
			enum eAimSpots : const int
			{
				eAimSpot_Head = 0,
				eAimSpot_Body = 1
			};

			bool Enabled = false;

			bool AimAtTribeMembers = false;

			int AimSpot = eAimSpot_Body;
		};

		sAimbotMainSettings MainSettings;
		sAimbotPlayerSettings Players;
	};

	struct s3DRadarFeatures
	{
		struct s3DRadarPlayerSettings
		{
			bool Enabled = false;
			bool Box = true;
			bool Name = true;
			bool ActiveItem = true;
			bool Level = true;
			bool Tribe = false;
			bool Distance = true;
			bool Healthbar = true;
			bool Torpor = true;
			bool Bones = true;
			//Filters
			bool DrawSleeping = false;
			bool DrawDead = false;
			bool DrawTribeMembers = true;
			//Colors
			DWORD Color_SleepingText = D3DCOLOR_ARGB(255, 61, 90, 230);
			DWORD Color_CorpseText = D3DCOLOR_ARGB(255, 164, 164, 164);
			DWORD Color_TribeMemberText = D3DCOLOR_ARGB(255, 27, 228, 20);

			DWORD Color_Box = RED(255);
			DWORD Color_Bones = WHITE(255);
			DWORD Color_NameText = D3DCOLOR_ARGB(255, 236, 29, 98);
			DWORD Color_ActiveItemText = D3DCOLOR_ARGB(255, 236, 244, 244);
			DWORD Color_TribeText = D3DCOLOR_ARGB(255, 216, 228, 53);
			DWORD Color_DistanceText = WHITE(255);
			DWORD Color_TorporBar = D3DCOLOR_ARGB(255, 60, 0, 255);

			DWORD Color_LevelText = WHITE(255);
		};

		struct s3DRadarDinoSettings
		{
			//MainFeatures
			bool Enabled = false;
			bool Name = true;
			bool Level = false;
			bool Distance = true;
			bool Healthbar = true;
			//Wild-Dino Filters
			bool DrawLandCarnivores = true;
			bool DrawLandHerbivores = true;
			bool DrawWaterCarnivores = true;
			bool DrawWaterHerbivores = true;
			//Other-Dino Filters
			bool DrawOnlyTamedDinos = false;
			bool DrawTribeDinos = true;
			//Filters
			float MaxDrawLandCarnivoreDistance = 350.0f;
			float MaxDrawLandHerbivoreDistance = 150.0f;

			float MaxDrawWaterCarnivoreDistance = 350.0f;
			float MaxDrawWaterHerbivoreDistance = 25.0f;

			//Colors

			DWORD Color_LandCarnivore = RED(255);
			DWORD Color_WaterCarnivore = ORANGE(255);
			DWORD Color_LandHerbivore = DARKORANGE(255);
			DWORD Color_WaterHerbivore = BLUE(255);

			DWORD Color_TamedText = D3DCOLOR_ARGB(255, 243, 22, 221);
			DWORD Color_TribeText = D3DCOLOR_ARGB(255, 27, 228, 20);
			DWORD Color_DistanceText = WHITE(255);
			DWORD Color_TorporBar = D3DCOLOR_ARGB(255, 60, 0, 255);

			DWORD Color_LevelText = WHITE(255);
			DWORD Color_LevelBk1 = BLACK(127);
			DWORD Color_LevelBk2 = GREEN(255);
		};

		struct s3DRadarStructureSettings
		{
			//MainFeatures
			bool Enabled = false;
			bool DrawItemCount = false;
			//Storage
			bool StorageBox = false;
			bool LargeStorageBox = false;
			bool Vault = false;
			bool GiantBeaverDam = false;
			bool ItemCache = false;
			bool Cryofridge = false;
			bool DedicatedStorage = false;
			bool PreservingBin = false;
			//Loot
			bool LootCrate = false;
			bool Artifact = false;
			//Stations
			bool Smithy = false;
			bool Fabricator = false;
			bool MortarAndPestle = false;
			//Tek
			bool TekTransmitter = false;
			bool TekForcefield = false;
			bool TekTeleporter = false;
			bool TekReplicator = false;
			//Other
			bool Generator =	false;
			bool TekGenerator = false;
			//Turrets
			bool AutoTurret =   false;
			bool HeavyTurret =  false;
			bool TekTurret =   false;

			DWORD Color_StorageText = D3DCOLOR_ARGB(255, 42, 218, 42);
			DWORD Color_LootText = D3DCOLOR_ARGB(255, 165, 42, 218);
			DWORD Color_StationsText = D3DCOLOR_ARGB(255, 203, 190, 190);
			DWORD Color_TekText = D3DCOLOR_ARGB(255, 24, 178, 225);
			DWORD Color_OthersText = D3DCOLOR_ARGB(255, 185, 184, 179);
			DWORD Color_TurretsText = D3DCOLOR_ARGB(255, 239, 176, 28);

			DWORD Color_ItemCount = D3DCOLOR_ARGB(255, 160, 160, 160);
		};
		s3DRadarPlayerSettings PlayerSettings;
		s3DRadarDinoSettings DinoSettings;
		s3DRadarStructureSettings StructureSettings;
	};
	struct sMiscSettings
	{
		struct MiscWeaponSettings
		{
			bool Enabled = false;
			bool NoCameraShake = false;
			bool NoSpread = false;
			bool NoAimSway = false;
		};
		MiscWeaponSettings Weapon;
	};
}

enum eMenuTabs : const int
{
	eTab_Aimbot = 0,
	eTab_3DRadar,
	eTab_Misc,
};

class Menu;
extern Menu eMenu;

class Menu
{
public:
	//Global Settings
	bool m_Initialized = false;
	bool m_IsOpen = true;
	eMenuTabs m_ActiveTab = eTab_Aimbot;
	//Features
	Features::sAimbotFeatures m_AimbotFeatures;
	Features::s3DRadarFeatures m_3DRadarFeatures;
	Features::sMiscSettings m_MiscFeatures;
	//Fonts
	ImFont* m_pMainFont = nullptr;
	ImFont* m_pSliderFont = nullptr;
private:
	bool ImGuiSetupColors();
	bool ImGuiColoredButton(const char* name, const ImVec2& Size, ImColor Color);
	//Tabs
	bool DrawAimbotTab();
	bool Draw3DRadarTab();
	void DrawMiscTab();
public:
	bool Initialize();
	//Rendering
	bool BeginImGuiScene();
	bool EndImGuiScene();
	void Render();
	void Work();
	static bool UpdateClickableState(bool MenuOpen);
	//Filters
	//bool IsDinoInEspFilter(SDK::APrimalDinoCharacter& Instance, float& Distance);
	//bool IsDinoInRadarFilter(SDK::APrimalDinoCharacter& Instance);

	//bool HandleMoveMenuEvent(const char* WindowName);

	void ResetDevice();
};

