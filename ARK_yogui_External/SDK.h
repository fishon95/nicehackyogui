#pragma once
#include <Windows.h>
#include "Globals.h"
#include "Driver.h"
#include "DirectXIncludes.h"

#include <string>

#define FUNCTION_UPDATEINFORMATION(x) inline x UpdateInformation() {if(this == nullptr) {return x();} return eDriver.read_mem<x>(this);}
#define FUNCTION_SETINFORMATION(x ,y) inline bool SetInformation(const x & inst) {if(this == nullptr) {return false;} return eDriver.write_mem<x>((PVOID)((uintptr_t)this + y), inst);}
#define FUNCTION_GETMEMBER(z, x, y) inline x Get##z() {if(this == nullptr) {return NULL;} return eDriver.read_mem<x>((PVOID)((uintptr_t)this + y));}
#define FUNCTION_SETMEMBER(z, c, x) inline bool Set##z(const c & arg) {if(this == nullptr) {return false;} return eDriver.write_mem<c>((PVOID)((uintptr_t)this + x), arg);}

namespace Offsets
{
	namespace ShooterGame
	{
		//const DWORD WorldProxy = 0x42B0F68;

		extern uintptr_t WorldProxy;
		extern uintptr_t UCanvasProxy;
		extern uintptr_t GNamesProxy;

		extern uintptr_t GUObjectArray;

		
	}

	namespace Classes
	{
		namespace UWorld
		{
			const DWORD PersistentLevel = 0xF8;
			const DWORD OwningGameInstance = 0x290;

			//Found by ptr scanning for local player, because other 
			const std::vector<DWORD> pLocalShooterCharacter = { 0x1C0, 0x0, 0xC0 };
		}
		namespace UCanvasProxy
		{
			const std::vector<DWORD> UCanvas = { 0x0, 0x20 };
		}
		namespace UWorldLevels
		{
			const DWORD Actors = 0x88;
			const DWORD ActorCount = 0x90;
		}
		namespace GameInstance
		{
			const DWORD LocalPlayers = 0x38;
		}
		namespace Player
		{
			const DWORD PlayerController = 0x30;
		}
		namespace PlayerController
		{
			const DWORD ShooterCharacter = 0x4D0;
			const DWORD PlayerCameraManager = 0x4F0;
			const DWORD StructurePlacer = 0x9C8;
			const DWORD ShooterHud = 0x4E8;
			const DWORD bLockedInputUI = 0x6A4;
		}
		namespace AController
		{
			const DWORD ControlRotation = 0x490;
		}
		namespace AActor
		{
			const DWORD RootComponent = 0x250;
		}
		namespace APawn
		{
			const DWORD PlayerState = 0x488;
		}
		namespace ACharacter
		{
			const DWORD Mesh = 0x4F8;
			const DWORD CharacterMovement = 0x500;
		}
		namespace APrimalCharacter
		{
			const DWORD TribeName = 0x790;
			const DWORD DescriptiveName = 0xBE8;
		}
		namespace AShooterCharacter
		{
			const DWORD PlayerName = 0x14B0;
			const DWORD RidingDino = 0x14C0;
			const DWORD Mesh1P = 0x14C8;
			const DWORD CurrentWeapon = 0x1708;
		}
		namespace APlayerCameraManager
		{
			const DWORD USceneComponent = 0x470;
			const DWORD FreeCamDistance = 0x1388;
		}
		namespace USkeletalMeshComponent
		{
			const DWORD SpaceBases = 0x680;
			const DWORD Materials = 0x678;
		}
		namespace AShooterWeapon
		{
			const DWORD UseFireCameraShakeScale = 0xCC4;
			const DWORD pFireCameraShake = 0x8F0;
			const DWORD pScopeOverlayMI = 0xB20;
			const DWORD CurrentAmmoInClip = 0xAB4;
			const DWORD pReloadCameraShake = 0x8F8;
			const DWORD pWeaponAmmoItemTemplate = 0x818;
			const DWORD pAssociatedPrimalItem = 0x618;
			const DWORD InstantConfig = 0xCC8;
			const DWORD ReloadAnim = 0x940;
			const DWORD WeaponData = 0x7F8;
			//AIM-DRIFT
			const DWORD AimDriftAngle = 0xB64;
			const DWORD AimDriftFrequency = 0xB6C;
		}
		namespace APrimalStructurePlacer
		{
			const DWORD CurrentPlacingStructure = 0x480;
		}
		namespace APrimalStructure
		{
			const DWORD PlacementRotOffset = 0x640;
		}
		namespace UCharacterMovementComponent
		{
			const DWORD RotationRate = 0x238;
			const DWORD AngleToStartRotationBraking = 0x3B8;
		}
		namespace APrimalWeaponBow
		{
			const DWORD PullingTimeForMaximumSpeed = 0xE38;
			const DWORD MinimumPullingTimeToFire = 0xE44;
			const DWORD m_pMaxPullCameraShake = 0xE50;
		}

		namespace UPrimalItem
		{
			const DWORD ItemCustomClass = 0x220;
			const DWORD DescriptiveNameBase = 0x328;
		}

		namespace AWorldSettings
		{
			const DWORD WorlGravityZ = 0xA10;
		}

		namespace APrimalWeaponGrenade
		{
			const DWORD CookingTime = 0xE7C;
		}
		namespace APrimalStructureTurret
		{
			const DWORD PlayerProneTargetOffset = 0xE98;
		}
		namespace ShooterHUD
		{
			const DWORD CurrentOpenedInventory = 0xA88;
		}
		namespace PrimalInventoryComponent
		{
			const DWORD StructureType = 0x17C;
			const DWORD CraftingItemSpeed = 0x194;
		}
	}
}


namespace SDK
{
	class WorldProxy;
	class UWorld;
	class GameInstance;
	class aLocalPlayers;
	class Player;
	class PlayerController;
	class APrimalCharacter;
	class ACharacter;
	class FString;
	class PersistentLevel;
	class UWorldAActors;
	class ActorIterator;
	class AActor;
	class USceneComponent;
	class FUObjectArray;
	class FChunkFixedUObjectArray;
	class UObjectArray_Object;
	class AShooterCharacter;
	class USkeletalMeshComponent;
	class TArrayLowerBodyNames;
	class AShooterWeapon;
	class UCharacterMovementComponent;
	class APawn;
	class APlayerState;
	class APrimalDinoCharacter;
	class APlayerCameraManager;
	class UCanvas;
	class FTransform;
	class APrimalStructure;
	class BoneMaterials;
	class UMovementComponent;
	class UNavMovementComponent;
	class UPawnMovementComponent;
	class APrimalStructurePlacer;

	class AShooterWeapon_Projectile;
	class APrimalWeaponBow;

	class UObject_FName
	{
	public:
		__int32 ComparisonIndex; //0x0000 
		__int32 Number; //0x0004 

	}; //Size=0x0008


	//Enums
	namespace Enums
	{

		namespace FItemDescriptiveNames
		{
			const std::wstring bow = L"Bow";
			const std::wstring compound_bow = L"Compound Bow";
			const std::wstring tek_bow = L"Tek Bow";
			const std::wstring cross_bow = L"Crossbow";

			const std::wstring bola = L"Bola";
			const std::wstring tek_rifle = L"Tek Rifle";
			const std::wstring rocket_launcher = L"Rocket Launcher";
			const std::wstring longneck_rifle = L"Longneck Rifle";
		};

		namespace ClassNames
		{
			const std::string tranq_dart = "PrimalItemAmmo_TranqDart_C";
			const std::string refined_tranq_dart = "PrimalItemAmmo_RefinedTranqDart_C";
		};

		enum EPrimalItemType : uint8_t
		{
			EPrimalItemType__MiscConsumable = 0,
			EPrimalItemType__Equipment = 1,
			EPrimalItemType__Weapon = 2,
			EPrimalItemType__Ammo = 3,
			EPrimalItemType__Structure = 4,
			EPrimalItemType__Resource = 5,
			EPrimalItemType__Skin = 6,
			EPrimalItemType__WeaponAttachment = 7,
			EPrimalItemType__Artifact = 8,
			EPrimalItemType__MAX = 9,
			EPrimalItemType__EPrimalItemType_MAX = 10
		};

		enum _ProjectileInfo : const int
		{
			ProjectileInfo_Dynamic,
			ProjectileInfo_Static,
			ProjectileInfo_AmmoType
		};
	}

	struct FMinimalViewInfo;
	//Functions
	namespace Functions
	{
		bool InitializeOffsets();
		bool WorldToScreen(SDK::UCanvas& Canvas, const Vec3& Position, Vec2* OutPos);

		void update_matrix(SDK::FMinimalViewInfo& MinimalViewInfo);

		bool GetBonePosition(SDK::USkeletalMeshComponent& Instance, int BoneIndex, Vec3* OutPos);
		bool GetBonePosition(SDK::USkeletalMeshComponent& Instance, FTransform* pBoneTransforms, int BoneIndex, Vec3* OutPos);
		bool GetBoneHeadIndex(SDK::BoneMaterials* pBoneMaterials, int* pOutIndex);

		bool IsInstantWeapon(SDK::AShooterWeapon& WeaponInstance, std::wstring& weapon_name);
	}

#define ARK_MALEBONES_RIGHTLEG 90, 88, 87, 1
#define ARK_MALEBONES_LEFTLEG 84, 82, 81, 1
#define ARK_MALEBONES_CHEST 1, 7
#define ARK_MALEBONES_RIGHTARM 62, 60, 57, 7
#define ARK_MALEBONES_LEFTARM 38, 36,33, 7

#define ARK_BONES_HEAD 8
#define ARK_BONES_PELVIS 1
#define ARK_BONES_NECK 7

#define ARK_FEMALEBONES_RIGHTLEG 92, 90, 89, 1
#define ARK_FEMALEBONES_LEFTLEG 86, 84, 83, 1
#define ARK_FEMALEBONES_CHEST 1, 7
#define ARK_FEMALEBONES_RIGHTARM 64, 62, 59, 7
#define ARK_FEMALEBONES_LEFTARM 38, 36,33, 7

	const std::vector<int> ARK_MALE_BONELIST = {
		ARK_MALEBONES_RIGHTLEG, -1, ARK_MALEBONES_LEFTLEG, -1, ARK_MALEBONES_CHEST, -1, ARK_MALEBONES_RIGHTARM,-1, ARK_MALEBONES_LEFTARM
	};
	const std::vector<int> ARK_FEMALE_BONELIST = {
		ARK_FEMALEBONES_RIGHTLEG, -1, ARK_FEMALEBONES_LEFTLEG, -1, ARK_FEMALEBONES_CHEST, -1, ARK_FEMALEBONES_RIGHTARM, -1, ARK_FEMALEBONES_LEFTARM
	};

	struct FTransform {
		D3DXVECTOR4 Rotation;
		D3DXVECTOR4 Translation;
		D3DXVECTOR4 Scale;

		D3DXMATRIX ToMatrixWithScale() {
			D3DXMATRIX m;

			ZeroMemory(&m, sizeof(D3DXMATRIX));

			float x2 = Rotation.x + Rotation.x;
			float y2 = Rotation.y + Rotation.y;
			float z2 = Rotation.z + Rotation.z;

			float xx2 = Rotation.x * x2;
			float yy2 = Rotation.y * y2;
			float zz2 = Rotation.z * z2;

			float yz2 = Rotation.y * z2;
			float wx2 = Rotation.w * x2;

			float xy2 = Rotation.x * y2;
			float wz2 = Rotation.w * z2;

			float xz2 = Rotation.x * z2;
			float wy2 = Rotation.w * y2;

			m._41 = Translation.x;
			m._42 = Translation.y;
			m._43 = Translation.z;
			m._44 = 1.0f;

			m._11 = (1.0f - (yy2 + zz2)) * Scale.x;
			m._22 = (1.0f - (xx2 + zz2)) * Scale.y;
			m._33 = (1.0f - (xx2 + yy2)) * Scale.z;

			m._32 = (yz2 - wx2) * Scale.z;
			m._23 = (yz2 + wx2) * Scale.y;

			m._21 = (xy2 - wz2) * Scale.y;
			m._12 = (xy2 + wz2) * Scale.x;

			m._31 = (xz2 + wy2) * Scale.z;
			m._13 = (xz2 - wy2) * Scale.x;

			return m;
		}
	};

	class UClass;


	

	class UObjectBase
	{
	public:
		PVOID p_vfptr; //0x0000 
		__int32 ObjectFlags; //0x0008 
		__int32 InternalIndex; //0x000C 
		UClass* pClass; //0x0010 
		UObject_FName Name; //0x0018 
		PVOID pOuter; //0x0020 
	public:
		FUNCTION_UPDATEINFORMATION(UObjectBase);

	}; //Size=0x0028

	class UField : public UObjectBase
	{
	public:
		char pad_0x0028[0x20]; //0x0028
		UField* pNext; //0x0048 
		char pad_0x0050[0x8]; //0x0050

	}; //Size=0x0058

	class UStruct : public UField
	{
	public:
		char pad_0x0058[0x28]; //0x0058
		UStruct* pSuperStruct; //0x0080 
		UStruct* pChildren; //0x0088 
		char pad_0x0090[0x40]; //0x0090

	}; //Size=0x00D0

	class UClass : public UStruct
	{
	public:
		char pad_0x00D0[0x38]; //0x00D0
	public:
		FUNCTION_UPDATEINFORMATION(UClass);
	};

	class FString
	{
	public:
		wchar_t Buffer[128]; //0x0000 
	public:
		FUNCTION_UPDATEINFORMATION(FString);

		void read_name(wchar_t* buffer, int len)
		{
			eDriver.read_in_buffer(this, buffer, sizeof(wchar_t) * len);
		}
	};


	class WorldProxy
	{
	public:
		UWorld* m_pUWorld; //0x0000 
	public:
		static WorldProxy GetInstance()
		{
			return eDriver.read_mem<WorldProxy>((PVOID)Offsets::ShooterGame::WorldProxy);
		}

	};

	class UWorld
	{
	public:
		char pad_0x0000[0xF8]; //0x0000
		PersistentLevel* m_pPersistantLevel; //0x00F8 
		char pad_0x0100[0x168]; //0x0100
		VOID* m_pLevels; //0x0268 
		char pad_0x0270[0x8]; //0x0270
		VOID* m_pNetworkActors; //0x0278 
		char pad_0x0280[0x10]; //0x0280
		GameInstance* m_pOwningGameInstance; //0x0290 
		char pad_0x0298[0x2C0]; //0x0298
	public:
		FUNCTION_UPDATEINFORMATION(UWorld);

		FUNCTION_GETMEMBER(PersistentLevel, PersistentLevel*, Offsets::Classes::UWorld::PersistentLevel);
		FUNCTION_GETMEMBER(OwningGameInstance, GameInstance*, Offsets::Classes::UWorld::OwningGameInstance);

		PersistentLevel* GetLevel(int index)
		{
			SDK::PersistentLevel* pLevels = (SDK::PersistentLevel*)this->UpdateInformation().m_pLevels;

			return eDriver.read_mem<SDK::PersistentLevel*>(reinterpret_cast<PVOID>((uintptr_t)pLevels + 0x8 * index));
		}

		AShooterCharacter* GetLocalShooterCharacter()
		{
			if (this == nullptr)
				return nullptr;

			AShooterCharacter* pInstance = (AShooterCharacter*)this;
			for (auto& off : Offsets::Classes::UWorld::pLocalShooterCharacter)
				pInstance = eDriver.read_mem<AShooterCharacter*>((PVOID)((uintptr_t)pInstance + off));

			return pInstance;
		}

	};

	class PersistentLevel
	{
	public:
		char pad_0x0000[0x88]; //0x0000
		UWorldAActors* m_pAActors; //0x0088 
		__int32 m_ActorCount; //0x0090 
	public:
		FUNCTION_UPDATEINFORMATION(PersistentLevel);

		FUNCTION_GETMEMBER(ActorArray, UWorldAActors*, Offsets::Classes::UWorldLevels::Actors);
		FUNCTION_GETMEMBER(ActorCount, __int32, Offsets::Classes::UWorldLevels::ActorCount);

	}; //Size=0x0108

	class UWorldAActors
	{
	public:
		APrimalCharacter* GetActorByIndex(size_t index)
		{
			if (this == nullptr)
				return nullptr;

			return eDriver.read_mem<APrimalCharacter*>((PVOID)((uintptr_t)this + 0x8 * index));
		}

	}; //Size=0x03E8

	class AWorldSettings
	{
	public:
		float GetWorldGravityZ()
		{
			if (!this)
				return 0.0f;
			return eDriver.read_mem<float>((PVOID)((uintptr_t)this + Offsets::Classes::AWorldSettings::WorlGravityZ));
		}
	};

	class GameInstance
	{
	public:
		char pad_0x0000[0x38]; //0x0000
		aLocalPlayers* m_pLocalPlayers; //0x0038 
	public:
		FUNCTION_UPDATEINFORMATION(GameInstance);

		FUNCTION_GETMEMBER(LocalPlayers, aLocalPlayers*, Offsets::Classes::GameInstance::LocalPlayers);
	};

	class aLocalPlayers
	{
	public:
		Player* GetPlayerByIndex(size_t index)
		{
			if (this == nullptr)
				return nullptr;

			return eDriver.read_mem<Player*>((PVOID)((uintptr_t)this + 0x8 * index));
		}
	};



	class Player
	{
	public:
		char pad_0x0000[0x30]; //0x0000
		PlayerController* m_pPlayerController; //0x0030
	public:
		FUNCTION_UPDATEINFORMATION(Player);

		FUNCTION_GETMEMBER(PlayerController, PlayerController*, Offsets::Classes::Player::PlayerController);
	};

	class FName
	{
	public:
		int ComparisonIndex;
		int Unk;
		int Number;
	public:
		FUNCTION_UPDATEINFORMATION(FName);
	};

	class AActor
	{
	public:
		PVOID vTable;
		char pad_0x0000[0x90]; //0x0000 98
		AActor* m_pOwner; //0x0098 
		char pad_0x00A0[0x91]; //0x00A0
		unsigned char m_bNetDormacy; //0x0131 
		char pad_0x0132[0x6E]; //0x0132
		FName CustomTag; //0x01A0 
		char pad_0x01AC[0x6C]; //0x01AC
		__int32 m_TargetingTeam; //0x0218 
		char pad_0x021C[0x34]; //0x021C
		USceneComponent* m_pRootComponent; //0x0250 
		char pad_0x0258[0x218]; //0x0258
	public:
		FUNCTION_UPDATEINFORMATION(AActor);

		FUNCTION_GETMEMBER(RootComponent, USceneComponent*, Offsets::Classes::AActor::RootComponent);
	};

	class AController : public AActor
	{
	public:
		char pad_0x0470[0x8]; //0x0470
		ACharacter* m_pCharacter; //0x0478 
		PVOID* m_pPlayerState; //0x0480 
		APawn* m_pPawn; //0x0488 
		Vec3 ControlRotation; //0x0490 
		char pad_0x049C[0x2C]; //0x049C
	public:
		FUNCTION_UPDATEINFORMATION(AController);
		//FUNCTION_GETMEMBER(ControlRotation, Vec3, Offsets::Classes::AController::ControlRotation);
		FUNCTION_SETMEMBER(ControlRotation, Vec3, Offsets::Classes::AController::ControlRotation);
	};



	class UUI_Inventory
	{
	public:
		char pad_0x0000[0x599]; //0x0000
		unsigned char m_HasRemoteInventory; //0x0599 
		char pad_0x059A[0x2]; //0x059A
		__int32 IndexToInventoryComponent; //0x059C 
		char pad_0x05A0[0x80]; //0x05A0
	public:
		FUNCTION_UPDATEINFORMATION(UUI_Inventory);
	}; //Size=0x0620

	class AShooterHUD
	{
	public:
		FUNCTION_GETMEMBER(CurrentOpenedInventory, UUI_Inventory*, Offsets::Classes::ShooterHUD::CurrentOpenedInventory);

	}; //Size=0x0C40

	class PlayerController : public AController
	{
	public:
		char pad_0x04C8[0x8]; //0x04C8
		AShooterCharacter* m_pPrimalCharacter; //0x04D0 
		char pad_0x04D8[0x10]; //0x04D8
		AShooterHUD* m_pShooterHUD; //0x04E8 
		APlayerCameraManager* m_pPlayerCameraManager; //0x04F0 
		char pad_0x04F8[0xD0]; //0x04F8
		char pad_0x05C9[0xDB]; //0x05C9
		unsigned char bLockedInputUI; //0x06A4 
		char pad_0x06A5[0x1B3]; //0x06A5
		char pad_0x0860[0x170]; //0x0860
		APrimalStructurePlacer* m_pStructurePlacer; //0x09C8 
	public:
		FUNCTION_UPDATEINFORMATION(PlayerController);

		FUNCTION_GETMEMBER(ShooterCharacter, AShooterCharacter*, Offsets::Classes::PlayerController::ShooterCharacter);
		FUNCTION_GETMEMBER(PlayerCameraManager, APlayerCameraManager*, Offsets::Classes::PlayerController::PlayerCameraManager);
		FUNCTION_GETMEMBER(PrimalStructurePlacer, APrimalStructurePlacer*, Offsets::Classes::PlayerController::StructurePlacer);
		FUNCTION_GETMEMBER(ShooterHud, AShooterHUD*, Offsets::Classes::PlayerController::ShooterHud);
		FUNCTION_GETMEMBER(bLockedInputUI, bool, Offsets::Classes::PlayerController::bLockedInputUI);
	};

	class APawn : public AActor
	{
	public:
		char pad_0x0470[0x4]; //0x0470
		float m_BaseEyeHeight; //0x0474 
		char pad_0x0478[0x10]; //0x0478
		APlayerState* m_pPlayerState; //0x0488 
		char pad_0x0490[0x48]; //0x0490
		Vec3 m_ControlInputVector; //0x04D8 
		char pad_0x04E4[0x14]; //0x04E4
	public:
		FUNCTION_UPDATEINFORMATION(APawn);

		FUNCTION_GETMEMBER(PlayerState, APlayerState*, Offsets::Classes::APawn::PlayerState);
	};

	class ACharacter : public APawn
	{
	public:
		USkeletalMeshComponent* m_pMesh; //0x04F8 
		UCharacterMovementComponent* m_pCharacterMovement; //0x0500 
		char pad_0x0508[0x68]; //0x0508
		Vec3 m_BaseTranslationOffset; //0x0570 
		char pad_0x057C[0x8]; //0x057C
		float m_CrouchedEyeHeight; //0x0584 
		float m_ProneEyeHeight; //0x0588 
		unsigned char                                      bIsCrouched : 1;
		unsigned char                                      bIsProne : 1;
		unsigned char                                      bCanEverProne : 1;
		unsigned char                                      bCanEverCrouch : 1;
		unsigned char                                      bReplicateDesiredRotation : 1;
		unsigned char                                      bUseBPOverrideCharacterSound : 1;
		unsigned char                                      bUseBPOverrideCharacterParticle : 1;
		unsigned char                                      bPressedJump : 1;
		char pad_0x058D[0x13B]; //0x058D
	public:
		FUNCTION_UPDATEINFORMATION(ACharacter);

		FUNCTION_GETMEMBER(SkeletalMesh, USkeletalMeshComponent*, Offsets::Classes::ACharacter::Mesh);
		FUNCTION_GETMEMBER(CharacterMovement, UCharacterMovementComponent*, Offsets::Classes::ACharacter::CharacterMovement);
	};

	class UPrimalCharacterStatusComponent
	{
		//Not Reversed Fully
	public:
		char pad_0x0000[0x6CC]; //0x0000
		__int32 m_BaseCharacterLevel; //0x06CC 
		__int32 m_ExtraCharacterLevel; //0x06D0 
		//char pad_0x06D4[0x1F4]; //0x06D4
	public:
		FUNCTION_UPDATEINFORMATION(UPrimalCharacterStatusComponent);

	}; //Size=0x08C8

	class UPrimalInventoryComponent
	{
	public:
		//char pad_0x0000[0x118]; //0x0000
		//InventoryItemsArray* m_aInventoryItems; //0x0118 
		//__int32 InventoryItems_count; //0x0120 
		//__int32 InventoryItems_maxcount; //0x0124 
		//AEquipedItems* m_aEquippedItems; //0x0128 
		//__int32 EquippedItems_count; //0x0130 
		//__int32 EquippedItems_maxcount; //0x0134 
		//char pad_0x0138[0x44]; //0x0138
		//__int32 InventoryType; //0x017C 
		//char pad_0x0180[0x348]; //0x0180
	public:
		FUNCTION_SETMEMBER(InventoryType, int, Offsets::Classes::PrimalInventoryComponent::StructureType);
		FUNCTION_SETMEMBER(CraftingItemSpeed, float, Offsets::Classes::PrimalInventoryComponent::CraftingItemSpeed);

	}; //Size=0x04C8

	class APrimalCharacter : public ACharacter
	{
	public:
		char pad_0x06C8[0xC8]; //0x06C8
		FString* m_pTribeName; //0x0790 
		__int32 m_pTribeNameLen; //0x0798 
		char pad_0x079C[0xE8]; //0x079C
		//unsigned char m_IsSleeping; //0x0884 
		unsigned char                                      bIsSleeping : 1;
		unsigned char                                      bWantsToRun : 1;
		unsigned char                                      bActiveRunToggle : 1;
		unsigned char                                      bIsBeingDragged : 1;
		unsigned char                                      bDisableSpawnDefaultController : 1;
		unsigned char                                      bIsDragging : 1;
		unsigned char                                      bIsDraggingWithGrapHook : 1;
		unsigned char                                      bDeathKeepCapsuleCollision : 1;
		char pad_0x0885[0x13]; //0x0885
		//unsigned char m_IsDead; //0x0898 
		unsigned char                                      bForceIKOnDedicatedServer : 1;
		unsigned char                                      bIgnoreAllImmobilizationTraps : 1;
		unsigned char                                      bForceTriggerIgnoredTraps : 1;
		unsigned char                                      bIsImmobilized : 1;
		unsigned char                                      bCanIgnoreWater : 1;
		unsigned char                                      bIsDead : 1;
		unsigned char                                      ReplicateAllBones : 1;
		unsigned char                                      AutoStopReplicationWhenSleeping : 1;
		char pad_0x0899[0x8]; //0x0899
		//unsigned char m_DinoType; //0x08A1 
		unsigned char                                      UnknownData14 : 1;
		unsigned char                                      bDieIfLeftWater : 1;
		unsigned char                                      bIsAmphibious : 1;
		unsigned char                                      bUseAmphibiousTargeting : 1;
		unsigned char                                      bIsWaterDino : 1;
		unsigned char                                      bIsFlyerDino : 1;
		unsigned char                                      bIgnoreTargetingCarnivores : 1;
		unsigned char                                      UnknownData15 : 1;
		char pad_0x08A2[0x26]; //0x08A2
		//unsigned char m_IsMounted; //0x08C8 
		unsigned char                                      bIsMounted : 1;
		unsigned char                                      bPreventTargetingByTurrets : 1;
		unsigned char                                      bDelayFootstepsUnderMinInterval : 1;
		unsigned char                                      bSleepingDisableIK : 1;
		unsigned char                                      bRagdollRetainAnimations : 1;
		unsigned char                                      bUseBPAddedAttachments : 1;
		unsigned char                                      bCanPlayLandingAnim : 1;
		unsigned char                                      bOnlyHasRunningAnimationWhenWalking : 1;
		char pad_0x08C9[0x63]; //0x08C9
		float m_Health; //0x092C 
		float m_MaxHealth; //0x0930 
		float m_CurrentTorpor; //0x0934 
		float m_MaxTorpor; //0x0938 
		char pad_0x093C[0xC]; //0x093C
		//unsigned char m_IsNPC; //0x0948 
		unsigned char                                      UnknownData28 : 2;                                        // 0x0948(0x0001)
		unsigned char                                      bInRagdoll : 1;                                           // 0x0948(0x0001) (Edit, EditConst)
		unsigned char                                      bIsNPC : 1;                                               // 0x0948(0x0001) (Edit, EditConst)
		unsigned char                                      UnknownData29 : 4;                                        // 0x0948(0x0001)
		char pad_0x0949[0x277]; //0x0949
		Vec3 N0000037F; //0x0BC0 
		char pad_0x0BCC[0x1C]; //0x0BCC
		FString* m_pDescriptiveName; //0x0BE8 
		__int32 m_DescriptiveNameLen; //0x0BF0 
		char pad_0x0BF4[0xB0]; //0x0BF4
		Vec2 m_fCurrentAimRot; //0x0CA4 
		char pad_0x0CAC[0x4]; //0x0CAC
		Vec3 m_fCurrentRootLoc; //0x0CB0
		char pad_0x0CBC[0x14]; //0x0CBC
		UPrimalCharacterStatusComponent* m_pCharacterStatusComponent; //0x0CD0 
		char pad_0x0CD8[0x8]; //0x0CD8
		UPrimalInventoryComponent* m_pMyInventoryComponent; //0x0CE0 
		char pad_0x0CE8[0x460]; //0x0CE8
	public:
		FUNCTION_UPDATEINFORMATION(APrimalCharacter);

		FUNCTION_GETMEMBER(TribeName, FString*, Offsets::Classes::APrimalCharacter::TribeName);
		FUNCTION_GETMEMBER(DescriptiveName, FString*, Offsets::Classes::APrimalCharacter::DescriptiveName);

		//Call from ptr
		inline bool IsValid(SDK::APrimalCharacter* pInstance)
		{
			SDK::APrimalCharacter Instance;
			if (pInstance)
				Instance = *pInstance;
			else
				Instance = this->UpdateInformation();

			if (!Instance.m_pDescriptiveName)
				return false;

			if (Instance.m_DescriptiveNameLen <= 0 || Instance.m_DescriptiveNameLen > 64)
				return false;

			if (Instance.m_bNetDormacy == 0x0)
				return false;

			if (Instance.m_Health < 0.0f || isnan(Instance.m_Health))
				return false;

			if (Instance.m_MaxHealth <= 0.f || isnan(Instance.m_MaxHealth))
				return false;

			//	if (Instance.m_CurrentTorpor < 0.0f || isnan(Instance.m_CurrentTorpor))
			//		return false;

			return true;
		}

	};


	class AShooterCharacter : public APrimalCharacter
	{
	public:
		char pad_0x1148[0x97]; //0x1148 //97
		unsigned char m_bWasProne; //0x11DF 
		char pad_0x11E0[0x28]; //0x11E0
		unsigned char                                      bIsTargeting : 1;
		unsigned char                                      bIsCrafting : 1;
		unsigned char                                      bIsRiding : 1;
		unsigned char                                      bIsIndoors : 1;
		unsigned char                                      bIsClimbing : 1;
		unsigned char                                      bIsViewingInventory : 1;
		unsigned char                                      bUseAlternateFallBlendspace : 1;
		unsigned char                                      bIsFemale : 1;
		char pad_0x1209[0x2A7]; //0x1209
		FString* m_PlayerName; //0x14B0 
		__int32 m_PlayerNameLen; //0x14B8 
		char pad_0x14BC[0x4]; //0x14BC
		APrimalDinoCharacter* m_pRidingDino; //0x14C0 
		USkeletalMeshComponent* m_Mesh1P; //0x14C8 
		PVOID* m_aLowerBodyPartRootBones; //0x14D0 
		char pad_0x14D8[0x18C]; //0x14D8
		Vec3 m_WeaponBobMagnitude; //0x1664 
		Vec3 m_WeaponBobPeriods; //0x1670 
		Vec3 m_WeaponBobOffsets; //0x167C 
		char pad_0x1688[0x80]; //0x1688
		AShooterWeapon* m_pCurrentWeapon; //0x1708 
		char pad_0x1710[0x334]; //0x1710
		unsigned char m_IsServerAdmin; //0x1A44 
		char pad_0x1A45[0x1E3]; //0x1A45
		unsigned char m_bPreventWeaponMovementAnims; //0x1C28 
		char pad_0x1C29[0xDF]; //0x1C29
	public:
		FUNCTION_UPDATEINFORMATION(AShooterCharacter);

		FUNCTION_GETMEMBER(PlayerName, FString*, Offsets::Classes::AShooterCharacter::PlayerName);
		FUNCTION_GETMEMBER(RidingDino, APrimalDinoCharacter*, Offsets::Classes::AShooterCharacter::RidingDino);
		FUNCTION_GETMEMBER(Mesh1P, USkeletalMeshComponent*, Offsets::Classes::AShooterCharacter::Mesh1P);
		FUNCTION_GETMEMBER(CurrentWeapon, AShooterWeapon*, Offsets::Classes::AShooterCharacter::CurrentWeapon);
	public:

	};

	class APrimalDinoCharacter : public APrimalCharacter
	{
	public:
		char pad_0x1148[0x48]; //0x1148
		Vec3 m_RidingAttackExtraVelocity; //0x1190 
		char pad_0x119C[0x194]; //0x119C
		AShooterCharacter* m_pRider; //0x1330 
		char pad_0x1338[0x14D]; //0x1338
		unsigned char                                      bIsCarnivore : 1;
		unsigned char                                      bAllowRidingInWater : 1;
		unsigned char                                      bUsesGender : 1;
		unsigned char                                      bTargetEverything : 1;
		unsigned char                                      bTamedWanderHarvestNonUsableHarvesting : 1;
		unsigned char                                      bEnableTamedWandering : 1;
		unsigned char                                      bEnableTamedMating : 1;
		unsigned char                                      bCollectVictimItems : 1;
		char pad_0x1486[0x30A]; //0x1486
		__int32 m_TamingTeamID; //0x1790 
		char pad_0x1794[0x30]; //0x1794
		__int32 m_TamedAggressionLevel; //0x17C4 
		//char pad_0x17C8[0x9A0]; //0x17C8 NOT CORRECT SIZE
	public:
		FUNCTION_UPDATEINFORMATION(APrimalDinoCharacter);

		bool IsTamed(SDK::APrimalDinoCharacter* pInstance)
		{
			SDK::APrimalDinoCharacter CurrentInstance;
			if (pInstance)
				CurrentInstance = *pInstance;
			else
				CurrentInstance = this->UpdateInformation();

			return CurrentInstance.m_TargetingTeam >= 50000;
		}
	};

	class FWeaponData
	{
	public:
		char pad_0x0000[0x8]; //0x0000
		__int32 m_AmmoPerClip; //0x0008 
		__int32 m_AmmoPerShot; //0x000C 
		unsigned char m_bSilentOverflowReload; //0x0010 
		char pad_0x0011[0x3]; //0x0011
		float m_TimeBetweenShots; //0x0014 
		float m_NoAnimReloadDuration; //0x0018 
		unsigned char m_bForceReloadDuration; //0x001C 
		char pad_0x001D[0x3]; //0x001D
		float m_ReloadDurationPerAmmoCount; //0x0020 
		char pad_0x0024[0x8]; //0x0024
	public:


	}; //Size=0x002C

	class FWeaponAnim
	{
	public:
		__int64 m_pPawn1p; //0x0000 
		__int64 m_pPawn3p; //0x0008 

	}; //Size=0x0010

	class FInstantWeaponData
	{
	public:
		float WeaponSpread; //0x0000 
		float TargetingSpreadMod; //0x0004 
		float FinalWeaponSpreadMultiplier; //0x0008 
		float FiringSpreadIncrement; //0x000C 
		float FiringSpreadMax; //0x0010 
		float WeaponRange; //0x0014 
		__int32 HitDamage; //0x0018 
		float DamageImpulse; //0x001C 
		PVOID* m_pDamageType; //0x0020 
		float ClientSideHitLeeway; //0x0028 
		float AllowedViewDotHitDir; //0x002C
	};

	class FItemNetInfo
	{
	public:
		__int32 ItemID1; //0x0000 
		__int32 ItemID2; //0x0004 
		char pad_0x0008[0xC]; //0x0008
		__int32 CustomItemID; //0x0014 
		char pad_0x0018[0x198]; //0x0018

	}; //Size=0x01B0

	class UPrimalItem
	{
	public:
		char pad_0x0000[0x160]; //0x0000
		PVOID* pItemSkinAddItemAttachments; //0x0160 
		char pad_0x0168[0x8]; //0x0168
		__int8 MyItemType; //0x0170 
		char pad_0x0171[0x9F]; //0x01
		__int32 ItemId1; //0x0210 
		__int32 ItemId2; //0x0214 
		char pad_0x0218[0x8]; //0x0218
		UClass* m_pItemCustomClass; //0x0220 
		char pad_0x0228[0x100]; //0x0228
		FString* m_pDescriptiveNameBase; //0x0328 
		__int32 m_DescriptiveNameLength; //0x0330 
		char pad_0x0334[0x234]; //0x0334
		__int32 WeaponClipAmmo; //0x0568 
		char pad_0x056C[0x574]; //0x056C
	public:
		FUNCTION_UPDATEINFORMATION(UPrimalItem);
		FUNCTION_GETMEMBER(ItemCustomClass, UClass*, Offsets::Classes::UPrimalItem::ItemCustomClass);
		FUNCTION_GETMEMBER(DescriptiveNameBase, FString*, Offsets::Classes::UPrimalItem::DescriptiveNameBase);

	}; //Size=0x0AE0

	class AShooterWeapon : public AActor
	{
	public:
		char pad_0x0470[0xE4]; //0x0470
		Vec3 FPVRelativeLocation; //0x0554 
		Vec3 FPVRelativeRotation; //0x0560 
		Vec3 FPVRelativeLocation_Targeting; //0x056C 
		Vec3 FPVRelativeRotation_Targeting; //0x0578 
		char pad_0x0584[0x8]; //0x0584
		float m_EndDoMeleeSwingTime; //0x058C 
		char pad_0x0590[0x88]; //0x0590
		UPrimalItem* m_pAssociatedPrimalItem; //0x0618 
		char pad_0x0620[0x10]; //0x0620
		FItemNetInfo ItemNetInfo; //0x0630 
		USkeletalMeshComponent* m_Mesh3P; //0x07E0 
		UPrimalItem* pWeaponAmmoItemTemplate; //0x07E8 
		char pad_0x07F0[0x8]; //0x07F0
		FWeaponData WeaponData; //0x07F8 
		char pad_0x0824[0x7C]; //0x0824
		AShooterCharacter* m_MyPawn; //0x08A0 
		USkeletalMeshComponent* m_Mesh1P; //0x08A8 
		char pad_0x08B0[0x40]; //0x08B0
		__int64 m_pFireCameraShake; //0x08F0 
		__int64 m_pReloadCameraShake; //0x08F8 
		char pad_0x0900[0x40]; //0x0900
		FWeaponAnim ReloadAnim; //0x0940 
		char pad_0x0950[0x98]; //0x0950
		FWeaponAnim MeleeAnim; //0x09E8 
		char pad_0x09F8[0xB8]; //0x09F8
		__int32 m_CurrentAmmo; //0x0AB0 
		__int32 m_CurrentAmmoInClip; //0x0AB4 
		char pad_0x0AB8[0x2]; //0x0AB8
		unsigned char m_IsDefaultWeapon; //0x0ABA 
		char pad_0x0ABB[0x2D]; //0x0ABB
		__int64 m_LastNotifyShotTime; //0x0AE8 
		char pad_0x0AF0[0x18]; //0x0AF0
		Vec3 VRTargetingModelOffset; //0x0B08 
		char pad_0x0B14[0xC]; //0x0B14
		__int64 m_ScopeOverlayMI; //0x0B20 
		char pad_0x0B28[0x3C]; //0x0B28
		//float m_AimDriftYawAngle; //0x0B64 
		//float m_AimDriftPitchAngle; //0x0B68 
		Vec2 m_AimDriftAngle;
		//float m_AimDriftYawFrequency; //0x0B6C 
		//float m_AimDriftPitchFrequency; //0x0B70 
		Vec2 m_AimDriftFrequency;
		char pad_0x0B74[0x140]; //0x0B74
		float m_FireCameraShakeSpreadScaleExponent; //0x0CB4 
		float m_FireCameraShakeSpreadScaleExponentLessThan; //0x0CB8 
		float m_FireCameraShakeSpreadScaleMultiplierLessThan; //0x0CBC 
		float m_FireCameraShakeSpreadScaleMultiplier; //0x0CC0 
		unsigned char m_bUseFireCameraShakeScale; //0x0CC4 
		char pad_0x0CC5[0x3]; //0x0CC5
		FInstantWeaponData InstantConfig; //0x0CC8 
		char pad_0x0CF8[0x24]; //0x0CF8
		Vec3 FPVMuzzleLocationOffset; //0x0D1C 
		Vec3 TPVMuzzleLocationOffset; //0x0D28 
		char pad_0x0D34[0x4C]; //0x0D34
	public:
		FUNCTION_UPDATEINFORMATION(AShooterWeapon);

		FUNCTION_SETMEMBER(FireCameraShake, uintptr_t, Offsets::Classes::AShooterWeapon::pFireCameraShake);
		FUNCTION_SETMEMBER(ReloadCameraShake, uintptr_t, Offsets::Classes::AShooterWeapon::pReloadCameraShake);

		FUNCTION_SETMEMBER(AimDriftAngle, Vec2, Offsets::Classes::AShooterWeapon::AimDriftAngle);
		FUNCTION_SETMEMBER(AimDriftFreq, Vec2, Offsets::Classes::AShooterWeapon::AimDriftFrequency);
		FUNCTION_SETMEMBER(WeaponData, FWeaponData, Offsets::Classes::AShooterWeapon::WeaponData);
		FUNCTION_SETMEMBER(ReloadAnim, FWeaponAnim, Offsets::Classes::AShooterWeapon::ReloadAnim);
		FUNCTION_SETMEMBER(InstantConfig, FInstantWeaponData, Offsets::Classes::AShooterWeapon::InstantConfig);
		FUNCTION_SETMEMBER(ScopeOverlay, uintptr_t, Offsets::Classes::AShooterWeapon::pScopeOverlayMI);
		FUNCTION_SETMEMBER(m_CurrentAmmoInClip, int, Offsets::Classes::AShooterWeapon::CurrentAmmoInClip);

		std::wstring get_name(AShooterWeapon* instance)
		{
			AShooterWeapon Weapon;
			std::wstring name = L"";

			if (instance)
				Weapon = *instance;
			else
				Weapon = this->UpdateInformation();

			if (!Weapon.m_pAssociatedPrimalItem)
				return L"Fists";

			SDK::UPrimalItem PrimalItem = Weapon.m_pAssociatedPrimalItem->UpdateInformation();

			if (PrimalItem.m_DescriptiveNameLength > 64 || PrimalItem.m_DescriptiveNameLength <= 0)
				return name;

			wchar_t* name_buff = new wchar_t[PrimalItem.m_DescriptiveNameLength];

			if (!name_buff)
				return name;

			PrimalItem.m_pDescriptiveNameBase->read_name(name_buff, PrimalItem.m_DescriptiveNameLength);

			name = name_buff;

			delete[] name_buff;

			return name;
		}

		bool IsOfType(AShooterWeapon* instance, const wchar_t* name)
		{
			AShooterWeapon Weapon;

			if (instance)
				Weapon = *instance;
			else
				Weapon = this->UpdateInformation();

			SDK::UPrimalItem PrimalItem = Weapon.m_pAssociatedPrimalItem->UpdateInformation();

			if (PrimalItem.m_DescriptiveNameLength > 64 || PrimalItem.m_DescriptiveNameLength <= 0)
				return false;

			wchar_t* name_buff = new wchar_t[PrimalItem.m_DescriptiveNameLength];

			if (!name_buff)
				return false;

			PrimalItem.m_pDescriptiveNameBase->read_name(name_buff, PrimalItem.m_DescriptiveNameLength);

			bool ret = (wcscmp(name_buff, name) == 0);

			delete[] name_buff;

			return ret;
		}

	};

	class USceneComponent
	{
	public:
		char pad_0x0000[0x158]; //0x0000
		Vec3 m_RelativeLocation; //0x0158 
		Vec3 m_RelativeRotation; //0x0164 
		Vec3 m_RelativeTranslation; //0x0170 
		char pad_0x017C[0x28]; //0x017C
		Vec3 m_ComponentVelocity; //0x01A4 
		char pad_0x01B0[0x38]; //0x01B0
	public:
		FUNCTION_UPDATEINFORMATION(USceneComponent);

	};

	class BoneTransformArray
	{
	public:
		FTransform GetBoneTransform(int Index)
		{
			if (this == nullptr)
				return FTransform();
			return eDriver.read_mem<FTransform>((PVOID)((uintptr_t)this + sizeof(FTransform) * Index));
		}
		//Important, make sure that allocated memory is large anough for specified arraysize
		bool GetBoneTransforms(FTransform* BoneTransforms, int ArraySize)
		{
			if (this == nullptr || BoneTransforms == nullptr)
				return false;

			eDriver.read_in_buffer(this, BoneTransforms, sizeof(FTransform) * ArraySize);

			return true;
		}
	};



	class BoneNames
	{
	public:
		FName GetBoneName(int Index)
		{
			if (this == nullptr)
				return FName();
			return eDriver.read_mem<FName>((PVOID)((uintptr_t)this + sizeof(FName) * Index));
		}
		//Important, make sure that allocated memory is large anough for specified arraysize
		bool GetBoneNames(FName* BoneNames, int ArraySize)
		{
			if (this == nullptr || BoneNames == nullptr)
				return false;

			eDriver.read_in_buffer(this, BoneNames, sizeof(FName) * ArraySize);

			return true;
		}

	};

	class BoneMaterials
	{
	public:
		char pad_0x0000[0x138]; //0x0000
		BoneNames* m_pBoneMaterials; //0x0138 
		__int32 m_MaterialCount; //0x0140
	public:
		FUNCTION_UPDATEINFORMATION(BoneMaterials);
	}; //Size=0x0148

	class USkeletalMeshComponent
	{
		//NOT PROPERLY REVERSED (NO INHERITANCE)
	public:
		char pad_0x0000[0xE0]; //0x0000
		FTransform ComponentToWorld; //0x00E0 
		char pad_0x0110[0x568]; //0x0110
		BoneMaterials* m_pMaterials; //0x0678 
		BoneTransformArray* m_pTArraySpaceBases; //0x0680 
		__int32 m_SpaceBasesSize; //0x0688 
	public:
		FUNCTION_UPDATEINFORMATION(USkeletalMeshComponent);

		FUNCTION_GETMEMBER(GetSpaceBases, BoneTransformArray*, Offsets::Classes::USkeletalMeshComponent::SpaceBases);
		FUNCTION_GETMEMBER(Materials, BoneMaterials*, Offsets::Classes::USkeletalMeshComponent::Materials);
	};


	struct FMinimalViewInfo
	{
	public:
		Vec3 m_Location; //0x0000 
		Vec3 m_Rotation; //0x000C 
		Vec3 m_AimRotation; //0x0018 
		unsigned char bUseTPVAim; //0x0024 
		char pad_0x0025[0x3]; //0x0025
		float m_FieldOfView; //0x0028 
		char pad_0x002C[0x1C]; //0x002C
	};

	struct FCameraCacheEntry
	{
	public:
		float m_TimeStamp; //0x0000 
		char pad_0x0004[0x4]; //0x0004
		FMinimalViewInfo MinimalViewInfo; //0x0008 
		char pad_0x0050[0x30]; //0x0050
	}; //Size=0x0080

	class APlayerCameraManager : public AActor
	{
	public:
		USceneComponent* m_pUSceneComponent; //0x0470 
		char pad_0x0478[0x8]; //0x0478
		float m_FieldOfView; //0x0480 
		char pad_0x0484[0x4C]; //0x0484
		FCameraCacheEntry CameraCacheEntry; //0x04D0 
		char pad_0x0550[0xE38]; //0x0550
		float m_FreeCamDistance; //0x1388 
		char pad_0x138C[0x4C]; //0x138C
		unsigned char m_bAlwaysApplyModifiers; //0x13D8 
		char pad_0x13D9[0x6F]; //0x13D9
	public:
		FUNCTION_UPDATEINFORMATION(APlayerCameraManager);

		FUNCTION_GETMEMBER(SceneComponent, USceneComponent*, Offsets::Classes::APlayerCameraManager::USceneComponent);

		FUNCTION_SETMEMBER(FreeCamDistance, float, Offsets::Classes::APlayerCameraManager::FreeCamDistance);
	}; //Size=0x1448

	class UCanvasProxy
	{
	public:
		static UCanvas* GetCanvas()
		{
			UCanvas* pCanvas = (UCanvas*)(Offsets::ShooterGame::UCanvasProxy);

			for (auto& off : Offsets::Classes::UCanvasProxy::UCanvas)
			{
				if (pCanvas)
					pCanvas = eDriver.read_mem<UCanvas*>((PVOID)((uintptr_t)pCanvas + off));
			}
			return pCanvas;
		}
	};

	class UCanvas
	{
	public:
		char pad_0x0000[0x30]; //0x0000
		float m_Width; //0x0030 
		float m_Height; //0x0034 
		char pad_0x0038[0x78]; //0x0038
		float m_ProjectionMatrix[16]; //0x00B0 
	public:
		FUNCTION_UPDATEINFORMATION(UCanvas);
	};

	class APlayerState
	{
	public:
		char pad_0x0000[0x468]; //0x0000
		float Score; //0x0468 
		__int32 Ping; //0x046C 
		FString* PlayerName; //0x0470 
		char pad_0x0478[0x18]; //0x0478
		__int32 m_PlayerID; //0x0490 
	public:
		FUNCTION_UPDATEINFORMATION(APlayerState);
		//FUNCTION_GETMEMBER(PlacementRotOffset, Vec3, Offsets::Classes::APrimalStructure::PlacementRotOffset);

	}; //Size=0x04C0

	class APrimalTargetableActor : public AActor
	{
	public:
		char pad_0x0470[0x78]; //0x0470
		FString* m_pDescriptiveName; //0x04E8 
		__int32 m_DescriptiveNameLength; //0x04F0 
		char pad_0x04F4[0x14]; //0x04F4
		float m_ReplicatedHealth; //0x0508 
		float m_Health; //0x050C 
		float m_MaxHealth; //0x0510 
		char pad_0x0514[0x3C]; //0x0514
		__int64 m_pStructureSettingsClass; //0x0550 
		char pad_0x0558[0x20]; //0x0558
	public:
		FUNCTION_UPDATEINFORMATION(APrimalTargetableActor);

	};

	class APrimalStructure : public APrimalTargetableActor
	{
	public:
		char pad_0x0578[0x64]; //0x0578
		char pad_0x05DD[0x63]; //0x05DD
		Vec3 PlacementRotOffset; //0x0640 
		Vec3 PlacementTraceRotOffset; //0x064C 
		Vec3 SnappingRotationOffset; //0x0658 
		char pad_0x0664[0x54]; //0x0664
		char pad_0x06B9[0x305]; //0x06B9
		char pad_0x09BF[0x121]; //0x09BF
	public:
		FUNCTION_UPDATEINFORMATION(APrimalStructure);

		//FUNCTION_GETMEMBER(PlacementRotOffset, Vec3, Offsets::Classes::APrimalStructure::PlacementRotOffset);
		FUNCTION_SETMEMBER(PlacementRotOffset, Vec3, Offsets::Classes::APrimalStructure::PlacementRotOffset);

	};

	class APrimalStructurePlacer : public AActor
	{
	public:
		FUNCTION_GETMEMBER(CurrentPlacingStructure, APrimalStructure*, Offsets::Classes::APrimalStructurePlacer::CurrentPlacingStructure);
	};

	class APrimalStructureItemContainer : public APrimalStructure
	{
	public:
		char pad_0x0AE0[0x124]; //0x0AE0
		__int32 CurrentItemCount; //0x0C04 
		__int32 MaxItemCount; //0x0C08 
		char pad_0x0C0C[0x144]; //0x0C0C
		unsigned char m_IsLocked; //0x0D50 
		char pad_0x0D51[0x1C]; //0x0D51
		unsigned char m_IsPowered; //0x0D6D 
		char pad_0x0D6E[0x9A]; //0x0D6E
	public:
		FUNCTION_UPDATEINFORMATION(APrimalStructureItemContainer);
	}; //Size=0x0E08



	class APrimalStructureItemContainer_SupplyCrate : public APrimalStructureItemContainer
	{
	public:
		float MinItemSets; //0x0E08 
		float MaxItemSets; //0x0E0C 
		char pad_0x0E10[0x8]; //0x0E10
		float m_MinQualityMultiplier; //0x0E18 
		float m_MaxQualityMultiplier; //0x0E1C 
		char pad_0x0E20[0x38]; //0x0E20
		__int32 m_RequiredLevelToAccess; //0x0E58 
		__int32 m_MaxLevelToAccess; //0x0E5C 
		char pad_0x0E60[0x48]; //0x0E60
	public:
		FUNCTION_UPDATEINFORMATION(APrimalStructureItemContainer_SupplyCrate);

	}; //Size=0x0EA8

	class FNameEntry
	{
	public:
		char pad_0x0000[0x10]; //0x0000
		char Buffer[64]; //0x317EE9C0 
	public:
		FUNCTION_UPDATEINFORMATION(FNameEntry);

	}; //Size=0x0038

	class GNameChunk
	{
	public:
		FNameEntry* GetEntryByInChunkIndex(int index)
		{
			return eDriver.read_mem<FNameEntry*>((PVOID)((uintptr_t)this + 0x8 * index));
		}
	};

	class GUObjectManager
	{
	public:
		static PVOID GetObjectEntry(int index)
		{
			if (index < 0 || index > 0x10000000)
				return nullptr;

			PVOID* chunk_array = eDriver.read_mem<PVOID*>(reinterpret_cast<PVOID>(Offsets::ShooterGame::GUObjectArray + 0x10));

			//static const int MaxTotalElements = 2 * 1024 * 1024;
			static const int ElementsPerChunk = 65536;

			int32_t ChunkIndex = index / ElementsPerChunk;
			int32_t WithinChunkIndex = index % ElementsPerChunk;

			PVOID pTargetChunk = eDriver.read_mem<PVOID>(reinterpret_cast<PVOID>((uintptr_t)chunk_array + 0x8 * ChunkIndex));

			int new_item_id = index - (ElementsPerChunk * ChunkIndex);

			PVOID pTargetObject = eDriver.read_mem<PVOID>(reinterpret_cast<PVOID>((uintptr_t)pTargetChunk + 0x10 * new_item_id));

			return pTargetObject;
		}

	};

	class GNamesManager
	{
	public:
		GNameChunk* GetChunk(int index)
		{
			return eDriver.read_mem<GNameChunk*>((PVOID)((uintptr_t)this + 0x8 * index));
		}
		FNameEntry* GetNameEntry(int index)
		{
			if (index < 0 || index > 0x10000000)
				return nullptr;

			static const int MaxTotalElements = 2 * 1024 * 1024;
			static const int ElementsPerChunk = 16384;

			int32_t ChunkIndex = index / ElementsPerChunk;
			int32_t WithinChunkIndex = index % ElementsPerChunk;

			GNameChunk* pTargetChunk = this->GetChunk(ChunkIndex);

			return pTargetChunk->GetEntryByInChunkIndex(WithinChunkIndex);
		}

	};

	class GNamesProxy
	{
	public:
		static GNamesManager* GetInstance()
		{
			return eDriver.read_mem<GNamesManager*>((PVOID)Offsets::ShooterGame::GNamesProxy);
		}

	}; //Size=0x00C0

	class UMovementComponent
	{
	public:
		char pad_0x0000[0xE4]; //0x0000
		Vec3 Velocity; //0x00E4 
		Vec3 PlaneConstraintNormal; //0x00F0 
		Vec3 PlanceConstraintOrigin; //0x00FC 
		char pad_0x0108[0x10]; //0x0108

	}; //Size=0x0118

	class UNavMovementComponent : public UMovementComponent
	{
	public:
		char pad_0x0118[0x14]; //0x0118
		char pad_0x012D[0x3]; //0x012D

	}; //Size=0x0130

	class UPawnMovementComponent : public UNavMovementComponent
	{
	public:
		APawn* m_pPawnOwner; //0x0130 
		char pad_0x0138[0x8]; //0x0138

	}; //Size=0x0140

	class UCharacterMovementComponent : public UPawnMovementComponent
	{
	public:
		char pad_0x0140[0x14]; //0x0140
		float m_MaxStepHeight; //0x0154 
		float m_JumpZVelocity; //0x0158 
		float m_JumpOffZFactor; //0x015C 
		unsigned char m_SlipOffEdges; //0x0160 
		char pad_0x0161[0xF]; //0x0161
		float m_WalkableFloorAngle; //0x0170 
		float m_WalkableFloorZ; //0x0174 
		char pad_0x0178[0x8]; //0x0178
		__int32 m_MovementMode; //0x0180 
		char pad_0x0184[0x3C]; //0x0184
		float m_GravityScale; //0x01C0 
		float m_GroundFriction; //0x01C4 
		float m_MaxWalkSpeed; //0x01C8 
		float m_MaxWalkSpeedCrouched; //0x01CC 
		float m_MaxWalkSpeedProne; //0x01D0 
		float m_MaxCustomMovementSpeed; //0x01D4 
		float m_MaxSwimSpeed; //0x01D8 
		float m_MaxFlySpeed; //0x01DC 
		char pad_0x01E0[0x18]; //0x01E0
		float m_MaxAcceleration; //0x01F8 
		char pad_0x01FC[0x3C]; //0x01FC
		Vec3 RotationRate; //0x0238 
		char pad_0x0248[0x8]; //0x0248
		float m_MaxOutOfWaterStepHeight; //0x0250 
		char pad_0x0254[0x94]; //0x0254
		float m_LedgeCheckThreshold; //0x02E8 
		char pad_0x02EC[0xA4]; //0x02EC
		__int32 m_DefaultLandMovementMode; //0x0390 
		char pad_0x0396[0x1A]; //0x0396
		float RotationAcceleration; //0x03B0 
		float RotationBraking; //0x03B4 
		float AngleToStartRotationBraking; //0x03B8 
		char pad_0x03BC[0x44C]; //0x03BC
	public:
		FUNCTION_UPDATEINFORMATION(UCharacterMovementComponent);
		FUNCTION_SETMEMBER(RotationRate, Vec3, Offsets::Classes::UCharacterMovementComponent::RotationRate);
		FUNCTION_SETMEMBER(AngleToStartRotationBraking, float, Offsets::Classes::UCharacterMovementComponent::AngleToStartRotationBraking);
	}; //Size=0x0808


	class AShooterWeapon_Projectile : public AShooterWeapon
	{
	public:
		char pad_0x0D80[0x8]; //0x0D80
		UClass* pProjectileClass; //0x0D88 
		char pad_0x0D90[0x1C]; //0x0D90
		Vec2 m_ProjectileSpread; //0x0DAC 
		char pad_0x0DB4[0x7C]; //0x0DB4
	public:
		FUNCTION_UPDATEINFORMATION(AShooterWeapon_Projectile);
	}; //Size=0x0E30


	class APrimalWeaponBow : public AShooterWeapon_Projectile
	{
	public:
		float MinimumInitialSpeed; //0x0E30 
		float MaximumInitialSpeed; //0x0E34 
		float PullingTimeForMaximumSpeed; //0x0E38 
		float DamageFactorForSlowArrows; //0x0E3C 
		float DamageFactorForFastArrows; //0x0E40 
		float MinimumPullingTimeToFire; //0x0E44 
		char pad_0x0E48[0x8]; //0x0E48
		__int64 m_pMaxPullCameraShake; //0x0E50 
		char pad_0x0E58[0x4C]; //0x0E58
		float PullingTime; //0x0EA4 
		float ProjectileSpeed; //0x0EA8 
		char pad_0x0EAC[0x4]; //0x0EAC
	public:
		FUNCTION_UPDATEINFORMATION(APrimalWeaponBow);
		FUNCTION_SETMEMBER(PullingTimeForMaximumSpeed, float, Offsets::Classes::APrimalWeaponBow::PullingTimeForMaximumSpeed);
		FUNCTION_SETMEMBER(MinimumPullingTimeToFire, float, Offsets::Classes::APrimalWeaponBow::MinimumPullingTimeToFire);
		FUNCTION_SETMEMBER(m_pMaxPullCameraShake, __int64, Offsets::Classes::APrimalWeaponBow::m_pMaxPullCameraShake);
	}; //Size=0x0EB0


	class APrimalWeaponGrenade : public AShooterWeapon_Projectile
	{
	public:
		char pad_0x0E30[0x30]; //0x0E30
		FName GrenadeBoneName; //0x0E60 
		char pad_0x0E68[0xC]; //0x0E68
		__int8 bIsThrowingGrenade; //0x0E75 
		__int8 bIsCookingGrenade; //0x0E76 
		__int8 bUseBlueprintSpeed; //0x0E77 
		char pad_0x0E78[0x4]; //0x0E78
		float CookingTime; //0x0E7C 
		Vec3 ProjectileOrigin; //0x0E80 
		Vec3 ProjectileShootDir; //0x0E8C 
		char pad_0x0E98[0x50]; //0x0E98
	public:
		FUNCTION_SETMEMBER(CookingTime, float, Offsets::Classes::APrimalWeaponGrenade::CookingTime);
	}; //Size=0x0EE8

	class APrimalStructureTurret : public APrimalStructureItemContainer
	{
	public:
		char pad_0x0E08[0x78]; //0x0E08
		Vec3 TurretAimRotOffset; //0x0E80 
		Vec3 AimTargetLocOffset; //0x0E8C 
		Vec3 PlayerProneTargetOffset; //0x0E98 
		char pad_0x0EA4[0xC]; //0x0EA4
		__int32 NumBullets; //0x0EB0 
		__int32 NumBulletsPerShot; //0x0EB4 
		char pad_0x0EB8[0x88]; //0x0EB8
	public:
		FUNCTION_UPDATEINFORMATION(APrimalStructureTurret);
		FUNCTION_SETMEMBER(PlayerProneTargetOffset, Vec3, Offsets::Classes::APrimalStructureTurret::PlayerProneTargetOffset);
	}; //Size=0x0F40


}

