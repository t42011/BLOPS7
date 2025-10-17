#pragma once

#include <vector>
#include <algorithm>
#include <cmath>
#include <print>
#include <format>
#include <Windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <Dwmapi.h>
#include <fstream>
#include <iostream>

#include <string>
#include <string_view>

#include <chrono>
#include <mutex>
#include <thread>

#include <queue>
#include <array>
#include <span>

#include <random>
#include <limits>

#include <expected>
#include <functional>

#define M_PI 3.14159265358979323846264338327950288419716939937510
#define M_PI_2 1.57079632679489661923f
#define Assert(_exp) ((void)0)

class FVector2D
{
public:
    FVector2D() : X(0.f), Y(0.f) {}
    FVector2D(float _X, float _Y) : X(_X), Y(_Y) {}
    ~FVector2D() {}

    float X;
    float Y;

    operator bool() { return bool(this->X && this->Y); }

    __forceinline FVector2D operator+(const FVector2D& V) const {
        return FVector2D(X + V.X, Y + V.Y);
    }

    __forceinline FVector2D operator+(float A) const {
        return FVector2D(X + A, Y + A);
    }

    __forceinline FVector2D operator-(const FVector2D& v) const {
        return FVector2D(X - v.X, Y - v.Y);
    }

    __forceinline FVector2D& operator-=(const FVector2D& v) {
        X -= v.X;
        Y -= v.Y;
        return *this;
    }

    inline FVector2D flip() const {
        return { Y, X };
    }

    bool IsZeroVector() const {
        return X == 0.0f && Y == 0.0f;
    }
};

class FVector
{
public:
    FVector() : X(0.f), Y(0.f), Z(0.f) {}
    FVector(float _x, float _y, float _z) : X(_x), Y(_y), Z(_z) {}
    ~FVector() {}

    float X;
    float Y;
    float Z;

    FVector Add(const FVector& other) const {
        return FVector(X + other.X, Y + other.Y, Z + other.Z);
    }

    inline float Dot(const FVector& v) const {
        return X * v.X + Y * v.Y + Z * v.Z;
    }

    inline float Distance(const FVector& v) const {
        return std::sqrt(std::pow(v.X - X, 2.0f) + std::pow(v.Y - Y, 2.0f) + std::pow(v.Z - Z, 2.0f));
    }

    inline double Length() const {
        return std::sqrt(X * X + Y * Y + Z * Z);
    }

    inline bool Empty() const {
        return !X && !Y && !Z;
    }

    inline void Normalize() {
        while (X > 89.0f) X -= 180.f;
        while (X < -89.0f) X += 180.f;
        while (Y > 180.f) Y -= 360.f;
        while (Y < -180.f) Y += 360.f;
    }

    inline FVector GetNormalized() const {
        float length = sqrtf(X * X + Y * Y + Z * Z);
        if (length == 0.0f)
            return { 0, 0, 0 };
        return { X / length, Y / length, Z / length };
    }

    float Length2D() const
    {
        return sqrt((X * X) + (Y * Y));
    }

    float DistTo(FVector ape)
    {
        return (*this - ape).Length();
    }

    float Dist2D(const FVector& ape) const
    {
        return (*this - ape).Length2D();
    }
    bool IsZero() const noexcept {
        return X <= 0.f && Y <= 0.f && Z <= 0.f;
    }

    bool IsValid() const {
        return !(std::isnan(X) || std::isinf(X) || std::isnan(Y) || std::isinf(Y) || std::isnan(Z) || std::isinf(Z));
    }

    inline float DistTo(const FVector& ape) const {
        return (*this - ape).Length();
    }

    

    inline FVector operator+(const FVector& v) const {
        return FVector(X + v.X, Y + v.Y, Z + v.Z);
    }

    

    inline FVector& operator+=(const FVector& other) {
        X += other.X;
        Y += other.Y;
        Z += other.Z;
        return *this;
    }

    inline FVector operator-(const FVector& v) const {
        return FVector(X - v.X, Y - v.Y, Z - v.Z);
    }

    inline FVector operator/(float ape) const {
        return { X / ape, Y / ape, Z / ape };
    }

    inline FVector& operator-=(const FVector& v) {
        X -= v.X;
        Y -= v.Y;
        Z -= v.Z;
        return *this;
    }

    inline void Clamp() {
        if (Y > 180.0f) Y = 180.0f;
        else if (Y < -180.0f) Y = -180.0f;

        if (X > 89.0f) X = 89.0f;
        else if (X < -89.0f) X = -89.0f;

        Z = 0;
    }

    inline FVector CalcAngle(const FVector& src, const FVector& dst) const {
        FVector delta = src - dst;
        double hyp = std::sqrt(delta.X * delta.X + delta.Y * delta.Y);
        FVector angle;
        angle.X = static_cast<float>(std::atan(delta.Z / hyp) * 57.295779513082f);
        angle.Y = static_cast<float>(std::atan(delta.Y / delta.X) * 57.295779513082f);
        if (delta.X >= 0.0) angle.Y += 180.0f;
        return angle;
    }

    inline FVector operator*(float flNum) const {
        return FVector(X * flNum, Y * flNum, Z * flNum);
    }

    inline double Size() const {
        return Length();
    }
};



struct Matrix4x4 {
    float M[4][4];
};


struct NameEntry
{
    int idx = 0;
    char name[0x24] = "NONE";
    char tag[0x40] = "NONE";
    char unk1[0x1C];
    int health;
    char unk2[0x38];
};


enum EEntityStance : __int16
{
    STAND = 0,                           // Standing stance
    CROUCH = 1,                          // Crouching stance
    PRONE = 2,                           // Prone position
    DOWNED = 3
};

inline bool IsValidVA(uintptr_t address) {
    return (address && address >= 0x10000 && address <= 0x7FFFFFFFFFFF);
}
struct clientbits
{
    uint32_t data[7];
};
struct key {
    int32_t ref0;
    int32_t ref1;
    int32_t ref2;
};
struct ref_def_t {
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
    FVector2D tan_half_fov;
    char pad1[8];
    char pad2[4];
    FVector axis[3];
};

inline FVector TransformVector(const FVector& Vec, const Matrix4x4& Matrix) {
    FVector result;
    result.X = Vec.X * Matrix.M[0][0] + Vec.Y * Matrix.M[1][0] + Vec.Z * Matrix.M[2][0] + Matrix.M[3][0];
    result.Y = Vec.X * Matrix.M[0][1] + Vec.Y * Matrix.M[1][1] + Vec.Z * Matrix.M[2][1] + Matrix.M[3][1];
    result.Z = Vec.X * Matrix.M[0][2] + Vec.Y * Matrix.M[1][2] + Vec.Z * Matrix.M[2][2] + Matrix.M[3][2];
    return result;
}


inline bool isBoneValid(const FVector& basePos, const FVector& bone) {
    float dx = bone.X - basePos.X;
    float dy = bone.Y - basePos.Y;
    float dz = bone.Z - basePos.Z;
    float dist3D = sqrtf(dx * dx + dy * dy + dz * dz);

    const float minDist3D = 1.0f;
    const float maxDist3D = 1000.0f;

    if (bone.X == 0.0f && bone.Y == 0.0f && bone.Z == 0.0f) return false;
    if (dist3D < minDist3D || dist3D > maxDist3D) return false;
    return true;
}

inline float distance_3d(const FVector& point1, const FVector& point2) {
    float dx = point1.X - point2.X;
    float dy = point1.Y - point2.Y;
    float dz = point1.Z - point2.Z;
    return sqrtf(dx * dx + dy * dy + dz * dz);
}


inline float GetGtcDistance(const FVector2D& v1, const FVector2D& v2) {
    float dx = v2.X - v1.X;
    float dy = v2.Y - v1.Y;
    return std::sqrt(dx * dx + dy * dy);
}

