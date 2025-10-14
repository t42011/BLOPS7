#include "../pch.h"
#include "InputManager.h"
#include "Registry.h"
#include "../Memory/Memory.h"

//TODO: Restart winlogon.exe when it doesn't exist.
bool c_keys::InitKeyboard()
{
	std::string win = registry.QueryValue("HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\CurrentBuild", e_registry_type::sz);
	int Winver = 0;
	if (!win.empty())
		Winver = std::stoi(win);
	else
		return false;
	std::string ubr = registry.QueryValue("HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\UBR", e_registry_type::dword);
	int Ubr = 0;
	if (!ubr.empty())
		Ubr = std::stoi(ubr);
	else
		return false;
	this->win_logon_pid = mem.GetPidFromName("winlogon.exe");
	if (Winver > 22000)
	{
		auto pids = mem.GetPidListFromName("csrss.exe");
		for (size_t i = 0; i < pids.size(); i++)
		{
			auto pid = pids[i];

			PVMMDLL_MAP_MODULEENTRY win32k_module_info;
			if (!VMMDLL_Map_GetModuleFromNameW(mem.vHandle, pid, const_cast<LPWSTR>(L"win32ksgd.sys"), &win32k_module_info, VMMDLL_MODULE_FLAG_NORMAL))
			{
				if (!VMMDLL_Map_GetModuleFromNameW(mem.vHandle, pid, const_cast<LPWSTR>(L"win32k.sys"), &win32k_module_info, VMMDLL_MODULE_FLAG_NORMAL))
				{
					LOG("failed to get module win32k info\n");
					return false;
				}
			}
			uintptr_t win32k_base = win32k_module_info->vaBase;
			size_t win32k_size = win32k_module_info->cbImageSize;
			//win32ksgd
			auto g_session_ptr = mem.FindSignature("48 8B 05 ? ? ? ? 48 8B 04 C8", win32k_base, win32k_base + win32k_size, pid);
			if (!g_session_ptr)
			{
				//win32k
				g_session_ptr = mem.FindSignature("48 8B 05 ? ? ? ? FF C9", win32k_base, win32k_base + win32k_size, pid);
				if (!g_session_ptr)
				{
					LOG("failed to find g_session_global_slots\n");
					return false;
				}
			}
			int relative = mem.Read<int>(g_session_ptr + 3, pid);
			uintptr_t g_session_global_slots = g_session_ptr + 7 + relative;
			uintptr_t user_session_state = 0;
			for (int i = 0; i < 4; i++)
			{
				user_session_state = mem.Read<uintptr_t>(mem.Read<uintptr_t>(mem.Read<uintptr_t>(g_session_global_slots, pid) + 8 * i, pid), pid);
				if (user_session_state > 0x7FFFFFFFFFFF)
					break;
			}

			PVMMDLL_MAP_MODULEENTRY win32kbase_module_info;
			if (!VMMDLL_Map_GetModuleFromNameW(mem.vHandle, pid, const_cast<LPWSTR>(L"win32kbase.sys"), &win32kbase_module_info, VMMDLL_MODULE_FLAG_NORMAL))
			{
				LOG("failed to get module win32kbase info\n");
				return false;
			}
			uintptr_t win32kbase_base = win32kbase_module_info->vaBase;
			size_t win32kbase_size = win32kbase_module_info->cbImageSize;

			//Unsure if this sig will work on all versions. (sig is from PostUpdateKeyStateEvent function. seems to exist in both older version and the new version of win32kbase that I have checked)
			uintptr_t ptr = mem.FindSignature("48 8D 90 ? ? ? ? E8 ? ? ? ? 0F 57 C0", win32kbase_base, win32kbase_base + win32kbase_size, pid);
			uint32_t session_offset = 0x0;
			if (ptr)
			{
				session_offset = mem.Read<uint32_t>(ptr + 3, pid);
				gafAsyncKeyStateExport = user_session_state + session_offset;

			}
			else
			{
				LOG("failed to find offset for gafAyncKeyStateExport\n");
				return false;
			}

			if (gafAsyncKeyStateExport > 0x7FFFFFFFFFFF) break;
		}
		if (gafAsyncKeyStateExport > 0x7FFFFFFFFFFF) {
			keyboard_valid = true;
			consecutive_failures = 0;
			return true;
		}
		return false;
	}
	else
	{
		PVMMDLL_MAP_EAT eat_map = NULL;
		PVMMDLL_MAP_EATENTRY eat_map_entry;
		bool result = VMMDLL_Map_GetEATU(mem.vHandle, mem.GetPidFromName("winlogon.exe") | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY, const_cast<LPSTR>("win32kbase.sys"), &eat_map);
		if (!result)
			return false;

		if (eat_map->dwVersion != VMMDLL_MAP_EAT_VERSION)
		{
			VMMDLL_MemFree(eat_map);
			eat_map_entry = NULL;
			return false;
		}

		for (int i = 0; i < eat_map->cMap; i++)
		{
			eat_map_entry = eat_map->pMap + i;
			if (strcmp(eat_map_entry->uszFunction, "gafAsyncKeyState") == 0)
			{
				gafAsyncKeyStateExport = eat_map_entry->vaFunction;

				break;
			}
		}

		VMMDLL_MemFree(eat_map);
		eat_map = NULL;
		if (gafAsyncKeyStateExport < 0x7FFFFFFFFFFF)
		{
			PVMMDLL_MAP_MODULEENTRY module_info;
			auto result = VMMDLL_Map_GetModuleFromNameW(mem.vHandle, mem.GetPidFromName("winlogon.exe") | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY, static_cast<LPCWSTR>(L"win32kbase.sys"), &module_info, VMMDLL_MODULE_FLAG_NORMAL);
			if (!result)
			{
				LOG("failed to get module info\n");
				return false;
			}

			char str[32];
			if (!VMMDLL_PdbLoad(mem.vHandle, mem.GetPidFromName("winlogon.exe") | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY, module_info->vaBase, str))
			{
				LOG("failed to load pdb\n");
				return false;
			}

			uintptr_t gafAsyncKeyState;
			if (!VMMDLL_PdbSymbolAddress(mem.vHandle, str, const_cast<LPSTR>("gafAsyncKeyState"), &gafAsyncKeyState))
			{
				LOG("failed to find gafAsyncKeyState\n");
				return false;
			}
			LOG("found gafAsyncKeyState at: 0x%p\n", gafAsyncKeyState);
		}
		if (gafAsyncKeyStateExport > 0x7FFFFFFFFFFF) {
			keyboard_valid = true;
			consecutive_failures = 0;
			return true;
		}
		return false;
	}
}

void c_keys::UpdateKeys()
{
	uint8_t previous_key_state_bitmap[64] = { 0 };
	memcpy(previous_key_state_bitmap, state_bitmap, 64);

	uint8_t temp_bitmap[64] = { 0 };
	BOOL read_result = VMMDLL_MemReadEx(mem.vHandle, this->win_logon_pid | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY, gafAsyncKeyStateExport, reinterpret_cast<PBYTE>(&temp_bitmap), 64, NULL, VMMDLL_FLAG_NOCACHE);

	if (!read_result) {
		consecutive_failures++;
		LOG("Keyboard read failed (attempt %d)\n", consecutive_failures);

		// After 5 consecutive failures, try to reinitialize
		if (consecutive_failures == 5) {
			LOG("Too many keyboard failures, attempting reinit...\n");

			// Check if winlogon PID changed
			int new_pid = mem.GetPidFromName("winlogon.exe");
			if (new_pid != 0 && new_pid != this->win_logon_pid) {
				LOG("winlogon.exe PID changed from %d to %d, reinitializing...\n", this->win_logon_pid, new_pid);
				this->win_logon_pid = new_pid;
				consecutive_failures = 0;
				keyboard_valid = false;
				if (InitKeyboard()) {
					LOG("Keyboard reinitialized successfully\n");
					keyboard_valid = true;
				}
				else {
					LOG("Keyboard reinit failed!\n");
				}
			}
			else {
				LOG("winlogon.exe PID unchanged, keyboard may be temporarily blocked\n");
			}
		}
		else if (consecutive_failures > 50) {
			// Cap the counter so it doesn't overflow
			consecutive_failures = 50;
			LOG("Keyboard still failing after many attempts, continuing with last known state\n");
		}
		// Keep using old state_bitmap, don't corrupt it
		return;
	}

	// Read successful, reset failure counter and update state
	consecutive_failures = 0;
	keyboard_valid = true;
	memcpy(state_bitmap, temp_bitmap, 64);

	for (int vk = 0; vk < 256; ++vk)
		if ((state_bitmap[(vk * 2 / 8)] & 1 << vk % 4 * 2) && !(previous_key_state_bitmap[(vk * 2 / 8)] & 1 << vk % 4 * 2))
			previous_state_bitmap[vk / 8] |= 1 << vk % 8;
}

bool c_keys::IsKeyDown(uint32_t virtual_key_code)
{
	if (gafAsyncKeyStateExport < 0x7FFFFFFFFFFF)
		return false;
	if (std::chrono::system_clock::now() - start > std::chrono::milliseconds(100))
	{
		UpdateKeys();
		start = std::chrono::system_clock::now();
	}
	return state_bitmap[(virtual_key_code * 2 / 8)] & 1 << virtual_key_code % 4 * 2;
}