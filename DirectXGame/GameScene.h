#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include "CameraController.h"

#include "MapChipField.h"
#include "Enemy.h"

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

	void GenerateBlocks();

	// 全ての当たり判定を行う
	void CheckAllCollisions();

private:

	// 終了フラグ
	bool finished_ = false;

	// カメラ
	KamataEngine::Camera camera_;


	// ブロックモデルデータ
	KamataEngine::Model* modelBlock_ = nullptr;

	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

	// マップチップフィールド
	MapChipField* mapChipField_;

	// 自キャラ
	Player* player_ = nullptr;

	// 3Dモデルデータ
	KamataEngine::Model* modelPlayer_ = nullptr;

	std::list<Enemy*> enemies_;

	KamataEngine::Model* modelEnemy_ = nullptr;

	CameraController* CController_ = nullptr;

	KamataEngine::Model* swordModel_;

	KamataEngine::Sprite* hpBarBg_ = nullptr;
	KamataEngine::Sprite* hpBarFg_ = nullptr;

	uint32_t hpBarBgTex_ = 0;
	uint32_t hpBarFgTex_ = 0;
};