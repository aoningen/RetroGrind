#include "DxLib.h"

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow
)
{
    //アンチエイリアスの有効化
    SetFullSceneAntiAliasingMode(4, 2);

    // 画面設定
    ChangeWindowMode(TRUE);
    SetGraphMode(1280, 720, 32);

    if (DxLib_Init() == -1) return -1;

    SetDrawScreen(DX_SCREEN_BACK);
    //背景色をグレーにする
    SetBackgroundColor(128, 128, 128);


    // --------------------------------------------------
    // 1. 3D描画・ライティングの全体設定
    // --------------------------------------------------
    //Zバッファ有効化
    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);

    SetUseBackCulling(TRUE);

    SetUseLighting(TRUE);
 
    SetLightEnable(TRUE);
    SetGlobalAmbientLight(GetColorF(1.0f, 1.0f, 1.0f, 1.0f));
    SetLightDirection(VGet(0.4f, -1.0f, 0.5f));

    // --------------------------------------------------
    // 2. モデルの読み込みと設定
    // --------------------------------------------------
    int stageHandle = MV1LoadModel("Assets/Models/RecordPlayer.mv1");
    int diskHandle = MV1LoadModel("Assets/Models/RecordDisk.mv1");

    if (stageHandle == -1 || diskHandle == -1)
    {
        DxLib_End();
        return -1;
    }

    //マテリアルの反射光を白に近付ける
    int matNum = MV1GetMaterialNum(diskHandle);
    for (int i = 0; i < matNum; i++)
    {
        MV1SetMaterialSpcColor(diskHandle, i, GetColorF(0.8f, 0.8f, 0.8f, 1.0f));
        MV1SetMaterialSpcPower(diskHandle, i, 20.0f);
    }

    // 企画書の通り、回転軸となる中央(0, 0, 0)に原点を配置[cite: 17]
    MV1SetPosition(stageHandle, VGet(0.0f, 0.0f, 0.0f));
    MV1SetPosition(diskHandle, VGet(0.0f, 0.0f, 0.0f));

    //もしレコード盤と土台の面が重なってチラつく(Zファイティング)場合、
    //diskHandleのY座標を調整する

    // スケール設定
    MV1SetScale(stageHandle, VGet(0.2f, 0.2f, 0.2f));
    MV1SetScale(diskHandle, VGet(0.2f, 0.2f, 0.2f));

    // --------------------------------------------------
    // 3. カメラ視点設定
    // --------------------------------------------------
    // Nearを10.0fに設定し、面同士の描画ブレを防止
    SetCameraNearFar(10.0f, 10000.0f);

    // 手前から少し見下ろす位置に配置
    SetCameraPositionAndTarget_UpVecY(
        VGet(0.0f, 450.0f,-250.0f),
        VGet(0.0f, 30.0f, 0.0f)
    );

    float diskRotationAngle = 0.0f;

    // --------------------------------------------------
    // メインループ
    // --------------------------------------------------
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        ClearDrawScreen();

        // 企画書仕様：プレイヤーの走りに合わせてレコード盤自体が回転する
        // 毎フレーム少しずつY軸（縦軸）を中心に回転
        diskRotationAngle += 0.02f;
        MV1SetRotationXYZ(diskHandle, VGet(0.0f, diskRotationAngle, 0.0f));

        // 描画実行
        MV1DrawModel(stageHandle);
        MV1DrawModel(diskHandle);

        DrawString(20, 20, "【Retro Grind】描画ベース・回転実装完了", GetColor(0, 255, 0));

        ScreenFlip();
    }

    MV1DeleteModel(stageHandle);
    MV1DeleteModel(diskHandle);

    DxLib_End();
    return 0;
}