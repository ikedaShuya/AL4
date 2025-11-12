#pragma once
#include "KamataEngine.h"
#include "Math.h"
#include <numbers>
#include <algorithm>

using namespace KamataEngine;

/// <summary>
/// 自キャラ
/// </summary>
class Player {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, Camera* camera, const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	// 自キャラ操作のON/OFFフラグ
	bool enableHorizontalMove_ = true;

	// 二段ジャンプのON/OFFフラグ
	bool enableJump_ = true;

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;

	// カメラ
	Camera* camera_ = nullptr;

	Vector3 velocity_ = {};

	static inline const float kAcceleration = 0.01f;

	static inline const float kAttenuation = 0.05f;

	static inline const float kLimitRunSpeed = 0.3f;

	// 接地状態フラグ
	bool onGround_ = false;
	
	// 重力加速度（下方向）
	static inline const float kGravityAcceleration = 0.12f;
	// 最大落下速度（下方向）
	static inline const float kLimitFallSpeed = 0.4f;
	// ジャンプ加速（上方向）
	static inline const float kJumpAcceleration = 1.3f;

	// 上下移動のON/OFFスイッチ
	bool enableVerticalMove_ = false;

	int jumpCount_ = 0;
	const int kMaxJumpCount = 2;

	const float kGroundHeight = -15.0f;
};