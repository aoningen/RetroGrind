#include "Stage.h"

Stage::Stage() : stageHandle(-1), diskHandle(-1), diskRotationAngle(0.0f) {}

Stage::~Stage() {}

bool Stage::Initialize(AssetManager& assetManager) {
    // AssetManager経由でモデルを読み込む（キー名を付けて管理）
    stageHandle = assetManager.LoadModel("Stage", "Assets/Models/RecordPlayer.mv1");
    diskHandle = assetManager.LoadModel("Disk", "Assets/Models/RecordDisk.mv1");

    if (stageHandle == -1 || diskHandle == -1) {
        return false;
    }

    // 初期配置とスケール設定（Zファイティング対策でYを0.1f浮かせる）
    MV1SetPosition(stageHandle, VGet(0.0f, 0.0f, 0.0f));
    MV1SetPosition(diskHandle, VGet(0.0f, 0.1f, 0.0f));

    MV1SetScale(stageHandle, VGet(0.2f, 0.2f, 0.2f));
    MV1SetScale(diskHandle, VGet(0.2f, 0.2f, 0.2f));

    return true;
}

void Stage::Update() {
    // レコード盤を回転させる
    diskRotationAngle += 0.02f;
    MV1SetRotationXYZ(diskHandle, VGet(0.0f, diskRotationAngle, 0.0f));
}

void Stage::Draw() {
    // モデルの描画
    MV1DrawModel(stageHandle);
    MV1DrawModel(diskHandle);
}