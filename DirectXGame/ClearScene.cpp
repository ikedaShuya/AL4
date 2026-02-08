#include "ClearScene.h"

using namespace KamataEngine;

void ClearScene::Initialize() {
	modelTitle_ = Model::CreateFromOBJ("clear", true);

	camera_.Initialize();

	worldTransformTitle_.Initialize();
	worldTransformTitle_.scale_ = {5.0f, 5.0f, 5.0f};

	modelSkyDome_ = Model::CreateFromOBJ("sky", true);
	skyDome_ = new SkyDome();
	skyDome_->Initialize(modelSkyDome_, &camera_);
}

void ClearScene::Update() {

	if (Input::GetInstance()->PushKey(DIK_SPACE)) {

		finished_ = true;
	}

	skyDome_->Update();

	WorldTransformUpdate(worldTransformTitle_);
}

void ClearScene::Draw() {

	Model::PreDraw();

	skyDome_->Draw();

	modelTitle_->Draw(worldTransformTitle_, camera_);

	Model::PostDraw();
}

ClearScene::~ClearScene() { 
	delete modelTitle_;
	delete skyDome_;
	delete modelSkyDome_;
}