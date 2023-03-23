#pragma once
#include "SDK.h"
#include "HackMain.h"

class Misc;
extern Misc eMisc;


class Misc
{
private:
	//sHackShooterWeaponData OldWeaponData;
private:
	//Weapons
	void RemoveCameraShake(SDK::AShooterWeapon* pShooterWeapon, SDK::AShooterWeapon& Instance);
	void RemoveAimSway(SDK::AShooterWeapon* pShooterWeapon, SDK::AShooterWeapon& Instance);
	void RemoveSpread(SDK::AShooterWeapon* pShooterWeapon, SDK::AShooterWeapon& Instance);
public:
	void Work();

	static Misc* GetInstance() { return &eMisc; }
};
