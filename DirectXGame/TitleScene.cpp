#include "TitleScene.h"

#include "Math.h"

using namespace KamataEngine;

void TitleScene::Initialize() {

	modelTitle_ = Model::CreateFromOBJ("title", true);

	// カメラ初期化
	camera_.Initialize();

	worldTransformTitle_.Initialize();
	worldTransformTitle_.scale_ = {5.0f, 5.0f, 5.0f};
}

void TitleScene::Update() {

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		finished_ = true;
	}

	WorldTransformUpdate(worldTransformTitle_);

}

void TitleScene::Draw() {

	// 3Dモデル描画前処理
	Model::PreDraw();

	modelTitle_->Draw(worldTransformTitle_, camera_);

	// 3Dモデル描画後処理
	Model::PostDraw();
}

TitleScene::~TitleScene() { delete modelTitle_; }
