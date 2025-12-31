#include "TitleScene.h"

using namespace KamataEngine;

void TitleScene::Initialize() {}

void TitleScene::Update() {

	if (Input::GetInstance()->PushKey(DIK_RETURN)) {
		finished_ = true;
	}

}

void TitleScene::Draw() {}

TitleScene::~TitleScene() {}
