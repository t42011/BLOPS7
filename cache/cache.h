#pragma once
#include <vector>
#include <atomic>
#include <cstdint>
#include <array>
#include "../Utility/Vectors.h"
#include "../Utility/Globals.h"
#include "../DMALibrary/Memory/Memory.h"
#include "decrypt.h"

using BYTE = uint8_t;
extern Memory mem;

class _GameCache;
extern _GameCache cache;

struct _PlayerCache {
	uintptr_t PlayerAddress = 0, PosPTR = 0, camPtr = 0, bonePtr = 0;
	uint16_t decryptedBoneIndex = 0; 
	FVector pos{}, cam{}, TempPos{};

	FVector helmet{}, head{}, neck{}, chest{}, mid{}, tummy{}, pelvis{};
	FVector leftShoulder{}, leftElbow{}, leftHand{};
	FVector rightShoulder{}, rightElbow{}, rightHand{};
	FVector leftThigh{}, leftKnee{}, leftAnkle{}, leftFoot{};
	FVector rightThigh{}, rightKnee{}, rightAnkle{}, rightFoot{};

	uint32_t stance = 0;
	BYTE teamId = 0;
	bool valid = false, visible = false;
	int index = -1;
	float distance = 0.0f;
	PlayerStatus status = ALIVE;

	FVector2D  RootW2S{}, headW2S{}, SHeadW2S{};
	FVector2D helmetW2S{}, neckW2S{}, chestW2S{}, midW2S{}, tummyW2S{}, pelvisW2S{};
	FVector2D leftShoulderW2S{}, leftElbowW2S{}, leftHandW2S{};
	FVector2D rightShoulderW2S{}, rightElbowW2S{}, rightHandW2S{};
	FVector2D leftThighW2S{}, leftKneeW2S{}, leftAnkleW2S{}, leftFootW2S{};
	FVector2D rightThighW2S{}, rightKneeW2S{}, rightAnkleW2S{}, rightFootW2S{};

	NameEntry NameEnt{};
	void ProcessBones(_PlayerCache& localplayer);
	void get_index(uint64_t client_info);
	void GetCamera(VMMDLL_SCATTER_HANDLE handle);
	void UpdateAttributes(VMMDLL_SCATTER_HANDLE handle);
	void UpdateBonePtr(VMMDLL_SCATTER_HANDLE handle);
	void updatePlayer(VMMDLL_SCATTER_HANDLE handle);
	void CameraUpdate(VMMDLL_SCATTER_HANDLE handle);
	void UpdateBones(VMMDLL_SCATTER_HANDLE handle);
	auto Retrieve_name_list() -> uint64_t;
	auto GetName(uint32_t p_index) -> NameEntry;


	_PlayerCache SetupLocalPlayer();

	void QueueNameRead(VMMDLL_SCATTER_HANDLE handle, uint64_t nameListBase) {
		mem.AddScatterReadRequest(handle, nameListBase + (index * Globals::Offset.name_array_size),
			&NameEnt, sizeof(NameEntry));

	}
};



class _GameCache {
public:
	std::vector<_PlayerCache> Players;
	std::array<_PlayerCache, 150> RenderPlayers;
	int RenderPlayerCount = 0;
	_PlayerCache LocalPlayer;
	uintptr_t ClientInfo{ 0 }, ClientBase{ 0 }, BoneBase{ 0 }, RefDefptr{ 0 }, NameArrayStart{ 0 };
	ref_def_t refDef{};
	clientbits visibilityBits{};
	FVector bone_origin{};
	std::mutex cacheMutex;

	void SwapBuffers(_PlayerCache& localplayer) {
		std::lock_guard<std::mutex> lock(cacheMutex);

		RenderPlayerCount = 0;
		for (auto& player : Players) {
			if (RenderPlayerCount >= 150) break;
			if (player.valid) {
				RenderPlayers[RenderPlayerCount++] = player;
			}
		}
		LocalPlayer = localplayer;
	}

	void UpdateScoreboardData(VMMDLL_SCATTER_HANDLE handle);
	bool IsVisible(uint32_t index);
};

inline _GameCache cache;



 

inline bool IsInGame() {
	return mem.Read<int>(mem.baseAddress + Globals::Offset.game_mode) > 4;
}

inline int PlayerCount() {
	return mem.Read<int>(mem.baseAddress + Globals::Offset.game_mode);
}

inline void _PlayerCache::get_index(uint64_t client_info)
{
		uintptr_t idx_ptr = mem.Read<uintptr_t>(cache.ClientInfo + Globals::Offset.local_index);
		index = mem.Read<int>(idx_ptr + Globals::Offset.local_index_pos);
		 
}

inline void _PlayerCache::GetCamera(VMMDLL_SCATTER_HANDLE handle) {
	mem.AddScatterReadRequest(handle, mem.baseAddress + Globals::Offset.camera_base, &camPtr, sizeof(camPtr));
}

inline void _PlayerCache::UpdateAttributes(VMMDLL_SCATTER_HANDLE handle) {
	mem.AddScatterReadRequest(handle, PlayerAddress + Globals::Offset.player_valid, &valid, sizeof(valid));
	mem.AddScatterReadRequest(handle, PlayerAddress + Globals::Offset.player_team, &teamId, sizeof(teamId));
	mem.AddScatterReadRequest(handle, PlayerAddress + Globals::Offset.player_pos, &PosPTR, sizeof(PosPTR));
	mem.AddScatterReadRequest(handle, PlayerAddress + Globals::Offset.player_stance, &stance, sizeof(stance));
}

inline void _PlayerCache::updatePlayer(VMMDLL_SCATTER_HANDLE handle) {
	if (!valid || PosPTR == 0) {
		return;
	}
	mem.AddScatterReadRequest(handle, PosPTR + 0x70, &pos, sizeof(pos));

}

inline void _PlayerCache::CameraUpdate(VMMDLL_SCATTER_HANDLE handle) {
	mem.AddScatterReadRequest(handle, camPtr + Globals::Offset.camera_pos, &cam, sizeof(cam));
	mem.AddScatterReadRequest(handle, cache.RefDefptr, &cache.refDef, sizeof(ref_def_t));
}

inline void _PlayerCache::UpdateBonePtr(VMMDLL_SCATTER_HANDLE handle) {
	if (index < 0) return;
	int IndexValid = decryptedBoneIndex > 0 ? decryptedBoneIndex : get_bone_index(index);
		uintptr_t addr = cache.BoneBase + (decryptedBoneIndex * Globals::Offset.bone_size) + Globals::Offset.bone_offset;
		if (!IsValidVA(addr)) return;
		mem.AddScatterReadRequest(handle, addr, &bonePtr, sizeof(bonePtr));
		
}

inline void _PlayerCache::UpdateBones(VMMDLL_SCATTER_HANDLE handle) {
	if (!Config.DrawBones || !bonePtr || !valid) {
		helmetW2S = headW2S = neckW2S = chestW2S = midW2S = tummyW2S = pelvisW2S =
			leftShoulderW2S = leftElbowW2S = leftHandW2S =
			rightShoulderW2S = rightElbowW2S = rightHandW2S =
			leftThighW2S = leftKneeW2S = leftAnkleW2S = leftFootW2S =
			rightThighW2S = rightKneeW2S = rightAnkleW2S = rightFootW2S = FVector2D{ 0, 0 };
		return;
	}
	struct BoneData { FVector* target; int boneID; };
	BoneData bones[] = {
		{&head, BONE_POS_HEAD}, {&neck, BONE_POS_NECK}, {&chest, BONE_POS_CHEST},
		{&leftShoulder, BONE_POS_LEFT_HAND_1}, {&leftElbow, BONE_POS_LEFT_HAND_2}, {&leftHand, BONE_POS_LEFT_HAND_4},
		{&rightShoulder, BONE_POS_RIGHT_HAND_1}, {&rightElbow, BONE_POS_RIGHT_HAND_2}, {&rightHand, BONE_POS_RIGHT_HAND_4},
		{&pelvis, BONE_POS_PELVIS},
		{&leftThigh, BONE_POS_LEFT_FOOT_1}, {&leftKnee, BONE_POS_LEFT_FOOT_2}, {&leftAnkle, BONE_POS_LEFT_FOOT_3}, {&leftFoot, BONE_POS_LEFT_FOOT_4},
		{&rightThigh, BONE_POS_RIGHT_FOOT_1}, {&rightKnee, BONE_POS_RIGHT_FOOT_2}, {&rightAnkle, BONE_POS_RIGHT_FOOT_3}, {&rightFoot, BONE_POS_RIGHT_FOOT_4}
	};
	for (auto& bone : bones) {
		mem.AddScatterReadRequest(handle, bonePtr + (bone.boneID * 0x20) + 0x10, bone.target, sizeof(FVector));
	}
}
inline void _GameCache::UpdateScoreboardData(VMMDLL_SCATTER_HANDLE handle) {
	if (ClientInfo == 0) return;

	// Read bone_origin ONCE outside the loop
	cache.bone_origin = mem.Read<FVector>(cache.ClientInfo + Globals::Offset.bone_base);

	for (auto& player : Players) {
		if (player.index < 0) continue;
		uintptr_t statusAddress = ClientInfo + Globals::Offset.scoreboard + (player.index * Globals::Offset.scoreboardsize) + 0x4;
		mem.AddScatterReadRequest(handle, statusAddress, &player.status, sizeof(int));
	}
}

inline auto _PlayerCache::Retrieve_name_list() -> uint64_t {
	auto ptr = mem.Read<uint64_t>(mem.baseAddress + Globals::Offset.name_array);
	if (!ptr) return 0;
	return ptr + Globals::Offset.name_array_pos;
}

inline auto _PlayerCache::GetName(uint32_t p_index) -> NameEntry {
	auto name = mem.Read<NameEntry>(_PlayerCache::Retrieve_name_list() + (p_index * Globals::Offset.name_array_size));
	return name;
}

namespace Game {
	void Loop(_PlayerCache& localplayer);
	void PlayerLoop(bool debug, _PlayerCache& localplayer);
}

inline bool world_to_screen(FVector& world_location, FVector2D& out, FVector& camera_pos,
	int screen_width, int screen_height, FVector2D& fov, FVector matricies[3]) {
	constexpr float MIN_Z_DISTANCE = 0.01f;
	constexpr float SCREEN_BORDER_MARGIN = -100.0f;

	auto local = world_location - camera_pos;

	auto trans = FVector{
		local.Dot(matricies[1]),
		local.Dot(matricies[2]),
		local.Dot(matricies[0])
	};

	if (trans.Z < MIN_Z_DISTANCE) return false;

	out.X = (screen_width * 0.5f) * (1.0f - (trans.X / fov.X / trans.Z));
	out.Y = (screen_height * 0.5f) * (1.0f - (trans.Y / fov.Y / trans.Z));

	return out.X >= SCREEN_BORDER_MARGIN &&
		out.Y >= SCREEN_BORDER_MARGIN &&
		out.X <= (cache.refDef.width - SCREEN_BORDER_MARGIN) &&
		out.Y <= (cache.refDef.height - SCREEN_BORDER_MARGIN);
}

inline bool isVisible(clientbits visible_bits, uint32_t index)
{
	auto bitmask = 0x80000000 >> (index & 0x1F);
	return cache.visibilityBits.data[index >> 5] & bitmask;
}

inline auto W2S(FVector world_position, FVector2D& screen_position, FVector cam_position) -> bool {
	return world_to_screen(world_position, screen_position, cam_position,
		GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
		cache.refDef.tan_half_fov, cache.refDef.axis);
}

struct WeaponEntry {
	char str[64];
};

 
inline void _PlayerCache::ProcessBones(_PlayerCache& localplayer) {
	if (!Config.DrawBones || !bonePtr || !valid) return;

	FVector basePos = pos;
	head = head + localplayer.cam;
	helmet = helmet + localplayer.cam;
	neck = neck + localplayer.cam;
	chest = chest + localplayer.cam;
	leftShoulder = leftShoulder + localplayer.cam;
	leftElbow = leftElbow + localplayer.cam;
	leftHand = leftHand + localplayer.cam;
	rightShoulder = rightShoulder + localplayer.cam;
	rightElbow = rightElbow + localplayer.cam;
	rightHand = rightHand + localplayer.cam;
	pelvis = pelvis + localplayer.cam;
	leftThigh = leftThigh + localplayer.cam;
	leftKnee = leftKnee + localplayer.cam;
	leftAnkle = leftAnkle + localplayer.cam;
	leftFoot = leftFoot + localplayer.cam;
	rightThigh = rightThigh + localplayer.cam;
	rightKnee = rightKnee + localplayer.cam;
	rightAnkle = rightAnkle + localplayer.cam;
	rightFoot = rightFoot + localplayer.cam;

	if (!isBoneValid(basePos, head)) headW2S = { 0, 0 };
	if (!isBoneValid(basePos, helmet)) helmetW2S = { 0, 0 };
	if (!isBoneValid(basePos, neck)) neckW2S = { 0, 0 };
	if (!isBoneValid(basePos, chest)) chestW2S = { 0, 0 };
	if (!isBoneValid(basePos, pelvis)) pelvisW2S = { 0, 0 };
	if (!isBoneValid(basePos, leftShoulder)) leftShoulderW2S = { 0, 0 };
	if (!isBoneValid(basePos, leftElbow)) leftElbowW2S = { 0, 0 };
	if (!isBoneValid(basePos, leftHand)) leftHandW2S = { 0, 0 };
	if (!isBoneValid(basePos, rightShoulder)) rightShoulderW2S = { 0, 0 };
	if (!isBoneValid(basePos, rightElbow)) rightElbowW2S = { 0, 0 };
	if (!isBoneValid(basePos, rightHand)) rightHandW2S = { 0, 0 };
	if (!isBoneValid(basePos, leftThigh)) leftThighW2S = { 0, 0 };
	if (!isBoneValid(basePos, leftKnee)) leftKneeW2S = { 0, 0 };
	if (!isBoneValid(basePos, leftAnkle)) leftAnkleW2S = { 0, 0 };
	if (!isBoneValid(basePos, leftFoot)) leftFootW2S = { 0, 0 };
	if (!isBoneValid(basePos, rightThigh)) rightThighW2S = { 0, 0 };
	if (!isBoneValid(basePos, rightKnee)) rightKneeW2S = { 0, 0 };
	if (!isBoneValid(basePos, rightAnkle)) rightAnkleW2S = { 0, 0 };
	if (!isBoneValid(basePos, rightFoot)) rightFootW2S = { 0, 0 };
}