#pragma once
#include "SDK.h"
#include "Math.h"
#include "HackMain.h"

class Aimbot;
extern Aimbot eAimbot;

struct sAimbotPlayer
{
	sHackHumanoid TargetPlayer;
	Vec3 AimPos;
	int TargetBoneIndex;
	float ScreenDist;
};

struct sAimbotNPC
{
	sHackNPC TargetNpc;
	Vec3 AimPos;
	int TargetBoneIndex;
	float ScreenDist;
};

class Aimbot
{
private:
	//const int AIM_KEY = 70;

	sAimbotPlayer m_LockedPlayer;
	sAimbotNPC m_LockedDino;

public:
	bool IsTargetingPlayer = false;
	bool IsTargetingDino = false;

	bool m_IsPlayerLocked = false;
	bool m_IsDinoLocked = false;
private:
	std::vector<sAimbotPlayer> GetPlayersInFOV();
	std::vector<sAimbotNPC> GetNPCsInFOV();

	void adjust_for_projectiles(std::wstring& weapon_name, SDK::UCharacterMovementComponent* character_movement, Vec3& local_pos, Vec3& origin_pos, Vec3* aim_pos);

	bool AimAtDinos();
	bool AimAtPlayers();
public:
	void Reset();
	void Work();

	static Aimbot* GetInstance() { return &eAimbot; }
};

