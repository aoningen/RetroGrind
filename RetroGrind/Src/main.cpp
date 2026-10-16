#include "DxLib.h"
#include "AssetManager.h"
#include "Stage.h"

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow
)
{
    // 高画質化（アンチエイリアス）
    SetFullSceneAntiAliasingMode(4, 2);

    ChangeWindowMode(TRUE);
    SetGraphMode(1280, 720, 32);

    if (DxLib_Init() == -1) return -1;

    SetDrawScreen(DX_SCREEN_BACK);

    // --------------------------------------------------
    // DxLibの全体設定（背景・Zバッファ・カリング・ライト）
    // --------------------------------------------------
    SetBackgroundColor(128, 128, 128); // グレー背景
    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);
    SetUseBackCulling(TRUE);

    SetLightEnable(TRUE);
    SetGlobalAmbientLight(GetColorF(0.4f, 0.4f, 0.4f, 1.0f));
    SetLightDirection(VGet(0.0f, -1.0f, 0.0f)); // 上から下への光

    // カメラの設定（左斜め上からの見下ろし視点）
    SetCameraNearFar(10.0f, 10000.0f);
    SetCameraPositionAndTarget_UpVecY(
        VGet(-150.0f, 450.0f, -400.0f),
        VGet(0.0f, 30.0f, 0.0f)
    );

    // --------------------------------------------------
    // クラスのインスタンス生成と初期化
    // --------------------------------------------------
    AssetManager assetManager;
    Stage stage;

    // ステージ初期化（内部でモデルをロード）
    if (!stage.Initialize(assetManager)) {
        DxLib_End();
        return -1;
    }

    // --------------------------------------------------
    // メインループ
    // --------------------------------------------------
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        ClearDrawScreen();

        // クラスの処理を呼び出す
        stage.Update();
        stage.Draw();

        ScreenFlip();
    }

    // ※whileループを抜けた後、assetManagerのデストラクタが自動で呼ばれ
    //   MV1DeleteModel(全モデルのメモリ解放) が安全に行われます。

    DxLib_End();
    return 0;
}