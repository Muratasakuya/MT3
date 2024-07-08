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

	float deltaTime = 1.0f / 60.0f;

	float angularVelocity = 3.14f;
	float angle = 0.0f;

	//bool isStart = false;

	Vector3 centerPos = { 0.0f,0.0f,0.0f };
	Vector3 pointPos = { 0.0f,0.0f,0.0f };
	float radius = 0.8f;

	bool isRotating = false;

	Camera camera;
	camera.Init();

	Grid grid;
	Sphere sphere;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始4
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		/*-------------------------------------------------------------*/
		// ImGui

		ImGui::Begin("Window");

		if (ImGui::Button("Start")) {
			isRotating = true;
			angle = 0.0f;
		}

		ImGui::End();

		/*-------------------------------------------------------------*/
		// 更新処理

		if (isRotating) {
			angle += angularVelocity * deltaTime;
			if (angle >= 2.0f * angularVelocity) {
				isRotating = false;
				angle = 0.0f; 
			}
		}

		pointPos.x = centerPos.x + std::cos(angle) * radius;
		pointPos.y = centerPos.y + std::sin(angle) * radius;
		pointPos.z = 0.0f;

		Matrix4x4 worldMatrix =
			MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, pointPos);

		grid.DrawGrid(camera.GetViewMatrix(), camera.GetProjectionMatrix(), camera.GetViewportMatrix());

		sphere.DrawSphere(worldMatrix, 0.05f, 0xffffffff,
			camera.GetViewMatrix(), camera.GetProjectionMatrix(), camera.GetViewportMatrix());

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
