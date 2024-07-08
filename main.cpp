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

	Camera camera;
	camera.Init();

	Spring spring{};
	spring.anchor = { 0.0f,0.0f,0.0f };
	spring.naturalLength = 1.0f;
	spring.stiffness = 100.0f;
	spring.dampingCoefficient = 2.0f;

	Ball ball{};
	ball.pos = { 1.2f,0.0f,0.0f };
	ball.mass = 2.0f;
	ball.radius = 0.05f;
	ball.color = 0x0000ffff;

	float deltaTime = 1.0f / 60.0f;

	Grid grid;
	Sphere sphere;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		/*-------------------------------------------------------------*/
		// ImGui

		ImGui::Begin("Window");

		if (ImGui::Button("Start")) {
			
			ball.pos.x = 1.2f;
			ball.velocity.x = 0.01f;
		}

		ImGui::End();

		/*-------------------------------------------------------------*/
		// 更新処理

		Matrix4x4 wvpMatrices[2];
		Matrix4x4 matrices[2];
		Vector3 worldPos[2] = {

			spring.anchor,
			ball.pos
		};
		Vector3 ndcPos;
		Vector3 screenPos[2];

		for (int i = 0; i < 2; i++) {

			matrices[i] = MakeAffineMatrix({ 0.02f,0.02f,0.02f }, { 0.0f,0.0f,0.0f }, worldPos[i]);
			wvpMatrices[i] = Multiply(matrices[i], Multiply(camera.GetViewMatrix(), camera.GetProjectionMatrix()));
			ndcPos = Transform(worldPos[i], wvpMatrices[i]);
			screenPos[i] = Transform(ndcPos, camera.GetViewportMatrix());
		}

		Vector3 diff = ball.pos - spring.anchor;
		float length = Length(diff);

		if (length != 0.0f) {

			Vector3 direction = Normalize(diff);
			Vector3 restPos = spring.anchor + direction * spring.naturalLength;
			Vector3 displacement = length * (ball.pos - restPos);
			Vector3 restoringForce = -spring.stiffness * displacement;
			Vector3 dampingForce = -spring.dampingCoefficient * ball.velocity;
			Vector3 force = restoringForce + dampingForce;

			ball.acceleration = force / ball.mass;
		}

		// 加速度も速度もどちらも秒を基準とした値
		// それが、1/60秒完(deltaTime)適応されたと考える
		ball.velocity += ball.acceleration * deltaTime;
		ball.pos += ball.velocity * deltaTime;

		Matrix4x4 ballWorldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, ball.pos);

		grid.DrawGrid(camera.GetViewMatrix(), camera.GetProjectionMatrix(), camera.GetViewportMatrix());

		Novice::DrawLine(
			static_cast<int>(screenPos[0].x), static_cast<int>(screenPos[0].y),
			static_cast<int>(screenPos[1].x), static_cast<int>(screenPos[1].y),
			0xffffffff);

		sphere.DrawSphere(ballWorldMatrix, ball.radius, ball.color,
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
