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

	AABBInfo aabbInfo;
	aabbInfo = {
		.min{-0.5f,-0.5f,-0.5f},
		.max{0.0f,0.0f,0.0f}
	};
	uint32_t aabbColor = 0xffffffff;

	AABB aabb;

	/*-------------------------------------------------------------*/
	// 球

	SphereInfo sphereInfo;
	sphereInfo = { 1.0f,{0.0f,0.0f,0.0f},0xffffffff };

	Sphere sphere;

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
		if (collision.AABB2SphereCheckCollision(aabbInfo, sphereInfo)) {

			aabbColor = 0x00ffffff;
		} else {

			aabbColor = 0xffffffff;
		}

		/*-------------------------------------------------------------*/
		// ImGui

		ImGui::Begin("AABB");

		ImGui::DragFloat3("aabb1.min", &aabbInfo.min.x,0.1f);
		ImGui::DragFloat3("aabb1.max", &aabbInfo.max.x, 0.1f);
		ImGui::DragFloat3("sphere.center", &sphereInfo.center.x, 0.1f);
		ImGui::DragFloat("sphere.radius", &sphereInfo.radius, 0.1f);

		ImGui::End();

#pragma region /// 値の制限 ///
		// 値の制限
		aabbInfo.min.x = (std::min)(aabbInfo.min.x, aabbInfo.max.x);
		aabbInfo.max.x = (std::max)(aabbInfo.min.x, aabbInfo.max.x);

		aabbInfo.min.y = (std::min)(aabbInfo.min.y, aabbInfo.max.y);
		aabbInfo.max.y = (std::max)(aabbInfo.min.y, aabbInfo.max.y);

		aabbInfo.min.z = (std::min)(aabbInfo.min.z, aabbInfo.max.z);
		aabbInfo.max.z = (std::max)(aabbInfo.min.z, aabbInfo.max.z);
#pragma endregion

		/*-------------------------------------------------------------*/
		// 描画処理

		// グリッド線の描画
		grid.DrawGrid(camera.GetViewMatrix(), camera.GetProjectionMatrix(), camera.GetViewportMatrix());

		// AABBびの描画
		aabb.DrawAABB(aabbInfo, camera.GetViewMatrix(), camera.GetProjectionMatrix(), camera.GetViewportMatrix(), aabbColor);

		// 球の描画
		sphere.DrawSphere(sphereInfo, camera.GetViewMatrix(), camera.GetProjectionMatrix(), camera.GetViewportMatrix());

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
