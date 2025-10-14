#pragma once
#include "../cache/cache.h"
class Aimbot {
public:
    static void aimbot(FVector2D target);
    static bool init();
    static void Triggerbot(FVector2D target);
    static void Loop(_GameCache& cache, _PlayerCache& localplayer);
};

struct AimbotTarget {
    FVector2D pos;
    int index;
    bool visible;
    float distance;
};

static bool HandleHotkeyMode(int key, int mode, bool& toggleState)
{
    static std::unordered_map<int, bool> wasPressed;
    bool isPressed = (GetAsyncKeyState(key) & 0x8000) || mem.GetKeyboard()->IsKeyDown(key);
    bool& w = wasPressed[key];

    switch (mode)
    {
    case 0: // toggle on rising edge
        if (isPressed && !w) toggleState = !toggleState;
        w = isPressed;
        return toggleState;

    case 1: // hold
        w = isPressed;
        return isPressed;

    default:
        return false;
    }
}


void aimAt(FVector2D target);
void Shoot();