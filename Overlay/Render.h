#pragma once

#include <Windows.h>
#include "imgui_internal.h"
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#include "Utility/Globals.h"
#include <dwmapi.h>
#include <functional>
#include <iostream>
#include <thread>
#include <mutex>
#include "../cache/cache.h"

static bool g_dwmCompositionDisabled = false;
static bool g_lastMenuState = false;
static int g_lastOverlayMode = -1;

class Render {
public:
	void Loop();
	void Init();
};

void SetOverlayMode(HWND hwnd, int mode, bool menuVisible);


   
 
