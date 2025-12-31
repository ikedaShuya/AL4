#include "TitleScene.h"

using namespace KamataEngine;

void TitleScene::Initialize() {}

void TitleScene::Update() {

	if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
		finished_ = true;
	}

}

void TitleScene::Draw() {}

TitleScene::~TitleScene() {}
