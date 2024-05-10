#include "MyMath.h"

#include "Sphere.h"
#include "Plane.h"
#include "Grid.h"
#include "Line.h"
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
	// 平面

	PlaneInfo planeInfo;
	planeInfo = { {0.0f,1.0f,0.0f},1.0f };

	Plane plane;

	/*-------------------------------------------------------------*/
	// ライン

	LineInfo lineInfo;
	lineInfo = { {-0.45f,0.41f,0.0f },{1.0f,0.58f,0.0f},LineType::LineSegment ,0xffffffff };

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

		// 当たり判定
		if (collision.Plane2LineCheckCollision(planeInfo, lineInfo)) {

			lineInfo.color = 0xff0000ff;
		}
		else {

			lineInfo.color = 0xffffffff;
		}

		/*-------------------------------------------------------------*/
		// ImGui

		ImGui::Begin("Plane");

		ImGui::DragFloat3("normal", &planeInfo.normal.x, 0.01f);
		// 正規化する
		planeInfo.normal = Normalize(planeInfo.normal);
		ImGui::SliderFloat("distance", &planeInfo.distance, -1.5f, 1.5f);

		ImGui::End();

		ImGui::Begin("Line");

		ImGui::DragFloat3("origin", &lineInfo.origin.x, 0.01f);
		ImGui::DragFloat3("diff", &lineInfo.diff.x, 0.01f);

		ImGui::End();

		/*-------------------------------------------------------------*/
		// 描画処理

		// グリッド線の描画
		grid.DrawGrid(camera.GetViewMatrix(), camera.GetProjectionMatrix(), camera.GetViewportMatrix());

		// 平面の描画
		plane.DrawPlane(planeInfo, camera.GetViewMatrix(), camera.GetProjectionMatrix(), camera.GetViewportMatrix());

		// ラインの描画
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
