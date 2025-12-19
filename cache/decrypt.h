#pragma once
#include <iostream>
#include <stdlib.h>
#include <Windows.h>
#include <codecvt>
#include <vector>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <mutex>
#include <atomic>
#include <thread>
#include "../Utility/Vectors.h"      
#include "../Utility/Globals.h"  
#include "../DMALibrary/Memory/Memory.h"

struct ref_def_t;
class vec3;

class refdef_class {

// Synchronized offset constants (kept in sync with Utility/Globals.h)
inline constexpr std::uintptr_t ref_def_ptr = 0xDE2A1E8;
inline constexpr std::uintptr_t name_array = 0xDF9B078;
inline constexpr std::uintptr_t name_array_pos = 0x3038;
inline constexpr std::uintptr_t name_array_size = 0xC8;
inline constexpr std::uintptr_t loot_ptr = 0xF4AC870;
inline constexpr std::uintptr_t loot_pos = 0x878;
inline constexpr std::uintptr_t camera_base = 0xDA6C100;
inline constexpr std::uintptr_t camera_pos = 0x1E4;
inline constexpr std::uintptr_t local_index = 0x177BB0;
inline constexpr std::uintptr_t local_index_pos = 0x3AC;
inline constexpr std::uintptr_t o_no_recoil = 0x149B54;
inline constexpr std::uintptr_t game_mode = 0xCDDBE08;
inline constexpr std::uintptr_t weapon_definitions = 0xDCC10F0;
inline constexpr std::uintptr_t distribute = 0xA78B640;
inline constexpr std::uint32_t o_visible_bit = 0x776F4;
inline constexpr std::uintptr_t Player_client_state_enum = 0x15077C;
inline constexpr std::uintptr_t scoreboard = 0x149BB8;
inline constexpr std::uintptr_t bone_base = 0x13DB10;
inline constexpr std::uintptr_t bone_size = 0x1C8;
inline constexpr std::uintptr_t bone_offset = 0xE0;
inline constexpr std::uintptr_t player_size = 0x24E8;
inline constexpr std::uintptr_t player_valid = 0x1694;
inline constexpr std::uintptr_t player_pos = 0x19E0;
inline constexpr std::uintptr_t player_team = 0x1680;
inline constexpr std::uintptr_t player_stance = 0x81D;
inline constexpr std::uintptr_t weapon_index = 0x2400;

public:
    auto retrieve_ref_def() -> uintptr_t
    {
        key encrypted = mem.Read<key>(mem.baseAddress + Globals::Offset.ref_def_ptr);
        DWORD lowerref = encrypted.ref0 ^ (encrypted.ref2 ^ (uint64_t)(mem.baseAddress + Globals::Offset.ref_def_ptr)) * ((encrypted.ref2 ^ (uint64_t)(mem.baseAddress + Globals::Offset.ref_def_ptr)) + 2);
        DWORD upperref = encrypted.ref1 ^ (encrypted.ref2 ^ (uint64_t)(mem.baseAddress + Globals::Offset.ref_def_ptr + 0x4)) * ((encrypted.ref2 ^ (uint64_t)(mem.baseAddress + Globals::Offset.ref_def_ptr + 0x4)) + 2); \
            return (uint64_t)upperref << 32 | lowerref;
    }
    ref_def_t ref_def_nn;
};
static refdef_class* decrypt_refdef = new refdef_class();

inline uintptr_t decrypt_client_info()
{
    const uint64_t mb = mem.baseAddress;
    uint64_t rax = mb, rbx = mb, rcx = mb, rdx = mb, rdi = mb, rsi = mb, r8 = mb, r9 = mb, r10 = mb, r11 = mb, r12 = mb, r13 = mb, r14 = mb, r15 = mb;

    r8 = mem.Read<uintptr_t>(mem.baseAddress + 0xC9B94C8);
    if (!r8)
        return r8;
    rdx = ~mem.GetPEBAddress();           //mov rdx, gs:[rax]
    rax = r8;               //mov rax, r8
    rax >>= 0x18;           //shr rax, 0x18
    r8 ^= rax;              //xor r8, rax
    rax = r8;               //mov rax, r8
    rax >>= 0x30;           //shr rax, 0x30
    rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
    r8 ^= rax;              //xor r8, rax
    rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
    rcx ^= mem.Read<uintptr_t>(mem.baseAddress + 0xB3B30F9);          //xor rcx, [0x0000000005107D5E]
    rax = 0x233F5F4AE79533B1;               //mov rax, 0x233F5F4AE79533B1
    r8 *= rax;              //imul r8, rax
    rax = 0x4FF2ED27F19D575D;               //mov rax, 0x4FF2ED27F19D575D
    rcx = ~rcx;             //not rcx
    r8 -= rdx;              //sub r8, rdx
    r8 += rax;              //add r8, rax
    rax = mem.baseAddress;            //lea rax, [0xFFFFFFFFF9D54C3D]
    r8 ^= rax;              //xor r8, rax
    r8 *= mem.Read<uintptr_t>(rcx + 0x19);          //imul r8, [rcx+0x19]
    return r8;
}
inline uintptr_t decrypt_client_base(uintptr_t clientInfo)
{
    const uint64_t mb = mem.baseAddress;
    uint64_t rax = mb, rbx = mb, rcx = mb, rdx = mb, rdi = mb, rsi = mb, r8 = mb, r9 = mb, r10 = mb, r11 = mb, r12 = mb, r13 = mb, r14 = mb, r15 = mb;
    r8 = mem.Read<uintptr_t>(clientInfo + 0x1df748);
    if (!r8)
        return r8;
    rbx = mem.GetPEBAddress();           //mov rbx, gs:[rax]
    rax = rbx;              //mov rax, rbx
    rax <<= 0x23;           //shl rax, 0x23
    rax = _byteswap_uint64(rax);            //bswap rax
    rax &= 0xF;
    switch (rax) {
    case 0:
    {
        r10 = mem.Read<uintptr_t>(mem.baseAddress + 0xB3B312A);           //mov r10, [0x00000000086864EA]
        rax = mem.baseAddress;            //lea rax, [0xFFFFFFFFFD2D3208]
        r8 -= rax;              //sub r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x1E;           //shr rax, 0x1E
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x3C;           //shr rax, 0x3C
        r8 ^= rax;              //xor r8, rax
        rax = mem.baseAddress;            //lea rax, [0xFFFFFFFFFD2D2FE9]
        r8 -= rax;              //sub r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x28;           //shr rax, 0x28
        r8 ^= rax;              //xor r8, rax
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r10;             //xor rax, r10
        rax = ~rax;             //not rax
        r8 *= mem.Read<uintptr_t>(rax + 0x9);           //imul r8, [rax+0x09]
        rax = 0x9CC8E0420ADA280D;               //mov rax, 0x9CC8E0420ADA280D
        r8 *= rax;              //imul r8, rax
        r8 += rbx;              //add r8, rbx
        rax = r8;               //mov rax, r8
        rax >>= 0x11;           //shr rax, 0x11
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x22;           //shr rax, 0x22
        r8 ^= rax;              //xor r8, rax
        return r8;
    }
    case 1:
    {
        r10 = mem.Read<uintptr_t>(mem.baseAddress + 0xB3B312A);           //mov r10, [0x0000000008686052]
        rax = r8;               //mov rax, r8
        rax >>= 0x9;            //shr rax, 0x09
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x12;           //shr rax, 0x12
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x24;           //shr rax, 0x24
        r8 ^= rax;              //xor r8, rax
        r8 ^= rbx;              //xor r8, rbx
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r10;             //xor rcx, r10
        rcx = ~rcx;             //not rcx
        r8 *= mem.Read<uintptr_t>(rcx + 0x9);           //imul r8, [rcx+0x09]
        rcx = mem.baseAddress;            //lea rcx, [0xFFFFFFFFFD2D2B6D]
        rax = rbx;              //mov rax, rbx
        rax -= rcx;             //sub rax, rcx
        rax += 0xFFFFFFFF9F0CFAED;              //add rax, 0xFFFFFFFF9F0CFAED
        r8 += rax;              //add r8, rax
        rax = 0x40ED86BABDEA8F5B;               //mov rax, 0x40ED86BABDEA8F5B
        r8 *= rax;              //imul r8, rax
        rax = 0xA7798517B7F399EA;               //mov rax, 0xA7798517B7F399EA
        r8 ^= rax;              //xor r8, rax
        rcx = mem.baseAddress + 0x755F7BDD;               //lea rcx, [0x00000000728CAAC0]
        rax = rcx;              //mov rax, rcx
        rax = ~rax;             //not rax
        rax ^= rbx;             //xor rax, rbx
        r8 += rax;              //add r8, rax
        rax = 0x459093E765583ADB;               //mov rax, 0x459093E765583ADB
        r8 *= rax;              //imul r8, rax
        return r8;
    }
    case 2:
    {
        r10 = mem.Read<uintptr_t>(mem.baseAddress + 0xB3B312A);           //mov r10, [0x0000000008685BFB]
        r15 = mem.baseAddress + 0xAC81;           //lea r15, [0xFFFFFFFFFD2DD73F]
        rax = 0xE03443781C6DB26D;               //mov rax, 0xE03443781C6DB26D
        r8 *= rax;              //imul r8, rax
        rax = 0x26676A6627BAC50C;               //mov rax, 0x26676A6627BAC50C
        r8 -= rax;              //sub r8, rax
        rax = 0x541ECC7788F37ADE;               //mov rax, 0x541ECC7788F37ADE
        r8 += rax;              //add r8, rax
        r8 += r15;              //add r8, r15
        rax = mem.baseAddress + 0x142;            //lea rax, [0xFFFFFFFFFD2D27BF]
        rax = ~rax;             //not rax
        rcx = rbx;              //mov rcx, rbx
        rcx = ~rcx;             //not rcx
        rcx -= rbx;             //sub rcx, rbx
        rcx += rax;             //add rcx, rax
        r8 += rcx;              //add r8, rcx
        rax = r8;               //mov rax, r8
        rax >>= 0x15;           //shr rax, 0x15
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x2A;           //shr rax, 0x2A
        r8 ^= rax;              //xor r8, rax
        rax = mem.baseAddress;            //lea rax, [0xFFFFFFFFFD2D261E]
        r8 -= rax;              //sub r8, rax
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r10;             //xor rax, r10
        rax = ~rax;             //not rax
        r8 *= mem.Read<uintptr_t>(rax + 0x9);           //imul r8, [rax+0x09]
        return r8;
    }
    case 3:
    {
        r10 = mem.Read<uintptr_t>(mem.baseAddress + 0xB3B312A);           //mov r10, [0x000000000868571F]
        r8 ^= rbx;              //xor r8, rbx
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r10;             //xor rcx, r10
        rcx = ~rcx;             //not rcx
        r8 *= mem.Read<uintptr_t>(rcx + 0x9);           //imul r8, [rcx+0x09]
        rax = 0x6C5618A3BE4C414;                //mov rax, 0x6C5618A3BE4C414
        r8 -= rax;              //sub r8, rax
        rax = 0xE98709096AD185CC;               //mov rax, 0xE98709096AD185CC
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rcx = mem.baseAddress + 0x5ED318FB;               //lea rcx, [0x000000005C003DDB]
        rax >>= 0xB;            //shr rax, 0x0B
        rcx = ~rcx;             //not rcx
        r8 ^= rax;              //xor r8, rax
        rcx *= rbx;             //imul rcx, rbx
        rax = r8;               //mov rax, r8
        rax >>= 0x16;           //shr rax, 0x16
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x2C;           //shr rax, 0x2C
        rcx ^= rax;             //xor rcx, rax
        r8 ^= rcx;              //xor r8, rcx
        rax = 0x22A1571E2E749CB;                //mov rax, 0x22A1571E2E749CB
        r8 *= rax;              //imul r8, rax
        rcx = mem.baseAddress + 0xF26D;           //lea rcx, [0xFFFFFFFFFD2E1649]
        rax = rbx;              //mov rax, rbx
        rax *= rcx;             //imul rax, rcx
        r8 += rax;              //add r8, rax
        return r8;
    }
    case 4:
    {
        r10 = mem.Read<uintptr_t>(mem.baseAddress + 0xB3B312A);           //mov r10, [0x00000000086852A5]
        rax = 0x54EE9012A77B3C0E;               //mov rax, 0x54EE9012A77B3C0E
        r8 ^= rax;              //xor r8, rax
        rax = mem.baseAddress;            //lea rax, [0xFFFFFFFFFD2D1CFC]
        rax += 0x432D;          //add rax, 0x432D
        rax += rbx;             //add rax, rbx
        r8 += rax;              //add r8, rax
        rax = 0xBE6A84FFF3304C3D;               //mov rax, 0xBE6A84FFF3304C3D
        r8 *= rax;              //imul r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x12;           //shr rax, 0x12
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x24;           //shr rax, 0x24
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x7;            //shr rax, 0x07
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0xE;            //shr rax, 0x0E
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x1C;           //shr rax, 0x1C
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x38;           //shr rax, 0x38
        r8 ^= rax;              //xor r8, rax
        rcx = mem.baseAddress + 0x72D0A311;               //lea rcx, [0x000000006FFDC43E]
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r10;             //xor rax, r10
        rax = ~rax;             //not rax
        r8 *= mem.Read<uintptr_t>(rax + 0x9);           //imul r8, [rax+0x09]
        rax = rbx;              //mov rax, rbx
        rax *= rcx;             //imul rax, rcx
        r8 -= rax;              //sub r8, rax
        rax = 0x598660DAA37ACC99;               //mov rax, 0x598660DAA37ACC99
        r8 ^= rax;              //xor r8, rax
        return r8;
    }
    case 5:
    {
        r10 = mem.Read<uintptr_t>(mem.baseAddress + 0xB3B312A);           //mov r10, [0x0000000008684DF4]
        rcx = 0xC088FB236BE68165;               //mov rcx, 0xC088FB236BE68165
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r10;             //xor rax, r10
        rax = ~rax;             //not rax
        rax = mem.Read<uintptr_t>(rax + 0x9);           //mov rax, [rax+0x09]
        rax *= rcx;             //imul rax, rcx
        r8 *= rax;              //imul r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x5;            //shr rax, 0x05
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0xA;            //shr rax, 0x0A
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x14;           //shr rax, 0x14
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x28;           //shr rax, 0x28
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0xB;            //shr rax, 0x0B
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x16;           //shr rax, 0x16
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x2C;           //shr rax, 0x2C
        r8 ^= rax;              //xor r8, rax
        rax = 0xF87FD44152069748;               //mov rax, 0xF87FD44152069748
        r8 ^= rax;              //xor r8, rax
        rax = mem.baseAddress;            //lea rax, [0xFFFFFFFFFD2D1991]
        rax += 0x1079;          //add rax, 0x1079
        rax += rbx;             //add rax, rbx
        r8 ^= rax;              //xor r8, rax
        rcx = mem.baseAddress;            //lea rcx, [0xFFFFFFFFFD2D1B81]
        rax = rbx;              //mov rax, rbx
        rax = ~rax;             //not rax
        rax -= rcx;             //sub rax, rcx
        rax += 0xFFFFFFFF968271AB;              //add rax, 0xFFFFFFFF968271AB
        r8 += rax;              //add r8, rax
        return r8;
    }
    case 6:
    {
        r10 = mem.Read<uintptr_t>(mem.baseAddress + 0xB3B312A);           //mov r10, [0x0000000008684976]
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r10;             //xor rax, r10
        rax = ~rax;             //not rax
        r8 *= mem.Read<uintptr_t>(rax + 0x9);           //imul r8, [rax+0x09]
        rcx = mem.baseAddress + 0x1EE2;           //lea rcx, [0xFFFFFFFFFD2D36FC]
        rax = rbx;              //mov rax, rbx
        rax ^= rcx;             //xor rax, rcx
        rcx = 0x30DABF93D6E4FB5;                //mov rcx, 0x30DABF93D6E4FB5
        r8 ^= rcx;              //xor r8, rcx
        r8 -= rax;              //sub r8, rax
        rax = 0xDB8B0AAFA542904;                //mov rax, 0xDB8B0AAFA542904
        r8 -= rbx;              //sub r8, rbx
        r8 -= rax;              //sub r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x22;           //shr rax, 0x22
        r8 ^= rax;              //xor r8, rax
        rax = 0xDF170407BBE28DB5;               //mov rax, 0xDF170407BBE28DB5
        r8 *= rax;              //imul r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x8;            //shr rax, 0x08
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x10;           //shr rax, 0x10
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x20;           //shr rax, 0x20
        r8 ^= rax;              //xor r8, rax
        return r8;
    }
    case 7:
    {
        r9 = mem.Read<uintptr_t>(mem.baseAddress + 0xB3B312A);            //mov r9, [0x000000000868453F]
        r15 = mem.baseAddress + 0xC177;           //lea r15, [0xFFFFFFFFFD2DD579]
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r9;              //xor rax, r9
        rax = ~rax;             //not rax
        r8 *= mem.Read<uintptr_t>(rax + 0x9);           //imul r8, [rax+0x09]
        rax = 0x378CE09B287B2D41;               //mov rax, 0x378CE09B287B2D41
        r8 ^= rax;              //xor r8, rax
        rax = 0x1C4A7DE2E2F8F68F;               //mov rax, 0x1C4A7DE2E2F8F68F
        r8 += rax;              //add r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x23;           //shr rax, 0x23
        rax ^= r8;              //xor rax, r8
        r8 = rbx + 0x1;                 //lea r8, [rbx+0x01]
        rax += rbx;             //add rax, rbx
        r8 *= r15;              //imul r8, r15
        r8 += rax;              //add r8, rax
        rax = 0xEBEA9B8B5714671D;               //mov rax, 0xEBEA9B8B5714671D
        r8 *= rax;              //imul r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0xE;            //shr rax, 0x0E
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x1C;           //shr rax, 0x1C
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x38;           //shr rax, 0x38
        r8 ^= rax;              //xor r8, rax
        return r8;
    }
    case 8:
    {
        r10 = mem.Read<uintptr_t>(mem.baseAddress + 0xB3B312A);           //mov r10, [0x0000000008684006]
        rax = mem.baseAddress;            //lea rax, [0xFFFFFFFFFD2D0CD7]
        r8 ^= rax;              //xor r8, rax
        rax = 0x3169FBDB3B875224;               //mov rax, 0x3169FBDB3B875224
        r8 += rax;              //add r8, rax
        rcx = mem.baseAddress + 0x88B9;           //lea rcx, [0xFFFFFFFFFD2D975A]
        rax = rcx;              //mov rax, rcx
        rax = ~rax;             //not rax
        rax *= rbx;             //imul rax, rbx
        r8 ^= rax;              //xor r8, rax
        r8 ^= rbx;              //xor r8, rbx
        rax = mem.baseAddress + 0x553;            //lea rax, [0xFFFFFFFFFD2D107E]
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x13;           //shr rax, 0x13
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x26;           //shr rax, 0x26
        r8 ^= rax;              //xor r8, rax
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rax = 0x49665D7F2AFA3F6B;               //mov rax, 0x49665D7F2AFA3F6B
        r8 *= rax;              //imul r8, rax
        rcx ^= r10;             //xor rcx, r10
        rax = mem.baseAddress + 0x11D125F7;               //lea rax, [0x000000000EFE320B]
        rax = ~rax;             //not rax
        rcx = ~rcx;             //not rcx
        rax *= rbx;             //imul rax, rbx
        r8 += rax;              //add r8, rax
        r8 *= mem.Read<uintptr_t>(rcx + 0x9);           //imul r8, [rcx+0x09]
        return r8;
    }
    case 9:
    {
        r9 = mem.Read<uintptr_t>(mem.baseAddress + 0xB3B312A);            //mov r9, [0x0000000008683BC0]
        r11 = mem.baseAddress + 0x7C81;           //lea r11, [0xFFFFFFFFFD2D8704]
        rax = rbx;              //mov rax, rbx
        rax *= r11;             //imul rax, r11
        r8 -= rax;              //sub r8, rax
        rax = mem.baseAddress;            //lea rax, [0xFFFFFFFFFD2D07AB]
        r8 -= rax;              //sub r8, rax
        rax = rbx;              //mov rax, rbx
        rax -= mem.baseAddress;           //sub rax, [rsp+0x78] -- didn't find trace -> use base
        rax += 0xFFFFFFFFFFFF4D38;              //add rax, 0xFFFFFFFFFFFF4D38
        r8 += rax;              //add r8, rax
        rax = 0xB294869EA09D48AA;               //mov rax, 0xB294869EA09D48AA
        r8 ^= rax;              //xor r8, rax
        rax = 0xDA6A9700AB4D27FD;               //mov rax, 0xDA6A9700AB4D27FD
        r8 *= rax;              //imul r8, rax
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r9;              //xor rax, r9
        rax = ~rax;             //not rax
        r8 *= mem.Read<uintptr_t>(rax + 0x9);           //imul r8, [rax+0x09]
        rax = 0x38632CDC13FD78A5;               //mov rax, 0x38632CDC13FD78A5
        r8 += rax;              //add r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x1D;           //shr rax, 0x1D
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x3A;           //shr rax, 0x3A
        r8 ^= rax;              //xor r8, rax
        return r8;
    }
    case 10:
    {
        r10 = mem.Read<uintptr_t>(mem.baseAddress + 0xB3B312A);           //mov r10, [0x00000000086836A7]
        r8 += rbx;              //add r8, rbx
        rax = 0x36164EFD786890C1;               //mov rax, 0x36164EFD786890C1
        r8 *= rax;              //imul r8, rax
        rax = 0x6F993F33D7A49418;               //mov rax, 0x6F993F33D7A49418
        r8 += rax;              //add r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x8;            //shr rax, 0x08
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x10;           //shr rax, 0x10
        r8 ^= rax;              //xor r8, rax
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rax = r8;               //mov rax, r8
        rcx ^= r10;             //xor rcx, r10
        rax >>= 0x20;           //shr rax, 0x20
        r8 ^= rax;              //xor r8, rax
        rcx = ~rcx;             //not rcx
        r8 *= mem.Read<uintptr_t>(rcx + 0x9);           //imul r8, [rcx+0x09]
        rax = 0xE88B55E25B8B057C;               //mov rax, 0xE88B55E25B8B057C
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x1A;           //shr rax, 0x1A
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x34;           //shr rax, 0x34
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x4;            //shr rax, 0x04
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x8;            //shr rax, 0x08
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x10;           //shr rax, 0x10
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x20;           //shr rax, 0x20
        r8 ^= rax;              //xor r8, rax
        return r8;
    }
    case 11:
    {
        r10 = mem.Read<uintptr_t>(mem.baseAddress + 0xB3B312A);           //mov r10, [0x000000000868327D]
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r10;             //xor rax, r10
        rax = ~rax;             //not rax
        r8 *= mem.Read<uintptr_t>(rax + 0x9);           //imul r8, [rax+0x09]
        rax = rbx;              //mov rax, rbx
        uintptr_t RSP_0xFFFFFFFFFFFFFFC8;
        RSP_0xFFFFFFFFFFFFFFC8 = mem.baseAddress + 0xA1FD;                //lea rax, [0xFFFFFFFFFD2DA349] : RBP+0xFFFFFFFFFFFFFFC8
        rax *= RSP_0xFFFFFFFFFFFFFFC8;          //imul rax, [rbp-0x38]
        r8 += rax;              //add r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x26;           //shr rax, 0x26
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0xA;            //shr rax, 0x0A
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x14;           //shr rax, 0x14
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x28;           //shr rax, 0x28
        r8 ^= rax;              //xor r8, rax
        rax = 0xC6A8E21F37CF3675;               //mov rax, 0xC6A8E21F37CF3675
        r8 *= rax;              //imul r8, rax
        rax = mem.baseAddress;            //lea rax, [0xFFFFFFFFFD2CFC42]
        rax += rbx;             //add rax, rbx
        r8 -= rax;              //sub r8, rax
        rax = mem.baseAddress;            //lea rax, [0xFFFFFFFFFD2CFD6E]
        r8 ^= rax;              //xor r8, rax
        return r8;
    }
    case 12:
    {
        r10 = mem.Read<uintptr_t>(mem.baseAddress + 0xB3B312A);           //mov r10, [0x0000000008682D4B]
        rax = 0x5D2901AC55739352;               //mov rax, 0x5D2901AC55739352
        r8 -= rax;              //sub r8, rax
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r10;             //xor rax, r10
        rax = ~rax;             //not rax
        r8 *= mem.Read<uintptr_t>(rax + 0x9);           //imul r8, [rax+0x09]
        rax = mem.baseAddress;            //lea rax, [0xFFFFFFFFFD2CF76D]
        r8 += rax;              //add r8, rax
        rax = 0x156D71AB28FBFAFF;               //mov rax, 0x156D71AB28FBFAFF
        r8 *= rax;              //imul r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x27;           //shr rax, 0x27
        r8 ^= rax;              //xor r8, rax
        r8 -= rbx;              //sub r8, rbx
        rax = r8;               //mov rax, r8
        rax >>= 0x17;           //shr rax, 0x17
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x2E;           //shr rax, 0x2E
        r8 ^= rax;              //xor r8, rax
        rax = mem.baseAddress;            //lea rax, [0xFFFFFFFFFD2CF7EE]
        r8 ^= rax;              //xor r8, rax
        return r8;
    }
    case 13:
    {
        r11 = mem.Read<uintptr_t>(mem.baseAddress + 0xB3B312A);           //mov r11, [0x0000000008682802]
        rax = r8;               //mov rax, r8
        rax >>= 0x1F;           //shr rax, 0x1F
        r8 ^= rax;              //xor r8, rax
        rcx = r8;               //mov rcx, r8
        rax = mem.baseAddress;            //lea rax, [0xFFFFFFFFFD2CF2EF]
        rcx >>= 0x3E;           //shr rcx, 0x3E
        rcx ^= r8;              //xor rcx, r8
        rdx = 0;                //and rdx, 0xFFFFFFFFC0000000
        rdx = _rotl64(rdx, 0x10);               //rol rdx, 0x10
        r8 = rbx;               //mov r8, rbx
        r8 = ~r8;               //not r8
        rdx ^= r11;             //xor rdx, r11
        r8 += rcx;              //add r8, rcx
        rdx = ~rdx;             //not rdx
        r8 -= rax;              //sub r8, rax
        r8 -= 0x6929AFAC;               //sub r8, 0x6929AFAC
        r8 *= mem.Read<uintptr_t>(rdx + 0x9);           //imul r8, [rdx+0x09]
        rax = r8;               //mov rax, r8
        rax >>= 0x18;           //shr rax, 0x18
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x30;           //shr rax, 0x30
        r8 ^= rax;              //xor r8, rax
        rax = 0x69651B1AF033619B;               //mov rax, 0x69651B1AF033619B
        r8 += rbx;              //add r8, rbx
        r8 *= rax;              //imul r8, rax
        rax = 0x29BBD1B30DFD9417;               //mov rax, 0x29BBD1B30DFD9417
        r8 *= rax;              //imul r8, rax
        rax = 0xA7B8F15C4FABBB6C;               //mov rax, 0xA7B8F15C4FABBB6C
        r8 ^= rax;              //xor r8, rax
        return r8;
    }
    case 14:
    {
        r9 = mem.Read<uintptr_t>(mem.baseAddress + 0xB3B312A);            //mov r9, [0x000000000868238C]
        r8 += rbx;              //add r8, rbx
        rax = mem.baseAddress + 0x8D0;            //lea rax, [0xFFFFFFFFFD2CF992]
        rax -= rbx;             //sub rax, rbx
        r8 += rax;              //add r8, rax
        rax = 0xBC0AAA7E98B1663A;               //mov rax, 0xBC0AAA7E98B1663A
        r8 ^= rax;              //xor r8, rax
        rax = 0x54D1F9305B205B45;               //mov rax, 0x54D1F9305B205B45
        r8 *= rax;              //imul r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0xA;            //shr rax, 0x0A
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x14;           //shr rax, 0x14
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x28;           //shr rax, 0x28
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x12;           //shr rax, 0x12
        r8 ^= rax;              //xor r8, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x24;           //shr rax, 0x24
        r8 ^= rax;              //xor r8, rax
        rax = 0xFFFFFFFFDE23E20A;               //mov rax, 0xFFFFFFFFDE23E20A
        rax -= rbx;             //sub rax, rbx
        rax -= mem.baseAddress;           //sub rax, [rsp+0x78] -- didn't find trace -> use base
        r8 += rax;              //add r8, rax
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r9;              //xor rax, r9
        rax = ~rax;             //not rax
        r8 *= mem.Read<uintptr_t>(rax + 0x9);           //imul r8, [rax+0x09]
        return r8;
    }
    case 15:
    {
        r10 = mem.Read<uintptr_t>(mem.baseAddress + 0xB3B312A);           //mov r10, [0x0000000008681F09]
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r10;             //xor rax, r10
        rax = ~rax;             //not rax
        rax = mem.Read<uintptr_t>(rax + 0x9);           //mov rax, [rax+0x09]
        uintptr_t RSP_0xFFFFFFFFFFFFFFB8;
        RSP_0xFFFFFFFFFFFFFFB8 = 0x3A27415DA31CA989;            //mov rax, 0x3A27415DA31CA989 : RBP+0xFFFFFFFFFFFFFFB8
        rax *= RSP_0xFFFFFFFFFFFFFFB8;          //imul rax, [rbp-0x48]
        r8 *= rax;              //imul r8, rax
        rax = 0x6F6A3BE0CADE4A54;               //mov rax, 0x6F6A3BE0CADE4A54
        r8 -= rax;              //sub r8, rax
        r8 -= rbx;              //sub r8, rbx
        rax = r8;               //mov rax, r8
        rax >>= 0x13;           //shr rax, 0x13
        rcx = rbx;              //mov rcx, rbx
        r8 ^= rax;              //xor r8, rax
        rcx = ~rcx;             //not rcx
        rax = mem.baseAddress + 0x76BB;           //lea rax, [0xFFFFFFFFFD2D6344]
        rax = ~rax;             //not rax
        rcx *= rax;             //imul rcx, rax
        rax = r8;               //mov rax, r8
        rax >>= 0x26;           //shr rax, 0x26
        r8 ^= rax;              //xor r8, rax
        r8 += rcx;              //add r8, rcx
        rax = r8;               //mov rax, r8
        rax >>= 0x28;           //shr rax, 0x28
        r8 ^= rax;              //xor r8, rax
        rax = 0x3224CE0A9BEB6A6E;               //mov rax, 0x3224CE0A9BEB6A6E
        r8 -= rax;              //sub r8, rax
        return r8;
    }
    }
}

inline uintptr_t decrypt_bone_base()
{
    uint64_t mb = mem.baseAddress;
    uint64_t rax = mb, rbx = mb, rcx = mb, rdx = mb, rdi = mb, rsi = mb, r8 = mb, r9 = mb, r10 = mb, r11 = mb, r12 = mb, r13 = mb, r14 = mb, r15 = mb;

    rdx = mem.Read<uintptr_t>(mem.baseAddress + 0x1193F498);
    if (!rdx)
        return rdx;

    r11 = mem.GetPEBAddress();           //mov r11, gs:[rax]
    rax = r11;              //mov rax, r11
    rax >>= 0x13;           //shr rax, 0x13
    rax &= 0xF;

    switch (rax)
    {
    case 0:
    {
        r10 = mem.Read<uintptr_t>(mem.baseAddress + 0xB3B321D);           //mov r10, [0x0000000007467706]
        r12 = mem.baseAddress + 0x8C93;           //lea r12, [0xFFFFFFFFFC0BD175]
        rax = r12;              //mov rax, r12
        rax -= r11;             //sub rax, r11
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x15;           //shr rax, 0x15
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x2A;           //shr rax, 0x2A
        rdx ^= rax;             //xor rdx, rax
        rdx += r11;             //add rdx, r11
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r10;             //xor rax, r10
        rax = ~rax;             //not rax
        rdx *= mem.Read<uintptr_t>(rax + 0x17);                 //imul rdx, [rax+0x17]
        rax = mem.baseAddress;            //lea rax, [0xFFFFFFFFFC0B415D]
        rdx ^= rax;             //xor rdx, rax
        rax = 0x860534C8C01FEA7B;               //mov rax, 0x860534C8C01FEA7B
        rdx *= rax;             //imul rdx, rax
        rax = 0xEE334BF3EC572D68;               //mov rax, 0xEE334BF3EC572D68
        rdx ^= rax;             //xor rdx, rax
        return rdx;
    }
    case 1:
    {
        r10 = mem.Read<uintptr_t>(mem.baseAddress + 0xB3B321D);           //mov r10, [0x00000000074672C4]
        r12 = mem.baseAddress + 0xDF5D;           //lea r12, [0xFFFFFFFFFC0C1FF1]
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r10;             //xor rax, r10
        rax = ~rax;             //not rax
        rdx *= mem.Read<uintptr_t>(rax + 0x17);                 //imul rdx, [rax+0x17]
        rax = 0x8A4B98169395E686;               //mov rax, 0x8A4B98169395E686
        rdx ^= rax;             //xor rdx, rax
        rax = 0xC3957EB9F84EC5AF;               //mov rax, 0xC3957EB9F84EC5AF
        rdx *= rax;             //imul rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0xE;            //shr rax, 0x0E
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x1C;           //shr rax, 0x1C
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x38;           //shr rax, 0x38
        rdx ^= rax;             //xor rdx, rax
        rax = mem.baseAddress + 0x31CB;           //lea rax, [0xFFFFFFFFFC0B6E32]
        rax -= r11;             //sub rax, r11
        rdx += rax;             //add rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0xD;            //shr rax, 0x0D
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x1A;           //shr rax, 0x1A
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x34;           //shr rax, 0x34
        rdx ^= rax;             //xor rdx, rax
        rax = r12;              //mov rax, r12
        rax = ~rax;             //not rax
        rax ^= r11;             //xor rax, r11
        rdx ^= rax;             //xor rdx, rax
        return rdx;
    }
    case 2:
    {
        r10 = mem.Read<uintptr_t>(mem.baseAddress + 0xB3B321D);           //mov r10, [0x0000000007466DB9]
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r10;             //xor rax, r10
        rax = ~rax;             //not rax
        rdx *= mem.Read<uintptr_t>(rax + 0x17);                 //imul rdx, [rax+0x17]
        rcx = mem.baseAddress + 0x47C2AE1B;               //lea rcx, [0x0000000043CDE985]
        rax = rcx;              //mov rax, rcx
        rax = ~rax;             //not rax
        rax ^= r11;             //xor rax, r11
        rax += r11;             //add rax, r11
        rdx += rax;             //add rdx, rax
        rax = 0x94073D91C803188D;               //mov rax, 0x94073D91C803188D
        rdx ^= rax;             //xor rdx, rax
        rax = 0x2EEA8A0831CE333B;               //mov rax, 0x2EEA8A0831CE333B
        rdx *= rax;             //imul rdx, rax
        rdx += r11;             //add rdx, r11
        rax = rdx;              //mov rax, rdx
        rax >>= 0x13;           //shr rax, 0x13
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x26;           //shr rax, 0x26
        rdx ^= rax;             //xor rdx, rax
        rax = 0xD4E2CCE5B7959CA0;               //mov rax, 0xD4E2CCE5B7959CA0
        rdx ^= rax;             //xor rdx, rax
        return rdx;
    }
    case 3:
    {
        r9 = mem.Read<uintptr_t>(mem.baseAddress + 0xB3B321D);            //mov r9, [0x0000000007466A08]
        r12 = mem.baseAddress + 0x114B;           //lea r12, [0xFFFFFFFFFC0B4923]
        rax = rdx;              //mov rax, rdx
        rax >>= 0x13;           //shr rax, 0x13
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x26;           //shr rax, 0x26
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x24;           //shr rax, 0x24
        rdx ^= rax;             //xor rdx, rax
        rax = 0x764F15DD269101D3;               //mov rax, 0x764F15DD269101D3
        rdx *= rax;             //imul rdx, rax
        rax = 0x34E81942B113C230;               //mov rax, 0x34E81942B113C230
        rdx -= rax;             //sub rdx, rax
        rax = 0x13805FC46F4FC36A;               //mov rax, 0x13805FC46F4FC36A
        rdx += rax;             //add rdx, rax
        rax = r11;              //mov rax, r11
        rax -= mem.baseAddress;           //sub rax, [rbp-0x50] -- didn't find trace -> use base
        rax += 0xFFFFFFFFFFFF85F3;              //add rax, 0xFFFFFFFFFFFF85F3
        rdx += rax;             //add rdx, rax
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r9;              //xor rax, r9
        rax = ~rax;             //not rax
        rdx *= mem.Read<uintptr_t>(rax + 0x17);                 //imul rdx, [rax+0x17]
        rax = r11;              //mov rax, r11
        rax ^= r12;             //xor rax, r12
        rdx ^= rax;             //xor rdx, rax
        return rdx;
    }
    case 4:
    {
        r9 = mem.Read<uintptr_t>(mem.baseAddress + 0xB3B321D);            //mov r9, [0x0000000007466491]
        rax = rdx;              //mov rax, rdx
        rax >>= 0x11;           //shr rax, 0x11
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x22;           //shr rax, 0x22
        rdx ^= rax;             //xor rdx, rax
        rax = 0x2CFB6FB2F3BAD3C;                //mov rax, 0x2CFB6FB2F3BAD3C
        rdx -= rax;             //sub rdx, rax
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r9;              //xor rax, r9
        rax = ~rax;             //not rax
        rdx *= mem.Read<uintptr_t>(rax + 0x17);                 //imul rdx, [rax+0x17]
        rax = 0xEED0F28134CE8447;               //mov rax, 0xEED0F28134CE8447
        rdx *= rax;             //imul rdx, rax
        rax = 0x52D4170A67BFFCB2;               //mov rax, 0x52D4170A67BFFCB2
        rdx ^= rax;             //xor rdx, rax
        rdx += r11;             //add rdx, r11
        rax = rdx;              //mov rax, rdx
        rax >>= 0x16;           //shr rax, 0x16
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x2C;           //shr rax, 0x2C
        rdx ^= rax;             //xor rdx, rax
        rdx ^= r11;             //xor rdx, r11
        return rdx;
    }
    case 5:
    {
        r10 = mem.Read<uintptr_t>(mem.baseAddress + 0xB3B321D);           //mov r10, [0x0000000007465EEE]
        r12 = mem.baseAddress + 0x654BDD13;               //lea r12, [0x00000000615709CC]
        rax = r12;              //mov rax, r12
        rax = ~rax;             //not rax
        rax += 1;               //inc rax
        rax += r11;             //add rax, r11
        rdx += rax;             //add rdx, rax
        rax = 0x4A2AFA53025C5181;               //mov rax, 0x4A2AFA53025C5181
        rdx *= rax;             //imul rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x28;           //shr rax, 0x28
        rdx ^= rax;             //xor rdx, rax
        rax = mem.baseAddress;            //lea rax, [0xFFFFFFFFFC0B2991]
        rcx = rax + 0xa045;             //lea rcx, [rax+0xA045]
        rax += 0x19B7DBCB;              //add rax, 0x19B7DBCB
        rax += r11;             //add rax, r11
        rcx += r11;             //add rcx, r11
        rcx ^= rax;             //xor rcx, rax
        rdx ^= rcx;             //xor rdx, rcx
        rax = 0x574A3A5B7408079B;               //mov rax, 0x574A3A5B7408079B
        rdx *= rax;             //imul rdx, rax
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r10;             //xor rax, r10
        rax = ~rax;             //not rax
        rdx *= mem.Read<uintptr_t>(rax + 0x17);                 //imul rdx, [rax+0x17]
        return rdx;
    }
    case 6:
    {
        r10 = mem.Read<uintptr_t>(mem.baseAddress + 0xB3B321D);           //mov r10, [0x0000000007465AF7]
        r12 = mem.baseAddress + 0x4951;           //lea r12, [0xFFFFFFFFFC0B7213]
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r10;             //xor rax, r10
        rax = ~rax;             //not rax
        rdx *= mem.Read<uintptr_t>(rax + 0x17);                 //imul rdx, [rax+0x17]
        rdx += r11;             //add rdx, r11
        rax = r11;              //mov rax, r11
        rax *= r12;             //imul rax, r12
        rdx += rax;             //add rdx, rax
        rcx = mem.baseAddress;            //lea rcx, [0xFFFFFFFFFC0B272B]
        rax = rcx + 0x1d37b933;                 //lea rax, [rcx+0x1D37B933]
        rax += r11;             //add rax, r11
        rdx += rax;             //add rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x9;            //shr rax, 0x09
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x12;           //shr rax, 0x12
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x24;           //shr rax, 0x24
        rdx ^= rax;             //xor rdx, rax
        rax = 0x6C2A29044A40E4C7;               //mov rax, 0x6C2A29044A40E4C7
        rdx *= rax;             //imul rdx, rax
        rdx ^= rcx;             //xor rdx, rcx
        rax = rdx;              //mov rax, rdx
        rax >>= 0x3;            //shr rax, 0x03
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x6;            //shr rax, 0x06
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0xC;            //shr rax, 0x0C
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x18;           //shr rax, 0x18
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x30;           //shr rax, 0x30
        rdx ^= rax;             //xor rdx, rax
        return rdx;
    }
    case 7:
    {
        r10 = mem.Read<uintptr_t>(mem.baseAddress + 0xB3B321D);           //mov r10, [0x0000000007465701]
        rax = mem.baseAddress;            //lea rax, [0xFFFFFFFFFC0B23A1]
        rdx += rax;             //add rdx, rax
        rax = 0x5F80490A38DB3901;               //mov rax, 0x5F80490A38DB3901
        rdx ^= rax;             //xor rdx, rax
        rax = 0x4EC9DC6A5902297D;               //mov rax, 0x4EC9DC6A5902297D
        rdx -= rax;             //sub rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x25;           //shr rax, 0x25
        rdx ^= rax;             //xor rdx, rax
        rcx = mem.baseAddress + 0xCEFB;           //lea rcx, [0xFFFFFFFFFC0BF3AD]
        rax = r11;              //mov rax, r11
        rax ^= rcx;             //xor rax, rcx
        rdx += rax;             //add rdx, rax
        rax = 0x92B34BC27C367071;               //mov rax, 0x92B34BC27C367071
        rdx *= rax;             //imul rdx, rax
        rdx -= r11;             //sub rdx, r11
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r10;             //xor rax, r10
        rax = ~rax;             //not rax
        rdx *= mem.Read<uintptr_t>(rax + 0x17);                 //imul rdx, [rax+0x17]
        return rdx;
    }
    case 8:
    {
        r10 = mem.Read<uintptr_t>(mem.baseAddress + 0xB3B321D);           //mov r10, [0x00000000074652CB]
        rcx = mem.baseAddress + 0x5723;           //lea rcx, [0xFFFFFFFFFC0B7757]
        rax = 0xE62DA6375F493113;               //mov rax, 0xE62DA6375F493113
        rdx *= rax;             //imul rdx, rax
        rax = mem.baseAddress;            //lea rax, [0xFFFFFFFFFC0B1E06]
        rdx -= rax;             //sub rdx, rax
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r10;             //xor rax, r10
        rax = ~rax;             //not rax
        rdx *= mem.Read<uintptr_t>(rax + 0x17);                 //imul rdx, [rax+0x17]
        rdx -= r11;             //sub rdx, r11
        rax = rdx;              //mov rax, rdx
        rax >>= 0xF;            //shr rax, 0x0F
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x1E;           //shr rax, 0x1E
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x3C;           //shr rax, 0x3C
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x3;            //shr rax, 0x03
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x6;            //shr rax, 0x06
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0xC;            //shr rax, 0x0C
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x18;           //shr rax, 0x18
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x30;           //shr rax, 0x30
        rdx ^= rax;             //xor rdx, rax
        rax = mem.baseAddress;            //lea rax, [0xFFFFFFFFFC0B1C78]
        rax += 0xFAB2;          //add rax, 0xFAB2
        rax += r11;             //add rax, r11
        rdx ^= rax;             //xor rdx, rax
        rax = r11;              //mov rax, r11
        rax ^= rcx;             //xor rax, rcx
        rdx ^= rax;             //xor rdx, rax
        return rdx;
    }
    case 9:
    {
        r10 = mem.Read<uintptr_t>(mem.baseAddress + 0xB3B321D);           //mov r10, [0x0000000007464D83]
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r10;             //xor rax, r10
        rax = ~rax;             //not rax
        rdx *= mem.Read<uintptr_t>(rax + 0x17);                 //imul rdx, [rax+0x17]
        rax = mem.baseAddress;            //lea rax, [0xFFFFFFFFFC0B1AAD]
        rcx = rax + 0x429d;             //lea rcx, [rax+0x429D]
        rax += 0xF1EC;          //add rax, 0xF1EC
        rax += r11;             //add rax, r11
        rcx += r11;             //add rcx, r11
        rdx += rcx;             //add rdx, rcx
        rdx ^= rax;             //xor rdx, rax
        rax = mem.baseAddress + 0x5304B0E6;               //lea rax, [0x000000004F0FCB69]
        rdx ^= r11;             //xor rdx, r11
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x22;           //shr rax, 0x22
        rdx ^= rax;             //xor rdx, rax
        rax = 0xEE899EDDAF56550;                //mov rax, 0xEE899EDDAF56550
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0xE;            //shr rax, 0x0E
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x1C;           //shr rax, 0x1C
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x38;           //shr rax, 0x38
        rdx ^= rax;             //xor rdx, rax
        rax = 0x39D515C223A57391;               //mov rax, 0x39D515C223A57391
        rdx *= rax;             //imul rdx, rax
        return rdx;
    }
    case 10:
    {
        r10 = mem.Read<uintptr_t>(mem.baseAddress + 0xB3B321D);           //mov r10, [0x0000000007464907]
        rcx = mem.baseAddress + 0x1A3D;           //lea rcx, [0xFFFFFFFFFC0B3094]
        rax = rdx;              //mov rax, rdx
        rax >>= 0xF;            //shr rax, 0x0F
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x1E;           //shr rax, 0x1E
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x3C;           //shr rax, 0x3C
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x13;           //shr rax, 0x13
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x26;           //shr rax, 0x26
        rdx ^= rax;             //xor rdx, rax
        rax = r11;              //mov rax, r11
        rax = ~rax;             //not rax
        rax *= rcx;             //imul rax, rcx
        rdx ^= rax;             //xor rdx, rax
        rax = 0x8330B389343DA675;               //mov rax, 0x8330B389343DA675
        rdx *= rax;             //imul rdx, rax
        rax = 0x5A325A7184C15E55;               //mov rax, 0x5A325A7184C15E55
        rdx -= rax;             //sub rdx, rax
        rax = 0xE28957C95B7E497;                //mov rax, 0xE28957C95B7E497
        rdx += rax;             //add rdx, rax
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r10;             //xor rax, r10
        rax = ~rax;             //not rax
        rdx *= mem.Read<uintptr_t>(rax + 0x17);                 //imul rdx, [rax+0x17]
        rdx -= r11;             //sub rdx, r11
        return rdx;
    }
    case 11:
    {
        r10 = mem.Read<uintptr_t>(mem.baseAddress + 0xB3B321D);           //mov r10, [0x000000000746443C]
        r15 = mem.baseAddress + 0x67B591A2;               //lea r15, [0x0000000063C0A3AE]
        rax = rdx;              //mov rax, rdx
        rax >>= 0x22;           //shr rax, 0x22
        rdx ^= rax;             //xor rdx, rax
        rax = r15;              //mov rax, r15
        rax = ~rax;             //not rax
        rax ^= r11;             //xor rax, r11
        rax += r11;             //add rax, r11
        rdx -= rax;             //sub rdx, rax
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r10;             //xor rax, r10
        rax = ~rax;             //not rax
        rdx *= mem.Read<uintptr_t>(rax + 0x17);                 //imul rdx, [rax+0x17]
        rdx ^= r11;             //xor rdx, r11
        rax = 0x112AEF7CBA9BEDF1;               //mov rax, 0x112AEF7CBA9BEDF1
        rdx *= rax;             //imul rdx, rax
        rax = 0x792205E77EAA6797;               //mov rax, 0x792205E77EAA6797
        rdx ^= rax;             //xor rdx, rax
        return rdx;
    }
    case 12:
    {
        r10 = mem.Read<uintptr_t>(mem.baseAddress + 0xB3B321D);           //mov r10, [0x0000000007464093]
        r12 = mem.baseAddress + 0x70E4B3E1;               //lea r12, [0x000000006CEFC23F]
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r10;             //xor rax, r10
        rax = ~rax;             //not rax
        rdx *= mem.Read<uintptr_t>(rax + 0x17);                 //imul rdx, [rax+0x17]
        rax = 0x33BF00DD8A073650;               //mov rax, 0x33BF00DD8A073650
        rdx -= rax;             //sub rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0xA;            //shr rax, 0x0A
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x14;           //shr rax, 0x14
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x28;           //shr rax, 0x28
        rdx ^= rax;             //xor rdx, rax
        rax = mem.baseAddress;            //lea rax, [0xFFFFFFFFFC0B0C10]
        rdx ^= rax;             //xor rdx, rax
        rax = r12;              //mov rax, r12
        rax = ~rax;             //not rax
        rdx += rax;             //add rdx, rax
        rax = 0x37300D9E69A77B2F;               //mov rax, 0x37300D9E69A77B2F
        rdx *= rax;             //imul rdx, rax
        rdx -= r11;             //sub rdx, r11
        return rdx;
    }
    case 13:
    {
        r10 = mem.Read<uintptr_t>(mem.baseAddress + 0xB3B321D);           //mov r10, [0x0000000007463CD1]
        r15 = mem.baseAddress + 0x666C9DA0;               //lea r15, [0x000000006277A83C]
        rax = r11;              //mov rax, r11
        rax ^= r15;             //xor rax, r15
        rdx -= rax;             //sub rdx, rax
        rax = 0x124569EA4125D98;                //mov rax, 0x124569EA4125D98
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x5;            //shr rax, 0x05
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0xA;            //shr rax, 0x0A
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x14;           //shr rax, 0x14
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x28;           //shr rax, 0x28
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x1A;           //shr rax, 0x1A
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x34;           //shr rax, 0x34
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rdx ^= rax;             //xor rdx, rax
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r10;             //xor rcx, r10
        rcx = ~rcx;             //not rcx
        rdx *= mem.Read<uintptr_t>(rcx + 0x17);                 //imul rdx, [rcx+0x17]
        rdx ^= r11;             //xor rdx, r11
        rax = 0xD83F30F92C64DF4F;               //mov rax, 0xD83F30F92C64DF4F
        rdx ^= rax;             //xor rdx, rax
        rax = 0xB69AFD2628432A9D;               //mov rax, 0xB69AFD2628432A9D
        rdx *= rax;             //imul rdx, rax
        return rdx;
    }
    case 14:
    {
        r10 = mem.Read<uintptr_t>(mem.baseAddress + 0xB3B321D);           //mov r10, [0x00000000074638AE]
        r12 = mem.baseAddress + 0x5113;           //lea r12, [0xFFFFFFFFFC0B578C]
        rax = rdx;              //mov rax, rdx
        rax >>= 0x1B;           //shr rax, 0x1B
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x36;           //shr rax, 0x36
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rdx ^= rax;             //xor rdx, rax
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r10;             //xor rcx, r10
        rcx = ~rcx;             //not rcx
        rdx *= mem.Read<uintptr_t>(rcx + 0x17);                 //imul rdx, [rcx+0x17]
        rax = 0xDC4274449EFE767B;               //mov rax, 0xDC4274449EFE767B
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x6;            //shr rax, 0x06
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0xC;            //shr rax, 0x0C
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x18;           //shr rax, 0x18
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x30;           //shr rax, 0x30
        rdx ^= rax;             //xor rdx, rax
        rax = r11;              //mov rax, r11
        rax ^= r12;             //xor rax, r12
        rdx -= rax;             //sub rdx, rax
        rax = 0x4480AA60A21867F9;               //mov rax, 0x4480AA60A21867F9
        rdx *= rax;             //imul rdx, rax
        rax = mem.baseAddress;            //lea rax, [0xFFFFFFFFFC0B0352]
        rax += 0xD03A;          //add rax, 0xD03A
        rax += r11;             //add rax, r11
        rdx += rax;             //add rdx, rax
        return rdx;
    }
    case 15:
    {
        r10 = mem.Read<uintptr_t>(mem.baseAddress + 0xB3B321D);           //mov r10, [0x0000000007463414]
        rcx = mem.baseAddress + 0x6744783A;               //lea rcx, [0x00000000634F7975]
        rdx += r11;             //add rdx, r11
        rax = r11;              //mov rax, r11
        rax = ~rax;             //not rax
        uintptr_t RSP_0xFFFFFFFFFFFFFFB0;
        RSP_0xFFFFFFFFFFFFFFB0 = mem.baseAddress + 0x642A39AC;            //lea rax, [0x0000000060353B9C] : RBP+0xFFFFFFFFFFFFFFB0
        rax ^= RSP_0xFFFFFFFFFFFFFFB0;          //xor rax, [rbp-0x50]
        rdx -= rax;             //sub rdx, rax
        rdx ^= r11;             //xor rdx, r11
        rdx ^= rcx;             //xor rdx, rcx
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r10;             //xor rax, r10
        rax = ~rax;             //not rax
        rdx *= mem.Read<uintptr_t>(rax + 0x17);                 //imul rdx, [rax+0x17]
        rax = 0x54750E0E4638841A;               //mov rax, 0x54750E0E4638841A
        rdx += rax;             //add rdx, rax
        rax = 0x17257FE07A931EB4;               //mov rax, 0x17257FE07A931EB4
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x4;            //shr rax, 0x04
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x8;            //shr rax, 0x08
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x10;           //shr rax, 0x10
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x20;           //shr rax, 0x20
        rdx ^= rax;             //xor rdx, rax
        rax = 0x7493CCED6314B08B;               //mov rax, 0x7493CCED6314B08B
        rdx *= rax;             //imul rdx, rax
        return rdx;
    }
    }
};
 

inline uintptr_t decrypt_bone_base_1()
{
	auto imageBase = mem.baseAddress;
	 
    uint64_t rax = imageBase, rbx = imageBase, rcx = imageBase, rdx = imageBase, rdi = imageBase, rsi = imageBase, r8 = imageBase, r9 = imageBase, r10 = imageBase, r11 = imageBase, r12 = imageBase, r13 = imageBase, r14 = imageBase, r15 = imageBase;
    rax = mem.Read<uintptr_t>(imageBase + 0x11DDC608);
    if (!rax)
        return rax;
    rbx = mem.GetPEBAddress();              //mov rbx, gs:[rcx]
    //failed to translate: mov [rsp+0xC0], r12
    rcx = rbx;              //mov rcx, rbx
    rcx >>= 0x13;           //shr rcx, 0x13
    rcx &= 0xF;
    switch (rcx) {
    case 0:
    {
        r10 = mem.Read<uintptr_t>(imageBase + 0xB8E152D);           //mov r10, [0x0000000008C1E393]
        r12 = imageBase + 0x8C93;               //lea r12, [0xFFFFFFFFFD345AF2]
        r11 = imageBase;                //lea r11, [0xFFFFFFFFFD33CE3D]
        rcx = r12;              //mov rcx, r12
        rcx -= rbx;             //sub rcx, rbx
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x15;           //shr rcx, 0x15
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x2A;           //shr rcx, 0x2A
        rax ^= rcx;             //xor rax, rcx
        rax += rbx;             //add rax, rbx
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r10;             //xor rcx, r10
        rcx = ~rcx;             //not rcx
        rax *= mem.Read<uintptr_t>(rcx + 0x17);             //imul rax, [rcx+0x17]
        rax ^= r11;             //xor rax, r11
        rcx = 0x860534C8C01FEA7B;               //mov rcx, 0x860534C8C01FEA7B
        rax *= rcx;             //imul rax, rcx
        rcx = 0xEE334BF3EC572D68;               //mov rcx, 0xEE334BF3EC572D68
        rax ^= rcx;             //xor rax, rcx
        return rax;
    }
    case 1:
    {
        r10 = mem.Read<uintptr_t>(imageBase + 0xB8E152D);           //mov r10, [0x0000000008C1DF6B]
        r15 = imageBase + 0xDF5D;               //lea r15, [0xFFFFFFFFFD34A988]
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r10;             //xor rcx, r10
        rcx = ~rcx;             //not rcx
        rax *= mem.Read<uintptr_t>(rcx + 0x17);             //imul rax, [rcx+0x17]
        rcx = 0x8A4B98169395E686;               //mov rcx, 0x8A4B98169395E686
        rax ^= rcx;             //xor rax, rcx
        rcx = 0xC3957EB9F84EC5AF;               //mov rcx, 0xC3957EB9F84EC5AF
        rax *= rcx;             //imul rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0xE;            //shr rcx, 0x0E
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x1C;           //shr rcx, 0x1C
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x38;           //shr rcx, 0x38
        rax ^= rcx;             //xor rax, rcx
        rcx = imageBase + 0x31CB;               //lea rcx, [0xFFFFFFFFFD33F7D8]
        rcx -= rbx;             //sub rcx, rbx
        rax += rcx;             //add rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0xD;            //shr rcx, 0x0D
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x1A;           //shr rcx, 0x1A
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x34;           //shr rcx, 0x34
        rax ^= rcx;             //xor rax, rcx
        rcx = r15;              //mov rcx, r15
        rcx = ~rcx;             //not rcx
        rcx ^= rbx;             //xor rcx, rbx
        rax ^= rcx;             //xor rax, rcx
        return rax;
    }
    case 2:
    {
        r10 = mem.Read<uintptr_t>(imageBase + 0xB8E152D);           //mov r10, [0x0000000008C1DA7E]
        r12 = imageBase + 0x47C2AE1B;           //lea r12, [0x0000000044F67359]
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r10;             //xor rcx, r10
        rcx = ~rcx;             //not rcx
        rax *= mem.Read<uintptr_t>(rcx + 0x17);             //imul rax, [rcx+0x17]
        rcx = r12;              //mov rcx, r12
        rcx = ~rcx;             //not rcx
        rcx ^= rbx;             //xor rcx, rbx
        rcx += rbx;             //add rcx, rbx
        rax += rcx;             //add rax, rcx
        rcx = 0x94073D91C803188D;               //mov rcx, 0x94073D91C803188D
        rax ^= rcx;             //xor rax, rcx
        rcx = 0x2EEA8A0831CE333B;               //mov rcx, 0x2EEA8A0831CE333B
        rax *= rcx;             //imul rax, rcx
        rax += rbx;             //add rax, rbx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x13;           //shr rcx, 0x13
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x26;           //shr rcx, 0x26
        rax ^= rcx;             //xor rax, rcx
        rcx = 0xD4E2CCE5B7959CA0;               //mov rcx, 0xD4E2CCE5B7959CA0
        rax ^= rcx;             //xor rax, rcx
        return rax;
    }
    case 3:
    {
        r9 = mem.Read<uintptr_t>(imageBase + 0xB8E152D);            //mov r9, [0x0000000008C1D6D6]
        r11 = imageBase;                //lea r11, [0xFFFFFFFFFD33C196]
        r12 = imageBase + 0x114B;               //lea r12, [0xFFFFFFFFFD33D2D6]
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x13;           //shr rcx, 0x13
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x26;           //shr rcx, 0x26
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x24;           //shr rcx, 0x24
        rax ^= rcx;             //xor rax, rcx
        rcx = 0x764F15DD269101D3;               //mov rcx, 0x764F15DD269101D3
        rax *= rcx;             //imul rax, rcx
        rcx = 0x34E81942B113C230;               //mov rcx, 0x34E81942B113C230
        rax -= rcx;             //sub rax, rcx
        rcx = 0x13805FC46F4FC36A;               //mov rcx, 0x13805FC46F4FC36A
        rax += rcx;             //add rax, rcx
        rcx = rbx;              //mov rcx, rbx
        rcx -= r11;             //sub rcx, r11
        rcx += 0xFFFFFFFFFFFF85F3;              //add rcx, 0xFFFFFFFFFFFF85F3
        rax += rcx;             //add rax, rcx
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r9;              //xor rcx, r9
        rcx = ~rcx;             //not rcx
        rax *= mem.Read<uintptr_t>(rcx + 0x17);             //imul rax, [rcx+0x17]
        rax ^= rbx;             //xor rax, rbx
        rax ^= r12;             //xor rax, r12
        return rax;
    }
    case 4:
    {
        r9 = mem.Read<uintptr_t>(imageBase + 0xB8E152D);            //mov r9, [0x0000000008C1D18D]
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x11;           //shr rcx, 0x11
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x22;           //shr rcx, 0x22
        rax ^= rcx;             //xor rax, rcx
        rcx = 0x2CFB6FB2F3BAD3C;                //mov rcx, 0x2CFB6FB2F3BAD3C
        rax -= rcx;             //sub rax, rcx
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r9;              //xor rcx, r9
        rcx = ~rcx;             //not rcx
        rax *= mem.Read<uintptr_t>(rcx + 0x17);             //imul rax, [rcx+0x17]
        rcx = 0xEED0F28134CE8447;               //mov rcx, 0xEED0F28134CE8447
        rax *= rcx;             //imul rax, rcx
        rcx = 0x52D4170A67BFFCB2;               //mov rcx, 0x52D4170A67BFFCB2
        rax ^= rcx;             //xor rax, rcx
        rax += rbx;             //add rax, rbx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x16;           //shr rcx, 0x16
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x2C;           //shr rcx, 0x2C
        rax ^= rcx;             //xor rax, rcx
        rax ^= rbx;             //xor rax, rbx
        return rax;
    }
    case 5:
    {
        r10 = mem.Read<uintptr_t>(imageBase + 0xB8E152D);           //mov r10, [0x0000000008C1CC0D]
        r11 = imageBase;                //lea r11, [0xFFFFFFFFFD33B6C8]
        r12 = imageBase + 0x654BDD13;           //lea r12, [0x00000000627F93CA]
        rcx = r12;              //mov rcx, r12
        rcx = ~rcx;             //not rcx
        //failed to translate: inc rcx
        rcx += rbx;             //add rcx, rbx
        rax += rcx;             //add rax, rcx
        rcx = 0x4A2AFA53025C5181;               //mov rcx, 0x4A2AFA53025C5181
        rax *= rcx;             //imul rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x28;           //shr rcx, 0x28
        rax ^= rcx;             //xor rax, rcx
        rdx = r11 + 0xa045;             //lea rdx, [r11+0xA045]
        rdx += rbx;             //add rdx, rbx
        rcx = r11 + 0x19b7dbcb;                 //lea rcx, [r11+0x19B7DBCB]
        rcx += rbx;             //add rcx, rbx
        rdx ^= rcx;             //xor rdx, rcx
        rax ^= rdx;             //xor rax, rdx
        rcx = 0x574A3A5B7408079B;               //mov rcx, 0x574A3A5B7408079B
        rax *= rcx;             //imul rax, rcx
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r10;             //xor rcx, r10
        rcx = ~rcx;             //not rcx
        rax *= mem.Read<uintptr_t>(rcx + 0x17);             //imul rax, [rcx+0x17]
        return rax;
    }
    case 6:
    {
        r10 = mem.Read<uintptr_t>(imageBase + 0xB8E152D);           //mov r10, [0x0000000008C1C837]
        r11 = imageBase;                //lea r11, [0xFFFFFFFFFD33B2F2]
        r12 = imageBase + 0x4951;               //lea r12, [0xFFFFFFFFFD33FC32]
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r10;             //xor rcx, r10
        rcx = ~rcx;             //not rcx
        rax *= mem.Read<uintptr_t>(rcx + 0x17);             //imul rax, [rcx+0x17]
        rax += rbx;             //add rax, rbx
        rcx = rbx;              //mov rcx, rbx
        rcx *= r12;             //imul rcx, r12
        rax += rcx;             //add rax, rcx
        rcx = r11 + 0x1d37b933;                 //lea rcx, [r11+0x1D37B933]
        rcx += rbx;             //add rcx, rbx
        rax += rcx;             //add rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x9;            //shr rcx, 0x09
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x12;           //shr rcx, 0x12
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x24;           //shr rcx, 0x24
        rax ^= rcx;             //xor rax, rcx
        rcx = 0x6C2A29044A40E4C7;               //mov rcx, 0x6C2A29044A40E4C7
        rax *= rcx;             //imul rax, rcx
        rax ^= r11;             //xor rax, r11
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x3;            //shr rcx, 0x03
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x6;            //shr rcx, 0x06
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0xC;            //shr rcx, 0x0C
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x18;           //shr rcx, 0x18
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x30;           //shr rcx, 0x30
        rax ^= rcx;             //xor rax, rcx
        return rax;
    }
    case 7:
    {
        r10 = mem.Read<uintptr_t>(imageBase + 0xB8E152D);           //mov r10, [0x0000000008C1C463]
        r11 = imageBase;                //lea r11, [0xFFFFFFFFFD33AF1E]
        r13 = imageBase + 0xCEFB;               //lea r13, [0xFFFFFFFFFD347E08]
        rax += r11;             //add rax, r11
        rcx = 0x5F80490A38DB3901;               //mov rcx, 0x5F80490A38DB3901
        rax ^= rcx;             //xor rax, rcx
        rcx = 0x4EC9DC6A5902297D;               //mov rcx, 0x4EC9DC6A5902297D
        rax -= rcx;             //sub rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x25;           //shr rcx, 0x25
        rax ^= rcx;             //xor rax, rcx
        rcx = rbx;              //mov rcx, rbx
        rcx ^= r13;             //xor rcx, r13
        rax += rcx;             //add rax, rcx
        rcx = 0x92B34BC27C367071;               //mov rcx, 0x92B34BC27C367071
        rax *= rcx;             //imul rax, rcx
        rax -= rbx;             //sub rax, rbx
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r10;             //xor rcx, r10
        rcx = ~rcx;             //not rcx
        rax *= mem.Read<uintptr_t>(rcx + 0x17);             //imul rax, [rcx+0x17]
        return rax;
    }
    case 8:
    {
        r10 = mem.Read<uintptr_t>(imageBase + 0xB8E152D);           //mov r10, [0x0000000008C1C045]
        r11 = imageBase;                //lea r11, [0xFFFFFFFFFD33AB05]
        r13 = imageBase + 0x5723;               //lea r13, [0xFFFFFFFFFD34021D]
        rcx = 0xE62DA6375F493113;               //mov rcx, 0xE62DA6375F493113
        rax *= rcx;             //imul rax, rcx
        rax -= r11;             //sub rax, r11
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r10;             //xor rcx, r10
        rcx = ~rcx;             //not rcx
        rax *= mem.Read<uintptr_t>(rcx + 0x17);             //imul rax, [rcx+0x17]
        rax -= rbx;             //sub rax, rbx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0xF;            //shr rcx, 0x0F
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x1E;           //shr rcx, 0x1E
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x3C;           //shr rcx, 0x3C
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x3;            //shr rcx, 0x03
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x6;            //shr rcx, 0x06
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0xC;            //shr rcx, 0x0C
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x18;           //shr rcx, 0x18
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x30;           //shr rcx, 0x30
        rax ^= rcx;             //xor rax, rcx
        rcx = r11 + 0xfab2;             //lea rcx, [r11+0xFAB2]
        rcx += rbx;             //add rcx, rbx
        rax ^= rcx;             //xor rax, rcx
        rax ^= rbx;             //xor rax, rbx
        rax ^= r13;             //xor rax, r13
        return rax;
    }
    case 9:
    {
        r10 = mem.Read<uintptr_t>(imageBase + 0xB8E152D);           //mov r10, [0x0000000008C1BB40]
        r11 = imageBase;                //lea r11, [0xFFFFFFFFFD33A5FB]
        r13 = imageBase + 0x5304B0E6;           //lea r13, [0x00000000503856D0]
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r10;             //xor rcx, r10
        rcx = ~rcx;             //not rcx
        rax *= mem.Read<uintptr_t>(rcx + 0x17);             //imul rax, [rcx+0x17]
        rax += rbx;             //add rax, rbx
        rcx = imageBase + 0x429D;               //lea rcx, [0xFFFFFFFFFD33E7F1]
        rax += rcx;             //add rax, rcx
        rcx = r11 + 0xf1ec;             //lea rcx, [r11+0xF1EC]
        rcx += rbx;             //add rcx, rbx
        rax ^= rcx;             //xor rax, rcx
        rax ^= rbx;             //xor rax, rbx
        rax ^= r13;             //xor rax, r13
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x22;           //shr rcx, 0x22
        rax ^= rcx;             //xor rax, rcx
        rcx = 0xEE899EDDAF56550;                //mov rcx, 0xEE899EDDAF56550
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0xE;            //shr rcx, 0x0E
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x1C;           //shr rcx, 0x1C
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x38;           //shr rcx, 0x38
        rax ^= rcx;             //xor rax, rcx
        rcx = 0x39D515C223A57391;               //mov rcx, 0x39D515C223A57391
        rax *= rcx;             //imul rax, rcx
        return rax;
    }
    case 10:
    {
        r10 = mem.Read<uintptr_t>(imageBase + 0xB8E152D);           //mov r10, [0x0000000008C1B6F3]
        r12 = imageBase + 0x1A3D;               //lea r12, [0xFFFFFFFFFD33BBF0]
        rcx = rax;              //mov rcx, rax
        rcx >>= 0xF;            //shr rcx, 0x0F
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x1E;           //shr rcx, 0x1E
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x3C;           //shr rcx, 0x3C
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x13;           //shr rcx, 0x13
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x26;           //shr rcx, 0x26
        rax ^= rcx;             //xor rax, rcx
        rcx = rbx;              //mov rcx, rbx
        rcx = ~rcx;             //not rcx
        rcx *= r12;             //imul rcx, r12
        rax ^= rcx;             //xor rax, rcx
        rcx = 0x8330B389343DA675;               //mov rcx, 0x8330B389343DA675
        rax *= rcx;             //imul rax, rcx
        rcx = 0x5A325A7184C15E55;               //mov rcx, 0x5A325A7184C15E55
        rax -= rcx;             //sub rax, rcx
        rcx = 0xE28957C95B7E497;                //mov rcx, 0xE28957C95B7E497
        rax += rcx;             //add rax, rcx
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r10;             //xor rcx, r10
        rcx = ~rcx;             //not rcx
        rax *= mem.Read<uintptr_t>(rcx + 0x17);             //imul rax, [rcx+0x17]
        rax -= rbx;             //sub rax, rbx
        return rax;
    }
    case 11:
    {
        r10 = mem.Read<uintptr_t>(imageBase + 0xB8E152D);           //mov r10, [0x0000000008C1B24E]
        r11 = imageBase + 0x67B591A2;           //lea r11, [0x0000000064E92EAB]
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x22;           //shr rcx, 0x22
        rax ^= rcx;             //xor rax, rcx
        rcx = r11;              //mov rcx, r11
        rcx = ~rcx;             //not rcx
        rcx ^= rbx;             //xor rcx, rbx
        rcx += rbx;             //add rcx, rbx
        rax -= rcx;             //sub rax, rcx
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r10;             //xor rcx, r10
        rcx = ~rcx;             //not rcx
        rax *= mem.Read<uintptr_t>(rcx + 0x17);             //imul rax, [rcx+0x17]
        rax ^= rbx;             //xor rax, rbx
        rcx = 0x112AEF7CBA9BEDF1;               //mov rcx, 0x112AEF7CBA9BEDF1
        rax *= rcx;             //imul rax, rcx
        rcx = 0x792205E77EAA6797;               //mov rcx, 0x792205E77EAA6797
        rax ^= rcx;             //xor rax, rcx
        return rax;
    }
    case 12:
    {
        r10 = mem.Read<uintptr_t>(imageBase + 0xB8E152D);           //mov r10, [0x0000000008C1AEC5]
        r11 = imageBase;                //lea r11, [0xFFFFFFFFFD339980]
        r12 = imageBase + 0x70E4B3E1;           //lea r12, [0x000000006E184D50]
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r10;             //xor rcx, r10
        rcx = ~rcx;             //not rcx
        rax *= mem.Read<uintptr_t>(rcx + 0x17);             //imul rax, [rcx+0x17]
        rcx = 0x33BF00DD8A073650;               //mov rcx, 0x33BF00DD8A073650
        rax -= rcx;             //sub rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0xA;            //shr rcx, 0x0A
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x14;           //shr rcx, 0x14
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x28;           //shr rcx, 0x28
        rax ^= rcx;             //xor rax, rcx
        rax ^= r11;             //xor rax, r11
        rcx = r12;              //mov rcx, r12
        rcx = ~rcx;             //not rcx
        rax += rcx;             //add rax, rcx
        rcx = 0x37300D9E69A77B2F;               //mov rcx, 0x37300D9E69A77B2F
        rax *= rcx;             //imul rax, rcx
        rax -= rbx;             //sub rax, rbx
        return rax;
    }
    case 13:
    {
        r10 = mem.Read<uintptr_t>(imageBase + 0xB8E152D);           //mov r10, [0x0000000008C1AB1C]
        r11 = imageBase + 0x666C9DA0;           //lea r11, [0x0000000063A03377]
        rcx = r11;              //mov rcx, r11
        rcx ^= rbx;             //xor rcx, rbx
        rax -= rcx;             //sub rax, rcx
        rcx = 0x124569EA4125D98;                //mov rcx, 0x124569EA4125D98
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x5;            //shr rcx, 0x05
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0xA;            //shr rcx, 0x0A
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x14;           //shr rcx, 0x14
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x28;           //shr rcx, 0x28
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x1A;           //shr rcx, 0x1A
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x34;           //shr rcx, 0x34
        rdx = 0;                //and rdx, 0xFFFFFFFFC0000000
        rax ^= rcx;             //xor rax, rcx
        rdx = _rotl64(rdx, 0x10);               //rol rdx, 0x10
        rdx ^= r10;             //xor rdx, r10
        rdx = ~rdx;             //not rdx
        rax *= mem.Read<uintptr_t>(rdx + 0x17);             //imul rax, [rdx+0x17]
        rax ^= rbx;             //xor rax, rbx
        rcx = 0xD83F30F92C64DF4F;               //mov rcx, 0xD83F30F92C64DF4F
        rax ^= rcx;             //xor rax, rcx
        rcx = 0xB69AFD2628432A9D;               //mov rcx, 0xB69AFD2628432A9D
        rax *= rcx;             //imul rax, rcx
        return rax;
    }
    case 14:
    {
        r10 = mem.Read<uintptr_t>(imageBase + 0xB8E152D);           //mov r10, [0x0000000008C1A704]
        r15 = imageBase + 0x5113;               //lea r15, [0xFFFFFFFFFD33E2D2]
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x1B;           //shr rcx, 0x1B
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x36;           //shr rcx, 0x36
        rdx = 0;                //and rdx, 0xFFFFFFFFC0000000
        rax ^= rcx;             //xor rax, rcx
        rdx = _rotl64(rdx, 0x10);               //rol rdx, 0x10
        rdx ^= r10;             //xor rdx, r10
        rdx = ~rdx;             //not rdx
        rax *= mem.Read<uintptr_t>(rdx + 0x17);             //imul rax, [rdx+0x17]
        rcx = 0xDC4274449EFE767B;               //mov rcx, 0xDC4274449EFE767B
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x6;            //shr rcx, 0x06
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0xC;            //shr rcx, 0x0C
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x18;           //shr rcx, 0x18
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x30;           //shr rcx, 0x30
        rax ^= rcx;             //xor rax, rcx
        rcx = r15;              //mov rcx, r15
        rcx ^= rbx;             //xor rcx, rbx
        rax -= rcx;             //sub rax, rcx
        rcx = 0x4480AA60A21867F9;               //mov rcx, 0x4480AA60A21867F9
        rax *= rcx;             //imul rax, rcx
        rcx = imageBase + 0xD03A;               //lea rcx, [0xFFFFFFFFFD345EE2]
        rcx += rbx;             //add rcx, rbx
        rax += rcx;             //add rax, rcx
        return rax;
    }
    case 15:
    {
        r10 = mem.Read<uintptr_t>(imageBase + 0xB8E152D);           //mov r10, [0x0000000008C1A288]
        r13 = imageBase + 0x642A39AC;           //lea r13, [0x00000000615DC6F4]
        r12 = imageBase + 0x6744783A;           //lea r12, [0x0000000064780577]
        rax += rbx;             //add rax, rbx
        rcx = rbx;              //mov rcx, rbx
        rcx = ~rcx;             //not rcx
        rcx ^= r13;             //xor rcx, r13
        rax -= rcx;             //sub rax, rcx
        rax ^= r12;             //xor rax, r12
        rax ^= rbx;             //xor rax, rbx
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r10;             //xor rcx, r10
        rcx = ~rcx;             //not rcx
        rax *= mem.Read<uintptr_t>(rcx + 0x17);             //imul rax, [rcx+0x17]
        rcx = 0x54750E0E4638841A;               //mov rcx, 0x54750E0E4638841A
        rax += rcx;             //add rax, rcx
        rcx = 0x17257FE07A931EB4;               //mov rcx, 0x17257FE07A931EB4
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x4;            //shr rcx, 0x04
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x8;            //shr rcx, 0x08
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x10;           //shr rcx, 0x10
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x20;           //shr rcx, 0x20
        rax ^= rcx;             //xor rax, rcx
        rcx = 0x7493CCED6314B08B;               //mov rcx, 0x7493CCED6314B08B
        rax *= rcx;             //imul rax, rcx
        return rax;
    }
    }
};

inline uint16_t get_bone_index(uint32_t bone_index)
{
    const uint64_t mb = mem.baseAddress;
    uint64_t rax = mb, rbx = mb, rcx = mb, rdx = mb, rdi = mb, rsi = mb, r8 = mb, r9 = mb, r10 = mb, r11 = mb, r12 = mb, r13 = mb, r14 = mb, r15 = mb;
    rbx = bone_index;
    rcx = rbx * 0x13C8;
    rax = 0xCB182C584BD5193;                //mov rax, 0xCB182C584BD5193
    r11 = mem.baseAddress;            //lea r11, [0xFFFFFFFFFD2C73EE]
    rax = _umul128(rax, rcx, (uintptr_t*)&rdx);             //mul rcx
    rax = rcx;              //mov rax, rcx
    r10 = 0xD6FB75C08B670E5B;               //mov r10, 0xD6FB75C08B670E5B
    rax -= rdx;             //sub rax, rdx
    rax >>= 0x1;            //shr rax, 0x01
    rax += rdx;             //add rax, rdx
    rax >>= 0xC;            //shr rax, 0x0C
    rax = rax * 0x1E7D;             //imul rax, rax, 0x1E7D
    rcx -= rax;             //sub rcx, rax
    rax = 0x4078E2A8FCDA18EF;               //mov rax, 0x4078E2A8FCDA18EF
    r8 = rcx * 0x1E7D;              //imul r8, rcx, 0x1E7D
    rax = _umul128(rax, r8, (uintptr_t*)&rdx);              //mul r8
    rdx >>= 0xB;            //shr rdx, 0x0B
    rax = rdx * 0x1FC4;             //imul rax, rdx, 0x1FC4
    r8 -= rax;              //sub r8, rax
    rax = 0xF0F0F0F0F0F0F0F1;               //mov rax, 0xF0F0F0F0F0F0F0F1
    rax = _umul128(rax, r8, (uintptr_t*)&rdx);              //mul r8
    rax = 0x624DD2F1A9FBE77;                //mov rax, 0x624DD2F1A9FBE77
    rdx >>= 0x6;            //shr rdx, 0x06
    rcx = rdx * 0x44;               //imul rcx, rdx, 0x44
    rax = _umul128(rax, r8, (uintptr_t*)&rdx);              //mul r8
    rax = r8;               //mov rax, r8
    rax -= rdx;             //sub rax, rdx
    rax >>= 0x1;            //shr rax, 0x01
    rax += rdx;             //add rax, rdx
    rax >>= 0x6;            //shr rax, 0x06
    rcx += rax;             //add rcx, rax
    rax = rcx * 0xFA;               //imul rax, rcx, 0xFA
    rcx = r8 * 0xFC;                //imul rcx, r8, 0xFC
    rcx -= rax;             //sub rcx, rax
    rax = mem.Read<uint16_t>(rcx + r11 * 1 + 0xA73F130);            //movzx eax, word ptr [rcx+r11*1+0xA73F130]
    r8 = rax * 0x13C8;              //imul r8, rax, 0x13C8
    rax = r10;              //mov rax, r10
    rax = _umul128(rax, r8, (uintptr_t*)&rdx);              //mul r8
    rax = r10;              //mov rax, r10
    rdx >>= 0xD;            //shr rdx, 0x0D
    rcx = rdx * 0x261B;             //imul rcx, rdx, 0x261B
    r8 -= rcx;              //sub r8, rcx
    r9 = r8 * 0x2F75;               //imul r9, r8, 0x2F75
    rax = _umul128(rax, r9, (uintptr_t*)&rdx);              //mul r9
    rdx >>= 0xD;            //shr rdx, 0x0D
    rax = rdx * 0x261B;             //imul rax, rdx, 0x261B
    r9 -= rax;              //sub r9, rax
    rax = 0x8FB823EE08FB823F;               //mov rax, 0x8FB823EE08FB823F
    rax = _umul128(rax, r9, (uintptr_t*)&rdx);              //mul r9
    rax = 0x579D6EE340579D6F;               //mov rax, 0x579D6EE340579D6F
    rdx >>= 0x5;            //shr rdx, 0x05
    rcx = rdx * 0x39;               //imul rcx, rdx, 0x39
    rax = _umul128(rax, r9, (uintptr_t*)&rdx);              //mul r9
    rdx >>= 0x6;            //shr rdx, 0x06
    rcx += rdx;             //add rcx, rdx
    rax = rcx * 0x176;              //imul rax, rcx, 0x176
    rcx = r9 * 0x178;               //imul rcx, r9, 0x178
    rcx -= rax;             //sub rcx, rax
    r14 = mem.Read<uint16_t>(rcx + r11 * 1 + 0xA7433A0);            //movsx r14d, word ptr [rcx+r11*1+0xA7433A0]
    return r14;
}

