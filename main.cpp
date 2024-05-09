#include "MyMath.h"

#include "Sphere.h"
#include "Grid.h"
#include "Camera.h"

const char kWindowTitle[] = "LE2B_26_ムラタ_サクヤ";

// 当たり判定
bool IsCollision(const SphereInfo& s1, const SphereInfo& s2) {

	float distance = Length(s1.center - s2.center);

	// 半径の合計より大きければ衝突
	if (distance <= s1.radius_ + s2.radius_) {

		return true;
	}
	else {

		return false;
	}
}

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
	// 球

	SphereInfo sphereInfo[2];
	sphereInfo[0] = { 1.0f,{2.0f,0.0f,0.0f},0xffffffff };
	sphereInfo[1] = { 1.0f,{0.0f,0.0f,0.0f},0xffffffff };

	Sphere sphere[2];

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
		if (IsCollision(sphereInfo[0], sphereInfo[1])) {

			// 当たっていれば赤
			sphereInfo[0].color = 0xff0000ff;
		}
		else {

			// 当たっていなければ白
			sphereInfo[0].color = 0xffffffff;
		}

		/*-------------------------------------------------------------*/
		// ImGui

		ImGui::Begin("Sphere1");

		ImGui::SliderFloat3("translate", &sphereInfo[0].center.x, -10.0f, 10.0f);
		ImGui::SliderFloat("radius", &sphereInfo[0].radius_, 0.0f, 10.0f);

		ImGui::End();

		ImGui::Begin("Sphere2");

		ImGui::SliderFloat3("translate", &sphereInfo[1].center.x, -10.0f, 10.0f);
		ImGui::SliderFloat("radius", &sphereInfo[1].radius_, 0.0f, 10.0f);

		ImGui::End();

		/*-------------------------------------------------------------*/
		// 描画処理

		// グリッド線の描画
		grid.DrawGrid(camera.GetViewMatrix(), camera.GetProjectionMatrix(), camera.GetViewportMatrix());

		// 球2つの描画
		for (int i = 0; i < 2; i++) {
			sphere[i].DrawSphere(sphereInfo[i], camera.GetViewMatrix(), camera.GetProjectionMatrix(), camera.GetViewportMatrix());
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
