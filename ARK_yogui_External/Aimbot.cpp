#include "Aimbot.h"
#include "NvidiaOverlay/NvidiaOverlay.h"
#include "Menu.h"

Aimbot eAimbot;

std::vector<sAimbotPlayer> Aimbot::GetPlayersInFOV()
{
	int TargetBoneIndex = NULL;

	std::vector<sAimbotPlayer> TargetList;

	const Vec2 CenterPos = { (float)eRenderer.m_Width / 2, (float)eRenderer.m_Height / 2 };

	for (int i = 0; i < eHackMain.m_aHumanoidCount; i++)
	{
		if (eHackMain.m_aHumanoidCount >= MAX_HUMANOIDS)
			return {};

		sHackHumanoid cPlayer = eHackMain.m_aHumanoidList[i];

		if (!cPlayer.pShooterCharacter)
			continue;

		if (cPlayer.pShooterCharacter == eHackMain.m_pLocalShooterCharacter)
			continue;

		if (cPlayer.ShooterCharacter.bIsDead)
			continue;

		if (cPlayer.ShooterCharacter.bIsSleeping)
			continue;

		if (cPlayer.IsLocalTribe && !eMenu.m_AimbotFeatures.Players.AimAtTribeMembers)
			continue;

		Vec3 TargetBonePos;

		if (eMenu.m_AimbotFeatures.Players.AimSpot == eMenu.m_AimbotFeatures.Players.eAimSpot_Head)
			TargetBoneIndex = ARK_BONES_HEAD;
		else if (eMenu.m_AimbotFeatures.Players.AimSpot == eMenu.m_AimbotFeatures.Players.eAimSpot_Body)
			TargetBoneIndex = ARK_BONES_PELVIS;
		else
			TargetBoneIndex = ARK_BONES_HEAD;

		if (!cPlayer.ShooterCharacter.m_pMesh)
			continue;

		cPlayer.SkeletalMesh = cPlayer.ShooterCharacter.m_pMesh->UpdateInformation();

		if (!SDK::Functions::GetBonePosition(cPlayer.SkeletalMesh, TargetBoneIndex, &TargetBonePos))
			continue;

		Vec2 ScreenPos;

		if (!SDK::Functions::WorldToScreen(eHackMain.m_UCanvas, TargetBonePos, &ScreenPos))
			continue;

		Vec2 delta = ScreenPos - CenterPos;

		float Distance = D3DXVec2Length(&delta);

		if (Distance > eMenu.m_AimbotFeatures.MainSettings.FieldOfView)
			continue;

		TargetList.push_back({ cPlayer, TargetBonePos, TargetBoneIndex, Distance });
	}
	return TargetList;
}




bool Aimbot::AimAtPlayers()
{
	sAimbotPlayer* BestTarget = nullptr;
	std::vector<sAimbotPlayer> TargetList;

	if (!eHackMain.m_pLocalPlayerController)
		return false;

	if (eMenu.m_AimbotFeatures.MainSettings.LockTarget && m_IsPlayerLocked
		&& m_LockedPlayer.TargetPlayer.pShooterCharacter && m_LockedPlayer.TargetPlayer.ShooterCharacter.m_pMesh)
	{
		m_LockedPlayer.TargetPlayer.ShooterCharacter = m_LockedPlayer.TargetPlayer.pShooterCharacter->UpdateInformation();

		if (m_LockedPlayer.TargetPlayer.ShooterCharacter.bIsDead)
		{
			m_IsPlayerLocked = false;
			return true;
		}

		m_LockedPlayer.TargetPlayer.SkeletalMesh = m_LockedPlayer.TargetPlayer.ShooterCharacter.m_pMesh->UpdateInformation();

		if (!SDK::Functions::GetBonePosition(m_LockedPlayer.TargetPlayer.SkeletalMesh, m_LockedPlayer.TargetBoneIndex, &m_LockedPlayer.AimPos))
		{
			m_IsPlayerLocked = false;
			return true;
		}

		BestTarget = &m_LockedPlayer;
	}
	else
	{
		TargetList = this->GetPlayersInFOV();

		if (TargetList.size() < 1)
			return false;

		float SmallestDist = FLT_MAX;
		for (auto& target : TargetList)
		{
			if (target.ScreenDist < SmallestDist)
			{
				BestTarget = &target;
				SmallestDist = target.ScreenDist;
			}
		}

		if (eMenu.m_AimbotFeatures.MainSettings.LockTarget && !m_IsPlayerLocked)
		{
			m_LockedPlayer = *BestTarget;
			m_IsPlayerLocked = true;
		}
	}

	if (!eHackMain.m_pPlayerCameraManager)
		return false;

	//Updating our position for calc angle
	eHackMain.m_PlayerCameraManager = eHackMain.m_pPlayerCameraManager->UpdateInformation();
	Vec3 LocalPos = { 0,0,0 };
	LocalPos = eHackMain.m_PlayerCameraManager.CameraCacheEntry.MinimalViewInfo.m_Location;



	if (!BestTarget)
		return false;

	if (!BestTarget->TargetPlayer.pShooterCharacter)
		return false;

	BestTarget->TargetPlayer.ShooterCharacter = BestTarget->TargetPlayer.pShooterCharacter->UpdateInformation();

	if (!BestTarget->TargetPlayer.ShooterCharacter.m_pMesh)
		return false;

	BestTarget->TargetPlayer.SkeletalMesh = BestTarget->TargetPlayer.ShooterCharacter.m_pMesh->UpdateInformation();

	Vec3 OriginPos = { BestTarget->TargetPlayer.SkeletalMesh.ComponentToWorld.Translation.x,
	BestTarget->TargetPlayer.SkeletalMesh.ComponentToWorld.Translation.y,
	BestTarget->TargetPlayer.SkeletalMesh.ComponentToWorld.Translation.z };

	Vec3 Angle = Math::CalculateAngle(LocalPos, BestTarget->AimPos);

	//if (eMenu.m_AimbotFeatures.Players.Smooth > 1.0f)
	//{
	//	Vec3 CurrentAimAngle = eHackMain.m_pLocalPlayerController->UpdateInformation().ControlRotation;

	//	Vec3 DeltaAngle = Angle - CurrentAimAngle;

	//	DeltaAngle.x /= eMenu.m_AimbotFeatures.Players.Smooth;
	//	DeltaAngle.y /= eMenu.m_AimbotFeatures.Players.Smooth;

	//	Angle = CurrentAimAngle + DeltaAngle;
	//	Math::ClampAngle(Angle);
	//}

	this->IsTargetingPlayer = true;

	eHackMain.m_pLocalPlayerController->SetControlRotation(Angle);
	return true;
}

void Aimbot::Reset()
{
	m_IsPlayerLocked = false;
	m_IsDinoLocked = false;
	IsTargetingDino = false;
	IsTargetingPlayer = false;

}

void Aimbot::Work()
{

	if (!eMenu.m_AimbotFeatures.Players.Enabled)// DINOS CHECK
		return this->Reset();

	if (!(GetAsyncKeyState(eMenu.m_AimbotFeatures.MainSettings.AimKey) & 0x8000))
		return this->Reset();

	if (!eHackMain.m_pPlayerCameraManager)
		return this->Reset();

	if (eMenu.m_AimbotFeatures.Players.Enabled && !this->m_IsDinoLocked)
		if (this->AimAtPlayers())
			return;

	//if (eMenu.m_AimbotFeatures.Dinos.Enabled && !this->m_IsPlayerLocked)
	//	if (this->AimAtDinos())
	//		return;

	this->Reset();
}
