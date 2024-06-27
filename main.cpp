#include "MyMath.h"

#include "OBB.h"
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
	// 当たり判定

	Collision collision;

	/*-------------------------------------------------------------*/
	// OBB

	// OBBの数
	const uint32_t OBBNUM = 2;

	OBBInfo obbInfo[OBBNUM];
	obbInfo[0] = {
		.center{-1.0f,0.0f,0.0f},
		.orientations =
		{{1.0f,0.0f,0.0f},
		{0.0f,1.0f,0.0f},
		{0.0f,0.0f,1.0f}},
		.size{0.83f,0.26f,0.24f}
	};
	obbInfo[1] = {
		.center{0.9f,0.66f,0.78f},
		.orientations =
		{{1.0f,0.0f,0.0f},
		{0.0f,1.0f,0.0f},
		{0.0f,0.0f,1.0f}},
		.size{0.5f,0.37f,0.5f}
	};

	uint32_t obbColor[OBBNUM];

	Vector3 obbRotate[OBBNUM]{};
	obbRotate[0] = { 0.0f,0.0f,0.0f };
	obbRotate[1] = { -0.05f,-2.49f,0.15f };

	// OBB
	OBB obb[OBBNUM];

	for (int i = 0; i < OBBNUM; i++) {

		// 生成
		obb[i].SetTranslate(obbInfo[i].center);
		obb[i].SetRotate(obbRotate[i]);

		obbColor[i] = 0xffffffff;
	}

	/*-------------------------------------------------------------*/
	// グリッド線

	Grid grid;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		/*-------------------------------------------------------------*/
		// ImGui

		ImGui::Begin("OBBInfo");

		ImGui::DragFloat3("obb1.rotate", &obbRotate[0].x, 0.01f);
		ImGui::DragFloat3("obb2.rotate", &obbRotate[1].x, 0.01f);
		ImGui::DragFloat3("obb1.center", &obbInfo[0].center.x, 0.01f);
		ImGui::DragFloat3("obb2.center", &obbInfo[1].center.x, 0.01f);

		ImGui::End();

		/*-------------------------------------------------------------*/
		// 更新処理

		// カメラの更新処理
		camera.Update();

		// OBBの更新処理
		for (int i = 0; i < OBBNUM; i++) {

			// 生成
			obb[i].SetTranslate(obbInfo[i].center);
			obb[i].SetRotate(obbRotate[i]);

			// 値の制限
			obbInfo[i].size.x = (std::max)(obbInfo[i].size.x, 0.0f);
			obbInfo[i].size.y = (std::max)(obbInfo[i].size.y, 0.0f);
			obbInfo[i].size.z = (std::max)(obbInfo[i].size.z, 0.0f);
		}

		// 衝突判定
		if (collision.OBB2OBBCheckCollision(obbInfo[0], obbInfo[1])) {

			obbColor[0] = 0xff0000ff;
		} else {

			obbColor[0] = 0xffffffff;
		}

		/*-------------------------------------------------------------*/
		// 描画処理

		// グリッド線の描画
		grid.DrawGrid(camera.GetViewMatrix(), camera.GetProjectionMatrix(), camera.GetViewportMatrix());

		// OBBの描画
		for (int i = 0; i < OBBNUM; i++) {

			obb[i].DrawOBB(obbInfo[i], Multiply(camera.GetViewMatrix(), camera.GetProjectionMatrix()), camera.GetViewportMatrix(), obbColor[i]);
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
