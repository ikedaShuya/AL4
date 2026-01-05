#include "Enemy.h"
#include "Player.h"
#include <numbers>

using namespace KamataEngine;

void Enemy::Initialize(Model* model, Camera* camera, const Vector3& position) {

	// NULLポインタチェック
	assert(model);
	assert(camera);

	model_ = model;
	camera_ = camera;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;

	// 速度を設定する
	velocity_ = {-kWalkSpeed, 0, 0};

	walkTimer_ = 0.0f;
}

void Enemy::Update(const Player* player) {

	if (isDead_) {
		return;
	}

	// 無敵時間カウント
	if (invincibleTimer_ > 0) {
		invincibleTimer_--;
	}

	// 状態時間更新
	stateTimer_ += 1.0f / 60.0f;

	switch (state_) {
	case EnemyState::Moving:
		// 移動処理
		{
			Vector3 totalVelocity = velocity_ + knockbackVelocity_;
			worldTransform_.translation_ += totalVelocity;

			// ノックバック減衰
			knockbackVelocity_.x *= kKnockbackAttenuation;

			// 移動時間終了したら待機へ
			if (stateTimer_ >= moveDuration_) {
				state_ = EnemyState::Waiting;
				stateTimer_ = 0.0f;
			}
		}
		break;

	case EnemyState::Waiting:
		// 停止して動かない
		// ノックバックは残す場合ここで処理（省略可）

		// 待機時間終了したら移動へ戻る
		if (stateTimer_ >= waitDuration_) {
			state_ = EnemyState::Moving;
			stateTimer_ = 0.0f;
		}
		break;
	}

	// 首フリフリ等の処理（移動中だけ首フリフリしたい場合は条件を追加）
	if (state_ == EnemyState::Moving && std::abs(knockbackVelocity_.x) < 0.01f) {
		const float t = stateTimer_;
		const float baseAngle = -1.5f;
		const float kNeckRollAngle = 1.0f;
		const float kNeckRollSpeed = 4.0f;
		const float kNeckNodAngle = 0.3f;
		const float kNeckNodSpeed = 2.0f;
		const float kBodySwayAmount = 0.01f;
		const float kBodySwaySpeed = 0.8f;

		worldTransform_.rotation_.y = baseAngle + std::sin(t * kNeckRollSpeed) * kNeckRollAngle;
		worldTransform_.rotation_.x = std::sin(t * kNeckNodSpeed) * kNeckNodAngle;
		worldTransform_.translation_.x += std::sin(t * kBodySwaySpeed) * kBodySwayAmount;
	} else if (std::abs(knockbackVelocity_.x) >= 0.01f) {
		// ノックバック時は向き固定
		const float baseAngle = -1.5f;
		worldTransform_.rotation_.y = baseAngle + std::numbers::pi_v<float>;
		worldTransform_.rotation_.x = 0.0f;
	} else {
		// 停止中に首角度を0にスムーズに戻す処理
		const float returnSpeed = 0.12f; // 補間速度（0〜1で調整）
		worldTransform_.rotation_.y = Lerp(worldTransform_.rotation_.y, -1.5f, returnSpeed);
		worldTransform_.rotation_.x = Lerp(worldTransform_.rotation_.x, 0.0f, returnSpeed);
		// 体の揺れもリセットしたい場合は同様に補間してください
	}

	CheckHitByPlayerSword(player);

	WorldTransformUpdate(worldTransform_);
}

void Enemy::Draw() { model_->Draw(worldTransform_, *camera_); }

KamataEngine::Vector3 Enemy::GetWorldPosition() {

	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

AABB Enemy::GetAABB() {
	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

void Enemy::OnCollision(const Player* player) { (void)player; }

void Enemy::CheckHitByPlayerSword(const Player* player) {
	if (isDead_ || invincibleTimer_ > 0) {
		return;
	}

	if (!player->IsAttacking()) {
		return;
	}

	AABB enemyAABB = GetAABB();
	AABB swordAABB = player->GetSwordAABB();

	// AABB同士の当たり判定
	bool hit = (enemyAABB.min.x <= swordAABB.max.x && enemyAABB.max.x >= swordAABB.min.x) && (enemyAABB.min.y <= swordAABB.max.y && enemyAABB.max.y >= swordAABB.min.y) &&
	           (enemyAABB.min.z <= swordAABB.max.z && enemyAABB.max.z >= swordAABB.min.z);

	if (hit) {
		OnHit(player);
	}
}

void Enemy::OnHit(const Player* player) {

	if (isDead_ || invincibleTimer_ > 0) {
		return;
	}

	hp_--;

	// Player::LRDirection を明示する
	float dir = (player->GetLRDirection() == Player::LRDirection::kRight) ? 1.0f : -1.0f;

	knockbackVelocity_.x = dir * kKnockbackPower;

	invincibleTimer_ = kInvincibleFrame;

	if (hp_ <= 0) {
		isDead_ = true;
	}
}