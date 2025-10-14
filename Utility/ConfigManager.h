#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <Windows.h>
#include <ShlObj.h>
#include "Globals.h"

namespace ConfigManager {

    inline const std::string& GetConfigDirectory() {
        static std::string cachedPath = []() {
            char documentsPath[MAX_PATH];
            if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, 0, documentsPath))) {
                std::string path = std::string(documentsPath) + "\\Makimura";
                CreateDirectoryA(path.c_str(), NULL);
                return path;
            }
            return std::string("");
        }();
        return cachedPath;
    }

    inline std::string GetConfigPath(const std::string& configName) {
        return GetConfigDirectory() + "\\" + configName + ".cfg";
    }

    inline std::string GetDefaultConfigPath() {
        return GetConfigDirectory() + "\\default.txt";
    }

    inline std::vector<std::string> GetConfigList() {
        std::vector<std::string> configs;
        std::string searchPath = GetConfigDirectory() + "\\*.cfg";

        WIN32_FIND_DATAA findData;
        HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);

        if (hFind != INVALID_HANDLE_VALUE) {
            do {
                std::string filename = findData.cFileName;
                size_t dotPos = filename.rfind('.');
                if (dotPos != std::string::npos) {
                    configs.push_back(filename.substr(0, dotPos));
                }
            } while (FindNextFileA(hFind, &findData));
            FindClose(hFind);
        }

        return configs;
    }

    inline bool SetDefaultConfig(const std::string& configName) {
        std::ofstream file(GetDefaultConfigPath());
        if (!file.is_open()) return false;
        file << configName;
        return true;
    }

    inline std::string GetDefaultConfig() {
        std::ifstream file(GetDefaultConfigPath());
        if (!file.is_open()) return "default";

        std::string defaultName;
        std::getline(file, defaultName);
        return defaultName.empty() ? "default" : defaultName;
    }

    inline bool DeleteConfig(const std::string& configName) {
        return DeleteFileA(GetConfigPath(configName).c_str()) != 0;
    }

    inline void WriteColor(std::ofstream& file, const char* name, const ImVec4& color) {
        file << name << "=" << color.x << "," << color.y << "," << color.z << "," << color.w << "\n";
    }

    inline bool SaveConfig(const std::string& configName) {
        std::string path = GetConfigPath(configName);
        std::ofstream file(path);

        if (!file.is_open()) return false;
        file << "ESPEnabled=" << Config.ESPEnabled << "\n";
        file << "DrawBones=" << Config.DrawBones << "\n";
        file << "DrawHeadCircle=" << Config.DrawHeadCircle << "\n";
        file << "DrawNames=" << Config.DrawNames << "\n";
        file << "DrawHP=" << Config.DrawHP << "\n";
        file << "DrawTraceline=" << Config.DrawTraceline << "\n";
        file << "TeamCheck=" << Config.TeamCheck << "\n";
        file << "BoxStyle=" << Config.BoxStyle << "\n";
        file << "maxDistance=" << Config.maxDistance << "\n";
        file << "PlayerThickness=" << Config.PlayerThickness << "\n";
        file << "itemsEnabled=" << Config.itemsEnabled << "\n";
        file << "DrawItemName=" << Config.DrawItemName << "\n";
        file << "DrawItemBox=" << Config.DrawItemBox << "\n";
        file << "MaxItemDistance=" << Config.MaxItemDistance << "\n";
        file << "ItemThickness=" << Config.ItemThickness << "\n";
        file << "EnableAimbot=" << Config.EnableAimbot << "\n";
        file << "IgnoreKnocked=" << Config.IgnoreKnocked << "\n";
        file << "MaxAimDistance=" << Config.MaxAimDistance << "\n";
		file << "TargetSelection=" << Config.TargetSelection << "\n";
		file << "TriggerDelay=" << Config.TriggerDelay << "\n";
		file << "MaxTriggerDistance=" << Config.MaxTriggerDistance << "\n";
        file << "fov=" << Config.fov << "\n";
        file << "VisibleOnly=" << Config.VisibleOnly << "\n";
		file << "AimbotVisibleOnly=" << Config.AimbotVisibleOnly << "\n";
		file << "AimbotKey=" << Config.AimbotKey << "\n";
		file << "EnableTrigger=" << Config.EnableTrigger << "\n";
		file << "MaxTriggerDistance=" << Config.MaxTriggerDistance << "\n";
		file << "TriggerbotKey=" << Config.TriggerbotKey << "\n";
        file << "smoothing=" << Globals::smoothing << "\n";
        file << "overlayMode=" << Config.overlayMode << "\n";
        file << "vsync=" << Config.vsync << "\n";
        file << "OpenMenuKey=" << Config.OpenMenuKey << "\n";
        WriteColor(file, "ColorEnemyBox", Config.ColorEnemyBox);
        WriteColor(file, "ColorEnemyName", Config.ColorEnemyName);
        WriteColor(file, "ColorTeamBox", Config.ColorTeamBox);
        WriteColor(file, "ColorTeamName", Config.ColorTeamName);
        WriteColor(file, "ColorBones", Config.ColorBones);
        WriteColor(file, "ColorLines", Config.ColorLines);
        WriteColor(file, "ColorDistance", Config.ColorDistance);
        WriteColor(file, "ColorHead", Config.ColorHead);
        WriteColor(file, "ColorHP", Config.ColorHP);
        WriteColor(file, "ColorItems", Config.ColorItems);
        WriteColor(file, "ColorWeapons", Config.ColorWeapons);
        WriteColor(file, "ColorFOV", Config.ColorFOV);
        WriteColor(file, "menuAccentColor", Config.menuAccentColor);
        WriteColor(file, "textColor", Config.textColor);
        WriteColor(file, "bgColor", Config.bgColor);
        WriteColor(file, "headerColor", Config.headerColor);

        return true;
    }

    inline ImVec4 ParseColor(const std::string& value) {
        ImVec4 color(1.0f, 1.0f, 1.0f, 1.0f);
        std::stringstream ss(value);
        std::string token;
        float* components[] = {&color.x, &color.y, &color.z, &color.w};

        for (int i = 0; i < 4 && std::getline(ss, token, ','); i++) {
            try {
                *components[i] = std::stof(token);
            } catch (...) {
            }
        }

        return color;
    }

    inline bool LoadConfig(const std::string& configName) {
        std::string path = GetConfigPath(configName);
        std::ifstream file(path);

        if (!file.is_open()) return false;

        std::string line;
        while (std::getline(file, line)) {
            size_t pos = line.find('=');
            if (pos == std::string::npos) continue;

            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);

            try {
                if (key == "ESPEnabled") Config.ESPEnabled = (value == "1");
                else if (key == "DrawBones") Config.DrawBones = (value == "1");
                else if (key == "DrawHeadCircle") Config.DrawHeadCircle = (value == "1");
                else if (key == "DrawNames") Config.DrawNames = (value == "1");
                else if (key == "DrawHP") Config.DrawHP = (value == "1");
                else if (key == "DrawTraceline") Config.DrawTraceline = (value == "1");
                else if (key == "TeamCheck") Config.TeamCheck = (value == "1");
                else if (key == "BoxStyle") Config.BoxStyle = std::stoi(value);
                else if (key == "maxDistance") Config.maxDistance = std::stof(value);
				else if (key == "VisibleOnly") Config.VisibleOnly = (value == "1");
				else if (key == "AimbotVisibleOnly") Config.AimbotVisibleOnly = (value == "1");
                else if (key == "AimbotKey") Config.AimbotKey = std::stoi(value);
				else if (key == "TriggerbotKey") Config.TriggerbotKey = std::stoi(value);
				else if (key == "EnableTrigger") Config.EnableTrigger = (value == "1");
				else if (key == "MaxTriggerDistance") Config.MaxTriggerDistance = std::stof(value);
                else if (key == "PlayerThickness") Config.PlayerThickness = std::stof(value);
                else if (key == "itemsEnabled") Config.itemsEnabled = (value == "1");
                else if (key == "DrawItemName") Config.DrawItemName = (value == "1");
                else if (key == "DrawItemBox") Config.DrawItemBox = (value == "1");
                else if (key == "MaxItemDistance") Config.MaxItemDistance = std::stof(value);
                else if (key == "ItemThickness") Config.ItemThickness = std::stof(value);
                else if (key == "EnableAimbot") Config.EnableAimbot = (value == "1");
                else if (key == "IgnoreKnocked") Config.IgnoreKnocked = (value == "1");
                else if (key == "MaxAimDistance") Config.MaxAimDistance = std::stof(value);
				else if (key == "TargetSelection") Config.TargetSelection = std::stoi(value);
				else if (key == "TriggerDelay") Config.TriggerDelay = std::stoi(value);
                else if (key == "fov") Config.fov = std::stof(value);
                else if (key == "smoothing") Globals::smoothing = std::stof(value);
                else if (key == "overlayMode") Config.overlayMode = std::stoi(value);
                else if (key == "vsync") Config.vsync = (value == "1");
                else if (key == "OpenMenuKey") Config.OpenMenuKey = std::stoi(value);
                else if (key == "ColorEnemyBox") Config.ColorEnemyBox = ParseColor(value);
                else if (key == "ColorEnemyName") Config.ColorEnemyName = ParseColor(value);
                else if (key == "ColorTeamBox") Config.ColorTeamBox = ParseColor(value);
                else if (key == "ColorTeamName") Config.ColorTeamName = ParseColor(value);
                else if (key == "ColorBones") Config.ColorBones = ParseColor(value);
                else if (key == "ColorLines") Config.ColorLines = ParseColor(value);
                else if (key == "ColorDistance") Config.ColorDistance = ParseColor(value);
                else if (key == "ColorHead") Config.ColorHead = ParseColor(value);
                else if (key == "ColorHP") Config.ColorHP = ParseColor(value);
                else if (key == "ColorItems") Config.ColorItems = ParseColor(value);
                else if (key == "ColorWeapons") Config.ColorWeapons = ParseColor(value);
                else if (key == "ColorFOV") Config.ColorFOV = ParseColor(value);
                else if (key == "menuAccentColor") Config.menuAccentColor = ParseColor(value);
                else if (key == "textColor") Config.textColor = ParseColor(value);
                else if (key == "bgColor") Config.bgColor = ParseColor(value);
                else if (key == "headerColor") Config.headerColor = ParseColor(value);
            } catch (...) {
            }
        }

        return true;
    }
}
