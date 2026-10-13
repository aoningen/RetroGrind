#include "DxLib.h"

// WinMain の注釈警告を解消するプロトタイプ宣言
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
    // 1. 3Dモデルの読み込み
    // --------------------------------------------------
    int stageHandle = MV1LoadModel("Assets/Models/RecordPlayer.mv1");
    int diskHandle = MV1LoadModel("Assets/Models/RecordDisk.mv1");
    // --------------------------------------------------
// マテリアルの環境光（アンビエント）とディフューズ色を白に強制設定して明るくする
// --------------------------------------------------
    int stageMatNum = MV1GetMaterialNum(stageHandle);
    for (int i = 0; i < stageMatNum; i++)
    {
        // ディフューズ（拡散光）とアンビエント（環境光）を白（1.0, 1.0, 1.0）にして黒潰れを防ぐ
        MV1SetMaterialDifColor(stageHandle, i, GetColorF(1.0f, 1.0f, 1.0f, 1.0f));
        MV1SetMaterialAmbColor(stageHandle, i, GetColorF(1.0f, 1.0f, 1.0f, 1.0f));
    }

    int diskMatNum = MV1GetMaterialNum(diskHandle);
    for (int i = 0; i < diskMatNum; i++)
    {
        MV1SetMaterialDifColor(diskHandle, i, GetColorF(1.0f, 1.0f, 1.0f, 1.0f));
        MV1SetMaterialAmbColor(diskHandle, i, GetColorF(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // 【診断 1】モデルの読み込み成功チェック
    if (stageHandle == -1 || diskHandle == -1)
    {
        // 読み込み失敗時は画面に赤文字を出して確認できるようにする
        while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
        {
            ClearDrawScreen();

            DrawString(50, 50, "【エラー】モデルの読み込みに失敗しました！", GetColor(255, 0, 0));
            DrawString(50, 80, "以下のファイルのパスまたは拡張子を確認してください:", GetColor(255, 255, 255));
            DrawString(70, 110, "・Assets/Models/RecordPlayer.mv1", GetColor(255, 255, 0));
            DrawString(70, 130, "・Assets/Models/RecordDisk.mv1", GetColor(255, 255, 0));
            DrawString(50, 170, "※ Visual Studio の作業ディレクトリ設定 ($(ProjectDir)) も確認してください", GetColor(200, 200, 200));

            ScreenFlip();
        }
        DxLib_End();
        return -1;
    }

    // --------------------------------------------------
    // 1. モデルの拡大率（スケール）を少し小さく調整する（1/10 または 1/5）
    // --------------------------------------------------
    MV1SetScale(stageHandle, VGet(0.2f, 0.2f, 0.2f));
    MV1SetScale(diskHandle, VGet(0.2f, 0.2f, 0.2f));

    // --------------------------------------------------
    // 2. カメラの画角・位置調整（引いた位置から見下ろす）
    // --------------------------------------------------
    SetCameraNearFar(1.0f, 10000.0f); // 描画限界距離

    SetCameraPositionAndTarget_UpVecY(
        VGet(0.0f, 400.0f, -600.0f), // 高めの遠い位置
        VGet(0.0f, 0.0f, 0.0f)        // 中央原点(0,0,0)を注視
    );
    

    float diskRotationAngle = 0.0f;

    // --------------------------------------------------
    // メインループ
    // --------------------------------------------------
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        ClearDrawScreen();

        // 更新処理：レコード盤を回転させる
        diskRotationAngle += 0.02f;
        MV1SetRotationXYZ(diskHandle, VGet(0.0f, diskRotationAngle, 0.0f));

        // 描画処理
        MV1DrawModel(stageHandle);
        MV1DrawModel(diskHandle);

        // デバッグ用画面表示
        DrawString(20, 20, "【成功】モデルの読み込みに成功しました！", GetColor(0, 255, 0));
        DrawString(20, 40, "画面上にモデルが表示されているか確認してください。", GetColor(255, 255, 255));

        ScreenFlip();
    }

    // モデルハンドルの削除
    MV1DeleteModel(stageHandle);
    MV1DeleteModel(diskHandle);

    DxLib_End();
    return 0;
}

//#include "DxLib.h"
//
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
//{
//    // 画面モード設定
//    ChangeWindowMode(TRUE);
//    SetGraphMode(1280, 720, 32);
//
//    if (DxLib_Init() == -1) return -1;
//    // 標準ライトの配置と有効化
//    SetLightEnable(TRUE);
//
//    SetDrawScreen(DX_SCREEN_BACK);
//
//    // 3Dモデルの読み込み
//    int stageHandle = MV1LoadModel("Assets/Models/RecordPlayer.mv1");
//    int diskHandle = MV1LoadModel("Assets/Models/RecordDisk.mv1");
//
//    // モデルが正常に読み込めたかチェック
//    if (stageHandle == -1 || diskHandle == -1)
//    {
//        // 読み込み失敗時はエラーメッセージを表示して終了
//        DxLib_End();
//        return -1;
//    }
//
//    // カメラの設定（レコードプレーヤー全体を見下ろす位置）
//    SetCameraPositionAndTarget_UpVecY(
//        VGet(0.0f, 150.0f, -200.0f), // カメラの位置
//        VGet(0.0f, 0.0f, 0.0f)        // 注視点（中心）
//    );
//
//    float diskRotationAngle = 0.0f;
//
//    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
//    {
//        ClearDrawScreen();
//
//        // --------------------------------------------------
//        // 更新処理：レコード盤を回転させる
//        // --------------------------------------------------
//        diskRotationAngle += 0.02f; // 回転速度
//        MV1SetRotationXYZ(diskHandle, VGet(0.0f, diskRotationAngle, 0.0f));
//
//        // --------------------------------------------------
//        // 描画処理
//        // --------------------------------------------------
//        // 1. 土台・アーム（Stage）の描画
//        MV1DrawModel(stageHandle);
//
//        // 2. レコード盤（Disk）の描画（回転状態）
//        MV1DrawModel(diskHandle);
//
//        ScreenFlip();
//    }
//
//    // モデルハンドルの削除
//    MV1DeleteModel(stageHandle);
//    MV1DeleteModel(diskHandle);
//
//    DxLib_End();
//    return 0;
//}