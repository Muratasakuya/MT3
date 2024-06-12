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

	OBBInfo obbInfo;
	obbInfo = {
		.center{-1.0f,0.0f,0.0f},
		.orientations =
		{{1.0f,0.0f,0.0f},
		{0.0f,1.0f,0.0f},
		{0.0f,0.0f,1.0f}},
		.size{0.5f,0.5f,0.5f}
	};
	uint32_t obbColor = 0xffffffff;

	OBB obb;

	obb.Initialize();

	obb.SetTranslate(obbInfo.center);

	/*-------------------------------------------------------------*/
	// 線

	LineInfo lineInfo;
	lineInfo = {
		.origin{-0.8f,-0.3f,0.0f},
		.diff{0.5f,0.5f,0.5f},
		.type{LineType::LineSegment},
		.color{0xffffffff},
	};

	Line line;

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
		// 更新処理

		// カメラの更新処理
		camera.Update();

		// OBBと球の当たり判定
		if (collision.OBB2LineCheckCollision(obb.GetRotate(), obbInfo, lineInfo)) {

			obbColor = 0xff0000ff;
		} else {

			obbColor = 0xffffffff;
		}

		/*-------------------------------------------------------------*/
		// ImGui

		ImGui::Begin("OBB");

		ImGui::DragFloat3("obb.center", &obbInfo.center.x, 0.01f);
		ImGui::DragFloat3("obb.size", &obbInfo.size.x, 0.01f);

		ImGui::End();

		ImGui::Begin("LineSegment");

		ImGui::DragFloat3("line.origin", &lineInfo.origin.x, 0.01f);
		ImGui::DragFloat3("line.diff", &lineInfo.diff.x, 0.01f);

		ImGui::End();

#pragma region /// 値の制限 ///
		obbInfo.size.x = (std::max)(obbInfo.size.x, 0.0f);
		obbInfo.size.y = (std::max)(obbInfo.size.y, 0.0f);
		obbInfo.size.z = (std::max)(obbInfo.size.z, 0.0f);
#pragma endregion

		obb.SetTranslate(obbInfo.center);

		/*-------------------------------------------------------------*/
		// 描画処理

		// グリッド線の描画
		grid.DrawGrid(camera.GetViewMatrix(), camera.GetProjectionMatrix(), camera.GetViewportMatrix());

		// OBBの描画
		obb.DrawOBB(obbInfo, Multiply(camera.GetViewMatrix(), camera.GetProjectionMatrix()), camera.GetViewportMatrix(), obbColor);

		// 線の描画
		line.DrawLine(lineInfo, camera.GetViewMatrix(), camera.GetProjectionMatrix(), camera.GetViewportMatrix());

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
