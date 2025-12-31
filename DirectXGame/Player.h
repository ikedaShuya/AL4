#pragma once
#include "KamataEngine.h"

/// <summary>
/// 自キャラ
/// </summary>
class Player {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	// プレイヤーの横移動入力を処理する
	void InputHorizontal();

	// 移動入力
	void InputMove();

	// ジャンプ処理を行う
	void ProcessJump();

	// 重力による落下処理を行う
	void ApplyGravity();

	// 左右
	enum class LRDirection {
		kRight,
		kLeft,
	};

private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;
	// モデル
	KamataEngine::Model* model_ = nullptr;

	// カメラ
	KamataEngine::Camera* camera_ = nullptr;

	KamataEngine::Vector3 velocity_ = {};

	static inline const float kAcceleration = 0.025f;

	static inline const float kAttenuation = 0.90f;

	static inline const float kLimitRunSpeed = 0.15f;

	LRDirection lrDirection_ = LRDirection::kRight;

	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;

	// 旋回時間<秒>
	static inline const float kTimeTurn = 0.3f;

	// 重力加速度（下方向）
	static inline const float kGravityAcceleration = 0.045f;
	// 最大落下速度（下方向）
	static inline const float kLimitFallSpeed = 0.42f;
	// ジャンプ加速（上方向）
	static inline const float kJumpAcceleration = 0.52f;

	const float kGroundHeight = 1.0f;

	// 地面にいるかどうか
	bool onGround_ = true;

	// ジャンプ回数カウント
	int jumpCount_ = 0;
	const int kMaxJumpCount = 2;
};