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

	// 3Dモデルデータの生成
	modelPlayer_ = Model::CreateFromOBJ("player", true);

	swordModel_ = Model::CreateFromOBJ("sword", true);

	modelEnemy_ = Model::CreateFromOBJ("enemy", true);

	GenerateFieldObjects();

	// カメラコントローラの初期化
	CController_ = new CameraController();
	CController_->Initialize(&camera_);
	CController_->SetTarget(player_);
	CController_->Reset();

	// 移動範囲の指定
	CameraController::Rect cameraArea = {10.556f, 100 - 12.0f, 11.0f, 13.3f};
	CController_->SetMovableArea(cameraArea);

	hpBarBgTex_ = TextureManager::Load("hp_bar_bg.png");
	hpBarFgTex_ = TextureManager::Load("hp_bar_fg.png");

	// 画像サイズそのまま
	hpBarBg_ = Sprite::Create(hpBarBgTex_, {100, 10});
	hpBarFg_ = Sprite::Create(hpBarFgTex_, {100, 10});

	// 表示位置（左上）
	hpBarBg_->SetPosition({120, 20});
	hpBarFg_->SetPosition({120, 20});

	hpBarBg_->SetAnchorPoint({0.0f, 0.0f});
	hpBarFg_->SetAnchorPoint({0.0f, 0.0f});

	modelSkyDome_ = Model::CreateFromOBJ("sky", true);
	skyDome_ = new SkyDome();                    
	skyDome_->Initialize(modelSkyDome_, &camera_);
}

void GameScene::Update() {

#ifdef _DEBUG
	ImGui::Begin("Debug Menu");
	// リロードボタン
	if (ImGui::Button("Reload")) {
		reloadRequested_ = true;
	}
	ImGui::End();
#endif

	// プレイヤー死亡判定
	if (player_->GetHp() <= 0) {
		result_ = GameResult::GameOver;
		finished_ = true;
		return;
	}

	// 敵全滅判定
	if (enemies_.empty()) {
		result_ = GameResult::Clear;
		finished_ = true;
		return;
	}

	skyDome_->Update();

	float hpRate = static_cast<float>(player_->GetHp()) / static_cast<float>(player_->GetMaxHp());

	// 横だけ縮める（高さは固定10）
	hpBarFg_->SetSize({100.0f * hpRate, 10.0f});

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

	skyDome_->Draw();

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

	// スプライト描画前処理
	Sprite::PreDraw();

	// HPバー描画
	hpBarFg_->Draw();
	hpBarBg_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
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

	delete hpBarBg_;
	delete hpBarFg_;

	delete skyDome_;
	delete modelSkyDome_;
}

void GameScene::GenerateFieldObjects() {

	// マップサイズ取得
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// ブロック用配列確保
	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t y = 0; y < numBlockVirtical; ++y) {
		worldTransformBlocks_[y].resize(numBlockHorizontal, nullptr);
	}

	// マップ走査
	for (uint32_t y = 0; y < numBlockVirtical; ++y) {
		for (uint32_t x = 0; x < numBlockHorizontal; ++x) {

			MapChipType type = mapChipField_->GetMapChipTypeByIndex(x, y);
			uint8_t subID = mapChipField_->GetMapChipSubIDByIndex(x, y);

			switch (type) {
			case MapChipType::kBlock:
				switch (subID) {
				case 0:
					break;
				default: {
					WorldTransform* worldTransform = new WorldTransform();
					worldTransform->Initialize();
					worldTransformBlocks_[y][x] = worldTransform;
					worldTransform->translation_ = mapChipField_->GetMapChipPositionByIndex(x, y);
				} break;
				}
				break;

			case MapChipType::kPlayer:
				assert(player_ == nullptr && "自キャラを二重に配置しようとしています");
				player_ = new Player();
				Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(x, y);
				player_->Initialize(modelPlayer_, swordModel_, &camera_, playerPosition);
				player_->SetMapChipField(mapChipField_);

				break;

			case MapChipType::kEnemy:
				switch (subID) {
				case 0:
					// 敵なし
					break;
				case 1: {
					// 敵生成
					Enemy* enemy = new Enemy();
					Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(x, y);
					enemy->Initialize(modelEnemy_, &camera_, enemyPosition);
					enemies_.push_back(enemy);
				} break;
				default:
					// 未定義のサブIDは無視
					break;
				}
				break;

			default:
				break;
			}
		}
	}
}

void GameScene::CheckAllCollisions() {

#pragma region 自キャラと敵キャラの当たり判定
	{ // 判定対象1と2の座標
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