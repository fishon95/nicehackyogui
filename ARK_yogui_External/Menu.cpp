#include "Menu.h"

#include "skCrypt.h"
#include "VMProtect/VMProtectSDK.h"

#include "NvidiaOverlay/NvidiaOverlay.h"

Menu eMenu;

bool Menu::ImGuiSetupColors()
{
	ImVec4* colors = ImGui::GetStyle().Colors;
	ImGuiStyle& style = (ImGui::GetStyle());
	style.FrameBorderSize = 0.0f;
	style.WindowTitleAlign.x = 0.50f;
	style.ChildRounding = 2;
	style.WindowRounding = 0;
	style.WindowPadding.x = 9;
	style.ItemInnerSpacing.x = 5;
	style.FramePadding.y = 1;
	style.ScrollbarSize = 3;
	colors[0] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[1] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[2] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[3] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[4] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
	colors[5] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[6] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[7] = ImVec4(0.61f, 0.15f, 0.07f, 1.00f);
	colors[8] = ImVec4(0.94f, 0.32f, 0.08f, 1.00f);
	colors[9] = ImVec4(0.92f, 0.35f, 0.08f, 1.00f);
	colors[10] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	colors[11] = ImVec4(0.74f, 0.14f, 0.14f, 1.00f);
	colors[12] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[13] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[14] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[15] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[16] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[17] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[18] = ImVec4(0.93f, 0.30f, 0.09f, 1.00f);
	colors[19] = ImVec4(0.87f, 0.10f, 0.18f, 1.00f);
	colors[20] = ImVec4(0.89f, 0.03f, 0.03f, 1.00f);
	colors[21] = ImVec4(0.75f, 0.08f, 0.08f, 1.00f);
	colors[22] = ImVec4(0.95f, 0.10f, 0.07f, 0.70f);
	colors[23] = ImVec4(0.92f, 0.27f, 0.09f, 1.00f);
	colors[24] = ImVec4(0.39f, 0.15f, 0.07f, 0.98f);
	colors[25] = ImVec4(0.98f, 0.41f, 0.26f, 1.00f);
	colors[26] = ImVec4(0.98f, 0.34f, 0.26f, 1.00f);
	colors[27] = ImVec4(0.99f, 0.96f, 0.00f, 0.50f);
	colors[28] = ImVec4(0.75f, 0.17f, 0.10f, 0.78f);
	colors[29] = ImVec4(0.75f, 0.28f, 0.10f, 1.00f);
	colors[30] = ImVec4(0.98f, 0.36f, 0.26f, 0.25f);
	colors[31] = ImVec4(0.98f, 0.41f, 0.26f, 0.67f);
	colors[32] = ImVec4(0.98f, 0.39f, 0.26f, 0.95f);
	colors[33] = ImVec4(0.91f, 0.32f, 0.13f, 0.86f);
	colors[34] = ImVec4(0.93f, 0.09f, 0.09f, 0.80f);
	colors[35] = ImVec4(0.30f, 0.17f, 0.14f, 1.00f);
	colors[36] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
	colors[37] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
	colors[38] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[39] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[40] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[41] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[42] = ImVec4(0.57f, 0.29f, 0.10f, 1.00f);
	colors[43] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[44] = ImVec4(0.61f, 0.13f, 0.11f, 1.00f);
	colors[45] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[46] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[47] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	return true;
}

bool Menu::ImGuiColoredButton(const char* name, const ImVec2& Size, ImColor Color)
{
	float sc = (1.0f / 255.f);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)Color);
	Color.Value.x += 10 * sc;
	Color.Value.y += 10 * sc;
	Color.Value.z += 10 * sc;
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)Color);
	Color.Value.x += 10 * sc;
	Color.Value.y += 10 * sc;
	Color.Value.z += 10 * sc;
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)Color);
	bool ret = ImGui::Button(name, Size);
	ImGui::PopStyleColor(3);
	return ret;
}

bool Menu::DrawAimbotTab()
{
	if (ImGui::CollapsingHeader(XorString("Aimbot-Settings:")))
	{
		ImGui::Checkbox(XorString("Lock-Target###AimLock"), &eMenu.m_AimbotFeatures.MainSettings.LockTarget);
		ImGui::Separator();
		ImGui::CostomSliderFloat(m_pSliderFont, XorString("Field-Of-View"), &eMenu.m_AimbotFeatures.MainSettings.FieldOfView, 1.f, 720.f);
		ImGui::Separator();
		ImGui::Checkbox(XorString("Draw-FieldOfView"), &eMenu.m_AimbotFeatures.MainSettings.DrawFOV);
	}
	if (ImGui::CollapsingHeader(XorString("Players:")))
	{
		ImGui::Checkbox(XorString("Enabled###AimbotPlayerEnabled"), &eMenu.m_AimbotFeatures.Players.Enabled);
		ImGui::Separator();
		ImGui::Checkbox(XorString("Aimt-At-TribeMembers###AimbotTribePlayers"), &eMenu.m_AimbotFeatures.Players.AimAtTribeMembers);
		ImGui::Separator();
		ImGui::Text(XorString("Aim-Spot: "));
		ImGui::SameLine();

		static const char* AimSpotModes[] = { "Head", "Body" };
		ImGui::Combo(XorString("###AimSpotSelection1"), &eMenu.m_AimbotFeatures.Players.AimSpot,
			AimSpotModes, IM_ARRAYSIZE(AimSpotModes));
	}
	return true;
}

bool Menu::Draw3DRadarTab()
{
	if (ImGui::CollapsingHeader(XorString("Players:")))
	{
		ImGui::Checkbox(XorString("Enabled###PlayerEnabled"), &eMenu.m_3DRadarFeatures.PlayerSettings.Enabled);
		ImGui::Separator();
		ImGui::Checkbox(XorString("Box###PlayerBox"), &eMenu.m_3DRadarFeatures.PlayerSettings.Box);
		ImGui::Checkbox(XorString("Bones###PlayerBones"), &eMenu.m_3DRadarFeatures.PlayerSettings.Bones);
		ImGui::Checkbox(XorString("Name###PlayerName"), &eMenu.m_3DRadarFeatures.PlayerSettings.Name);
		ImGui::Checkbox(XorString("Active-Item###PlayerItem"), &eMenu.m_3DRadarFeatures.PlayerSettings.ActiveItem);
		ImGui::Checkbox(XorString("Level###PlayerLevel"), &eMenu.m_3DRadarFeatures.PlayerSettings.Level);
		ImGui::Checkbox(XorString("Tribe###PlayerTribe"), &eMenu.m_3DRadarFeatures.PlayerSettings.Tribe);
		ImGui::Checkbox(XorString("Distance###PlayerDist"), &eMenu.m_3DRadarFeatures.PlayerSettings.Distance);
		ImGui::Checkbox(XorString("Health###PlayerHealth"), &eMenu.m_3DRadarFeatures.PlayerSettings.Healthbar);
		ImGui::Checkbox(XorString("Torpor###PlayerTorpor"), &eMenu.m_3DRadarFeatures.PlayerSettings.Torpor);
		ImGui::Separator();
		ImGui::Checkbox(XorString("Draw-Dead###PlayersDrawDead"), &eMenu.m_3DRadarFeatures.PlayerSettings.DrawDead);
		ImGui::Checkbox(XorString("Draw-Sleepers###PlayersDrawSleepers"), &eMenu.m_3DRadarFeatures.PlayerSettings.DrawSleeping);
		ImGui::Checkbox(XorString("Draw-TribeMembers###PlayersDrawTribe"), &eMenu.m_3DRadarFeatures.PlayerSettings.DrawTribeMembers);
	}

	if (ImGui::CollapsingHeader(XorString("Dinosaures:")))
	{
		ImGui::Checkbox(XorString("Enabled###DinosEnabled"), &eMenu.m_3DRadarFeatures.DinoSettings.Enabled);
		ImGui::Separator();
		ImGui::Checkbox(XorString("Name###DinoName"), &eMenu.m_3DRadarFeatures.DinoSettings.Name);
		ImGui::Checkbox(XorString("Level###DinoLevel"), &eMenu.m_3DRadarFeatures.DinoSettings.Level);
		ImGui::Checkbox(XorString("Distance###DinoDist"), &eMenu.m_3DRadarFeatures.DinoSettings.Distance);
		ImGui::Checkbox(XorString("Health###DinoHealth"), &eMenu.m_3DRadarFeatures.DinoSettings.Healthbar);
		ImGui::Separator();
		ImGui::Checkbox(XorString("Draw-LandHerbivores"), &eMenu.m_3DRadarFeatures.DinoSettings.DrawLandHerbivores);
		ImGui::Checkbox(XorString("Draw-LandCarinivores"), &eMenu.m_3DRadarFeatures.DinoSettings.DrawLandCarnivores);
		ImGui::Checkbox(XorString("Draw-WaterHerbivores"), &eMenu.m_3DRadarFeatures.DinoSettings.DrawWaterHerbivores);
		ImGui::Checkbox(XorString("Draw-WaterCarnivores"), &eMenu.m_3DRadarFeatures.DinoSettings.DrawWaterCarnivores);
		ImGui::Separator();
		ImGui::Checkbox(XorString("Draw-OnlyTamedDinos"), &eMenu.m_3DRadarFeatures.DinoSettings.DrawOnlyTamedDinos);
		ImGui::Checkbox(XorString("Draw-TribeDinos"), &eMenu.m_3DRadarFeatures.DinoSettings.DrawTribeDinos);
		ImGui::Separator();
		ImGui::CostomSliderFloat(m_pSliderFont, XorString("Land-Carnivore-DrawDistance"), &eMenu.m_3DRadarFeatures.DinoSettings.MaxDrawLandCarnivoreDistance, 1.f, 500.f);
		ImGui::CostomSliderFloat(m_pSliderFont, XorString("Water-Carnivore-DrawDistance"), &eMenu.m_3DRadarFeatures.DinoSettings.MaxDrawWaterCarnivoreDistance, 1.f, 500.f);
		ImGui::CostomSliderFloat(m_pSliderFont, XorString("Land-Herbivore-DrawDistance"), &eMenu.m_3DRadarFeatures.DinoSettings.MaxDrawLandHerbivoreDistance, 1.f, 500.f);
		ImGui::CostomSliderFloat(m_pSliderFont, XorString("Water-Herbivore-DrawDistance"), &eMenu.m_3DRadarFeatures.DinoSettings.MaxDrawWaterHerbivoreDistance, 1.f, 500.f);
	}
	if (ImGui::CollapsingHeader(XorString("Structures:")))
	{
		ImGui::Checkbox(XorString("Enabled###StructuresEnabled"), &eMenu.m_3DRadarFeatures.StructureSettings.Enabled);
		ImGui::Checkbox(XorString("Draw-Item-Count###StructuresItemCountEnabled"), &eMenu.m_3DRadarFeatures.StructureSettings.DrawItemCount);
		ImGui::Separator();
		ImGui::Checkbox(XorString("Small-Storage-Box###SmallStorage"), &eMenu.m_3DRadarFeatures.StructureSettings.StorageBox);
		ImGui::Checkbox(XorString("Large-Storage-Box###LargeStorageBox"), &eMenu.m_3DRadarFeatures.StructureSettings.LargeStorageBox);
		ImGui::Checkbox(XorString("Vault###Vault"), &eMenu.m_3DRadarFeatures.StructureSettings.Vault);
		ImGui::Checkbox(XorString("Item-Cache###ItemCache"), &eMenu.m_3DRadarFeatures.StructureSettings.ItemCache);
		ImGui::Checkbox(XorString("Giant-Beaver-Dam###GiantBeaverDam"), &eMenu.m_3DRadarFeatures.StructureSettings.GiantBeaverDam);
		ImGui::Checkbox(XorString("Cryofridge###Cryo"), &eMenu.m_3DRadarFeatures.StructureSettings.Cryofridge);
		ImGui::Checkbox(XorString("Dedicated-Storage###DediStorage"), &eMenu.m_3DRadarFeatures.StructureSettings.DedicatedStorage);
		ImGui::Checkbox(XorString("Preserving-Bin###PreservinbBin"), &eMenu.m_3DRadarFeatures.StructureSettings.PreservingBin);
		ImGui::Separator();
		ImGui::Checkbox(XorString("Loot-Crate###LootCrate"), &eMenu.m_3DRadarFeatures.StructureSettings.LootCrate);
		ImGui::Checkbox(XorString("Artifact###Artifact"), &eMenu.m_3DRadarFeatures.StructureSettings.Artifact);
		ImGui::Separator();
		ImGui::Checkbox(XorString("Smithy###Smithy"), &eMenu.m_3DRadarFeatures.StructureSettings.Smithy);
		ImGui::Checkbox(XorString("Fabricator###Fabricator"), &eMenu.m_3DRadarFeatures.StructureSettings.Fabricator);
		ImGui::Checkbox(XorString("Mortar-Pestle###MortarPestle"), &eMenu.m_3DRadarFeatures.StructureSettings.MortarAndPestle);
		ImGui::Separator();
		ImGui::Checkbox(XorString("Tek-Transmitter###TekTrans"), &eMenu.m_3DRadarFeatures.StructureSettings.TekTransmitter);
		ImGui::Checkbox(XorString("Tek-Replicator###TekRepli"), &eMenu.m_3DRadarFeatures.StructureSettings.TekReplicator);
		ImGui::Checkbox(XorString("Tek-Teleporter###TekTele"), &eMenu.m_3DRadarFeatures.StructureSettings.TekTeleporter);
		ImGui::Checkbox(XorString("Tek-Forcefield###TekForce"), &eMenu.m_3DRadarFeatures.StructureSettings.TekForcefield);
		ImGui::Separator();
		ImGui::Checkbox(XorString("Automated-Turret###AutomatedTurret"), &eMenu.m_3DRadarFeatures.StructureSettings.AutoTurret);
		ImGui::Checkbox(XorString("Heavy-Turret###HeavyTurret"), &eMenu.m_3DRadarFeatures.StructureSettings.HeavyTurret);
		ImGui::Checkbox(XorString("Tek-Turret###TekTurret"), &eMenu.m_3DRadarFeatures.StructureSettings.TekTurret);
		ImGui::Separator();
		ImGui::Checkbox(XorString("Power-Generator###Genny"), &eMenu.m_3DRadarFeatures.StructureSettings.Generator);
		ImGui::Checkbox(XorString("Tek-Generator###TekGenny"), &eMenu.m_3DRadarFeatures.StructureSettings.TekGenerator);
	}
	return true;
}

void Menu::DrawMiscTab()
{
	if (ImGui::CollapsingHeader(XorString("Weapon-Exploits:"), ImGuiTreeNodeFlags_Leaf))
	{
		ImGui::Checkbox(XorString("Enabled###MiscWepEnabled"), &eMenu.m_MiscFeatures.Weapon.Enabled);
		ImGui::Separator();
		ImGui::Checkbox(XorString("No-Camera-Shake###NoCamShake"), &eMenu.m_MiscFeatures.Weapon.NoCameraShake);
		ImGui::Checkbox(XorString("No-Spread###NoSpread"), &eMenu.m_MiscFeatures.Weapon.NoSpread);
		ImGui::Checkbox(XorString("No-AimSway###NoAimSway"), &eMenu.m_MiscFeatures.Weapon.NoAimSway);
	}
}

bool Menu::Initialize()
{
	VMProtectBeginMutation("Menu.init.1");

	if (m_Initialized)
		return true;

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = 1.0f / 60.0f;
#if(!USE_GAMEBAR)
	ImGui_ImplWin32_Init(eNvidiaOverlay.m_hOverlayHandle);
	ImGui_ImplDX9_Init(eRenderer.GetDevice());
#else
	ImGui_ImplWin32_Init(eDXWindow.m_hWindowHandle);
	ImGui_ImplDX9_Init(eDXWindow.m_pDevice);
#endif

	m_pMainFont = io.Fonts->AddFontFromFileTTF(XorString("C:\\Windows\\Fonts\\Candara.ttf"), 14);
	m_pSliderFont = io.Fonts->AddFontFromFileTTF(XorString("C:\\Windows\\Fonts\\Candara.ttf"), 12);

	if (m_pMainFont == nullptr || m_pSliderFont == nullptr)
		return false;

	//if (!eKeyListener.RegisterKey(MENU_KEY, 300))
	//	return false;

	this->ImGuiSetupColors();

	m_Initialized = true;

	VMProtectEnd();

	return m_Initialized;
}

bool Menu::BeginImGuiScene()
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	return true;
}

bool Menu::EndImGuiScene()
{
	ImGui::EndFrame();

	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	return true;
}

void Menu::Render()
{
	if (!this->m_Initialized || !this->m_IsOpen)
		return;

	//ImGui::ShowDemoWindow();

	ImGui::SetNextWindowSize(ImVec2(337, 337));
	ImGui::Begin(XorString("yogui"), &eMenu.m_IsOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	{
		const ImVec2 ButtonSize = ImVec2(101, 20);
		if (this->ImGuiColoredButton(XorString("Aimbot"), ButtonSize, ImColor::ImColor(100, 17, 30)))
		{
			m_ActiveTab = eTab_Aimbot;
		}
		ImGui::SameLine();
		if (this->ImGuiColoredButton(XorString("Visuals"), ButtonSize, ImColor::ImColor(100, 17, 30)))
		{
			m_ActiveTab = eTab_3DRadar;
		}
		ImGui::SameLine();
		if (this->ImGuiColoredButton(XorString("Misc"), ButtonSize, ImColor::ImColor(100, 17, 30)))
		{
			m_ActiveTab = eTab_Misc;
		}
		ImGui::Separator();

		switch (m_ActiveTab)
		{
		case eTab_Aimbot:
			this->DrawAimbotTab();
			break;
		case eTab_3DRadar:
			this->Draw3DRadarTab();
			break;
		case eTab_Misc:
			this->DrawMiscTab();
			break;
		default:
			break;
		}
	}
	ImGui::End();
}

void Menu::Work()
{
	if (GetAsyncKeyState(VK_F1) & 1)
	{
		eMenu.m_IsOpen = !eMenu.m_IsOpen;
		this->UpdateClickableState(m_IsOpen);
		if(!m_IsOpen)
			SetForegroundWindow(eNvidiaOverlay.m_TargetHandle); 
	}
	if (m_IsOpen)
		SetForegroundWindow(eNvidiaOverlay.m_hOverlayHandle);
}

bool Menu::UpdateClickableState(bool MenuOpen)
{
	if (!eNvidiaOverlay.m_Initialized)
		return false;
	long style = GetWindowLongA(eNvidiaOverlay.m_hOverlayHandle, GWL_EXSTYLE);
	if (MenuOpen)
	{
		style &= ~WS_EX_LAYERED;
		SetWindowLongA(eNvidiaOverlay.m_hOverlayHandle,
			GWL_EXSTYLE, style);
		SetForegroundWindow(eNvidiaOverlay.m_hOverlayHandle);
	}
	else
	{
		style |= WS_EX_LAYERED;
		SetWindowLongA(eNvidiaOverlay.m_hOverlayHandle,
			GWL_EXSTYLE, style);
	}
	return true;
}

void Menu::ResetDevice()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	HRESULT hr = NULL;

	if (eRenderer.GetDevice())
		hr = eRenderer.GetDevice()->Reset(&eRenderer.m_d3dpp);

	if (hr == D3DERR_INVALIDCALL)
		IM_ASSERT(0);
	ImGui_ImplDX9_CreateDeviceObjects();
}
