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

	Pendulum pendulum{};
	pendulum.anchor = { 0.0f,1.0f,0.0f };
	pendulum.lenght = 0.8f;
	pendulum.angle = 0.7f;
	pendulum.angularVelocity = 0.0f;
	pendulum.angularAcceleration = 0.0f;

	Camera camera;
	camera.Init();

	Grid grid;
	Sphere sphere;

	bool isStart = false;

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

			isStart = true;
		}

		ImGui::End();

		/*-------------------------------------------------------------*/
		// 更新処理

		if (isStart) {

			pendulum.angularAcceleration =
				-(9.8f / pendulum.lenght) * std::sin(pendulum.angle);
			pendulum.angularVelocity += pendulum.angularAcceleration * deltaTime;
			pendulum.angle += pendulum.angularVelocity * deltaTime;
		}

		Vector3 pointPos{};
		pointPos.x = pendulum.anchor.x + std::sin(pendulum.angle) * pendulum.lenght;
		pointPos.y = pendulum.anchor.y - std::cos(pendulum.angle) * pendulum.lenght;
		pointPos.z = pendulum.anchor.z;

		Matrix4x4 worldMatrix =
			MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, pointPos);

		Matrix4x4 wvpMatrices[2];
		Matrix4x4 matrices[2];
		Vector3 worldPos[2] = {

			pendulum.anchor,
			pointPos
		};
		Vector3 ndcPos;
		Vector3 screenPos[2];

		for (int i = 0; i < 2; i++) {

			matrices[i] = MakeAffineMatrix({ 0.02f,0.02f,0.02f }, { 0.0f,0.0f,0.0f }, worldPos[i]);
			wvpMatrices[i] = Multiply(matrices[i], Multiply(camera.GetViewMatrix(), camera.GetProjectionMatrix()));
			ndcPos = Transform(worldPos[i], wvpMatrices[i]);
			screenPos[i] = Transform(ndcPos, camera.GetViewportMatrix());
		}

		grid.DrawGrid(camera.GetViewMatrix(), camera.GetProjectionMatrix(), camera.GetViewportMatrix());

		Novice::DrawLine(
			static_cast<int>(screenPos[0].x), static_cast<int>(screenPos[0].y),
			static_cast<int>(screenPos[1].x), static_cast<int>(screenPos[1].y),
			0xffffffff);

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
