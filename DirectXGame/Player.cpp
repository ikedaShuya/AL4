#define NOMINMAX
#include "Player.h"
#include "Math.h"
#include <algorithm>
#include <numbers>

using namespace KamataEngine;

void Player::Initialize(Model* model, Camera* camera) { // NULLポインタチェック
	assert(model);
	assert(camera);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	camera_ = camera;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.scale_ = {3.0f, 3.0f, 3.0f};
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

void Player::Update() {

	InputMove();

	ProcessJump();

	worldTransform_.translation_ += velocity_;

	// 地面判定
	if (worldTransform_.translation_.y <= kGroundHeight) {
		worldTransform_.translation_.y = kGroundHeight;
		velocity_.y = 0.0f;

		// 地面に着地した瞬間だけカウントリセット
		if (!onGround_) {
			jumpCount_ = 0;
		}

		onGround_ = true;
	} else {
		onGround_ = false;
	}

	// 旋回制御
	if (turnTimer_ > 0.0f) {

		turnTimer_ = std::max(turnTimer_ - (1.0f / 60.0f), 0.0f);

		// 左右の自キャラ角度テーブル
		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		// 自キャラの角度を設定する
		worldTransform_.rotation_.y = EaseInOut(destinationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);
	}

	// 行列を定数バッファに転送
	WorldTransformUpdate(worldTransform_);
}

void Player::Draw() {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, *camera_);
}

void Player::InputMove() {

	InputHorizontal();
	if (!onGround_) {
		ApplyGravity();
	}
}

void Player::InputHorizontal() {

	// 左右入力がある？
	bool pushD = Input::GetInstance()->PushKey(DIK_D);
	bool pushA = Input::GetInstance()->PushKey(DIK_A);

	if (!(pushD || pushA)) {
		velocity_.x *= (1.0f - kAttenuation);
		return;
	}

	// 向き変更
	if (pushD && lrDirection_ != LRDirection::kRight) {
		lrDirection_ = LRDirection::kRight;
		turnFirstRotationY_ = worldTransform_.rotation_.y;
		turnTimer_ = kTimeTurn;
	} else if (pushA && lrDirection_ != LRDirection::kLeft) {
		lrDirection_ = LRDirection::kLeft;
		turnFirstRotationY_ = worldTransform_.rotation_.y;
		turnTimer_ = kTimeTurn;
	}

	// 加速
	float accel = kAcceleration;

	if (pushD) {
		velocity_.x += accel;
	} else if (pushA) {
		velocity_.x -= accel;
	}

	// 最大速度制限
	velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
}

void Player::ApplyGravity() {

	// 空中のみ重力
	if (onGround_) {
		return;
	}

	// 落下速度
	velocity_ += Vector3(0, -kGravityAcceleration, 0);

	// 2段ジャンプ中は落下を少し抑える
	if (velocity_.y < 0.0f && jumpCount_ == 2) {
		velocity_.y *= 0.98f;
	}

	// 落下速度制限
	velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
}

void Player::ProcessJump() {

	if (!Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		return;
	}

	if (jumpCount_ >= kMaxJumpCount) {
		return;
	}

	if (jumpCount_ == 0) {
		velocity_.y = kJumpAcceleration;
	} else {
		velocity_.y = kJumpAcceleration * 1.0f;
	}

	onGround_ = false;
	jumpCount_++;
}