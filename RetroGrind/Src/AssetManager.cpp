#include "AssetManager.h"

AssetManager::AssetManager() {}
AssetManager::~AssetManager() {
	ReleaseAll();	//クラス破壊時全モデル削除
}

int AssetManager::LoadModel(const std::string& key, const std::string& path) {
    // 既に読み込み済みのキーがあれば、新しく読み込まずに既存のハンドルを返す
    if (models.find(key) != models.end()) {
        return models[key];
    }

    // 新規読み込み
    int handle = MV1LoadModel(path.c_str());
    if (handle != -1) {
        models[key] = handle; // マップに登録
    }
    return handle;
}

int AssetManager::GetModel(const std::string& key) {
    if (models.find(key) != models.end()) {
        return models[key];
    }
    return -1; // 見つからなかった場合はエラー値(-1)を返す
}

void AssetManager::ReleaseAll() {
    for (auto& pair : models) {
        MV1DeleteModel(pair.second);
    }
    models.clear();
}