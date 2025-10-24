#include "GameScene.h"

void GameScene::Initialize() {

#ifdef _DEBUG
	// デバックカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
#endif

	// 3Dモデルデータの生成
	modelPlayer_ = Model::CreateFromOBJ("cube", true);

	// カメラの初期化
	camera_.Initialize();

	// 自キャラの生成
	player_ = new Player();
	Vector3 playerPosition = {0, 0, 0.0f};
	// 自キャラの初期化
	player_->Initialize(modelPlayer_, &camera_, playerPosition);
}

void GameScene::Update() {

#ifdef _DEBUG

	// デバックカメラの更新
	debugCamera_->Update();

	if (Input::GetInstance()->TriggerKey(DIK_F1)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}

	// カメラの処理
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		// ビュープロジェクション行列の転送
		camera_.TransferMatrix();
	} else {
		// ビュープロジェクション行列の更新と転送
		camera_.UpdateMatrix();
	}

#endif

	// 自キャラの更新
	player_->Update();
}

void GameScene::Draw() {

	// 3Dモデル描画前処理
	Model::PreDraw();

	// 自キャラの描画
	player_->Draw();

	// 3Dモデル描画後処理
	Model::PostDraw();
}

GameScene::~GameScene() {

#ifdef _DEBUG

	delete debugCamera_;

#endif

	// モデルデータの解放
	delete modelPlayer_;

	// 自キャラの解放
	delete player_;
}