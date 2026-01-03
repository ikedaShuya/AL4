#include "GameScene.h"

using namespace KamataEngine;

void GameScene::Initialize() {

	// カメラの初期化
	camera_.Initialize();

	// 3Dモデルデータの生成
	modelPlayer_ = Model::CreateFromOBJ("player", true);

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(modelPlayer_, &camera_);
}

void GameScene::Update() {

	if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
		finished_ = true;
	}

	player_->Update();

}

void GameScene::Draw() {

	// 3Dモデル描画前処理
	Model::PreDraw();

	// 自キャラの描画
	player_->Draw();

	// 3Dモデル描画後処理
	Model::PostDraw();

}

GameScene::~GameScene() {

	// モデルデータの解放
	delete modelPlayer_;

	// 自キャラの解放
	delete player_;

}