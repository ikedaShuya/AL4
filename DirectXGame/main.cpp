#include "ClearScene.h"
#include "GameOverScene.h"
#include "GameScene.h"
#include "KamataEngine.h"
#include "TitleScene.h"
#include <Windows.h>

using namespace KamataEngine;

GameScene* gameScene = nullptr;
TitleScene* titleScene = nullptr;
ClearScene* clearScene = nullptr;
GameOverScene* gameOverScene = nullptr;

// シーン（型）
enum class Scene {
	kUnknown = 0,
	kTitle,
	kGame,
	kClear,
	kGameOver,
};

// 現在シーン（型）
Scene scene = Scene::kUnknown;

static void ChangeScene() {
	switch (scene) {
	case Scene::kTitle:
		if (titleScene->IsFinished()) {
			// シーン変更
			scene = Scene::kGame;
			// 旧シーンの解放
			delete titleScene;
			titleScene = nullptr;
			// 新シーンの生成と初期化
			gameScene = new GameScene;
			gameScene->Initialize();
		}
		break;
	case Scene::kGame:
		if (gameScene->IsFinished()) {

			if (gameScene->GetResult() == GameResult::Clear) {

				// シーン変更
				scene = Scene::kClear;
				// 旧シーンの解放
				delete gameScene;
				gameScene = nullptr;
				// 新シーンの生成と初期化
				clearScene = new ClearScene;
				clearScene->Initialize();
			} else if (gameScene->GetResult() == GameResult::GameOver) {

				// シーン変更
				scene = Scene::kGameOver;
				// 旧シーンの解放
				delete gameScene;
				gameScene = nullptr;
				// 新シーンの生成と初期化
				gameOverScene = new GameOverScene;
				gameOverScene->Initialize();
			}
		}
		break;
	case Scene::kClear:
		if (clearScene->IsFinished()) {

			// シーン変化
			scene = Scene::kTitle;
			// 旧シーンの解放
			delete clearScene;
			clearScene = nullptr;
			// 新シーンの生成と初期化
			titleScene = new TitleScene;
			titleScene->Initialize();
		}
		break;
	case Scene::kGameOver:
		if (gameOverScene->IsFinished()) {

			// シーン変化
			scene = Scene::kTitle;
			// 旧シーンの解放
			delete gameOverScene;
			gameOverScene = nullptr;
			// 新シーンの生成と初期化
			titleScene = new TitleScene;
			titleScene->Initialize();
		}
	}
}

static void UpdateScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Update();
		break;
	case Scene::kGame:
		gameScene->Update();
		break;
	case Scene::kClear:
		clearScene->Update();
		break;
	case Scene::kGameOver:
		gameOverScene->Update();
		break;
	}
}

static void DrawScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Draw();
		break;
	case Scene::kGame:
		gameScene->Draw();
		break;
	case Scene::kClear:
		clearScene->Draw();
		break;
	case Scene::kGameOver:
		gameOverScene->Draw();
		break;
	}
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// エンジンの初期化
	Initialize(L"LE2C_04_イケダ_シュウヤ_エアリーナイト");

	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	ImGuiManager* imGuiManager = ImGuiManager::GetInstance();

	// 最初のシーンの初期化
	scene = Scene::kTitle;
	titleScene = new TitleScene;
	titleScene->Initialize();

	// メインループ
	while (true) {
		// エンジンの更新
		if (Update()) {
			break;
		}

		imGuiManager->Begin();

		// シーン切り替え
		ChangeScene();
		// 現在シーン更新
		UpdateScene();

		imGuiManager->End();

		// 描画開始
		dxCommon->PreDraw();

		// 現在シーンの描画
		DrawScene();

		imGuiManager->Draw();

		// 描画終了
		dxCommon->PostDraw();
	}

	delete titleScene;
	delete gameScene;

	// エンジンの終了処理
	Finalize();

	return 0;
}