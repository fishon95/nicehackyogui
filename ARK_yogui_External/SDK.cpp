#include "SDK.h"

#include "VMProtect/VMProtectSDK.h"
#include "skCrypt.h"
#include "Globals.h"

#include "NvidiaOverlay/NvidiaOverlay.h"

#include "SigScanner.h"

#include "HackMain.h"

uintptr_t Offsets::ShooterGame::WorldProxy = NULL;
uintptr_t Offsets::ShooterGame::UCanvasProxy = NULL;
uintptr_t Offsets::ShooterGame::GNamesProxy = NULL;
uintptr_t Offsets::ShooterGame::GUObjectArray = NULL;

bool SDK::Functions::InitializeOffsets()
{
	VMProtectBeginMutation("SDK.init()");
	if (!Globals::Modules::ShooterGame)
		return false;

	//uintptr_t ResolvedAddress = eMemory.ResolveRelativeAddress(result, 3, 7);

	if (!eSigScanner.SelectRegion(Globals::Modules::ShooterGame, XorString(".text")))
		return false;



	eSigScanner.AddSignature({ ("\x48\x8B\x05\x00\x00\x00\x00\x48\x39\x81\x00\x00\x00\x00\x74\x00\xFF\xC3"), ("xxx????xxx????x?xx"), &Offsets::ShooterGame::WorldProxy });

	eSigScanner.AddSignature({ ("\x8D\x41\xFF\x48\x63\xC8\x48\x63\xC2\x48\x23\xC8\x41\x8B\x0C\x88\x83\xF9\xFF\x74"), ("xxxxxxxxxxxxxxxxxxxx"), &Offsets::ShooterGame::UCanvasProxy });

	eSigScanner.AddSignature({ ("\x48\x89\x83\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x89\x1D"), ("xxx????x????xxx"), &Offsets::ShooterGame::GNamesProxy });

	//eSigScanner.AddSignature({ ("\x3b\x3d\x00\x00\x00\x00\x7c\x00\x3b\x3d"), ("xx????x?xx"), &Offsets::ShooterGame::GUObjectArray });


	if (!eSigScanner.LocateSignatures())
	{
		printf("Failed locating signatures!\n");
		return false;
	}

	//Offsets
	Offsets::ShooterGame::UCanvasProxy += 21;
	Offsets::ShooterGame::GNamesProxy += 0xC;


	//printf("UWorldProxy signature located at : 0x%llX\n", Offsets::ShooterGame::WorldProxy);
	//printf("UCanvasProxy signature located at : 0x%llX\n", Offsets::ShooterGame::UCanvasProxy);
	//printf("GNamesProxy signature located at : 0x%llX\n", Offsets::ShooterGame::GNamesProxy);
	//printf("GUObjectArray signature located at : 0x%llX\n", Offsets::ShooterGame::GUObjectArray);
	

	

	Offsets::ShooterGame::WorldProxy = SigScanner::ResolveRelativeAddress(Offsets::ShooterGame::WorldProxy, 3, 7);
	Offsets::ShooterGame::UCanvasProxy = SigScanner::ResolveRelativeAddress(Offsets::ShooterGame::UCanvasProxy, 3, 7);
	Offsets::ShooterGame::GNamesProxy = SigScanner::ResolveRelativeAddress(Offsets::ShooterGame::GNamesProxy, 3, 7);
	//Offsets::ShooterGame::GUObjectArray = SigScanner::ResolveRelativeAddress(Offsets::ShooterGame::GUObjectArray, 2, 6);


	//printf("UWorldProxy resolved at : 0x%llX\n", Offsets::ShooterGame::WorldProxy);
	//printf("UCanvasProxy resolved at : 0x%llX\n", Offsets::ShooterGame::UCanvasProxy);
	//printf("GNamesProxy resolved at : 0x%llX\n", Offsets::ShooterGame::GNamesProxy);
	//printf("GUObjectArray resolved at : 0x%llX\n", Offsets::ShooterGame::GUObjectArray);

	//system("pause");
	//DBG_STOP();

	VMProtectEnd();

	return true;
}

bool SDK::Functions::GetBonePosition(SDK::USkeletalMeshComponent& Instance, int BoneIndex, Vec3* OutPos)
{
	if (!OutPos)
		return false;

	//DBG_PRINT("SpaceBases(0x%llX) Size(%i)\n", Instance.m_pTArraySpaceBases, Instance.m_SpaceBasesSize);

	if (!Instance.m_pTArraySpaceBases || Instance.m_SpaceBasesSize <= 0)
		return false;

	if (BoneIndex >= Instance.m_SpaceBasesSize || BoneIndex <= 0)
		return false;

	FTransform BoneTransform = Instance.m_pTArraySpaceBases->GetBoneTransform(BoneIndex);

	//DBG_PRINT("BoneTransform.Translation(%f %f %f %f)\n", BoneTransform.Translation.x, BoneTransform.Translation.y, BoneTransform.Translation.z);
	//DBG_PRINT("ComponentToWorld.Translation(%f %f %f %f)\n", Instance.ComponentToWorld.Translation.x, Instance.ComponentToWorld.Translation.y, Instance.ComponentToWorld.Translation.z);

	D3DXMATRIX BoneTransformMatrix = BoneTransform.ToMatrixWithScale();
	D3DXMATRIX ComponentToWorldMatrix = Instance.ComponentToWorld.ToMatrixWithScale();

	D3DXMATRIX BoneMatrix;
	D3DXMatrixMultiply(&BoneMatrix, &BoneTransformMatrix, &ComponentToWorldMatrix);

	*OutPos = { BoneMatrix._41, BoneMatrix._42, BoneMatrix._43 };

	//DBG_PRINT("BonePos(%f %f %f)\n", OutPos->x, OutPos->y, OutPos->z);
	//system("pause");

	return true;
}

bool SDK::Functions::GetBonePosition(SDK::USkeletalMeshComponent& Instance, FTransform* pBoneTransforms, int BoneIndex, Vec3* OutPos)
{
	if (!OutPos || !pBoneTransforms)
		return false;

	//DBG_PRINT("SpaceBases(0x%llX) Size(%i)\n", Instance.m_pTArraySpaceBases, Instance.m_SpaceBasesSize);

	if (!Instance.m_pTArraySpaceBases || Instance.m_SpaceBasesSize <= 0)
		return false;

	if (BoneIndex >= Instance.m_SpaceBasesSize || BoneIndex <= 0)
		return false;

	FTransform BoneTransform = pBoneTransforms[BoneIndex];

	//DBG_PRINT("BoneTransform.Translation(%f %f %f %f)\n", BoneTransform.Translation.x, BoneTransform.Translation.y, BoneTransform.Translation.z);
	//DBG_PRINT("ComponentToWorld.Translation(%f %f %f %f)\n", Instance.ComponentToWorld.Translation.x, Instance.ComponentToWorld.Translation.y, Instance.ComponentToWorld.Translation.z);

	D3DXMATRIX BoneTransformMatrix = BoneTransform.ToMatrixWithScale();
	D3DXMATRIX ComponentToWorldMatrix = Instance.ComponentToWorld.ToMatrixWithScale();

	D3DXMATRIX BoneMatrix;
	D3DXMatrixMultiply(&BoneMatrix, &BoneTransformMatrix, &ComponentToWorldMatrix);

	*OutPos = { BoneMatrix._41, BoneMatrix._42, BoneMatrix._43 };

	//DBG_PRINT("BonePos(%f %f %f)\n", OutPos->x, OutPos->y, OutPos->z);
	//system("pause");

	return true;
}

bool SDK::Functions::GetBoneHeadIndex(SDK::BoneMaterials* pBoneMaterials, int* pOutIndex)
{
	if (!pBoneMaterials || !pOutIndex)
		return false;

	if (!eHackMain.m_pGNamesManager)
		return false;

	SDK::BoneMaterials Materials = pBoneMaterials->UpdateInformation();

	if (Materials.m_MaterialCount <= 0 || Materials.m_MaterialCount > 0x10000)
		return false;

	if (!Materials.m_pBoneMaterials)
		return false;

	SDK::FName* BoneNamesArray = new SDK::FName[Materials.m_MaterialCount];

	if (!BoneNamesArray)
		return false;

	Materials.m_pBoneMaterials->GetBoneNames(BoneNamesArray, Materials.m_MaterialCount);

	bool BoneFound = false;

	for (int i = 0; i < Materials.m_MaterialCount; i++)
	{
		SDK::FNameEntry* pNameEntry = nullptr;

		if (eHackMain.m_pGNamesManager)
			pNameEntry = eHackMain.m_pGNamesManager->GetNameEntry(BoneNamesArray[i].ComparisonIndex);

		if (!pNameEntry)
		{
			delete[] BoneNamesArray;
			return false;
		}

		std::string BoneName = pNameEntry->UpdateInformation().Buffer;

		if (BoneName.find(XorString("head")) != std::string::npos)
		{
			*pOutIndex = i;
			BoneFound = true;
			break;
		}

		if (BoneName.find(XorString("Head")) != std::string::npos)
		{
			*pOutIndex = i;
			BoneFound = true;
			break;
		}

		if (BoneName.find(XorString("Jaw")) != std::string::npos)
		{
			*pOutIndex = i;
			BoneFound = true;
			break;
		}
		if (BoneName.find(XorString("jaw")) != std::string::npos)
		{
			*pOutIndex = i;
			BoneFound = true;
			break;
		}

		/*if (BoneName.find(XorString("eye")) != std::string::npos)
		{
			*pOutIndex = i;
			BoneFound = true;
			break;
		}*/
	}

	if (BoneNamesArray)
		delete[] BoneNamesArray;

	return BoneFound;
}

bool SDK::Functions::IsInstantWeapon(SDK::AShooterWeapon& WeaponInstance, std::wstring& weapon_name)
{
	static const std::vector<std::wstring> weapon_names
	{
		L"Fabricated Sniper Rifle",
		L"Assault Rifle",
		L"Longneck Rifle",
		L"Pump-Action Shotgun",
		L"Shotgun",
		L"Simple Pistol",
		L"Fabricated Pistol"
	};

	for (auto& wep_name : weapon_names)
	{
		if (wcscmp(wep_name.c_str(), weapon_name.c_str()) == 0)
			return true;
	}

	return false;
}

static float p[16] =
{
  1, 0, 0, 0,
  0, 1, 0, 0,
  0, 0, 1, 0,
  0, 0, 0, 1
};
static float v[16] =
{
  1, 0, 0, 0,
  0, 1, 0, 0,
  0, 0, 1, 0,
  0, 0, 0, 1
};
static float pv[16] =
{
  1, 0, 0, 0,
  0, 1, 0, 0,
  0, 0, 1, 0,
  0, 0, 0, 1
};

void calc_view(float * v, Vec3 & Rotation, Vec3 & Location)
{
	float cy, cp, cr;
	float sy, sp, sr;
	float th;
	/*Decompose rotation into sin/cosine pairs*/
	th = D3DX_PI * Rotation.x / 180;
	cy = cos(th);
	sy = sin(th);
	th = D3DX_PI * Rotation.y / 180;
	cp = cos(th);
	sp = sin(th);
	th = D3DX_PI * Rotation.z / 180;
	cr = cos(th);
	sr = sin(th);
	/*Multiplies 3 axial matrices together to get the result*/
	v[0] = cp * cy;
	v[1] = cp * sy;
	v[2] = sp;
	v[3] = 0;

	v[4] = sr * sp*cy - cr * sy;
	v[5] = sr * sp*sy + cr * cy;
	v[6] = -sr * cp;
	v[7] = 0;

	v[8] = -(cr*sp*cy + sr * sy);
	v[9] = cy * sr - cr * sp*sy;
	v[10] = cr * cp;
	v[11] = 0;

	v[12] = Location.x;
	v[13] = Location.y;
	v[14] = Location.z;
	v[15] = 1;
}

void calcProj(float fov)
{
	// diddle the fov a bit
	if (fov < 1e-3f)
		fov = 1e-3f;
	fov *= (float)(D3DX_PI / 360.f);

	// Unreal does a dance, lets skip it
	float x = 1;
	float y = (float)eRenderer.m_Width / eRenderer.m_Height;

	// Now we got our stuff, lets build the matrix
	float t = (float)(tan(fov));

	p[0] = x / t;
	p[1] = 0;
	p[2] = 0;
	p[3] = 0;

	p[4] = 0;
	p[5] = y / t;
	p[6] = 0;
	p[7] = 0;

	p[8] = 0;
	p[9] = 0;
	p[10] = 0;
	p[11] = 1;

	p[12] = 0;
	p[13] = 0;
	p[14] = 3;
	p[15] = 0;
}

void rot(float * m, float * a, float * b)
{
	m[0] = a[0] * b[0] + a[1] * b[4] + a[2] * b[8] + a[3] * b[12];
	m[1] = a[0] * b[1] + a[1] * b[5] + a[2] * b[9] + a[3] * b[13];
	m[2] = a[0] * b[2] + a[1] * b[6] + a[2] * b[10] + a[3] * b[14];
	m[3] = a[0] * b[3] + a[1] * b[7] + a[2] * b[11] + a[3] * b[15];
	m[4] = a[4] * b[0] + a[5] * b[4] + a[6] * b[8] + a[7] * b[12];
	m[5] = a[4] * b[1] + a[5] * b[5] + a[6] * b[9] + a[7] * b[13];
	m[6] = a[4] * b[2] + a[5] * b[6] + a[6] * b[10] + a[7] * b[14];
	m[7] = a[4] * b[3] + a[5] * b[7] + a[6] * b[11] + a[7] * b[15];
	m[8] = a[8] * b[0] + a[9] * b[4] + a[10] * b[8] + a[11] * b[12];
	m[9] = a[8] * b[1] + a[9] * b[5] + a[10] * b[9] + a[11] * b[13];
	m[10] = a[8] * b[2] + a[9] * b[6] + a[10] * b[10] + a[11] * b[14];
	m[11] = a[8] * b[3] + a[9] * b[7] + a[10] * b[11] + a[11] * b[15];
	m[12] = a[12] * b[0] + a[13] * b[4] + a[14] * b[8] + a[15] * b[12];
	m[13] = a[12] * b[1] + a[13] * b[5] + a[14] * b[9] + a[15] * b[13];
	m[14] = a[12] * b[2] + a[13] * b[6] + a[14] * b[10] + a[15] * b[14];
	m[15] = a[12] * b[3] + a[13] * b[7] + a[14] * b[11] + a[15] * b[15];
}

void calc_v(Vec3 rotation, Vec3 location)
{
	static float x2[16] =
	{
	  1, 0, 0, 0,
	  0, 1, 0, 0,
	  0, 0, 1, 0,
	  0, 0, 0, 1
	};
	static float y2[16] =
	{
	  1, 0, 0, 0,
	  0, 1, 0, 0,
	  0, 0, 1, 0,
	  0, 0, 0, 1
	};
	static float z2[16] =
	{
	  1, 0, 0, 0,
	  0, 1, 0, 0,
	  0, 0, 1, 0,
	  0, 0, 0, 1
	};
	// decompose rotatio into sin/cosine pairs
	float th = (float)(D3DX_PI * rotation.y / 180);
	float cy = (float)(cos(th));
	float sy = (float)(sin(th));
	 th = (float)(D3DX_PI * rotation.x / 180);
	float cp = (float)(cos(th));
	float sp = (float)(sin(th));
	 th = (float)(D3DX_PI * rotation.z / 180);
	float cr = (float)(cos(th));
	float sr = (float)(sin(th));
	/*Multiplies 3 axial matrices together to get the result*/

	//Multiply X and Y matrices together to get XY, stored in z2
	x2[0] = cy;
	x2[1] = -sy;
	x2[2] = 0;
	x2[3] = 0;

	x2[4] = sy;
	x2[5] = cy;
	x2[6] = 0;
	x2[7] = 0;

	x2[8] = 0;
	x2[9] = 0;
	x2[10] = 1;
	x2[11] = 0;

	x2[12] = 0;
	x2[13] = 0;
	x2[14] = 0;
	x2[15] = 1;

	y2[0] = cp;
	y2[1] = 0;
	y2[2] = -sp;
	y2[3] = 0;

	y2[4] = 0;
	y2[5] = 1;
	y2[6] = 0;
	y2[7] = 0;

	y2[8] = sp;
	y2[9] = 0;
	y2[10] = cp;
	y2[11] = 0;

	y2[12] = 0;
	y2[13] = 0;
	y2[14] = 0;
	y2[15] = 1;
	rot(z2, x2, y2);

	//Multiply XY with Z matrix to get XYZ and store in y2
	x2[0] = 1;
	x2[1] = 0;
	x2[2] = 0;
	x2[3] = 0;

	x2[4] = 0;
	x2[5] = cr;
	x2[6] = sr;
	x2[7] = 0;

	x2[8] = 0;
	x2[9] = -sr;
	x2[10] = cr;
	x2[11] = 0;

	x2[12] = 0;
	x2[13] = 0;
	x2[14] = 0;
	x2[15] = 1;
	rot(y2, z2, x2);

	//Add translation to XYZ
	x2[0] = 1;
	x2[1] = 0;
	x2[2] = 0;
	x2[3] = 0;

	x2[4] = 0;
	x2[5] = 1;
	x2[6] = 0;
	x2[7] = 0;

	x2[8] = 0;
	x2[9] = 0;
	x2[10] = 1;
	x2[11] = 0;

	x2[12] = -location.x;
	x2[13] = -location.y;
	x2[14] = -location.z;
	x2[15] = 1;
	rot(z2, x2, y2);

	//Multiply XYZ by flip matrix to get it out of unreal coordinates
	x2[0] = 0;
	x2[1] = 0;
	x2[2] = 1;
	x2[3] = 0;

	x2[4] = 1;
	x2[5] = 0;
	x2[6] = 0;
	x2[7] = 0;

	x2[8] = 0;
	x2[9] = 1;
	x2[10] = 0;
	x2[11] = 0;

	x2[12] = 0;
	x2[13] = 0;
	x2[14] = 0;
	x2[15] = 1;
	rot(v, z2, x2);
}

void SDK::Functions::update_matrix(SDK::FMinimalViewInfo& MinimalViewInfo)
{
	// Create out individual matrices
	calcProj(MinimalViewInfo.m_FieldOfView);
	calc_v(MinimalViewInfo.m_Rotation, MinimalViewInfo.m_Location);

	// then multiply them all together to get our beautiful matrix
	rot(pv, v, p);
}

bool SDK::Functions::WorldToScreen(SDK::UCanvas& Canvas, const Vec3& Position, Vec2* OutPos)
{
	float* ViewMatrix = pv;

	/*Ensure W component is not, nor close to, 0*/
	float w = Position.x * ViewMatrix[3] + Position.y * ViewMatrix[7] + Position.z * ViewMatrix[11] + ViewMatrix[15];

	if (w <= 1e-3)
		return false;

	/*Transform the point by pv and divide result by w to get device coords*/
	float invw = (float)1.0 / w;
	float dx = Position.x * ViewMatrix[0] + Position.y * ViewMatrix[4] + Position.z * ViewMatrix[8] + ViewMatrix[12];
	float dy = Position.x * ViewMatrix[1] + Position.y * ViewMatrix[5] + Position.z * ViewMatrix[9] + ViewMatrix[13];

	/*Device coords are -1~1, so scale them up to window coords*/
	OutPos->x = eRenderer.m_Width * (float)(0.5 + 0.5 * dx * invw);
	OutPos->y = eRenderer.m_Height * (float)(0.5 - 0.5 * dy * invw);

	if (OutPos->x > eRenderer.m_Width + 2 || OutPos->y > eRenderer.m_Height + 2)
		return false;


	return true;
}