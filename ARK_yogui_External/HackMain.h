#pragma once
#include <Windows.h>
#include <vector>
#include "SDK.h"

#define MAX_HUMANOIDS 64
#define MAX_DINOS 126
#define MAX_STRUCTURES 126

#define MAX_NPC_DIST 100000.0f
#define MAX_STRUCT_DIST 500.0f

enum eHackStructureType
{
	HackStructureType_Storage = 0,
	HackStructureType_Turret = 1,
	HackStructureType_LootCrate = 2,
	HackStructureType_Artifact = 3,
	HackStructureType_WorkStation = 4,
	HackStructureType_Tek = 5,
	HackStructureType_Other = 6
};

class sHackHumanoid
{
public:
	SDK::AShooterCharacter* pShooterCharacter = nullptr;
	SDK::AShooterCharacter ShooterCharacter;

	SDK::USkeletalMeshComponent SkeletalMesh;

	SDK::UPrimalCharacterStatusComponent StatusComponent;

	std::wstring PlayerName;
	std::wstring TribeName;

	bool IsLocalTribe;
	float Distance;
};

struct sHackNPC
{
	SDK::APrimalDinoCharacter* pPrimalDinoCharacter = nullptr;
	SDK::APrimalDinoCharacter PrimalDinoCharacter;

	SDK::USkeletalMeshComponent SkeletalMesh;

	SDK::UPrimalCharacterStatusComponent StatusComponent;

	std::wstring DescriptiveName;
	std::wstring TribeName;

	int HeadBoneIndex;

	bool IsLocalTribe;
	float Distance;
};

struct sHackStructure
{
	eHackStructureType StructureType;

	SDK::APrimalStructure PrimalStructure;
	SDK::APrimalStructure* pPrimalStructure;


	SDK::USceneComponent SceneComponent;
	SDK::USceneComponent* pSceneComponent;

	int current_items, max_items;

	std::wstring DescriptiveName;
	float Distnace;
};

struct sStructureFilter
{
	eHackStructureType StructureType;
	std::wstring DescriptiveName;
	bool* pEnabled;
};

struct sBoneMapEntry
{
	std::wstring DescriptiveName;
	int HeadBoneIndex;
};

class BoneMap;
extern BoneMap eBoneMap;

class BoneMap
{
private:
	std::vector<sBoneMapEntry> m_BoneEntries;
public:
	bool FindEntry(std::wstring& name, sBoneMapEntry* pOutEntry);
	bool GetHeadBoneIndex(SDK::USkeletalMeshComponent& Instance, std::wstring& DescName, int* pOutIndex);
};

class hActorIterator
{
private:
	int saved_actors = 0;
	int current_index = 0;
public:
	std::vector<SDK::AActor**> m_allocated_regions;
public:
	bool UpdateInformation(SDK::PersistentLevel& persistent_level);
	bool Next(SDK::AActor** out_actor);

	void Free();
};

class HackMain;
extern HackMain eHackMain;

class HackMain
{
private:
	int m_LocalNPCCount;
	int m_LocalStructureCount;
	int m_LocalHumanoidCount;
public:
	hActorIterator m_ActorItterator;

	SDK::AShooterCharacter* m_pLocalShooterCharacter = nullptr;
	SDK::AShooterCharacter m_LocalShooterCharacter;

	SDK::AShooterCharacter* m_pLocalControledCharacter = nullptr;

	SDK::PlayerController* m_pLocalPlayerController = nullptr;
	SDK::PlayerController m_LocalPlayerController;

	SDK::APlayerCameraManager* m_pPlayerCameraManager = nullptr;
	SDK::APlayerCameraManager m_PlayerCameraManager;

	SDK::UCanvas* m_pUCanvas = nullptr;
	SDK::UCanvas m_UCanvas;

	SDK::UWorld* m_pUWorld = nullptr;
	SDK::UWorld m_UWorld;

	SDK::GNamesManager* m_pGNamesManager = nullptr;

	SDK::AWorldSettings* m_pAWorldSettings = nullptr;

	std::wstring m_LocaTribeName;

	bool m_IsInstantWeapon = false;

	int m_aHumanoidCount;
	int m_aNPCCount;
	int m_aStructureCount;

	sHackHumanoid* m_aHumanoidList = nullptr;
	sHackNPC* m_aNPCList = nullptr;
	sHackStructure* m_aStructureList = nullptr;

	std::vector<sStructureFilter> m_StructureFilter;

	DWORD proc_id;
private:
	SDK::AShooterCharacter* GetLocalShooterCharacter();
	bool UpdateLocalInformation();
	bool UpdateEntities();

	bool IsStructureInFilter(std::wstring& descriptive_name, eHackStructureType* type);

	void AddNPCCharacter(SDK::APrimalDinoCharacter* pDinoCharacter, std::wstring& descriptive_name, float& distance);
	void AddHumanoid(SDK::AShooterCharacter* pShooterCharacter, float& distance);
	void AddStructure(SDK::APrimalStructure* pPrimalStructure, float& distance);
public:
	bool Initialize();
	bool UpdateInformation();

	void Reset();

public:


};

int main_thread();