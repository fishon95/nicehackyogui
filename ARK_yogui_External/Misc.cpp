#include "Misc.h"
#include "Menu.h"

Misc eMisc;

void Misc::RemoveCameraShake(SDK::AShooterWeapon* pShooterWeapon, SDK::AShooterWeapon& Instance)
{
	if (!pShooterWeapon)
		return;

	pShooterWeapon->SetFireCameraShake(NULL);
	pShooterWeapon->SetReloadCameraShake(NULL);

	return;
}

void Misc::RemoveAimSway(SDK::AShooterWeapon* pShooterWeapon, SDK::AShooterWeapon& Instance)
{
	static const Vec2 ZeroVector = { 0.0f, 0.0f };

	if (!pShooterWeapon)
		return;

	pShooterWeapon->SetAimDriftAngle(ZeroVector);
	pShooterWeapon->SetAimDriftFreq(ZeroVector);
}

void Misc::RemoveSpread(SDK::AShooterWeapon* pShooterWeapon, SDK::AShooterWeapon& Instance)
{
	SDK::FInstantWeaponData WeaponData = Instance.InstantConfig;

	WeaponData.WeaponSpread = 0.0f;
	WeaponData.TargetingSpreadMod = 0.0f;
	WeaponData.FinalWeaponSpreadMultiplier = 0.0f;
	WeaponData.FiringSpreadIncrement = 0.0f;
	WeaponData.FiringSpreadMax = 0.0f;

	pShooterWeapon->SetInstantConfig(WeaponData);
}

void Misc::Work()
{
	if (!eHackMain.m_pLocalShooterCharacter || !eHackMain.m_pLocalPlayerController)
		return;
	if (eMenu.m_MiscFeatures.Weapon.Enabled)
	{
		SDK::AShooterWeapon* pShooterWeapon = eHackMain.m_LocalShooterCharacter.m_pCurrentWeapon;

		if (pShooterWeapon)
		{
			SDK::AShooterWeapon WeaponInstance = pShooterWeapon->UpdateInformation();

			if (WeaponInstance.m_pAssociatedPrimalItem)
			{
				SDK::UPrimalItem ItemInstance = WeaponInstance.m_pAssociatedPrimalItem->UpdateInformation();
				std::wstring weapon_name = pShooterWeapon->get_name(&WeaponInstance);

				if (ItemInstance.MyItemType == SDK::Enums::EPrimalItemType__Weapon)
				{
					if (eMenu.m_MiscFeatures.Weapon.NoCameraShake)
						this->RemoveCameraShake(pShooterWeapon, WeaponInstance);
					if (eMenu.m_MiscFeatures.Weapon.NoSpread)
						this->RemoveSpread(pShooterWeapon, WeaponInstance);
					if (eMenu.m_MiscFeatures.Weapon.NoAimSway)
						this->RemoveAimSway(pShooterWeapon, WeaponInstance);
				}
			}

		}
	}
}
