#pragma once
#include "KamataEngine.h"

// 前方宣言
class Player;

/// <summary>
/// カメラコントローラ
/// </summary>
class CameraController {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(KamataEngine::Camera* camera);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	void SetTarget(Player* target) { target_ = target; }

	void Reset();

	// 矩形
	struct Rect {
		float left = 0.0f;   // 左端
		float right = 1.0f;  // 右端
		float bottom = 0.0f; // 下端
		float top = 1.0f;    // 上端
	};

	void SetMovableArea(Rect area) { movableArea_ = area; }

private:
	// カメラ（スライド6）
	KamataEngine::Camera* camera_ = nullptr;

	Player* target_ = nullptr;

	// 追従対象とカメラの座標の差（オフセット）
	KamataEngine::Vector3 targetOffset_ = {0, 0, -15.0f};

	// カメラ移動範囲
	Rect movableArea_ = {0, 100, 0, 20};

	// カメラの目標座標
	KamataEngine::Vector3 destination_ = {0.0f, 0.0f, 0.0f};

	// 座標補間割合
	static inline const float kInterpolationRate = 0.1f;

	// 速度掛け率
	static inline const float kVelocityBias = 5.0f;

	// 追従対象の各方向へのカメラ移動範囲
	static inline const Rect targetMargin = {-12.0f, 12.0f, -3.0f, 6.0f};
};