#include "Menu.h"
#include "../Utility/ConfigManager.h"


void DrawMainMenu() {
	const char* tabs[] = { "Visuals", "Aimbot", "Misc", "Radar", "Settings" };
	int tabCount = sizeof(tabs) / sizeof(tabs[0]);

	float buttonHeight = 40.0f;
	float totalButtonHeight = tabCount * buttonHeight;
	float availableForButtons = ImGui::GetContentRegionAvail().y - 100;
	float spacingBetweenButtons = (availableForButtons - totalButtonHeight) / (tabCount + 1);
    spacingBetweenButtons = (std::max)(spacingBetweenButtons, 8.0f);

	ImGui::Dummy(ImVec2(0, spacingBetweenButtons * 0.6f));

	if (Config.headerFont) ImGui::PushFont(Config.headerFont);

	for (int i = 0; i < tabCount; ++i) {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));

		if (ImGui::Button(tabs[i], ImVec2(270, buttonHeight))) {
			selectedTab = i;
			currentPage = 1;
		}

		ImGui::PopStyleColor(3);
		ImGui::Dummy(ImVec2(0, spacingBetweenButtons * 0.7f));
	}

	if (Config.headerFont) ImGui::PopFont();
}
void DrawSidebar() {
	static bool wasDownLastFrame = false;

    bool isDown = (GetAsyncKeyState(Config.OpenMenuKey) & 0x8000 || mem.GetKeyboard()->IsKeyDown(Config.OpenMenuKey));

	if (isDown && !wasDownLastFrame) {
		Config.sidebarOpen = !Config.sidebarOpen;
		SetOverlayMode(Config.overlayHWND, Config.overlayMode, Config.sidebarOpen);
	}
	wasDownLastFrame = isDown;

	if (!Config.sidebarOpen) return;

	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4 darkBg = Config.bgColor;
	ImVec4 sectionBg = Config.headerColor;
	ImVec4 accentRed = Config.menuAccentColor;

	ImGui::SetNextWindowPos(ImVec2(Globals::screenWidth - 300, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(300, Globals::screenHeight), ImGuiCond_Always);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, darkBg);
	ImGui::PushStyleColor(ImGuiCol_Border, accentRed);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(18, 18));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 7.0f);

	ImGui::Begin("##Sidebar", nullptr,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoScrollbar);


	if (Config.logoTexture != 0) {
		float texW = static_cast<float>(Config.logoWidth);
		float texH = static_cast<float>(Config.logoHeight);
		if (texW <= 0 || texH <= 0) { texW = 1080.0f; texH = 608.0f; }

		// Calculate available content width (accounting for padding)
		float contentWidth = ImGui::GetContentRegionAvail().x;

		// Scale to fit content width while maintaining aspect ratio
		float scale = contentWidth / texW;
		float drawW = contentWidth;
		float drawH = texH * scale;

		// Get current cursor position to draw background rect
		ImVec2 cursorPos = ImGui::GetCursorScreenPos();

		// Draw a background rectangle matching the menu background color to hide logo edges
		ImGui::GetWindowDrawList()->AddRectFilled(
			cursorPos,
			ImVec2(cursorPos.x + drawW, cursorPos.y + drawH),
			ImColor(darkBg),
			0.0f
		);

		// Draw logo with no border
		ImGui::Image(Config.logoTexture, ImVec2(drawW, drawH),
			ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(0, 0, 0, 0));

		ImGui::Spacing();
	}

	ImGui::Separator();
	ImGui::Spacing();


	if (Config.logoFont) {
		ImGui::PushFont(Config.logoFont);
		const char* title = "Makimura.DeV";
		float txtW = ImGui::CalcTextSize(title).x;
		ImGui::SetCursorPosX((300.0f - txtW) * 0.5f);
		ImGui::TextColored(accentRed, "%s", title);
		ImGui::PopFont();
	}
	else {
		ImGui::SetCursorPosX((300.0f - ImGui::CalcTextSize("Makimura.dev[Fallback]").x) * 0.5f);
		ImGui::TextColored(accentRed, "Makimura.Dev[Fallback]");
	}

	ImGui::Spacing();
	if (Config.headerFont) {
		ImGui::PushFont(Config.headerFont);
		const char* sub = "COD Manager";
		float subW = ImGui::CalcTextSize(sub).x;
		ImGui::SetCursorPosX((300.0f - subW) * 0.5f);
		ImGui::TextColored(ImVec4(0.9f, 0.9f, 0.9f, 1.0f), "%s", sub);
		ImGui::PopFont();
	}

	ImGui::Spacing();
	

	float metricsHeight = 100.0f;
	float availableHeight = ImGui::GetContentRegionAvail().y;
	float mainContentHeight = availableHeight - metricsHeight - 60;

	ImGui::BeginChild("Content", ImVec2(0, mainContentHeight), false, ImGuiWindowFlags_NoScrollbar);

	if (Config.regularFont) ImGui::PushFont(Config.regularFont);


	if (currentPage == 0) {

		DrawMainMenu();
	}
	else {

		switch (selectedTab) {
		case 0: DrawVisualsTab(); break;
		case 1: DrawAimbotTab();  break;
		case 2: DrawMiscTab();    break;
		case 3: DrawRadarTab();  break;
		case 4: DrawSettingsTab(); break;
		}
	}

	if (Config.regularFont) ImGui::PopFont();
	ImGui::EndChild();


	if (currentPage > 0) {
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::PushStyleColor(ImGuiCol_Button, Config.headerColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(
			Config.headerColor.x * 1.2f,
			Config.headerColor.y * 1.2f,
			Config.headerColor.z * 1.2f,
			Config.headerColor.w
		));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(
			Config.headerColor.x * 0.8f,
			Config.headerColor.y * 0.8f,
			Config.headerColor.z * 0.8f,
			Config.headerColor.w
		));

		if (Config.headerFont) ImGui::PushFont(Config.headerFont);

		float buttonWidth = 270;
		float buttonHeight = 40.0f;
		ImGui::SetCursorPosX((300.0f - buttonWidth) * 0.5f);

		if (ImGui::Button("Back", ImVec2(buttonWidth, buttonHeight))) {
			currentPage = 0;
		}

		if (Config.headerFont) ImGui::PopFont();
		ImGui::PopStyleColor(3);
	}


	ImGui::Separator();
	ImGui::Spacing();

	ImVec2 metricsStart = ImGui::GetCursorScreenPos();
	ImVec2 metricsSize = ImVec2(264, 70);

	ImGui::GetWindowDrawList()->AddRectFilled(
		metricsStart,
		ImVec2(metricsStart.x + metricsSize.x, metricsStart.y + metricsSize.y),
		ImColor(Config.headerColor),
		4.0f
	);

	ImGui::GetWindowDrawList()->AddRect(
		metricsStart,
		ImVec2(metricsStart.x + metricsSize.x, metricsStart.y + metricsSize.y),
		ImColor(accentRed),
		4.0f,
		0,
		2.0f
	);

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

	if (Config.regularFont) ImGui::PushFont(Config.regularFont);

	std::ostringstream renderFpsStream;
	renderFpsStream << std::fixed << std::setprecision(0) << "OVERLAY: " << ImGui::GetIO().Framerate << " FPS";
	std::string renderFpsText = renderFpsStream.str();

	ImGui::SetCursorPosX((300.0f - ImGui::CalcTextSize(renderFpsText.c_str()).x) * 0.5f);
	ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.5f, 1.0f), "%s", renderFpsText.c_str());

	std::string gameFpsText = "DMA: " + std::to_string(Config.readFPS) + " FPS";
	ImGui::SetCursorPosX((300.0f - ImGui::CalcTextSize(gameFpsText.c_str()).x) * 0.5f);
	ImGui::TextColored(ImVec4(0.4f, 0.8f, 178.0f, 1.0f), "%s", gameFpsText.c_str());

	if (Config.regularFont) ImGui::PopFont();

	ImGui::End();
	ImGui::PopStyleVar(3);
	ImGui::PopStyleColor(2);
}


 void AddVerticalSpacing(float spacing) {
    ImGui::Dummy(ImVec2(0.0f, spacing));
    ImGui::Spacing();
}

 

static void HoverTooltip(const char* txt)
{
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("%s", txt);
}
static void DrawNamedColorPicker(const char* name, ImVec4& color)
{
    ImGui::Text("%s", name); 
    ImGuiColorEditFlags flags = ImGuiColorEditFlags_PickerHueWheel
        | ImGuiColorEditFlags_AlphaBar
        | ImGuiColorEditFlags_NoSidePreview
        | ImGuiColorEditFlags_NoInputs
        | ImGuiColorEditFlags_NoOptions;
    std::string id = std::string("##picker_") + name; 
    ImGui::ColorEdit4(id.c_str(), (float*)&color, flags);
    ImGui::Spacing();
}

 

 void DrawVisualsTab() {
    if (Config.headerFont) {
        ImGui::PushFont(Config.headerFont);
        ImGui::TextColored(ImVec4(1, 1, 1, 1), "VISUALS");
        ImGui::PopFont();
    }
    else {
        ImGui::TextColored(ImVec4(1, 1, 1, 1), "VISUALS");
    }

    ImGui::Separator();
    ImGui::Spacing();
    ImGui::PushItemWidth(-1);
    //also maybe tree node//

    auto SectionHeader = [&](const char* label, ImVec4 base, std::function<void()> body) {
        ScopedStyleColor c1(ImGuiCol_Header, base);
        ScopedStyleColor c2(ImGuiCol_HeaderHovered, ImVec4(base.x + 0.2f, base.y + 0.2f, base.z + 0.2f, 0.9f));
        ScopedStyleColor c3(ImGuiCol_HeaderActive, ImVec4(base.x + 0.3f, base.y + 0.3f, base.z + 0.3f, 1.0f));
        if (ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_DefaultOpen)) {
            AddVerticalSpacing(8.0f);
            body();
            AddVerticalSpacing(10.0f);
        }
        };

  
    SectionHeader("Player ESP", { 0.8f,0.3f,0.0f,0.7f }, [&]() {
        // Row 1: ESP and Bones
        ImGui::Checkbox("ESP", &Config.ESPEnabled);
        ImGui::SameLine(130);
        ImGui::Checkbox("Bones", &Config.DrawBones);

        // Row 2: Show Head and Show Name
        ImGui::Checkbox("Show Head", &Config.DrawHeadCircle);
        ImGui::SameLine(130);
        ImGui::Checkbox("Show Name", &Config.DrawNames);

        // Row 3: Health Bar and Snaplines
        ImGui::Checkbox("Health Bar", &Config.DrawHP);
        ImGui::SameLine(130);
        ImGui::Checkbox("Snaplines", &Config.DrawTraceline);

        // Row 4: Show Team
        ImGui::Checkbox("Show Team", &Config.TeamCheck);
        ImGui::SameLine(130);
        ImGui::Checkbox("Visible Only", &Config.VisibleOnly);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Box Style");
        ImGui::PushItemWidth(-1);
        ImGui::Combo("##boxstyle", &Config.BoxStyle, "Corner\0Full\0");
        ImGui::PopItemWidth();
        ImGui::Spacing();

        ImGui::Text("Max Player Distance");
        ImGui::SliderFloat("##DrawDistance", &Config.maxDistance, 10.f, 1000.f, "%.0f m");
        });

   
    SectionHeader("Item ESP", { 0.2f,0.7f,0.2f,0.7f }, [&]() {
        // Row 1: Enable Items and Draw Name
        ImGui::Checkbox("Enable Items", &Config.itemsEnabled);
        ImGui::SameLine(130);
        ImGui::Checkbox("Draw Name", &Config.DrawItemName);

        // Row 2: Draw Box
        ImGui::Checkbox("Draw Box", &Config.DrawItemBox);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Max Item Distance");
        ImGui::SliderFloat("##ItemDrawDistance", &Config.MaxItemDistance, 10.f, 1000.f, "%.0f m");
        });
    SectionHeader("ESP Colors", { 0.1f, 0.6f, 0.9f, 0.7f }, [&]() {
        ImGui::Text("Select ESP Element");

        static int selectedESPColor = 0;
        const char* espItems[] = {
            "Enemy Box",  "Team Box",
            "Bones", "Head Circle", "Items", "SnapLine", "FOV Circle"
        };

        ImGui::PushItemWidth(-1);
        if (ImGui::BeginCombo("##espcombo", espItems[selectedESPColor])) {
            for (int i = 0; i < IM_ARRAYSIZE(espItems); i++) {
                bool isSelected = (selectedESPColor == i);
                if (ImGui::Selectable(espItems[i], isSelected))
                    selectedESPColor = i;
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        ImGui::PopItemWidth();
        ImGui::Spacing();

        ImGui::Text("Edit Color");
        switch (selectedESPColor) {
        case 0: ImGui::ColorEdit4("##colorwheel", (float*)&Config.ColorEnemyBox, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel); break;
        case 1: ImGui::ColorEdit4("##colorwheel", (float*)&Config.ColorTeamBox, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel); break;
        case 2: ImGui::ColorEdit4("##colorwheel", (float*)&Config.ColorBones, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel); break;
        case 3: ImGui::ColorEdit4("##colorwheel", (float*)&Config.ColorHead, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel); break;
        case 4: ImGui::ColorEdit4("##colorwheel", (float*)&Config.ColorItems, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel); break;
        case 5: ImGui::ColorEdit4("##colorwheel", (float*)&Config.ColorLines, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel); break;
        case 6: ImGui::ColorEdit4("##colorwheel", (float*)&Config.ColorFOV, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel); break;
        }
        ImGui::Spacing();

        ImGui::Text("Item Thickness");
        ImGui::SliderFloat("##itemthickness", &Config.ItemThickness, 0.5f, 5.0f, "%.1f");
        ImGui::Spacing();

        ImGui::Text("Player Thickness");
        ImGui::SliderFloat("##playerthickness", &Config.PlayerThickness, 0.5f, 5.0f, "%.1f");

        ImGui::PopItemWidth();
        });
}
 void DrawSettingsTab()
{
    if (Config.headerFont) {
        ImGui::PushFont(Config.headerFont);
        ImGui::TextColored(ImVec4(1, 1, 1, 1), "SETTINGS");
        ImGui::PopFont();
    }
    else {
        ImGui::TextColored(ImVec4(1, 1, 1, 1), "SETTINGS");
    }

    ImGui::Separator();
    ImGui::Spacing();

    ImGui::PushItemWidth(-1);

    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.8f, 0.2f, 0.6f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.9f, 0.3f, 0.7f, 0.8f));
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(1.0f, 0.4f, 0.8f, 1.0f));
  
    if (ImGui::CollapsingHeader("Configs", ImGuiTreeNodeFlags_DefaultOpen)) {
        static char configName[64] = "default";
        static std::string statusMessage = "";
        static float statusTimer = 0.0f;
        static std::vector<std::string> configList;
        static int selectedConfigIndex = -1;
        static bool needRefresh = true;

        // Refresh config list
        if (needRefresh) {
            configList = ConfigManager::GetConfigList();
            needRefresh = false;
        }

        // Current Default Config Display
        std::string currentDefault = ConfigManager::GetDefaultConfig();
        ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.3f, 1.0f), "Default: %s", currentDefault.c_str());
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Config List
        ImGui::Text("Available Configs:");
        ImGui::BeginChild("ConfigList", ImVec2(0, 120), true);

        for (int i = 0; i < configList.size(); i++) {
            bool isDefault = (configList[i] == currentDefault);
            ImGui::PushID(i);

            if (isDefault) {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.3f, 1.0f, 0.3f, 1.0f));
            }

            bool selected = (selectedConfigIndex == i);
            if (ImGui::Selectable((configList[i] + (isDefault ? " [DEFAULT]" : "")).c_str(), selected)) {
                selectedConfigIndex = i;
                strncpy_s(configName, configList[i].c_str(), sizeof(configName) - 1);
            }

            if (isDefault) {
                ImGui::PopStyleColor();
            }

            // Double-click to load
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
                if (ConfigManager::LoadConfig(configList[i])) {
                    statusMessage = "Loaded: " + configList[i];
                    statusTimer = 3.0f;
                }
            }

            ImGui::PopID();
        }

        if (configList.empty()) {
            ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "No configs found");
        }

        ImGui::EndChild();
        ImGui::Spacing();

        // Config Name Input
        ImGui::Text("Config Name:");
        ImGui::InputText("##configname", configName, sizeof(configName));
        ImGui::Spacing();

        // Action Buttons Row 1
        float availableWidth = ImGui::GetContentRegionAvail().x;
        float buttonWidth = (availableWidth - 8.0f) / 3.0f; // 3 buttons with small spacing

        if (ImGui::Button("Save", ImVec2(buttonWidth, 28))) {
            if (strlen(configName) > 0) {
                if (ConfigManager::SaveConfig(std::string(configName))) {
                    statusMessage = "Saved: " + std::string(configName);
                    statusTimer = 3.0f;
                    needRefresh = true;
                } else {
                    statusMessage = "Failed to save!";
                    statusTimer = 3.0f;
                }
            }
        }
        ImGui::SameLine(0, 4.0f);

        if (ImGui::Button("Load", ImVec2(buttonWidth, 28))) {
            if (ConfigManager::LoadConfig(std::string(configName))) {
                statusMessage = "Loaded: " + std::string(configName);
                statusTimer = 3.0f;
            } else {
                statusMessage = "Config not found!";
                statusTimer = 3.0f;
            }
        }
        ImGui::SameLine(0, 4.0f);

        if (ImGui::Button("Delete", ImVec2(buttonWidth, 28))) {
            if (strlen(configName) > 0 && selectedConfigIndex >= 0) {
                if (ConfigManager::DeleteConfig(std::string(configName))) {
                    statusMessage = "Deleted: " + std::string(configName);
                    statusTimer = 3.0f;
                    needRefresh = true;
                    selectedConfigIndex = -1;
                    strcpy_s(configName, "default");
                } else {
                    statusMessage = "Failed to delete!";
                    statusTimer = 3.0f;
                }
            }
        }

        ImGui::Spacing();

        // Set Default & Refresh Buttons
        availableWidth = ImGui::GetContentRegionAvail().x;
        float button2Width = (availableWidth - 4.0f) / 2.0f; // 2 buttons with small spacing

        if (ImGui::Button("Set as Default", ImVec2(button2Width, 28))) {
            if (strlen(configName) > 0) {
                if (ConfigManager::SetDefaultConfig(std::string(configName))) {
                    statusMessage = "Default set: " + std::string(configName);
                    statusTimer = 3.0f;
                } else {
                    statusMessage = "Failed to set default!";
                    statusTimer = 3.0f;
                }
            }
        }
        ImGui::SameLine(0, 4.0f);

        if (ImGui::Button("Refresh List", ImVec2(button2Width, 28))) {
            needRefresh = true;
            statusMessage = "List refreshed";
            statusTimer = 2.0f;
        }

        ImGui::Spacing();

        // Status Message
        if (statusTimer > 0.0f) {
            statusTimer -= ImGui::GetIO().DeltaTime;
            ImVec4 color = (statusMessage.find("Failed") != std::string::npos ||
                           statusMessage.find("not found") != std::string::npos)
                          ? ImVec4(1.0f, 0.3f, 0.3f, 1.0f)
                          : ImVec4(0.3f, 1.0f, 0.3f, 1.0f);
            ImGui::TextColored(color, "%s", statusMessage.c_str());
        }

        ImGui::Spacing();
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "Tip: Double-click config to load");
        ImGui::Spacing();
    }
    ImGui::PopStyleColor(3);

    ImGui::Separator();
    ImGui::Spacing();

  
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.5f, 0.2f, 0.8f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.6f, 0.3f, 0.9f, 0.8f));
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.7f, 0.4f, 1.0f, 1.0f));

    if (ImGui::CollapsingHeader("Menu", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Text("Monitor");
        static int currentMonitor = 0;
        int monitorCount = GetSystemMetrics(SM_CMONITORS);
        ImGui::PushItemWidth(-1);
        if (ImGui::Combo("##monitor", &currentMonitor,
            [](void*, int idx, const char** out_text) {
                static char buffer[64];
                snprintf(buffer, sizeof(buffer), "Monitor %d", idx + 1);
                *out_text = buffer;
                return true;
            }, nullptr, monitorCount)) {
            SwitchToMonitor(Config.overlayHWND, currentMonitor);
        }
        ImGui::PopItemWidth();
        ImGui::Spacing();

        ImGui::Text("Overlay Mode");
        const char* overlayModes[] = { "Transparent ESP", "Black Fuser" };
        ImGui::PushItemWidth(-1);
        if (ImGui::Combo("##overlaymode", &Config.overlayMode, overlayModes, 2)) {
            SetOverlayMode(Config.overlayHWND, Config.overlayMode, Config.sidebarOpen);
        }
        ImGui::PopItemWidth();
        ImGui::Spacing();
    }

   
    ImGui::PopStyleColor(3);
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.8f, 0.3f, 0.0f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(1.0f, 0.4f, 0.0f, 0.8f));
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(1.0f, 0.5f, 0.0f, 1.0f));

    if (ImGui::CollapsingHeader("Options", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Text("Vsync");
        ImGui::Checkbox("##showfps", &Config.vsync);
        ImGui::Spacing();
        ImGui::Text("Menu Keybind");
        static int currentKeyIndex = 0;
        const char* keyItems[] = { "INSERT", "DELETE", "HOME", "END", "F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","F11","F12" };
        const int keyValues[] = { VK_INSERT,VK_DELETE,VK_HOME,VK_END,VK_F1,VK_F2,VK_F3,VK_F4,VK_F5,VK_F6,VK_F7,VK_F8,VK_F9,VK_F10,VK_F11,VK_F12 };
        ImGui::PushItemWidth(-1);
        if (ImGui::BeginCombo("##menukey", keyItems[currentKeyIndex])) {
            for (int i = 0; i < IM_ARRAYSIZE(keyItems); i++) {
                bool isSelected = (currentKeyIndex == i);
                if (ImGui::Selectable(keyItems[i], isSelected)) {
                    currentKeyIndex = i;
                }
                if (isSelected) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
       
        ImGui::PopItemWidth();
        ImGui::Spacing();

        ImGui::Text("Refresh Cheat");
        if (ImGui::Button("Refresh##refreshcheat", ImVec2(-1, 0))) {
            Config.refreshcheat = !Config.refreshcheat;
        }
        ImGui::Spacing();

        ImGui::Text("Theme Editor");
        static int selectedColor = 0;
        const char* colorItems[] = { "Menu Accent", "Text Color", "Background", "Header Color" };
        ImGui::PushItemWidth(-1);
        if (ImGui::BeginCombo("##themecombo", colorItems[selectedColor])) {
            for (int i = 0; i < IM_ARRAYSIZE(colorItems); i++) {
                bool isSelected = (selectedColor == i);
                if (ImGui::Selectable(colorItems[i], isSelected)) selectedColor = i;
                if (isSelected) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        ImGui::PopItemWidth();
        ImGui::Spacing();

        ImGui::Text("Edit Color");
        switch (selectedColor)
        {
        case 0: ImGui::ColorEdit4("##colorwheel", (float*)&Config.menuAccentColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel); break;
        case 1: ImGui::ColorEdit4("##colorwheel", (float*)&Config.textColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel); break;
        case 2: ImGui::ColorEdit4("##colorwheel", (float*)&Config.bgColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel); break;
        case 3: ImGui::ColorEdit4("##colorwheel", (float*)&Config.headerColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel); break;
        }
        ImGui::Spacing();
        if (ImGui::Button("Reset Colors", ImVec2(-1, 0)))
        {
            Config.menuAccentColor = ImVec4(0.8f, 0.2f, 0.6f, 1.0f);
            Config.textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
            Config.bgColor = ImVec4(0.1f, 0.1f, 0.1f, 0.9f);
            Config.headerColor = ImVec4(0.5f, 0.2f, 0.8f, 1.0f);
        }
        ImGui::Spacing();
    }
    ImGui::PopStyleColor(3);

}



 void DrawMiscTab() {
    if (Config.headerFont) ImGui::PushFont(Config.headerFont);
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "MISCELLANEOUS");
    if (Config.headerFont) ImGui::PopFont();
    ImGui::Separator();

}

void DrawRadarTab() {
    if (Config.headerFont) ImGui::PushFont(Config.headerFont);
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "RADAR SETTINGS");
    if (Config.headerFont) ImGui::PopFont();
    ImGui::Separator();


}

void DrawAimbotTab() {
    if (Config.headerFont) {
        ImGui::PushFont(Config.headerFont);
        ImGui::TextColored(ImVec4(1, 1, 1, 1), "AIMBOT");
        ImGui::PopFont();
    }
    else {
        ImGui::TextColored(ImVec4(1, 1, 1, 1), "AIMBOT");
    }

    ImGui::Separator();
    ImGui::Spacing();
    ImGui::PushItemWidth(-1);

    auto SectionHeader = [&](const char* label, ImVec4 base, std::function<void()> body) {
        ScopedStyleColor c1(ImGuiCol_Header, base);
        ScopedStyleColor c2(ImGuiCol_HeaderHovered, ImVec4(base.x + 0.2f, base.y + 0.2f, base.z + 0.2f, 0.9f));
        ScopedStyleColor c3(ImGuiCol_HeaderActive, ImVec4(base.x + 0.3f, base.y + 0.3f, base.z + 0.3f, 1.0f));
        if (ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_DefaultOpen)) {
            AddVerticalSpacing(8.0f);
            body();
            AddVerticalSpacing(10.0f);
        }
        };

    SectionHeader("Aimbot", {0.8f,0.3f,0.0f,0.7f}, [&]() {
        ImGui::Checkbox("Enable Aimbot", &Config.EnableAimbot);
        ImGui::Checkbox("Ignore Knocked", &Config.IgnoreKnocked);
		ImGui::Checkbox("Visible Check", &Config.AimbotVisibleOnly);
        ImGui::Checkbox("Ignore Team", &Config.ignoreteam);
        HoverTooltip("Useful for FFA Modes ");
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Keybind("Aimbot Hotkey", &Config.AimbotKey, &Config.AimbotMode,false);

        ImGui::Spacing();

        //target selection combo
      

        ImGui::Spacing();

        ImGui::Text("Smoothing");
        ImGui::SliderFloat("##aimbot_smooth", &Globals::smoothing, 1.0f, 20.0f, "%.1f");
        HoverTooltip("Higher = slower, smoother aim");
        ImGui::Spacing();

        ImGui::Text("FOV Radius (pixels)");
        ImGui::SliderFloat("##aimbot_fov", &Config.fov, 10.0f, 500.0f, "%.0f px");
        HoverTooltip("FOV circle radius in pixels from screen center");
        ImGui::Spacing();

        ImGui::Text("Max Aim Distance");
        ImGui::SliderFloat("##MaxAimdist", &Config.MaxAimDistance, 10.f, 1000.f, "%.0f m");
        });
    
 SectionHeader("Triggerbot", {0.8f,0.0f,0.4f,0.2f}, [&]() {
         // Row 1: Enable Items and Draw Name
         ImGui::Checkbox("Enable Triggerbot ", &Config.EnableTrigger);
         HoverTooltip("Uses Existing Aimbot Pipeline");
        

         // Row 2: Draw Box
         ImGui::Keybind("Triggerbot Hotkey", &Config.TriggerbotKey, &Config.TriggerbotMode,false);
  
         ImGui::Spacing();

         ImGui::Spacing();

		 ImGui::Text("Triggerbot Delay (ms)");
		 ImGui::SliderInt("##triggerdelay", &Config.TriggerDelay, 0, 500, "%d ms");
       
         ImGui::Spacing();
         ImGui::Separator();
         ImGui::Spacing();

         ImGui::Text("Max Triggerbot Distance");
         ImGui::SliderFloat("##MaxTriggerdist", &Config.MaxTriggerDistance, 10.f, 1000.f, "%.0f m");
         });
  
}


