#include "GameOverScene.h"

using namespace KamataEngine;

void GameOverScene::Initialize() {
	modelTitle_ = Model::CreateFromOBJ("gameOver", true);

	camera_.Initialize();

	worldTransformTitle_.Initialize();
	worldTransformTitle_.scale_ = {5.0f, 5.0f, 5.0f};
}

void GameOverScene::Update() {
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {

		finished_ = true;
	}

	WorldTransformUpdate(worldTransformTitle_);
}

void GameOverScene::Draw() {

	Model::PreDraw();

	modelTitle_->Draw(worldTransformTitle_, camera_);

	Model::PostDraw();
}

GameOverScene::~GameOverScene() { delete modelTitle_; }