#include "ESP.h"
#include "NvidiaOverlay/NvidiaOverlay.h"
#include <Windows.h>
#include <iostream>
#include <algorithm>

#include "VMProtect/VMProtectSDK.h"

#include "Menu.h"

#undef max
#undef min

ESP eESP;

void ESP::RenderHumanoids()
{
	for (int i = 0; i < eHackMain.m_aHumanoidCount; i++)
	{
		sHackHumanoid sCurrentPlayer = eHackMain.m_aHumanoidList[i]; 

		if (!sCurrentPlayer.pShooterCharacter || !sCurrentPlayer.ShooterCharacter.m_pRootComponent)
				continue;

		if (sCurrentPlayer.Distance <= 0)
			continue;

		sCurrentPlayer.ShooterCharacter = sCurrentPlayer.pShooterCharacter->UpdateInformation();
		//SDK::USceneComponent SceneComponent = sCurrentPlayer.ShooterCharacter.m_pRootComponent->UpdateInformation();

		Vec3 Origin = { sCurrentPlayer.SkeletalMesh.ComponentToWorld.Translation.x,sCurrentPlayer.SkeletalMesh.ComponentToWorld.Translation.y,
		sCurrentPlayer.SkeletalMesh.ComponentToWorld.Translation.z };//sCurrentPlayer.ShooterCharacter.m_fCurrentRootLoc;

		Vec2 ScreenPos;
		if (!SDK::Functions::WorldToScreen(eHackMain.m_UCanvas, Origin, &ScreenPos))
			continue;

		if (sCurrentPlayer.ShooterCharacter.bIsSleeping)
		{
			if (eMenu.m_3DRadarFeatures.PlayerSettings.DrawSleeping)
				this->DrawSleeper(ScreenPos, sCurrentPlayer);
			continue;
		}

		if (sCurrentPlayer.ShooterCharacter.bIsDead)
		{
			if (eMenu.m_3DRadarFeatures.PlayerSettings.DrawDead)
				this->DrawCorpse(ScreenPos, sCurrentPlayer);
			continue;
		}

		Vec3 BoxTopPos = this->GetBoxHeight(Origin, sCurrentPlayer.ShooterCharacter);

		Vec2 ScreenTopPos;
		if (!SDK::Functions::WorldToScreen(eHackMain.m_UCanvas, BoxTopPos, &ScreenTopPos))
			continue;

		//This gets offset when Horizontal text is rendered
		Vec2 OffsetPos = ScreenPos;
		if (eMenu.m_3DRadarFeatures.PlayerSettings.Box)
		{
			DWORD target_color = eMenu.m_3DRadarFeatures.PlayerSettings.Color_Box;

			if (sCurrentPlayer.IsLocalTribe)
				target_color = eMenu.m_3DRadarFeatures.PlayerSettings.Color_TribeText;

			this->DrawPlayerCornerBox(ScreenPos, ScreenTopPos, sCurrentPlayer.ShooterCharacter, target_color);
		}


		Vec2 HealthBarPos = ScreenTopPos;
		HealthBarPos.y -= 6;
		if (eMenu.m_3DRadarFeatures.PlayerSettings.Healthbar)
			this->DrawEntityHealthbar(HealthBarPos, sCurrentPlayer.ShooterCharacter.m_Health, sCurrentPlayer.ShooterCharacter.m_MaxHealth);

		if (eMenu.m_3DRadarFeatures.PlayerSettings.Bones)
			this->DrawPlayerBones(sCurrentPlayer, eMenu.m_3DRadarFeatures.PlayerSettings.Color_Bones);


		if (eMenu.m_3DRadarFeatures.PlayerSettings.Name)
		{
			DWORD target_color = eMenu.m_3DRadarFeatures.PlayerSettings.Color_NameText;

			if (sCurrentPlayer.IsLocalTribe)
				target_color = eMenu.m_3DRadarFeatures.PlayerSettings.Color_TribeText;

			this->DrawEnityName(OffsetPos, sCurrentPlayer.PlayerName, target_color);
		}

		if (eMenu.m_3DRadarFeatures.PlayerSettings.Level)
		{
			int level = sCurrentPlayer.StatusComponent.m_BaseCharacterLevel + sCurrentPlayer.StatusComponent.m_ExtraCharacterLevel;
			this->DrawEntityLevel(OffsetPos, level, eMenu.m_3DRadarFeatures.PlayerSettings.Name ? &sCurrentPlayer.PlayerName : nullptr);
		}

		if (eMenu.m_3DRadarFeatures.PlayerSettings.ActiveItem)
			this->DrawPlayerActiveItem(OffsetPos, sCurrentPlayer.ShooterCharacter, eMenu.m_3DRadarFeatures.PlayerSettings.Color_ActiveItemText);

		if (eMenu.m_3DRadarFeatures.PlayerSettings.Tribe)
			this->DrawEnityName(OffsetPos, sCurrentPlayer.TribeName.c_str(), eMenu.m_3DRadarFeatures.PlayerSettings.Color_TribeText);

		if (eMenu.m_3DRadarFeatures.PlayerSettings.Distance)
			this->DrawEntityDistance(OffsetPos, eMenu.m_3DRadarFeatures.PlayerSettings.Color_DistanceText, sCurrentPlayer.Distance);

	}
}

void ESP::RenderDinos()
{
	for (int i = 0; i < eHackMain.m_aNPCCount; i++)
	{
		sHackNPC sCurrentNpc = eHackMain.m_aNPCList[i];

		if (!sCurrentNpc.pPrimalDinoCharacter)
			continue;

		if (!sCurrentNpc.PrimalDinoCharacter.m_pDescriptiveName)
			continue;

		if (sCurrentNpc.PrimalDinoCharacter.bIsDead)
			continue;

		//We can reread the position here

		sCurrentNpc.SkeletalMesh = sCurrentNpc.PrimalDinoCharacter.m_pMesh->UpdateInformation();

		Vec3 Origin = { sCurrentNpc.SkeletalMesh.ComponentToWorld.Translation.x,sCurrentNpc.SkeletalMesh.ComponentToWorld.Translation.y,
		sCurrentNpc.SkeletalMesh.ComponentToWorld.Translation.z };//sCurrentPlayer.ShooterCharacter.m_fCurrentRootLoc;


		Vec2 ScreenPos;
		if (!SDK::Functions::WorldToScreen(eHackMain.m_UCanvas, Origin, &ScreenPos))
			continue;

		//This gets offset when Horizontal text is rendered
		Vec2 OffsetPos = ScreenPos;

		DWORD TargetColor = this->GetDinoColor(sCurrentNpc);

		Vec2 HealthBarPos = { ScreenPos.x, ScreenPos.y - 4 };

		if (eMenu.m_3DRadarFeatures.DinoSettings.Healthbar)
			this->DrawEntityHealthbar(HealthBarPos, sCurrentNpc.PrimalDinoCharacter.m_Health, sCurrentNpc.PrimalDinoCharacter.m_MaxHealth);


		if (eMenu.m_3DRadarFeatures.DinoSettings.Name)
			this->DrawEnityName(OffsetPos, sCurrentNpc.DescriptiveName, TargetColor);

		if (eMenu.m_3DRadarFeatures.DinoSettings.Level)
		{
			int level = sCurrentNpc.StatusComponent.m_BaseCharacterLevel + sCurrentNpc.StatusComponent.m_ExtraCharacterLevel;
			this->DrawEntityLevel(OffsetPos, level, eMenu.m_3DRadarFeatures.DinoSettings.Name ? &sCurrentNpc.DescriptiveName : nullptr);
		}

		if (eMenu.m_3DRadarFeatures.DinoSettings.Distance)
			this->DrawEntityDistance(OffsetPos, eMenu.m_3DRadarFeatures.DinoSettings.Color_DistanceText, sCurrentNpc.Distance);


		//eRenderer.Text(OffsetPos.x, OffsetPos.y, WHITE(255), centered, FontIDS_MainFont, true, "0x%llX", sCurrentNpc.pPrimalDinoCharacter);

		//sCurrentNpc.PrimalDinoCharacter.

	}
}

void ESP::RenderStructures()
{
	for (int i = 0; i < eHackMain.m_aStructureCount; i++)
	{
		sHackStructure cStructure = eHackMain.m_aStructureList[i];

		Vec2 ScreenPos;
		if (!SDK::Functions::WorldToScreen(eHackMain.m_UCanvas, cStructure.SceneComponent.m_RelativeLocation, &ScreenPos))
			continue;

		if (cStructure.StructureType == HackStructureType_LootCrate)
		{
			SDK::APrimalStructureItemContainer_SupplyCrate SupplyCrate = ((SDK::APrimalStructureItemContainer_SupplyCrate*)cStructure.pPrimalStructure)->UpdateInformation();

			this->DrawEntityLevel(ScreenPos, SupplyCrate.m_RequiredLevelToAccess, &cStructure.DescriptiveName);
		}
		else if (cStructure.StructureType == HackStructureType_Turret)
		{
			SDK::APrimalStructureTurret TurretStructure = reinterpret_cast<SDK::APrimalStructureTurret*>(cStructure.pPrimalStructure)->UpdateInformation();

			if (TurretStructure.NumBulletsPerShot)
			{
				int shots_left = 0;
				if (TurretStructure.NumBullets >= TurretStructure.NumBulletsPerShot)
					shots_left = TurretStructure.NumBullets / TurretStructure.NumBulletsPerShot;
				else
					shots_left = TurretStructure.NumBullets;
				this->DrawEntityLevel(ScreenPos, shots_left, &cStructure.DescriptiveName);
			}
		}

		DWORD color = this->GetStructureColor(cStructure.StructureType);

		if (eMenu.m_3DRadarFeatures.StructureSettings.DrawItemCount && cStructure.StructureType != HackStructureType_Turret
			&& cStructure.current_items < cStructure.max_items)
			this->DrawStructureName(ScreenPos, cStructure.DescriptiveName, cStructure.current_items, cStructure.max_items, color);
		else
			this->DrawEnityName(ScreenPos, cStructure.DescriptiveName.c_str(), color);

		/*	if (eMenu.m_3DRadarFeatures.StructureSettings.DrawItemCount)
			{
				if (cStructure.max_items > 0 || cStructure.current_items > 0)
				{
					this->DrawEnityItemCount(ScreenPos, cStructure.current_items, cStructure.max_items, eMenu.m_3DRadarFeatures.StructureSettings.Color_ItemCount);
				}
			}*/

		this->DrawEntityDistance(ScreenPos, color, cStructure.Distnace);
	}
}

void ESP::RenderStatics()
{
	VMProtectBeginMutation("RenderStatics");
	//Debug Information
	{
		if (eRenderer.m_Attached)
		{
			//eRenderer.Text(1, 1, WHITE(255), lefted, FontIDS::FontIDS_MainFont, true, XorString(("Press [END] to exit")));
			//eRenderer.Text(1, 18, WHITE(255), lefted, FontIDS_MainFont, true, XorString("m_aHumanoidCount -> %i"), eHackMain.m_aHumanoidCount);
			//eRenderer.Text(1, 36, WHITE(255), lefted, FontIDS_MainFont, true, XorString("m_aNPCCount -> %i"), eHackMain.m_aNPCCount);
			//
			//eRenderer.Text(1, 54, WHITE(255), lefted, FontIDS_MainFont, true, XorString("m_Regions -> %i"), eHackMain.m_ActorItterator.m_allocated_regions.size());
		}

		/*if (eHackMain.m_pLocalShooterCharacter)
		{
			eRenderer.Text(1, 72, WHITE(255), lefted, FontIDS_MainFont, true, (char*)"LocalHealth -> %f", eHackMain.m_LocalShooterCharacter.m_Health);
		}*/
	}
	//Crosshair
	{
		eRenderer.Circle(eRenderer.m_Width / 2, eRenderer.m_Height / 2, 2, 6, WHITE(255));

	}
	VMProtectEnd();
}

DWORD ESP::GetStructureColor(int structure_type)
{
	switch (structure_type)
	{
	case HackStructureType_Storage:
		return eMenu.m_3DRadarFeatures.StructureSettings.Color_StorageText;
		break;
	case HackStructureType_Turret:
		return eMenu.m_3DRadarFeatures.StructureSettings.Color_TurretsText;
		break;
	case HackStructureType_LootCrate:
		return eMenu.m_3DRadarFeatures.StructureSettings.Color_LootText;
		break;
	case HackStructureType_Artifact:
		return eMenu.m_3DRadarFeatures.StructureSettings.Color_LootText;
		break;
	case HackStructureType_WorkStation:
		return eMenu.m_3DRadarFeatures.StructureSettings.Color_StationsText;
		break;
	case HackStructureType_Tek:
		return eMenu.m_3DRadarFeatures.StructureSettings.Color_TekText;
		break;
	case HackStructureType_Other:
		return eMenu.m_3DRadarFeatures.StructureSettings.Color_OthersText;
		break;
	default:
		break;
	}
}

DWORD ESP::GetDinoColor(sHackNPC& Instance)
{
	DWORD TargetColor = NULL;

	if (Instance.IsLocalTribe)
	{
		TargetColor = eMenu.m_3DRadarFeatures.DinoSettings.Color_TribeText;
		return TargetColor;
	}

	if (Instance.PrimalDinoCharacter.IsTamed(&Instance.PrimalDinoCharacter))
	{
		TargetColor = eMenu.m_3DRadarFeatures.DinoSettings.Color_TamedText;
		return TargetColor;
	}

	if (Instance.PrimalDinoCharacter.bIsCarnivore)
	{
		if (Instance.PrimalDinoCharacter.bIsWaterDino)
		{
			TargetColor = eMenu.m_3DRadarFeatures.DinoSettings.Color_WaterCarnivore;
		}
		else
		{
			TargetColor = eMenu.m_3DRadarFeatures.DinoSettings.Color_LandCarnivore;
		}
	}
	else
	{
		if (Instance.PrimalDinoCharacter.bIsWaterDino)
		{
			TargetColor = eMenu.m_3DRadarFeatures.DinoSettings.Color_WaterHerbivore;
		}
		else
		{
			TargetColor = eMenu.m_3DRadarFeatures.DinoSettings.Color_LandHerbivore;
		}
	}
	return TargetColor;
}

DWORD ESP::GetHealthColor(int Health, int MaxHealth)
{
	return D3DCOLOR_ARGB(255, 10, 220, 10);
}

Vec3 ESP::GetBoxHeight(Vec3& Origin, SDK::AShooterCharacter& Character)
{
	Vec3 EyePos = Origin;

	if (Character.m_BaseEyeHeight < Character.m_CrouchedEyeHeight)// 64 40 2
		EyePos.z += Character.m_CrouchedEyeHeight * 1.8f;
	else
		EyePos.z += Character.m_BaseEyeHeight * 2.5f;

	return EyePos;
}

void ESP::DrawEnityName(Vec2& Origin, const std::wstring& name, const DWORD& Color)
{
	if (name.length() < 1)
		return;


	eRenderer.Text(Origin.x, Origin.y, Color, centered, FontIDS_MainFont, true, (char*)"%ls", name.c_str());

	//Offsetting the text
	Origin.y += 16;

}

void ESP::DrawStructureName(Vec2& Origin, const std::wstring& name, const int& items, const int& max_items, const DWORD& Color)
{
	if (items > max_items)
		return;

	if (name.length() < 1)
		return;


	eRenderer.Text(Origin.x, Origin.y, Color, centered, FontIDS_MainFont, true, (char*)"%ls [%i/%i]", name.c_str(), items, max_items);

	//Offsetting the text
	Origin.y += 16;
}

void ESP::DrawEntityLevel(Vec2& Origin, int& level, std::wstring* name)
{
	const int FontSize = 14;


	eRenderer.Text(Origin.x, Origin.y, eMenu.m_3DRadarFeatures.DinoSettings.Color_LevelText, centered, FontIDS_MainFont, true, "LVL (%i)", level);

	Origin.y += 16;
}

void ESP::DrawEntityDistance(Vec2& Position, DWORD& Color, float& Distance)
{

	eRenderer.Text(Position.x, Position.y, Color, centered, FontIDS_MainFont, true, (char*)XorString("[%im]"), (int)(Distance / 100));

	//Offsetting the text
	Position.y += 16;
}

void ESP::DrawEntityHealthbar(Vec2& Position, float& health, float& maxhealth)
{
	const int width = 32;
	const int height = 4;

	if (health <= 0 || maxhealth <= 0)
		return;

	if (health > maxhealth)
		return;

	int barwidth = health * width / maxhealth;

	if (barwidth < 0)
		return;

	DWORD Color = this->GetHealthColor((int)health, (int)maxhealth);


	eRenderer.FilledBox(Position.x - width / 2 - 1, Position.y - height / 2 - 1, width + 2, height + 2, BLACK(255));
	eRenderer.FilledBox(Position.x - width / 2, Position.y - height / 2, barwidth, height, Color);

	Position.y -= 6;
}

void ESP::DrawEntityTorporbar(Vec2& Position, float& torpor, float& maxtorpor, DWORD& Color)
{
	const int width = 32;
	const int height = 4;

	if (torpor <= 0 || maxtorpor <= 0)
		return;

	int barwidth = torpor * width / maxtorpor;

	if (barwidth < 0)
		return;


	eRenderer.FilledBox(Position.x - width / 2 - 1, Position.y - height / 2 - 1, width + 2, height + 2, BLACK(255));
	eRenderer.FilledBox(Position.x - width / 2, Position.y - height / 2, barwidth, height, Color);

}

void ESP::DrawPlayerCornerBox(Vec2& Origin, Vec2& TopPos, SDK::AShooterCharacter& ShooterCharacter, DWORD& Color)
{
	int height = TopPos.y - Origin.y;
	int width = NULL;

	if (ShooterCharacter.bIsCrouched)
		width = (float)height / 1.5f;
	else if (ShooterCharacter.bIsProne)
		width = (float)height;
	else
		width = (float)height / 2.f;

	box_width = -width;
	box_height = height;

	//eRenderer.CornerBoxOutlined(Origin.x - width / 2, Origin.y, width, height, 1, 1, Color, BLACK(255));

	eRenderer.CornerBoxOutlined(Origin.x - width / 2, Origin.y, width, height, 4, 1, Color, BLACK(255));

}

void ESP::DrawPlayerActiveItem(Vec2& Origin, SDK::AShooterCharacter& ShooterCharacter, DWORD& Color)
{
	if (!ShooterCharacter.m_pCurrentWeapon)
		return;

	std::wstring active_item = ShooterCharacter.m_pCurrentWeapon->get_name(nullptr);

	if (active_item.length() <= 1 || active_item.length() > 64)
		return;


	eRenderer.Text(Origin.x, Origin.y, Color, centered, FontIDS_MainFont, true, (char*)"%ls", active_item.c_str());

	Origin.y += 16;
}

void ESP::DrawPlayerBones(sHackHumanoid& Player, DWORD& Color)
{
	if ((int)(Player.Distance / 100) >= MAX_BONE_RENDDIST)
		return;

	const std::vector<int>* pTargetBoneList = nullptr;
	if (Player.ShooterCharacter.bIsFemale)
		pTargetBoneList = &SDK::ARK_FEMALE_BONELIST;
	else
		pTargetBoneList = &SDK::ARK_MALE_BONELIST;

	static SDK::FTransform BoneTransforms[100];

	SDK::USkeletalMeshComponent SkeletalMeshComponent = Player.ShooterCharacter.m_pMesh->UpdateInformation();

	if (SkeletalMeshComponent.m_SpaceBasesSize <= 0 || SkeletalMeshComponent.m_SpaceBasesSize >= 100)
		return;

	SkeletalMeshComponent.m_pTArraySpaceBases->GetBoneTransforms(BoneTransforms, SkeletalMeshComponent.m_SpaceBasesSize);

	//Pre-Draw stuff not to w2s same bones all the time
	Vec2 PelvisScreenPos, NeckScreenPos; Vec3 Buffer;

	{
		if (!SDK::Functions::GetBonePosition(SkeletalMeshComponent, BoneTransforms, ARK_BONES_NECK, &Buffer))
			return;

		if (!SDK::Functions::WorldToScreen(eHackMain.m_UCanvas, Buffer, &NeckScreenPos))
			return;

		if (!SDK::Functions::GetBonePosition(SkeletalMeshComponent, BoneTransforms, ARK_BONES_PELVIS, &Buffer))
			return;

		if (!SDK::Functions::WorldToScreen(eHackMain.m_UCanvas, Buffer, &PelvisScreenPos))
			return;
	}



	Vec2 ScreenPos1, ScreenPos2;
	//Vec3 BonePos1, BonePos2;

	for (int i = 0; i < (pTargetBoneList->size() - 1); i++)
	{
		int BoneIndex = (*pTargetBoneList)[i];
		int NextBoneIndex = (*pTargetBoneList)[i + 1];

		if (BoneIndex == -1 || NextBoneIndex == -1)
			continue;

		{
			if (BoneIndex == ARK_BONES_PELVIS)
			{
				ScreenPos1 = PelvisScreenPos;
			}
			else if (BoneIndex == ARK_BONES_NECK)
			{
				ScreenPos1 = NeckScreenPos;
			}
			else
			{
				if (!SDK::Functions::GetBonePosition(SkeletalMeshComponent, BoneTransforms, BoneIndex, &Buffer))
					return;

				if (!SDK::Functions::WorldToScreen(eHackMain.m_UCanvas, Buffer, &ScreenPos1))
					return;

			}
		}

		{
			if (NextBoneIndex == ARK_BONES_PELVIS)
			{
				ScreenPos2 = PelvisScreenPos;
			}
			else if (NextBoneIndex == ARK_BONES_NECK)
			{
				ScreenPos2 = NeckScreenPos;
			}
			else
			{
				if (!SDK::Functions::GetBonePosition(SkeletalMeshComponent, BoneTransforms, NextBoneIndex, &Buffer))
					return;

				if (!SDK::Functions::WorldToScreen(eHackMain.m_UCanvas, Buffer, &ScreenPos2))
					return;

			}
		}
#if(!USE_GAMEBAR)
		eRenderer.ThickLine(ScreenPos1.x, ScreenPos1.y, ScreenPos2.x, ScreenPos2.y, 2, eMenu.m_3DRadarFeatures.PlayerSettings.Color_Bones, true);
#else
		gamebar_interface.draw_line({ (int)ScreenPos1.x, (int)ScreenPos1.y, (int)ScreenPos2.x, (int)ScreenPos2.y, 2, eMenu.m_3DRadarFeatures.PlayerSettings.Color_Bones });
#endif
	}

}

void ESP::DrawSleeper(Vec2& Origin, sHackHumanoid& Player)
{
	Vec2 OffsetPos = Origin;
	if (eMenu.m_3DRadarFeatures.PlayerSettings.Tribe)
		this->DrawEnityName(OffsetPos, Player.TribeName.c_str(), eMenu.m_3DRadarFeatures.PlayerSettings.Color_TribeText);

	if (eMenu.m_3DRadarFeatures.PlayerSettings.Name)
		this->DrawEnityName(OffsetPos, Player.PlayerName, eMenu.m_3DRadarFeatures.PlayerSettings.Color_SleepingText);

	if (eMenu.m_3DRadarFeatures.PlayerSettings.Distance)
		this->DrawEntityDistance(OffsetPos, eMenu.m_3DRadarFeatures.PlayerSettings.Color_DistanceText, Player.Distance);

}

void ESP::DrawCorpse(Vec2& Origin, sHackHumanoid& Player)
{
	if (eMenu.m_3DRadarFeatures.PlayerSettings.Name)
		this->DrawEnityName(Origin, L"*Corpse*", eMenu.m_3DRadarFeatures.PlayerSettings.Color_CorpseText);

	if (eMenu.m_3DRadarFeatures.PlayerSettings.Distance)
		this->DrawEntityDistance(Origin, eMenu.m_3DRadarFeatures.PlayerSettings.Color_DistanceText, Player.Distance);

}

void ESP::Render()
{
	eHackMain.m_PlayerCameraManager = eHackMain.m_pPlayerCameraManager->UpdateInformation();
	SDK::Functions::update_matrix(eHackMain.m_PlayerCameraManager.CameraCacheEntry.MinimalViewInfo);
	

	//if (eHackMain.m_pUCanvas != nullptr)
	{
	
		if (eMenu.m_3DRadarFeatures.DinoSettings.Enabled)
			this->RenderDinos();

		if (eMenu.m_3DRadarFeatures.PlayerSettings.Enabled)
			this->RenderHumanoids();

		if (eMenu.m_3DRadarFeatures.StructureSettings.Enabled)
			this->RenderStructures();


	}

	this->RenderStatics();
}
