#include "MyMath.h"

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
	// AABB

	AABBInfo aabb1Info;
	aabb1Info = {
		.min{-0.5f,-0.5f,-0.5f},
		.max{0.0f,0.0f,0.0f}
	};
	uint32_t aabb1Color = 0xffffffff;

	AABBInfo aabb2Info;
	aabb2Info = {
		.min{0.2f,0.2f,0.2f},
		.max{1.0f,1.0f,1.0f}
	};
	uint32_t aabb2Color = 0xffffffff;

	AABB aabb1;
	AABB aabb2;

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

		// AABB同士の当たり判定
		if (collision.AABB2AABBCheckCollision(aabb1Info, aabb2Info)) {

			aabb1Color = 0x00ffffff;
		} else {

			aabb1Color = 0xffffffff;
		}

		/*-------------------------------------------------------------*/
		// ImGui

		ImGui::Begin("AABB");

		ImGui::DragFloat3("aabb1.min", &aabb1Info.min.x,0.1f);
		ImGui::DragFloat3("aabb1.max", &aabb1Info.max.x, 0.1f);
		ImGui::DragFloat3("aabb2.min", &aabb2Info.min.x, 0.1f);
		ImGui::DragFloat3("aabb2.max", &aabb2Info.max.x, 0.1f);

		ImGui::End();

#pragma region /// 値の制限 ///
		// 値の制限
		aabb1Info.min.x = (std::min)(aabb1Info.min.x, aabb1Info.max.x);
		aabb1Info.max.x = (std::max)(aabb1Info.min.x, aabb1Info.max.x);

		aabb1Info.min.y = (std::min)(aabb1Info.min.y, aabb1Info.max.y);
		aabb1Info.max.y = (std::max)(aabb1Info.min.y, aabb1Info.max.y);

		aabb1Info.min.z = (std::min)(aabb1Info.min.z, aabb1Info.max.z);
		aabb1Info.max.z = (std::max)(aabb1Info.min.z, aabb1Info.max.z);

		aabb2Info.min.x = (std::min)(aabb2Info.min.x, aabb2Info.max.x);
		aabb2Info.max.x = (std::max)(aabb2Info.min.x, aabb2Info.max.x);

		aabb2Info.min.y = (std::min)(aabb2Info.min.y, aabb2Info.max.y);
		aabb2Info.max.y = (std::max)(aabb2Info.min.y, aabb2Info.max.y);

		aabb2Info.min.z = (std::min)(aabb2Info.min.z, aabb2Info.max.z);
		aabb2Info.max.z = (std::max)(aabb2Info.min.z, aabb2Info.max.z);
#pragma endregion

		/*-------------------------------------------------------------*/
		// 描画処理

		// グリッド線の描画
		grid.DrawGrid(camera.GetViewMatrix(), camera.GetProjectionMatrix(), camera.GetViewportMatrix());

		aabb1.DrawAABB(aabb1Info, camera.GetViewMatrix(), camera.GetProjectionMatrix(), camera.GetViewportMatrix(), aabb1Color);
		aabb2.DrawAABB(aabb2Info, camera.GetViewMatrix(), camera.GetProjectionMatrix(), camera.GetViewportMatrix(), aabb2Color);


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
