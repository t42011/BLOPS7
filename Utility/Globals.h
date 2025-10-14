#pragma once 
#include <Windows.h>
#include <iostream>
#include <vector>
#include <atomic>
#include <string>
#include <cstdint>
#include "../ImGUI/imgui.h"
//sigs  48 8D 0D ? ? ? ? 48 8B D6 33 0D ? ? ? ? 4C 8D 4D refdef

struct OFF {
    std::uintptr_t ref_def_ptr = 0xC9C5540;
    std::uintptr_t name_array = 0xCB34778;
    std::uintptr_t name_array_pos = 0x3038;
    std::uintptr_t name_array_size = 0xC8;
    std::uintptr_t loot_ptr = 0xE4617C8;
	std::uintptr_t loot_pos = 0x180;
    std::uintptr_t camera_base = 0xC655530;
    std::uintptr_t camera_pos = 0x204;
    std::uintptr_t local_index = 0x1CCB28;
    std::uintptr_t local_index_pos = 0x394;
    std::uintptr_t recoil = 0x1B87B0;
    std::uintptr_t game_mode  = 0xB9CC808;
    std::uintptr_t weapon_definitions = 0xC7A4AB0;
    std::uintptr_t distribute = 0x96F6BF8;
    uint32_t o_visible_bit = 0xDC484;
    uint32_t o_no_recoil = 0x1A1654;
    uint32_t Player_client_state_enum = 0x100DA0;
    std::uintptr_t scoreboard = 0x1A1670;
    std::uintptr_t scoreboardsize = 0x78;
    std::uintptr_t bone_base = 0x80800;
    std::uintptr_t bone_size = 0x1B8;
    std::uintptr_t bone_offset = 0xD8;
    std::uintptr_t player_size = 0x23A0;
    std::uintptr_t player_valid = 0xF3C;
    std::uintptr_t player_pos = 0x2330;
    std::uintptr_t player_team = 0x22C2;
    std::uintptr_t player_stance = 0x197C;
    std::uintptr_t weapon_index = 0x15F4;
};

struct Settings {
    //imgui
    // 
    bool NoRecoil = false;
    int FPS = 0;
	int fps_limit = 144;
	bool vsync = false;
    bool BoxESP = true;
    bool HealthBar = true;
    bool NameESP = true;
    bool DistanceESP = true;
    bool Snaplines = false;
    bool TeamESP = true;
    bool running = true;
    bool TeamCheck = true;
    bool VisibleOnly = true;
    int overlayMode = 1;
    int target_monitor = -1;
    int monitor_enum_state = 0;
    int readFPS = 0;
    float maxDistance = 300.0f;
    float MaxItemDistance = 300.0f;
    bool ESPEnabled = true;
    bool DrawHeadCircle = true;
    bool DrawBones = false;
    bool DrawHP = true;
    bool DrawNames = true;
    bool DrawTraceline = false;
    bool itemsEnabled = true;
    bool DrawItemName = true;
    bool DrawItemBox = true;
    bool DistanceEnabled = true;
    bool refreshcheat = false;

    //aimbot 
    bool EnableAimbot = true;
    bool IgnoreKnocked = true;
    bool AimbotVisibleOnly = false;
    bool ignoreteam = true;
    float MaxAimDistance = 300.0f;
    int AimbotMode = 1;
    int AimbotKey = VK_RIGHT;
	//target selection 0=fov, 1=distance.
	int TargetSelection = 1;
    //triggerbot
    int TriggerbotKey = VK_SHIFT;
    int TriggerbotMode = 1;
    bool EnableTrigger = false;
	//triggerbot delay in ms
	int TriggerDelay = 20;
	float MaxTriggerDistance = 300.0f;
    //rage
	bool RageMode = true;

    int BoxStyle = 1;
    float ItemThickness = 1.5f;
	float PlayerThickness = 3.5f;
    ImFont* logoFont = nullptr;
    ImFont* headerFont = nullptr;
    ImFont* regularFont = nullptr;
    HWND overlayHWND = nullptr;
    int OpenMenuKey = VK_INSERT;
    // Enemy colors
    ImVec4 ColorEnemyBox = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // red
    ImVec4 ColorEnemyName = ImVec4(1.0f, 0.3f, 0.3f, 1.0f); // light red

    // Team colors
    ImVec4 ColorTeamBox = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); // green
    ImVec4 ColorTeamName = ImVec4(0.3f, 1.0f, 0.3f, 1.0f); // light green

    // General ESP colors
    ImVec4 ColorBones = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // white
    ImVec4 ColorLines = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // white
    ImVec4 ColorDistance = ImVec4(0.8f, 0.8f, 0.8f, 1.0f); // grey
    ImVec4 ColorHead = ImVec4(0.6f, 0.6f, 1.0f, 1.0f); // bluish
    ImVec4 ColorHP = ImVec4(1.0f, 0.3f, 0.3f, 1.0f); // red
    ImVec4 ColorItems = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // white
    ImVec4 ColorWeapons = ImVec4(1.0f, 0.5f, 0.2f, 1.0f); // orange
    ImVec4 ColorFOV = ImVec4(1.0f, 1.0f, 1.0f, 0.8f); // white (slightly transparent)
    ImVec4 menuAccentColor = ImVec4(0.8f, 0.3f, 0.0f, 1.0f);
    ImVec4 textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    ImVec4 bgColor = ImVec4(0.1f, 0.1f, 0.1f, 0.9f);
    ImVec4 headerColor = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
    float fov = 90.0f;
    bool sidebarOpen = true;
    ImTextureID logoTexture = 0;
    int logoWidth = 1280;
    int logoHeight = 608;
};
inline Settings Config;

enum PlayerStatus : int {
    ALIVE = 0,
    KNOCKED = 2,
    DEAD = 4
};

struct Scoreboard {
    int index;
    int status;
    int points;
    int networkPing;
    int deaths;
    int teamId;
    int isBot;
    int kills;
};

enum AimType {
    AIM_NONE,
    BPRO,
    NET,
    MEMORY
};

enum BONE_INDEX : unsigned long
{
    BONE_POS_HELMET = 8,
    BONE_POS_HEAD = 7,
    BONE_POS_NECK = 6,
    BONE_POS_CHEST = 5,
    BONE_POS_MID = 4,
    BONE_POS_TUMMY = 3,
    BONE_POS_PELVIS = 2,

    BONE_POS_RIGHT_FOOT_1 = 21,
    BONE_POS_RIGHT_FOOT_2 = 22,
    BONE_POS_RIGHT_FOOT_3 = 23,
    BONE_POS_RIGHT_FOOT_4 = 24,

    BONE_POS_LEFT_FOOT_1 = 17,
    BONE_POS_LEFT_FOOT_2 = 18,
    BONE_POS_LEFT_FOOT_3 = 19,
    BONE_POS_LEFT_FOOT_4 = 20,

    BONE_POS_LEFT_HAND_1 = 13,
    BONE_POS_LEFT_HAND_2 = 14,
    BONE_POS_LEFT_HAND_3 = 15,
    BONE_POS_LEFT_HAND_4 = 16,

    BONE_POS_RIGHT_HAND_1 = 9,
    BONE_POS_RIGHT_HAND_2 = 10,
    BONE_POS_RIGHT_HAND_3 = 11,
    BONE_POS_RIGHT_HAND_4 = 12
};


class Globals {
public:
    static inline AimType aimType = AIM_NONE;
    static inline int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    static inline int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    static inline float smoothing = 4.0f;
    static inline std::string GameTitle = "cod.exe";
    static inline bool isFullScreen = false;
    static inline OFF Offset{};
};


