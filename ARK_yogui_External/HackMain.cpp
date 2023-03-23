#include "HackMain.h"
#include "Globals.h"

#include "Menu.h"

#include "VMProtect/VMProtectSDK.h"

#include <ctime>

#include "Math.h"

uintptr_t Globals::Modules::ShooterGame = NULL;

HackMain eHackMain;
BoneMap eBoneMap;





bool BoneMap::FindEntry(std::wstring& name, sBoneMapEntry* pOutEntry)
{
	for (auto& entry : m_BoneEntries)
	{
		if (wcscmp(name.c_str(), entry.DescriptiveName.c_str()) != 0)
			continue;
		if (pOutEntry)
			*pOutEntry = entry;
		return true;
	}
	return false;
}

bool BoneMap::GetHeadBoneIndex(SDK::USkeletalMeshComponent& Instance, std::wstring& DescName, int* pOutIndex)
{
	if (!pOutIndex)
		return false;

	sBoneMapEntry BoneEntry;
	if (this->FindEntry(DescName, &BoneEntry))
	{
		*pOutIndex = BoneEntry.HeadBoneIndex;
		return true;
	}

	if (!SDK::Functions::GetBoneHeadIndex(Instance.m_pMaterials, pOutIndex))
		return false;

	this->m_BoneEntries.push_back({ DescName, *pOutIndex });

	return true;
}

bool hActorIterator::UpdateInformation(SDK::PersistentLevel& persistent_level)
{
	if (!persistent_level.m_pAActors || persistent_level.m_ActorCount <= 0 || persistent_level.m_ActorCount > 0x100000)
		return false;

	saved_actors = persistent_level.m_ActorCount;

	int regions_needed = (persistent_level.m_ActorCount * sizeof(SDK::AActor*)) / 0x1000 + 1;

	if (((persistent_level.m_ActorCount * sizeof(SDK::AActor*)) % 0x1000) > 0)
		regions_needed++;

	if (regions_needed > this->m_allocated_regions.size())
	{
		int current_size = this->m_allocated_regions.size();
		for (int i = current_size; i < regions_needed; i++)
		{
			PVOID allocated_region = VirtualAlloc(NULL, 0x1000, MEM_COMMIT, PAGE_READWRITE);

			if (!allocated_region)
				return false;

			this->m_allocated_regions.push_back((SDK::AActor**)allocated_region);
		}
	}

	if (regions_needed > this->m_allocated_regions.size())
		return false;

	DWORD size = 0x1000;
	DWORD offset = 0x0;
	int region_count = 0;
	for (auto& region : this->m_allocated_regions)
	{
		if (!region)
			continue;

		if (region_count == (this->m_allocated_regions.size() - 1))
		{
			size = (persistent_level.m_ActorCount * sizeof(SDK::AActor*)) % 0x1000;
		}

		eDriver.read_in_buffer(reinterpret_cast<PVOID>((uintptr_t)persistent_level.m_pAActors + offset), region, size);

		//DBG_PRINT("ReadRegion[%i] : region[0] -> 0x%llX\n", region_count, region[0]);

		region_count++;
		offset += 0x1000;
	}

	current_index = 0;

	return true;
}

bool hActorIterator::Next(SDK::AActor** out_actor)
{
	if (saved_actors <= current_index || current_index < 0)
		return false;

	int current_region = (current_index * sizeof(SDK::AActor*)) / 0x1000;

	//DBG_PRINT("current_region -> %i\n", current_region);

	if (current_region >= this->m_allocated_regions.size())
		return false;

	int new_index = ((current_index * sizeof(SDK::AActor*)) % 0x1000) / 8;

	//DBG_PRINT("new_index -> %i\n", new_index);

	if (new_index >= 512)
		return false;


	//DBG_PRINT("out_actor -> 0x%llX\n", this->m_allocated_regions.at(current_region)[new_index]);

	if (out_actor)
		*out_actor = this->m_allocated_regions.at(current_region)[new_index];

	current_index++;

	return true;
}

void hActorIterator::Free()
{
	for (auto& region : this->m_allocated_regions)
	{
		if (region)
			VirtualFree(region, 0x1000, MEM_RELEASE);
	}

	this->m_allocated_regions.resize(0);
}

bool HackMain::UpdateLocalInformation()
{
	VMProtectBeginMutation("hMain.UpdateLocalInfo");
	// m_pLocalShooterCharacter gets updated in the function
	this->GetLocalShooterCharacter();

	if (!m_pLocalShooterCharacter)
		return false;

	m_LocalShooterCharacter = m_pLocalShooterCharacter->UpdateInformation();

	if (m_LocalShooterCharacter.m_pTribeName && m_LocalShooterCharacter.m_pTribeNameLen > 1 && m_LocalShooterCharacter.m_pTribeNameLen <= 64)
		m_LocaTribeName = m_LocalShooterCharacter.m_pTribeName->UpdateInformation().Buffer;
	else
		m_LocaTribeName = L"";

	//Getting the camera

		//Gets read in GetLocalShooterCharacter
	if (m_LocalPlayerController.m_pPlayerCameraManager)
	{
		m_pPlayerCameraManager = m_LocalPlayerController.m_pPlayerCameraManager;
		m_PlayerCameraManager = m_pPlayerCameraManager->UpdateInformation();
	}

	//m_pUCanvas = SDK::UCanvasProxy::GetCanvas();
	m_pGNamesManager = SDK::GNamesProxy::GetInstance();

	/*if (m_LocalShooterCharacter.m_pCurrentWeapon)
	{
		SDK::AShooterWeapon weapon_instance = m_LocalShooterCharacter.m_pCurrentWeapon->UpdateInformation();

		std::wstring weapon_name = m_LocalShooterCharacter.m_pCurrentWeapon->get_name(nullptr);

			if (SDK::Functions::IsInstantWeapon(weapon_instance, weapon_name))
				this->m_IsInstantWeapon = true;
	}*/


	VMProtectEnd();

	return true;
}

bool HackMain::UpdateEntities()
{
	if (!m_pUWorld || !m_UWorld.m_pPersistantLevel)
		return false;

	SDK::PersistentLevel PersistentLevel = m_UWorld.m_pPersistantLevel->UpdateInformation();

	/*static int level = 0;
	if (GetAsyncKeyState(VK_LEFT) & 1)
		level--;
	if (GetAsyncKeyState(VK_RIGHT) & 1)
		level++;

	DBG_PRINT("level : %i\n", level);

	SDK::PersistentLevel PersistentLevel = m_pUWorld->GetLevel(level)->UpdateInformation();*/

	if (PersistentLevel.m_ActorCount <= 1 || !PersistentLevel.m_pAActors || PersistentLevel.m_ActorCount >= 100000)
		return false;

	//if (!eHackMain.m_ActorItterator.UpdateInformation(PersistentLevel))
		//return false;

	//system("cls");

	SDK::AActor* CurrentActor = nullptr;
	m_LocalHumanoidCount = 0; m_LocalNPCCount = 0; m_LocalStructureCount = 0;

	int timer = clock();

	//test_actors.clear();



	for (int i = 0; i < PersistentLevel.m_ActorCount; i++)//while (eHackMain.m_ActorItterator.Next(&CurrentActor))
	{
		if (!PersistentLevel.m_pAActors)
			break;

		CurrentActor = PersistentLevel.m_pAActors->GetActorByIndex(i);

		if (!CurrentActor)
			continue;




		SDK::APrimalCharacter* pPrimalCharacter = (SDK::APrimalCharacter*)CurrentActor;
		SDK::APrimalStructure* pPrimalStructure = (SDK::APrimalStructure*)CurrentActor;

		SDK::APrimalCharacter PrimalCharacter = pPrimalCharacter->UpdateInformation();

		if (PrimalCharacter.IsValid(&PrimalCharacter))
		{
			float Distance = Math::GetDistance(eHackMain.m_LocalShooterCharacter.m_fCurrentRootLoc, PrimalCharacter.m_fCurrentRootLoc);

			//	DBG_PRINT("%ls -> %f\n", PrimalCharacter.m_pDescriptiveName->UpdateInformation().Buffer, PrimalCharacter.m_Health);

			//std::wstring buffer = PrimalCharacter.m_pDescriptiveName->UpdateInformation().Buffer;

			if (PrimalCharacter.m_DescriptiveNameLen <= 0 || PrimalCharacter.m_DescriptiveNameLen >= 64)
				continue;

			wchar_t name[64] = { 0 };
			RtlZeroMemory(name, 64 * sizeof(wchar_t));

			PrimalCharacter.m_pDescriptiveName->read_name(name, PrimalCharacter.m_DescriptiveNameLen);

			name[63] = L'\0';

			if (PrimalCharacter.bIsNPC)
			{
				//Dino
				if (Distance > MAX_NPC_DIST)
					continue;

				std::wstring sname = name;
				this->AddNPCCharacter((SDK::APrimalDinoCharacter*)pPrimalCharacter, sname, Distance);
			}
			else if (std::wcscmp(name, L"PrimalCharacter") == 0)
			{
				//Player
				//if (!eMenu.m_3DRadarFeatures.PlayerSettings.DrawLocalPlayer)
				{
					if (m_pLocalShooterCharacter == pPrimalCharacter)
						continue;
				}

				if (!eMenu.m_3DRadarFeatures.PlayerSettings.DrawDead && PrimalCharacter.bIsDead)
					continue;

				this->AddHumanoid((SDK::AShooterCharacter*)pPrimalCharacter, Distance);
			}
		}
		else
		{
			//Structure
			SDK::APrimalStructure PrimalStructure = pPrimalStructure->UpdateInformation();

			if (!PrimalStructure.m_pRootComponent)
				continue;

			SDK::USceneComponent sRootComp = PrimalStructure.m_pRootComponent->UpdateInformation();

			//if (sRootComp.m_RelativeLocation == NULL)
			//	continue;

			float Distance = Math::GetDistance(eHackMain.m_LocalShooterCharacter.m_fCurrentRootLoc, sRootComp.m_RelativeLocation);


			//if(PrimalStructure.m_DescriptiveNameLength > 0 && PrimalStructure.m_DescriptiveNameLength < 64)
			//	DBG_PRINT("Structure %ls (0x%llX)\n", PrimalStructure.m_pDescriptiveName->UpdateInformation().Buffer, pPrimalStructure);

			this->AddStructure((SDK::APrimalStructure*)pPrimalCharacter, Distance);
		}

		if (clock() - timer > 5000)
			break;
	}

	m_aHumanoidCount = m_LocalHumanoidCount;
	m_aNPCCount = m_LocalNPCCount;
	m_aStructureCount = m_LocalStructureCount;

	//eHackMain.m_ActorItterator.ClearRegions();

	return true;
}

bool HackMain::IsStructureInFilter(std::wstring& descriptive_name, eHackStructureType* type)
{
	for (auto& filter : this->m_StructureFilter)
	{
		std::wstring str = descriptive_name;
		if (str.find(filter.DescriptiveName) != std::string::npos)
		{
			if (type)
				*type = filter.StructureType;

			return *(filter.pEnabled);
		}
	}

	return false;
}

void HackMain::AddNPCCharacter(SDK::APrimalDinoCharacter* pDinoCharacter, std::wstring& descriptive_name, float& distance)
{
	sHackNPC current_npc;

	if (!pDinoCharacter)
		return;

	if (m_LocalNPCCount >= (MAX_DINOS - 1))
		return;

	if (distance >= MAX_NPC_DIST)
		return;

	current_npc.pPrimalDinoCharacter = pDinoCharacter;
	current_npc.PrimalDinoCharacter = pDinoCharacter->UpdateInformation();

	if (current_npc.PrimalDinoCharacter.bIsDead)
		return;

	if (eMenu.m_3DRadarFeatures.DinoSettings.DrawOnlyTamedDinos && !current_npc.PrimalDinoCharacter.IsTamed(&current_npc.PrimalDinoCharacter))
		return;

	float new_dist = (distance / 100);

	//DBG_PRINT("new_dist -> %i (%i)\n", new_dist, eMenu.m_3DRadarFeatures.DinoSettings.MaxDrawLandCarnivoreDistance);
	bool is_valid_land_carnevour = (current_npc.PrimalDinoCharacter.bIsCarnivore && !current_npc.PrimalDinoCharacter.bIsWaterDino);

	bool is_valid_water_carnevour = (current_npc.PrimalDinoCharacter.bIsCarnivore && current_npc.PrimalDinoCharacter.bIsWaterDino);

	bool is_valid_land_herbevour = (!current_npc.PrimalDinoCharacter.bIsCarnivore && !current_npc.PrimalDinoCharacter.bIsWaterDino);

	bool is_valid_water_herbevour = (!current_npc.PrimalDinoCharacter.bIsCarnivore && current_npc.PrimalDinoCharacter.bIsWaterDino);


	if (!eMenu.m_3DRadarFeatures.DinoSettings.DrawLandCarnivores && is_valid_land_carnevour)
		return;

	if (!eMenu.m_3DRadarFeatures.DinoSettings.DrawWaterCarnivores && is_valid_water_carnevour)
		return;

	if (!eMenu.m_3DRadarFeatures.DinoSettings.DrawLandHerbivores && is_valid_land_herbevour)
		return;

	if (!eMenu.m_3DRadarFeatures.DinoSettings.DrawWaterHerbivores && is_valid_water_herbevour)
		return;

	if (is_valid_land_carnevour && new_dist >= eMenu.m_3DRadarFeatures.DinoSettings.MaxDrawLandCarnivoreDistance)
		return;

	if (is_valid_water_carnevour && new_dist >= eMenu.m_3DRadarFeatures.DinoSettings.MaxDrawWaterCarnivoreDistance)
		return;

	if (is_valid_land_herbevour && new_dist >= eMenu.m_3DRadarFeatures.DinoSettings.MaxDrawLandHerbivoreDistance)
		return;

	if (is_valid_water_herbevour && new_dist >= eMenu.m_3DRadarFeatures.DinoSettings.MaxDrawWaterHerbivoreDistance)
		return;

	if (current_npc.PrimalDinoCharacter.m_pTribeName)
		current_npc.TribeName = current_npc.PrimalDinoCharacter.m_pTribeName->UpdateInformation().Buffer;

	if (this->m_LocaTribeName.length() > 1)
	{
		current_npc.IsLocalTribe = (wcscmp(current_npc.TribeName.c_str(), this->m_LocaTribeName.c_str()) == 0);

		if (!eMenu.m_3DRadarFeatures.DinoSettings.DrawTribeDinos && current_npc.IsLocalTribe)
			return;

	}
	else
	{
		current_npc.IsLocalTribe = false;
	}

	if (!current_npc.PrimalDinoCharacter.m_pRootComponent || !current_npc.PrimalDinoCharacter.m_pMesh)
		return;

	if (current_npc.PrimalDinoCharacter.m_pRootComponent->UpdateInformation().m_RelativeLocation == NULL)
		return;

	current_npc.SkeletalMesh = current_npc.PrimalDinoCharacter.m_pMesh->UpdateInformation();
	current_npc.DescriptiveName = descriptive_name;

	if (!eBoneMap.GetHeadBoneIndex(current_npc.SkeletalMesh, descriptive_name, &current_npc.HeadBoneIndex))
		return;

	if (!current_npc.PrimalDinoCharacter.m_pCharacterStatusComponent)
		return;

	current_npc.StatusComponent = current_npc.PrimalDinoCharacter.m_pCharacterStatusComponent->UpdateInformation();

	current_npc.Distance = distance;



	//current_npc.IsLocalTribe = (strc)


	this->m_aNPCList[m_LocalNPCCount] = current_npc;
	m_LocalNPCCount++;
}

void HackMain::AddHumanoid(SDK::AShooterCharacter* pShooterCharacter, float& distance)
{
	sHackHumanoid player;

	//DBG_PRINT("Adding humanoid 0x%llX\n", pShooterCharacter);

	if (m_LocalHumanoidCount >= (MAX_HUMANOIDS - 1) || !pShooterCharacter)
		return;

	player.pShooterCharacter = pShooterCharacter;
	player.ShooterCharacter = pShooterCharacter->UpdateInformation();

	if (!player.ShooterCharacter.m_pCharacterStatusComponent || !player.ShooterCharacter.m_pMesh || !player.ShooterCharacter.m_PlayerName)
		return;

	player.StatusComponent = player.ShooterCharacter.m_pCharacterStatusComponent->UpdateInformation();
	player.SkeletalMesh = player.ShooterCharacter.m_pMesh->UpdateInformation();

	if (player.ShooterCharacter.m_pTribeName)
	{
		player.TribeName = player.ShooterCharacter.m_pTribeName->UpdateInformation().Buffer;

		if (this->m_LocaTribeName.length() > 1)
			player.IsLocalTribe = (wcscmp(player.TribeName.c_str(), this->m_LocaTribeName.c_str()) == 0);
		else
			player.IsLocalTribe = false;

	}


	player.PlayerName = player.ShooterCharacter.m_PlayerName->UpdateInformation().Buffer;

	if (distance <= 0)
		return;

	player.Distance = distance;

	this->m_aHumanoidList[m_LocalHumanoidCount] = player;
	m_LocalHumanoidCount++;

	//DBG_PRINT("Added humanoid 0x%llX\n", pShooterCharacter);
}

void HackMain::AddStructure(SDK::APrimalStructure* pPrimalStructure, float& distance)
{
	if (m_LocalStructureCount >= (MAX_STRUCTURES - 1))
		return;

	if (distance / 100 >= MAX_STRUCT_DIST)
		return;

	sHackStructure structure;

	structure.pPrimalStructure = pPrimalStructure;
	structure.PrimalStructure = pPrimalStructure->UpdateInformation();

	if (structure.PrimalStructure.m_DescriptiveNameLength > 64 || structure.PrimalStructure.m_DescriptiveNameLength <= 0 || !structure.PrimalStructure.m_pDescriptiveName)
		return;

	structure.DescriptiveName = structure.PrimalStructure.m_pDescriptiveName->UpdateInformation().Buffer;

#if(USE_DEBUGMODE)
	if (WinApi.GetAsyncKeyState(VK_F4))
	{
		printf("[Structure] %ls : 0x%llX\n", structure.DescriptiveName.c_str(), pPrimalStructure);
	}
#endif

	eHackStructureType type;
	if (!this->IsStructureInFilter(structure.DescriptiveName, &type))
		return;

	structure.StructureType = type;

	if (!structure.PrimalStructure.m_pRootComponent)
		return;

	structure.pSceneComponent = structure.PrimalStructure.m_pRootComponent;
	structure.SceneComponent = structure.PrimalStructure.m_pRootComponent->UpdateInformation();

	structure.Distnace = distance;

	SDK::APrimalStructureItemContainer* pContainerStructure = reinterpret_cast<SDK::APrimalStructureItemContainer*>(structure.pPrimalStructure);

	SDK::APrimalStructureItemContainer ItemContainer = pContainerStructure->UpdateInformation();

	structure.current_items = ItemContainer.CurrentItemCount;
	structure.max_items = ItemContainer.MaxItemCount;


	this->m_aStructureList[m_LocalStructureCount] = structure;
	m_LocalStructureCount++;
}

bool HackMain::Initialize()
{
	if (m_aHumanoidList && m_aNPCList && m_aStructureList)
		return true;

	m_aHumanoidList = new sHackHumanoid[MAX_HUMANOIDS];
	m_aNPCList = new sHackNPC[MAX_DINOS];
	m_aStructureList = new sHackStructure[MAX_STRUCTURES];


	if (!m_aHumanoidList || !m_aNPCList || !m_aStructureList)
		return false;

	//Storage
	m_StructureFilter.push_back({ HackStructureType_Storage, L"Giant Beaver Dam", &eMenu.m_3DRadarFeatures.StructureSettings.GiantBeaverDam });
	m_StructureFilter.push_back({ HackStructureType_Storage, L"Item Cache", &eMenu.m_3DRadarFeatures.StructureSettings.ItemCache });
	m_StructureFilter.push_back({ HackStructureType_Storage, L"Storage Box", &eMenu.m_3DRadarFeatures.StructureSettings.StorageBox });
	m_StructureFilter.push_back({ HackStructureType_Storage, L"Large Storage Box", &eMenu.m_3DRadarFeatures.StructureSettings.LargeStorageBox });
	m_StructureFilter.push_back({ HackStructureType_Storage, L"Vault", &eMenu.m_3DRadarFeatures.StructureSettings.Vault });
	m_StructureFilter.push_back({ HackStructureType_Storage, L"Cryofridge", &eMenu.m_3DRadarFeatures.StructureSettings.Cryofridge });
	m_StructureFilter.push_back({ HackStructureType_Storage, L"Dedicated Storage", &eMenu.m_3DRadarFeatures.StructureSettings.DedicatedStorage });
	m_StructureFilter.push_back({ HackStructureType_Storage, L"Preserving Bin", &eMenu.m_3DRadarFeatures.StructureSettings.PreservingBin });
	//Turrets
	m_StructureFilter.push_back({ HackStructureType_Turret, L"Automated Turret", &eMenu.m_3DRadarFeatures.StructureSettings.AutoTurret });

	m_StructureFilter.push_back({ HackStructureType_Turret, L"Heavy Automated Turret", &eMenu.m_3DRadarFeatures.StructureSettings.HeavyTurret });
	m_StructureFilter.push_back({ HackStructureType_Turret, L"Heavy Auto Turret", &eMenu.m_3DRadarFeatures.StructureSettings.HeavyTurret });

	m_StructureFilter.push_back({ HackStructureType_Turret, L"Tek Turret", &eMenu.m_3DRadarFeatures.StructureSettings.TekTurret });
	//Loot
	m_StructureFilter.push_back({ HackStructureType_LootCrate, L"Loot Crate", &eMenu.m_3DRadarFeatures.StructureSettings.LootCrate });
	m_StructureFilter.push_back({ HackStructureType_Artifact,  L"Artifact", &eMenu.m_3DRadarFeatures.StructureSettings.Artifact });
	//Workstations
	m_StructureFilter.push_back({ HackStructureType_WorkStation, L"Smithy", &eMenu.m_3DRadarFeatures.StructureSettings.Smithy });
	m_StructureFilter.push_back({ HackStructureType_WorkStation, L"Fabricator", &eMenu.m_3DRadarFeatures.StructureSettings.Fabricator });
	m_StructureFilter.push_back({ HackStructureType_WorkStation, L"Mortar and Pestle", &eMenu.m_3DRadarFeatures.StructureSettings.MortarAndPestle });
	//Tek
	m_StructureFilter.push_back({ HackStructureType_Tek, L"Tek Transmitter", &eMenu.m_3DRadarFeatures.StructureSettings.TekTransmitter });
	m_StructureFilter.push_back({ HackStructureType_Tek, L"Tek Forcefield", &eMenu.m_3DRadarFeatures.StructureSettings.TekForcefield });
	m_StructureFilter.push_back({ HackStructureType_Tek, L"Tek Teleporter", &eMenu.m_3DRadarFeatures.StructureSettings.TekTeleporter });
	m_StructureFilter.push_back({ HackStructureType_Tek, L"Tek Replicator", &eMenu.m_3DRadarFeatures.StructureSettings.TekReplicator });
	//Others
	m_StructureFilter.push_back({ HackStructureType_Other, L"Power Generator", &eMenu.m_3DRadarFeatures.StructureSettings.Generator });
	m_StructureFilter.push_back({ HackStructureType_Tek, L"Tek Generator", &eMenu.m_3DRadarFeatures.StructureSettings.TekGenerator });

	return true;
}

bool HackMain::UpdateInformation()
{
	m_pUWorld = SDK::WorldProxy::GetInstance().m_pUWorld;

	if (!m_pUWorld)
	{
		this->Reset();
		return false;
	}

	if (m_pUWorld)
	{
		SDK::UWorld cWorld = m_pUWorld->UpdateInformation();

		if (cWorld.m_pOwningGameInstance)
			m_UWorld = cWorld;
	}

	this->UpdateLocalInformation();
	this->UpdateEntities();

	//printf("LocalCharacter : 0x%llX\n", eHackMain.m_pLocalPlayerController);
	//printf("m_pUSceneComponent : 0x%llX\n", eHackMain.m_PlayerCameraManager.m_pUSceneComponent);
	//printf("NPCs : %i\n", eHackMain.m_aNPCCount);

	return true;
}

void HackMain::Reset()
{
	//m_pLocalShooterCharacter = nullptr;
}

SDK::AShooterCharacter* HackMain::GetLocalShooterCharacter()
{
	if (!m_pUWorld)
		return nullptr;

	if (!m_UWorld.m_pOwningGameInstance)
		return nullptr;

	SDK::aLocalPlayers* pLocalPlayers = m_UWorld.m_pOwningGameInstance->GetLocalPlayers();

	if (!pLocalPlayers)
		return nullptr;

	SDK::Player* pLocalPlayer = pLocalPlayers->GetPlayerByIndex(0);

	if (!pLocalPlayer)
		return nullptr;

	m_pLocalPlayerController = pLocalPlayer->GetPlayerController();

	if (!m_pLocalPlayerController)
		return nullptr;

	m_LocalPlayerController = m_pLocalPlayerController->UpdateInformation();

	//m_pLocalShooterCharacter = this->m_pUWorld->GetLocalShooterCharacter();

	SDK::AShooterCharacter* pControlledCharacter = m_LocalPlayerController.m_pPrimalCharacter;

	if (!pControlledCharacter)
		return nullptr;

	SDK::AShooterCharacter ControlledCharacter = pControlledCharacter->UpdateInformation();

	if (!ControlledCharacter.m_pDescriptiveName)
		return nullptr;

	//Means that we are mounted on a DINO and we keep past reference of local player object
	if (wcscmp(ControlledCharacter.m_pDescriptiveName->UpdateInformation().Buffer, L"PrimalCharacter") == 0)
	{
		m_pLocalShooterCharacter = pControlledCharacter;
		m_LocalShooterCharacter = ControlledCharacter;
	}

	m_pLocalControledCharacter = pControlledCharacter;

	return m_pLocalShooterCharacter;
}