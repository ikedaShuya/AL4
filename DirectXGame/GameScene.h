#pragma once
#include "KamataEngine.h"
#include "MapChipField.h"

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
};