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

	// 3Dモデル描画後処理
	Model::PostDraw();

}

GameScene::~GameScene() {

	// マップチップフィールドの解放
	delete mapChipField_;
}

void GameScene::GenerateBlocks() {

	// マップサイズ取得
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 配列サイズ確保
	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t y = 0; y < numBlockVirtical; ++y) {
		worldTransformBlocks_[y].resize(numBlockHorizontal, nullptr);
	}

	// ブロック生成
	for (uint32_t y = 0; y < numBlockVirtical; ++y) {
		for (uint32_t x = 0; x < numBlockHorizontal; ++x) {

			// マップチップ情報取得
			MapChipType type = mapChipField_->GetMapChipTypeByIndex(x, y);
			uint8_t subID = mapChipField_->GetMapChipSubIDByIndex(x, y);

			// ブロック以外は生成しない
			if (type != MapChipType::kBlock) {
				continue;
			}

			// subID が 0 は「何も置かない」扱い
			if (subID == 0) {
				continue;
			}

			// ブロック生成
			WorldTransform* worldTransform = new WorldTransform();
			worldTransform->Initialize();
			worldTransform->translation_ = mapChipField_->GetMapChipPositionByIndex(x, y);

			worldTransformBlocks_[y][x] = worldTransform;
		}
	}
}