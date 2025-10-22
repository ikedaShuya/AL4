#pragma once
#include "KamataEngine.h"
#include <cmath>

using namespace KamataEngine;

/// <summary>
/// 数学
/// </summary>
class Math {
public:
	// --- ベクトル演算（要素ごとの基本計算） ---
	static Vector3 Add(const Vector3& v1, const Vector3& v2);
	static Vector3 Subtract(const Vector3& v1, const Vector3& v2);
	static Vector3 Multiply(float scalar, const Vector3& v);

	// --- ベクトルの性質・幾何演算 ---
	static float Length(const Vector3& v);
	static Vector3 Normalize(const Vector3& v);
	static float Dot(const Vector3& v1, const Vector3& v2);
	static Vector3 Cross(const Vector3& v1, const Vector3& v2);

	// --- ベクトルの補間・曲線補間 ---
	static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);
	static Vector3 Bezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, float t);

	// --- 行列演算（加減算・乗算・逆行列など） ---
	static Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);
	static Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);
	static Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
	static Matrix4x4 Inverse(const Matrix4x4& m);

	// --- 行列生成（スケール・回転・移動・射影など） ---
	static Matrix4x4 MakeScaleMatrix(const Vector3& scale);
	static Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
	static Matrix4x4 MakeRotateXMatrix(float radian);
	static Matrix4x4 MakeRotateYMatrix(float radian);
	static Matrix4x4 MakeRotateZMatrix(float radian);
	static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
	static Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);
	static Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

	// --- 座標変換 ---
	static Vector3 Transform(const Vector3& v, const Matrix4x4& m);
};