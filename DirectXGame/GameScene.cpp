#include "GameScene.h"

using namespace KamataEngine;

void GameScene::Initialize() {}

void GameScene::Update() {

	if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
		finished_ = true;
	}

}

void GameScene::Draw() {

	// 3Dモデル描画前処理
	Model::PreDraw();

	// 3Dモデル描画後処理
	Model::PostDraw();

}