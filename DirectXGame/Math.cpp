#include "Math.h"

const float PI = 3.141592654f;

// --- ベクトル演算 ---
Vector3 Math::Add(const Vector3& v1, const Vector3& v2) { return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z}; }

Vector3 Math::Subtract(const Vector3& v1, const Vector3& v2) { return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z}; }

Vector3 Math::Multiply(float scalar, const Vector3& v) { return {v.x * scalar, v.y * scalar, v.z * scalar}; }

// --- 行列 ---


// --- ベクトル補助 ---

// --- 補間 ---