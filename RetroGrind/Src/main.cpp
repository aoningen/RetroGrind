#include "DxLib.h"

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow
)
{
    // 画面モード設定
    ChangeWindowMode(TRUE);
    SetGraphMode(1280, 720, 32);

    if (DxLib_Init() == -1) return -1;

    SetDrawScreen(DX_SCREEN_BACK);

    // --------------------------------------------------
    // 3D描画・ライティングの全体設定
    // --------------------------------------------------
    // 【追加】Zバッファを有効にして、奥のものが手前に透けないようにする
    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);

    SetUseBackCulling(TRUE);                                // 両面描画（ポリゴンの欠け防止）
    SetGlobalAmbientLight(GetColorF(0.4f, 0.4f, 0.4f, 1.0f)); // 環境光の設定
    SetLightEnable(TRUE);

    // --------------------------------------------------
    // 1. 3Dモデルの読み込み (.mv1)
    // --------------------------------------------------
    int stageHandle = MV1LoadModel("Assets/Models/RecordPlayer.mv1");
    int diskHandle = MV1LoadModel("Assets/Models/RecordDisk.mv1");

    if (stageHandle == -1 || diskHandle == -1)
    {
        DxLib_End();
        return -1;
    }

    // --------------------------------------------------
    // 2. 配置およびスケール（拡大率）の設定
    // --------------------------------------------------
    MV1SetPosition(stageHandle, VGet(0.0f, 0.0f, 0.0f));
    MV1SetPosition(diskHandle, VGet(0.0f, 5.0f, 0.0f));
    // 前回綺麗に映っていた 0.2f に設定
    MV1SetScale(stageHandle, VGet(0.2f, 0.2f, 0.2f));
    MV1SetScale(diskHandle, VGet(0.2f, 0.2f, 0.2f));

    // --------------------------------------------------
    // 3. カメラ視点設定
    // --------------------------------------------------
    SetCameraNearFar(10.0f, 10000.0f);

    // 手前から少し見下ろす位置に配置
    SetCameraPositionAndTarget_UpVecY(
        VGet(0.0f, 250.0f, -400.0f), // カメラの位置 (X, Y, Z)
        VGet(0.0f, 30.0f, 0.0f)       // 注視点
    );

    float diskRotationAngle = 0.0f;

    // --------------------------------------------------
    // メインループ
    // --------------------------------------------------
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        ClearDrawScreen();

        // 回転更新処理
        diskRotationAngle += 0.02f;
        MV1SetRotationXYZ(diskHandle, VGet(0.0f, diskRotationAngle, 0.0f));

        // 描画実行
        MV1DrawModel(stageHandle);
        MV1DrawModel(diskHandle);

        DrawString(20, 20, "【成功】モデル描画正常化確認中", GetColor(0, 255, 0));

        ScreenFlip();
    }

    // メモリ解除
    MV1DeleteModel(stageHandle);
    MV1DeleteModel(diskHandle);

    DxLib_End();
    return 0;
}