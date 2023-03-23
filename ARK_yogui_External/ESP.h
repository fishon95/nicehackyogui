#pragma once
#include "HackMain.h"
#include "SDK.h"
#include "DirectXIncludes.h"

class ESP;
extern ESP eESP;

class ESP
{
private:
	const int MAX_BONE_RENDDIST = 100;

	int box_width;
	int box_height;
private:
	//Renderables
	void RenderHumanoids();
	void RenderDinos();
	void RenderStructures();
	void RenderStatics();

	//Functions
	DWORD GetHealthColor(int Health, int MaxHealth);
	Vec3 GetBoxHeight(Vec3& Origin, SDK::AShooterCharacter& Character);
	//Individual-Draws
	void DrawEnityName(Vec2& Origin, const std::wstring& name, const DWORD& Color);
	void DrawStructureName(Vec2& Origin, const std::wstring& name, const int& items, const int& max_items, const DWORD& Color);
	void DrawEntityLevel(Vec2& Origin, int& level, std::wstring* name = nullptr);
	void DrawEntityDistance(Vec2& Position, DWORD& Color, float& Distance);
	void DrawEntityHealthbar(Vec2& Position, float& health, float& maxhealth);
	void DrawEntityTorporbar(Vec2& Position, float& torpor, float& maxtorpor, DWORD& Color);
	//Player Draws
	void DrawPlayerCornerBox(Vec2& Origin, Vec2& TopPos, SDK::AShooterCharacter& ShooterCharacter, DWORD& Color);

	void DrawPlayerActiveItem(Vec2& Origin, SDK::AShooterCharacter& ShooterCharacter, DWORD& Color);
	void DrawPlayerBones(sHackHumanoid& Player, DWORD& Color);
	void DrawSleeper(Vec2& Origin, sHackHumanoid& Player);
	void DrawCorpse(Vec2& Origin, sHackHumanoid& Player);
public:

	void Render();

	static DWORD GetStructureColor(int structure_type);
	static DWORD GetDinoColor(sHackNPC& Instance);
	static ESP* GetInstance() { return &eESP; }
};

