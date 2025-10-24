#include "Player.h"

void Player::Initialize(Model* model, Camera* camera, const Vector3& position) {

	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	camera_ = camera;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	// worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

void Player::Update() {

	if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {

		// 左右加速
		Vector3 acceleration = {};
		if (Input::GetInstance()->PushKey(DIK_RIGHT)) {

			// 左移動中の右入力
			if (velocity_.x < 0.0f) {
				// 速度と逆方向に入力中は急ブレーキ
				velocity_.x *= (1.0f - kAttenuation);
			}

			acceleration.x += kAcceleration;
		} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {

			// 右移動中の左入力
			if (velocity_.x > 0.0f) {
				// 速度と逆方向に入力中は急ブレーキ
				velocity_.x *= (1.0f - kAttenuation);
			}

			acceleration.x -= kAcceleration;
		}
		// 加速/減速
		velocity_ += acceleration;

		// 最大速度制限
		velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
	} else {
		velocity_.x *= (1.0f - kAttenuation);
	}

	if (Input::GetInstance()->PushKey(DIK_UP) || Input::GetInstance()->PushKey(DIK_DOWN)) {

		// 上下加速
		Vector3 acceleration = {};
		if (Input::GetInstance()->PushKey(DIK_UP)) {

			// 下移動中の上入力
			if (velocity_.y < 0.0f) {
				// 速度と逆方向に入力中は急ブレーキ
				velocity_.y *= (1.0f - kAttenuation);
			}

			acceleration.y += kAcceleration;

		} else if (Input::GetInstance()->PushKey(DIK_DOWN)) {

			// 上移動中の下入力
			if (velocity_.y > 0.0f) {
				// 速度と逆方向に入力中は急ブレーキ
				velocity_.y *= (1.0f - kAttenuation);
			}

			acceleration.y -= kAcceleration;
		}

		// 加速/減速
		velocity_ += acceleration;

		// 最大速度制限
		velocity_.y = std::clamp(velocity_.y, -kLimitRunSpeed, kLimitRunSpeed);

	} else {
		// 入力がないときは減速
		velocity_.y *= (1.0f - kAttenuation);
	}

	// 移動
	worldTransform_.translation_ += velocity_;

	// 行列を定数バッファに転送
	WorldMatrix(worldTransform_);
}

void Player::Draw() {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, *camera_);
}