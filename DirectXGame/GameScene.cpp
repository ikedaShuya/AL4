#include "GameScene.h"
#include "Math.h"

using namespace KamataEngine;

void GameScene::Initialize() {

	// カメラの初期化
	camera_.Initialize();

	// ブロックモデルデータの生成
	modelBlock_ = Model::CreateFromOBJ("cube", true);

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	GenerateBlocks();

	// 3Dモデルデータの生成
	modelPlayer_ = Model::CreateFromOBJ("player", true);

	swordModel_ = Model::CreateFromOBJ("sword", true);

	// 自キャラの生成
	player_ = new Player();
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1, 18);
	// 自キャラの初期化
	player_->Initialize(modelPlayer_, swordModel_, &camera_, playerPosition);

	player_->SetMapChipField(mapChipField_);
	modelEnemy_ = Model::CreateFromOBJ("enemy", true);

	 // 敵の数
	const int enemyCount = 5;

	// 敵の配置する行は地面の上（y=17）
	const int enemyY = 18;

	// 敵のx座標候補リストを作成（0〜99のうち間隔を空けて5箇所）
	// ここでは単純に均等に配置する例
	std::vector<int> enemyXs;
	int spacing = 100 / enemyCount;
	for (int i = 0; i < enemyCount; ++i) {
		enemyXs.push_back(spacing / 2 + i * spacing);
	}

	// 敵を配置
	for (int i = 0; i < enemyCount; ++i) {
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(enemyXs[i], enemyY);
		newEnemy->Initialize(modelEnemy_, &camera_, enemyPosition);
		enemies_.push_back(newEnemy);
	}

	// カメラコントローラの初期化
	CController_ = new CameraController();
	CController_->Initialize(&camera_);
	CController_->SetTarget(player_);
	CController_->Reset();

	// 移動範囲の指定
	CameraController::Rect cameraArea = {10.156f, 100 - 12.0f, 6.0f, 6.0f};
	CController_->SetMovableArea(cameraArea);
}

void GameScene::Update() {

	if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
		finished_ = true;
	}

	// デスフラグの立った敵を削除
	enemies_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	CController_->Update();

	// ブロックの更新
	for (std::vector<WorldTransform*>& worldTransformBlcokLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlcokLine) {
			if (!worldTransformBlock)
				continue;

			WorldTransformUpdate(*worldTransformBlock);
		}
	}

	for (Enemy* enemy_ : enemies_) {
		enemy_->Update(player_);
	}

	player_->Update();

	// 全ての当たり判定を行う
	CheckAllCollisions();
}

void GameScene::Draw() {

	// 3Dモデル描画前処理
	Model::PreDraw();

	// ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlcokLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlcokLine) {
			if (!worldTransformBlock)
				continue;

			modelBlock_->Draw(*worldTransformBlock, camera_);
		}
	}

	// 自キャラの描画
	player_->Draw();

	for (Enemy* enemy_ : enemies_) {
		enemy_->Draw();
	}

	// 3Dモデル描画後処理
	Model::PostDraw();
}

GameScene::~GameScene() {

	// マップチップフィールドの解放
	delete mapChipField_;

	delete modelBlock_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	// モデルデータの解放
	delete modelPlayer_;

	// 自キャラの解放
	delete player_;

	delete modelEnemy_;

	for (Enemy* enemy_ : enemies_) {
		delete enemy_;
	}
}

void GameScene::GenerateBlocks() {

	// 要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更する
	// 列数を設定（縦方向のブロック数)
	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		// 1列の要素数を設定（横方向のブロック数)
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	// ブロックの生成
	// キューブの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {

				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::CheckAllCollisions(){

#pragma region 自キャラと敵キャラの当たり判定
    {// 判定対象1と2の座標
     AABB aabb1, aabb2;

// 自キャラの座標
aabb1 = player_->GetAABB();

// 自キャラと敵弾全ての当たり判定
for (Enemy* enemy_ : enemies_) {
	// 敵弾の座標
	aabb2 = enemy_->GetAABB();

	// AABB同士の交差判定
	if (IsCollision(aabb1, aabb2)) {
		// 自キャラの衝突時関数を呼び出す
		player_->OnCollision(enemy_);
		// 敵の衝突時関数を呼び出す
		enemy_->OnCollision(player_);
	}
}
}
#pragma endregion

#pragma region 近接攻撃と敵キャラの当たり判定（攻撃）
{
	// 攻撃中のみ判定する
	if (!player_->IsAttacking()) {
		return;
	}

	AABB attackAABB = player_->GetSwordAABB();

	for (Enemy* enemy_ : enemies_) {
		AABB enemyAABB = enemy_->GetAABB();

		if (IsCollision(attackAABB, enemyAABB)) {
			enemy_->OnHit(player_); // ダメージ処理
		}
	}
}
#pragma endregion
}