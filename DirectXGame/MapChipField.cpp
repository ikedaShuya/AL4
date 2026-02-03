#include "MapChipField.h"
#include <fstream>
#include <map>
#include <sstream>
#include <cassert>

using namespace KamataEngine;

namespace {
std::map<char, MapChipType> mapChipTypeTable = {
    {'0', MapChipType::kBlank },
    {'B', MapChipType::kBlock},
    {'P', MapChipType::kPlayer},
    {'E', MapChipType::kEnemy},
};
}

void MapChipField::ResetMapChipData() {
	// マップチップデータをリセット
	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVirtical);
	for (std::vector<MapChipDataUnit>& mapChipDataLine : mapChipData_.data) {
		mapChipDataLine.resize(kNumBlockHorizontal);
	}
}

void MapChipField::LoadMapChipCsv(const std::string& filePath) {

	// マップチップデータをリセット
	ResetMapChipData();

	// ファイルを開く
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

	// マップチップCSV
	std::stringstream mapChipCsv;
	// ファイルの内容を文字列ストリームにコピー
	mapChipCsv << file.rdbuf();
	// ファイルを閉じる
	file.close();

	// CSVからマップチップデータを読み込む
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		std::string line;
		getline(mapChipCsv, line);

		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {

			std::string word;
			getline(line_stream, word, ',');

			// 空白の場合はスキップ
			if (word.empty()) {
				continue;
			}

			// 先頭文字がいずれかのマップチップ種別に該当するか確認
			if (!mapChipTypeTable.contains(word[kChipType])) {
				continue;
			}

			// 先頭文字でマップチップのタイプを判別
			mapChipData_.data[i][j].type = mapChipTypeTable[word[kChipType]];

			// サブIDを含まない場合はスキップ（0番で確定)
			if (word.size() <= kChipSubID) {
				continue;
			}

			// マップチップのサブIDを設定
			mapChipData_.data[i][j].subID = static_cast<uint8_t>(word[kChipSubID] - '0');
		}
	}
}

MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) {

	if (xIndex >= kNumBlockHorizontal) {
		return MapChipType::kBlank;
	}
	if (yIndex >= kNumBlockVirtical) {
		return MapChipType::kBlank;
	}

	return mapChipData_.data[yIndex][xIndex].type;
}

Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) { return Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlockVirtical - 1 - yIndex), 0); }

MapChipField::IndexSet MapChipField::GetMapChipIndexSetByPosition(const Vector3& position) {

	IndexSet indexSet = {};
	indexSet.xIndex = static_cast<uint32_t>((position.x + kBlockWidth / 2.0f) / kBlockWidth);
	indexSet.yIndex = kNumBlockVirtical - 1 - static_cast<uint32_t>(position.y + kBlockHeight / 2.0f / kBlockHeight);
	return indexSet;
}

MapChipField::Rect MapChipField::GetRectByIndex(uint32_t xIndex, uint32_t yIndex) {

	// 指定ブロックの中心座標を取得する
	Vector3 center = GetMapChipPositionByIndex(xIndex, yIndex);

	Rect rect{};
	rect.left = center.x - kBlockWidth / 2.0f;
	rect.right = center.x + kBlockWidth / 2.0f;
	rect.bottom = center.y - kBlockWidth / 2.0f;
	rect.top = center.y + kBlockWidth / 2.0f;

	return rect;
}

uint8_t MapChipField::GetMapChipSubIDByIndex(uint32_t xIndex, uint32_t yIndex) {

	// 横方向の範囲チェック
	if (xIndex >= kNumBlockHorizontal) {
		return 0;
	}

	// 縦方向の範囲チェック
	if (yIndex >= kNumBlockVirtical) {
		return 0;
	}

	// 指定マスのサブIDを返す
	return mapChipData_.data[yIndex][xIndex].subID;
}