#pragma once
#include <string>
#include <unordered_map>
#include "DxLib.h"

class AssetManager {
private:
	std::unordered_map<std::string, int>models;

public:
	AssetManager();
	~AssetManager();

	//モデルを読み込み、読み込み済みのものはそのまま返す
	int LoadModel(const std::string& key, const std::string& path);

	//読み込み済みのモデルハンドルを取得する
	int GetModel(const std::string& key);

	//全メモリの解放
	void ReleaseAll();
};