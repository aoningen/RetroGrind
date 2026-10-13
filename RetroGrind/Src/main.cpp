#include "DxLib.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // 画面モード設定
    ChangeWindowMode(TRUE);
    SetGraphMode(1280, 720, 32);

    if (DxLib_Init() == -1) return -1;

    SetDrawScreen(DX_SCREEN_BACK);

    // 3Dモデルの読み込み (.mv1 または .fbx)
    int stageHandle = MV1LoadModel("Stage.mv1");
    int diskHandle = MV1LoadModel("Disk.mv1");

    // カメラの設定（レコードプレーヤー全体を見下ろす位置）
    SetCameraPositionAndTarget_UpVecY(
        VGet(0.0f, 150.0f, -200.0f), // カメラの位置
        VGet(0.0f, 0.0f, 0.0f)        // 注視点（中心）
    );

    float diskRotationAngle = 0.0f;

    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        ClearDrawScreen();

        // --------------------------------------------------
        // 更新処理：レコード盤を回転させる
        // --------------------------------------------------
        diskRotationAngle += 0.02f; // 回転速度
        MV1SetRotationXYZ(diskHandle, VGet(0.0f, diskRotationAngle, 0.0f));

        // --------------------------------------------------
        // 描画処理
        // --------------------------------------------------
        // 1. 土台・アーム（Stage）の描画
        MV1DrawModel(stageHandle);

        // 2. レコード盤（Disk）の描画（回転状態）
        MV1DrawModel(diskHandle);

        ScreenFlip();
    }

    // モデルハンドルの削除
    MV1DeleteModel(stageHandle);
    MV1DeleteModel(diskHandle);

    DxLib_End();
    return 0;
}