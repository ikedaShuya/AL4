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
		onGround_ = true; // ★着地
	} else {
		onGround_ = false; // ★空中
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

void Player::ProcessJump() {

	// 地面にいないならジャンプ不可
	if (!onGround_) {
		return;
	}

	// スペースキーが押された瞬間だけ
	if (!Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		return;
	}

	// ジャンプ
	velocity_.y = kJumpAcceleration;
	onGround_ = false; // 空中扱いにする
}

void Player::ApplyGravity() {

	velocity_.y -= kGravityAcceleration;

	// 落下速度制限
	if (velocity_.y < -kLimitFallSpeed) {
		velocity_.y = -kLimitFallSpeed;
	}
}