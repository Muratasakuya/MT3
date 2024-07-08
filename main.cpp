#include "MyMath.h"

#include "Bezier.h"
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
	// 肩、肘、手

	// 座標
	Vector3 translates[3] = {

		{0.2f,1.0f,0.0f},
		{0.4f,0.0f,0.0f},
		{0.3f,0.0f,0.0f}
	};

	// 回転角
	Vector3 rotates[3] = {

		{0.0f,0.0f,-6.8f},
		{0.0f,0.0f,-1.4f},
		{0.0f,0.0f,0.0f}
	};

	// 色
	uint32_t colors[3] = {

		0xff0000ff,
		0x00ff00ff,
		0x0000ffff
	};

	// 球
	Matrix4x4 worldMatrices[3];
	SphereInfo sphereInfo[3];
	Sphere sphere[3];
	for (int i = 0; i < 3; i++) {

		sphereInfo[i] = { 0.05f,translates[i],rotates[i],colors[i] };
		worldMatrices[i] = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotates[i], translates[i]);
	}

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		/*-------------------------------------------------------------*/
		// ImGui

		ImGui::Begin("Hierarchy");

		ImGui::DragFloat3("Shoulder translate", &translates[0].x, 0.05f);
		ImGui::DragFloat3("Arm translate", &translates[1].x, 0.05f);
		ImGui::DragFloat3("Hand translate", &translates[2].x, 0.05f);
		ImGui::DragFloat3("Shoulder rotate", &rotates[0].x, 0.01f);
		ImGui::DragFloat3("Arm rotate", &rotates[1].x, 0.01f);
		ImGui::DragFloat3("Hand rotate", &rotates[2].x, 0.01f);

		ImGui::End();

		/*-------------------------------------------------------------*/
		// 更新処理

		// カメラの更新処理
		camera.Update();

		worldMatrices[0] = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotates[0], translates[0]);
		worldMatrices[1] = Multiply(MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotates[1], translates[1]), worldMatrices[0]);
		worldMatrices[2] = Multiply(MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotates[2], translates[2]), worldMatrices[1]);

		/*-------------------------------------------------------------*/
		// 描画処理

		// グリッド線の描画
		grid.DrawGrid(camera.GetViewMatrix(), camera.GetProjectionMatrix(), camera.GetViewportMatrix());

		// 制御点の球の描画
		for (int i = 0; i < 3; i++) {

			sphere[i].DrawSphere(worldMatrices[i], sphereInfo[i], camera.GetViewMatrix(), camera.GetProjectionMatrix(), camera.GetViewportMatrix());
		}


		Matrix4x4 wvpMatrices[3];
		Vector3 worldPos[3] = {

			{worldMatrices[0].m[3][0],worldMatrices[0].m[3][1],worldMatrices[0].m[3][2]},
			{worldMatrices[1].m[3][0],worldMatrices[1].m[3][1],worldMatrices[1].m[3][2]},
			{worldMatrices[2].m[3][0],worldMatrices[2].m[3][1],worldMatrices[2].m[3][2]},
		};
		Matrix4x4 matrices[3];
		Vector3 ndcPos;
		Vector3 screenPos[3];

		for (int i = 0; i < 3; i++) {

			matrices[i] = MakeAffineMatrix({ 0.02f,0.02f,0.02f }, { 0.0f,0.0f,0.0f }, worldPos[i]);
			wvpMatrices[i] = Multiply(matrices[i], Multiply(camera.GetViewMatrix(), camera.GetProjectionMatrix()));
			ndcPos = Transform(worldPos[i], wvpMatrices[i]);
			screenPos[i] = Transform(ndcPos, camera.GetViewportMatrix());
		}

		Novice::DrawLine(
			static_cast<int>(screenPos[0].x), static_cast<int>(screenPos[0].y),
			static_cast<int>(screenPos[1].x), static_cast<int>(screenPos[1].y),
			0xffffffff);

		Novice::DrawLine(
			static_cast<int>(screenPos[1].x), static_cast<int>(screenPos[1].y),
			static_cast<int>(screenPos[2].x), static_cast<int>(screenPos[2].y),
			0xffffffff);

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
