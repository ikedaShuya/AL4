#pragma once
#include "KamataEngine.h"

class MapChipField;

/// <summary>
/// 自キャラ
/// </summary>
class Player {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

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

	// マップとの当たり判定情報
	struct CollisionMapInfo {
		bool ceiling = false;
		bool landing = false;
		bool hitWall = false;
		KamataEngine::Vector3 move;
	};

	// マップ衝突判定
	void CheckMapCollision(CollisionMapInfo& info);

	void CheckMapCollisionUp(CollisionMapInfo& info);
	void CheckMapCollisionDown(CollisionMapInfo& info);
	void CheckMapCollisionRight(CollisionMapInfo& info);
	void CheckMapCollisionLeft(CollisionMapInfo& info);

	// 角
	enum Corner {
		kRightBottom, // 右下
		kLeftBottom,  // 左下
		kRightTop,    // 右上
		kLeftTop,     // 左上

		kNumCorner // 要素数
	};

	KamataEngine::Vector3 CornerPosition(const KamataEngine::Vector3& center, Corner corner);

	// 接地状態の切り替え処理
	void UpdateOnGround(const CollisionMapInfo& info);

	// 壁に接触している場合の処理
	void UpdateOnWall(const CollisionMapInfo& info);

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }

	const KamataEngine::Vector3& GetVelocity() const { return velocity_; }

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
	bool onGround_ = false;

	// マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;

	// 着地時の速度減衰率
	static inline const float kAttenuationLanding = 0.03f;

	// 着地時の速度減衰率
	static inline const float kAttenuationWall = 0.2f;

	static inline const float kBlank = 0.04f;

	// 微小な数値
	static inline const float kGroundSearchHeight = 0.06f;

	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	int jumpBufferTimer_ = 0;
	static constexpr int kJumpBufferFrame = 5;

	// ジャンプ回数カウント
	int jumpCount_ = 0;
	const int kMaxJumpCount = 2;
};