#include "Skydome.h"
#include "Math.h"

using namespace KamataEngine;

void Skydome::Initialize(Model* model, Camera* camera) {

	// NULLポインタチェック
	assert(model);
	assert(camera);

	model_ = model;
	camera_ = camera;
	worldTransform_.Initialize();
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
}

void Skydome::Update() {

	// ワールド変換行列を定数バッファに転送
	WorldTransformUpdate(worldTransform_);
}

void Skydome::Draw() {

	// 3Dモデル描画
	model_->Draw(worldTransform_, *camera_);
}