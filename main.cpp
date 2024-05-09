#include "MyMath.h"

#include "Sphere.h"
#include "Plane.h"
#include "Grid.h"
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
	// 球

	SphereInfo sphereInfo;
	sphereInfo = { 1.0f,{0.0f,0.0f,0.0f},0xffffffff };

	Sphere sphere;

	/*-------------------------------------------------------------*/
	// 平面

	PlaneInfo planeInfo;
	planeInfo = { {0.0f,1.0f,0.0f},1.0f };

	Plane plane;

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

		// 当たり判定
		if (collision.Sphere2PlaneCheckCollision(sphereInfo, planeInfo)) {

			// 当たっていれば赤
			sphereInfo.color = 0xff0000ff;
		}
		else {

			// 当たっていなければ白
			sphereInfo.color = 0xffffffff;
		}

		/*-------------------------------------------------------------*/
		// ImGui

		ImGui::Begin("Sphere1");

		ImGui::SliderFloat3("translate", &sphereInfo.center.x, -10.0f, 10.0f);
		ImGui::SliderFloat("radius", &sphereInfo.radius, 0.0f, 10.0f);

		ImGui::End();

		ImGui::Begin("Plane");

		ImGui::DragFloat3("normal", &planeInfo.normal.x, 0.01f);
		// 正規化する
		planeInfo.normal = Normalize(planeInfo.normal);
		ImGui::SliderFloat("distance", &planeInfo.distance, -1.5f, 1.5f);

		ImGui::End();

		/*-------------------------------------------------------------*/
		// 描画処理

		// グリッド線の描画
		grid.DrawGrid(camera.GetViewMatrix(), camera.GetProjectionMatrix(), camera.GetViewportMatrix());

		// 平面の描画
		plane.DrawPlane(planeInfo, camera.GetViewMatrix(), camera.GetProjectionMatrix(), camera.GetViewportMatrix());

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
