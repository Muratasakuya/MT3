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

	Vector3 a{ 0.2f,1.0f,0.0f };
	Vector3 b{ 2.4f,3.1f,1.2f };
	Vector3 c = a + b;
	Vector3 d = a - b;
	Vector3 e = a * 2.4f;
	Vector3 rotate{ 0.4f,1.43f,-0.8f };
	Matrix4x4 rotateXMatrix = MakePitchMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeYawMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRollMatrix(rotate.z);
	Matrix4x4 rotateMatrix = rotateXMatrix * rotateYMatrix * rotateZMatrix;

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

		ImGui::Text("c : %f , %f, %f", c.x, c.y, c.z);
		ImGui::Text("d : %f , %f, %f", d.x, d.y, d.z);
		ImGui::Text("e : %f , %f, %f", e.x, e.y, e.z);

		ImGui::Text("rotateMatrix:");
		ImGui::Text("%f, %f, %f, %f", rotateMatrix.m[0][0], rotateMatrix.m[0][1], rotateMatrix.m[0][2], rotateMatrix.m[0][3]);
		ImGui::Text("%f, %f, %f, %f", rotateMatrix.m[1][0], rotateMatrix.m[1][1], rotateMatrix.m[1][2], rotateMatrix.m[1][3]);
		ImGui::Text("%f, %f, %f, %f", rotateMatrix.m[2][0], rotateMatrix.m[2][1], rotateMatrix.m[2][2], rotateMatrix.m[2][3]);
		ImGui::Text("%f, %f, %f, %f", rotateMatrix.m[3][0], rotateMatrix.m[3][1], rotateMatrix.m[3][2], rotateMatrix.m[3][3]);

		ImGui::End();

		/*-------------------------------------------------------------*/

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
