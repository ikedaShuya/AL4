#define NOMINMAX
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

	if (enableHorizontalMove_) {
		if (onGround_) {

			if (Input::GetInstance()->PushKey(DIK_D) || Input::GetInstance()->PushKey(DIK_A)) {

				// 左右加速
				Vector3 acceleration = {};
				if (Input::GetInstance()->PushKey(DIK_D)) {

					// 左移動中の右入力
					if (velocity_.x < 0.0f) {
						// 速度と逆方向に入力中は急ブレーキ
						velocity_.x *= (1.0f - kAttenuation);
					}

					acceleration.x += kAcceleration;
				} else if (Input::GetInstance()->PushKey(DIK_A)) {

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

			// 空中
		} else {
			// 落下速度
			velocity_ += Vector3(0, -kGravityAcceleration, 0);

			if (velocity_.y < 0 && jumpCount_ == 2) {
				velocity_.y *= 0.98f; // 2段ジャンプ中は少し減速
			}

			if (Input::GetInstance()->PushKey(DIK_D)) {
				velocity_.x += kAcceleration * 0.5f; // 空中では半分だけ効く
			} else if (Input::GetInstance()->PushKey(DIK_A)) {
				velocity_.x -= kAcceleration * 0.5f;
			}

			// 落下速度制限
			velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
		}
	} else {
		velocity_.x *= (1.0f - kAttenuation);
	}

	if (enableVerticalMove_) {
		if (Input::GetInstance()->PushKey(DIK_W) || Input::GetInstance()->PushKey(DIK_S)) {

			// 上下加速
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_W)) {

				// 下移動中の上入力
				if (velocity_.y < 0.0f) {
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.y *= (1.0f - kAttenuation);
				}

				acceleration.y += kAcceleration;

			} else if (Input::GetInstance()->PushKey(DIK_S)) {

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
	}

	if (enableJump_) {
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			// ジャンプ可能かチェック
			if (jumpCount_ < kMaxJumpCount) {
				// ジャンプ加速
				if (jumpCount_ == 0) {
					velocity_.y = kJumpAcceleration; // 通常ジャンプ
				} else {
					velocity_.y = kJumpAcceleration * 0.8f; // 2段目は少し弱め
				}
				onGround_ = false;
				jumpCount_++;
			}
		}
	}

	// 移動
	worldTransform_.translation_ += velocity_;

	// 着地フラグ
	bool landing = false;

	// 地面との当たり判定
	// 下降中?
	if (velocity_.y < 0) {
		// Y座標が地面以下になったら着地
		if (worldTransform_.translation_.y <= kGroundHeight) {
			landing = true;
		}
	}

	// 接地判定
	if (onGround_) {
		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			// 空中状態に移行
			onGround_ = false;
		}
	} else {
		// 着地
		if (landing) {
			// めり込み排斥
			worldTransform_.translation_.y = kGroundHeight;
			// 摩擦で横方向速度が減衰する
			velocity_.x *= (1.0f - kAttenuation);
			// 下方向速度をリセット
			velocity_.y = 0.0f;
			// 接地状態に移行
			onGround_ = true;
			// ジャンプ回数をリセット
			jumpCount_ = 0;
		}
	}

	// 行列を定数バッファに転送
	WorldMatrix(worldTransform_);
}

void Player::Draw() {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, *camera_);
}