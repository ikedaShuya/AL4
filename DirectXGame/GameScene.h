#pragma once
#include "CameraController.h"
#include "KamataEngine.h"
#include "Player.h"

#include "Enemy.h"
#include "MapChipField.h"
#include "SkyDome.h"

enum class GameResult { None, Clear, GameOver };

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {
public:
	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	~GameScene();

	// デスフラグのgetter
	bool IsFinished() const { return finished_; }

	void GenerateFieldObjects();

	// 全ての当たり判定を行う
	void CheckAllCollisions();

	// リロード要求フラグのgetter
	bool GetReloadRequested() const { return reloadRequested_; }

private:
	// 終了フラグ
	bool finished_ = false;

	// カメラ
	KamataEngine::Camera camera_;

	// ブロックモデルデータ
	KamataEngine::Model* modelBlock_ = nullptr;

	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

	// マップチップフィールド
	MapChipField* mapChipField_ = nullptr;

	// 自キャラ
	Player* player_ = nullptr;

	// 3Dモデルデータ
	KamataEngine::Model* modelPlayer_ = nullptr;

	std::list<Enemy*> enemies_;

	KamataEngine::Model* modelEnemy_ = nullptr;

	CameraController* CController_ = nullptr;

	KamataEngine::Model* swordModel_ = nullptr;

	KamataEngine::Sprite* hpBarBg_ = nullptr;
	KamataEngine::Sprite* hpBarFg_ = nullptr;

	uint32_t hpBarBgTex_ = 0;
	uint32_t hpBarFgTex_ = 0;

	SkyDome* skyDome_ = nullptr;

	// 3Dモデル
	KamataEngine::Model* modelSkyDome_ = nullptr;

	GameResult result_ = GameResult::None;	

	// リロード要求フラグ
	bool reloadRequested_ = false;
};