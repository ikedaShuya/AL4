#include "GameScene.h"

using namespace KamataEngine;

void GameScene::Initialize() {}

void GameScene::Update() {

	if (Input::GetInstance()->PushKey(DIK_RETURN)) {
		finished_ = true;
	}

}

void GameScene::Draw() {}