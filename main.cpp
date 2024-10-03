#include "MyMath.h"

#include "Bezier.h"
#include "AABB.h"
#include "Sphere.h"
#include "Grid.h"
#include "Line.h"
#include "Triangle.h"
#include "Plane.h"
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

	Ball ball;
	ball.pos = { 0.8f,0.7f,0.3f };
	ball.mass = 2.0f;
	ball.radius = 0.05f;
	ball.acceleration = { 0.0f,-9.8f,0.0f };
	ball.velocity = { 0.0f,0.0f,0.0f };

	Camera camera;
	camera.Init();

	Grid grid;
	Sphere sphere;
	Collision collision;

	bool isStart = false;

	PlaneInfo planeInfo{};
	planeInfo.normal = Normalize({ -0.2f,0.9f,-0.3f });
	planeInfo.distance = 0.0f;
	Plane plane;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始4
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		/*-------------------------------------------------------------*/
		// ImGui

		Vector3 reflect = Reflect({ 4,3,5 }, { sqrtf(6) / 6,sqrtf(6) / 3,sqrtf(6) / 6 });

		ImGui::Begin("Window");

		if (ImGui::Button("Start")) {

			isStart = true;
		}

		ImGui::Text("%f,%f,%f", ball.pos.x, ball.pos.y, ball.pos.z);

		ImGui::End();

		/*-------------------------------------------------------------*/
		// 更新処理

		if (isStart) {

			ball.velocity += ball.acceleration * deltaTime;
			ball.pos += ball.velocity * deltaTime;

			if (collision.Sphere2PlaneCheckCollision(SphereInfo{ 0.05f ,ball.pos }, planeInfo)) {

				Vector3 reflected = Reflect(ball.velocity, planeInfo.normal);
				Vector3 projectToNormal = Project(reflected, planeInfo.normal);
				Vector3 movingDirection = reflected - projectToNormal;
				ball.velocity = projectToNormal + movingDirection;
			}
		}

		Matrix4x4 worldMatrix =
			MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, ball.pos);

		grid.DrawGrid(camera.GetViewMatrix(), camera.GetProjectionMatrix(), camera.GetViewportMatrix());

		sphere.DrawSphere(worldMatrix, ball.radius, 0xffffffff,
			camera.GetViewMatrix(), camera.GetProjectionMatrix(), camera.GetViewportMatrix());

		// 平面の描画
		plane.DrawPlane(planeInfo, camera.GetViewMatrix(), camera.GetProjectionMatrix(), camera.GetViewportMatrix());

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
