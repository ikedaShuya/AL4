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

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(modelPlayer_, &camera_);

	modelEnemy_ = Model::CreateFromOBJ("enemy", true);

	for (int32_t i = 0; i < 2; ++i) {
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(14 + i * 2, 18);
		newEnemy->Initialize(modelEnemy_, &camera_, enemyPosition);
		enemies_.push_back(newEnemy);
	}

}

void GameScene::Update() {

	if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
		finished_ = true;
	}

	// ブロックの更新
	for (std::vector<WorldTransform*>& worldTransformBlcokLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlcokLine) {
			if (!worldTransformBlock)
				continue;

			WorldTransformUpdate(*worldTransformBlock);
		}
	}

	for (Enemy* enemy_ : enemies_) {
		enemy_->Update();
	}

	player_->Update();

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