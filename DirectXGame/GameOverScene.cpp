#include "GameOverScene.h"

using namespace KamataEngine;

void GameOverScene::Initialize() {
	modelTitle_ = Model::CreateFromOBJ("gameOver", true);

	camera_.Initialize();

	worldTransformTitle_.Initialize();
	worldTransformTitle_.scale_ = {5.0f, 5.0f, 5.0f};

	modelSkyDome_ = Model::CreateFromOBJ("sky", true);
	skyDome_ = new SkyDome();
	skyDome_->Initialize(modelSkyDome_, &camera_);
}

void GameOverScene::Update() {
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {

		finished_ = true;
	}

	skyDome_->Update();

	WorldTransformUpdate(worldTransformTitle_);
}

void GameOverScene::Draw() {

	Model::PreDraw();

	modelTitle_->Draw(worldTransformTitle_, camera_);

	skyDome_->Draw();

	Model::PostDraw();
}

GameOverScene::~GameOverScene() { 
	delete modelTitle_;
	delete skyDome_;
	delete modelSkyDome_;
}