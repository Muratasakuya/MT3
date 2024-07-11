#include "MyMath.h"

#include "Bezier.h"
#include "CatmullRom.h"
#include "AABB.h"
#include "Sphere.h"
#include "Grid.h"
#include "Line.h"
#include "Triangle.h"
#include "Camera.h"
#include "Collision.h"

const char kWindowTitle[] = "LE2B_26_ムラタ_サクヤ";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	/*-------------------------------------------------------------*/
	// カメラ

	Camera camera;
	// 初期化
	camera.Init();

	/*-------------------------------------------------------------*/
	// グリッド線

	Grid grid;

	/*-------------------------------------------------------------*/
	// スプライン曲線

	// 各制御点
	Vector3 controlPoints[4] = {
		{-0.8f,0.58f,1.0f},
		{1.76f,1.0f,-0.3f},
		{0.94f,-0.7f,2.3f},
		{-0.53f,-0.26f,-0.15f},
	};

	// 制御点の球
	SphereInfo sphereInfo[4];
	Sphere sphere[4];
	for (int i = 0; i < 4; i++) {

		sphereInfo[i] = { 0.01f,controlPoints[i],0x000000ff };
	}

	CatmullRom catmullRom;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		/*-------------------------------------------------------------*/
		// 更新処理

		// カメラの更新処理
		camera.Update();


		/*-------------------------------------------------------------*/
		// ImGui
		
		ImGui::Begin("CatmullRom");

		ImGui::DragFloat3("controlPoints[0]", &controlPoints[0].x, 0.1f);
		ImGui::DragFloat3("controlPoints[1]", &controlPoints[1].x, 0.1f);
		ImGui::DragFloat3("controlPoints[2]", &controlPoints[2].x, 0.1f);
		ImGui::DragFloat3("controlPoints[3]", &controlPoints[3].x, 0.1f);

		ImGui::End();

		// 制御点の更新
		for (int i = 0; i < 3; i++) {

			sphereInfo[i] = { 0.01f,controlPoints[i],0x000000ff };
		}

		/*-------------------------------------------------------------*/
		// 描画処理

		// グリッド線の描画
		grid.DrawGrid(camera.GetViewMatrix(), camera.GetProjectionMatrix(), camera.GetViewportMatrix());

		// スプライン曲線の描画
		catmullRom.DrawCatmullRom(
			controlPoints[1], controlPoints[2], controlPoints[3], controlPoints[0],
			camera.GetViewMatrix(), camera.GetProjectionMatrix(), camera.GetViewportMatrix(), 0x0000ffff);
		// スプライン曲線の描画
		catmullRom.DrawCatmullRom(
			controlPoints[0], controlPoints[1], controlPoints[2], controlPoints[3],
			camera.GetViewMatrix(), camera.GetProjectionMatrix(), camera.GetViewportMatrix(), 0x0000ffff);
		// スプライン曲線の描画
		catmullRom.DrawCatmullRom(
			controlPoints[3], controlPoints[0], controlPoints[1], controlPoints[2],
			camera.GetViewMatrix(), camera.GetProjectionMatrix(), camera.GetViewportMatrix(), 0x0000ffff);

		// 制御点の球の描画
		for (int i = 0; i < 4; i++) {

			sphere[i].DrawSphere(sphereInfo[i], camera.GetViewMatrix(), camera.GetProjectionMatrix(), camera.GetViewportMatrix());
		}

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
