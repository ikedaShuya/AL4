#include "ClearScene.h"

using namespace KamataEngine;

void ClearScene::Initialize() {
	modelTitle_ = Model::CreateFromOBJ("clear", true);

	camera_.Initialize();

	worldTransformTitle_.Initialize();
	worldTransformTitle_.scale_ = {5.0f, 5.0f, 5.0f};
}

void ClearScene::Update() {

	if (Input::GetInstance()->PushKey(DIK_SPACE)) {

		finished_ = true;
	}

	WorldTransformUpdate(worldTransformTitle_);
}

void ClearScene::Draw() {

	Model::PreDraw();

	modelTitle_->Draw(worldTransformTitle_, camera_);

	Model::PostDraw();
}

ClearScene::~ClearScene() { delete modelTitle_; }