#pragma once
#include "AssetManager.h"

class Stage {
private:
    int stageHandle;
    int diskHandle;
    float diskRotationAngle;

public:
    Stage();
    ~Stage();

    // 初期化（AssetManagerを受け取りモデルをセットする）
    bool Initialize(AssetManager& assetManager);

    // 更新処理（レコード盤の回転）
    void Update();

    // 描画処理
    void Draw();
};