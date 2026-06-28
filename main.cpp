//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
#include "Blaster.h"
#include "Camera.h"
#include "EnemyMgr.h"
#include "FadeInOut.h"
#include "GameMgr.h"
#include "GLFWWrap.h"
#include "LineRenderer.h"
#include "MainDrone.h"
#include "Renderer3D.h"
#include "Sequence.h"
#include "TimeMgr.h"

int main() {
	Sequence::init();



	//===== メインループ =====
	while ( !glfwWindowShouldClose(GLFWWrap::window) ) {
		// 時間の更新
		TimeMgr::update();
		// イベント処理
		glfwPollEvents();
		// 画面のクリア (背景色と深度バッファ)
		GLFWWrap::clearWindowBuffer(glm::vec3(0.75f), 1.0f);

		//--- 物理更新 (固定ステップ) ---
		GameMgr::doPhase();

		//--- 描画セクション ---
		// オブジェクト描画　全ての Renderer3D インスタンスをまとめて描画
		Renderer3D::drawAllRenderers();

		Renderer3D::drawAllShadowOn(); // 影の描画

		LineRenderer::drawAllLineRenderers();	// ライン描画（3Dオブジェクトの後）



		// デス演出
		//FadeInOut::fadeInOut(black, TimeMgr::getDeltaTime(), 2.0f);

		GLFWWrap::deltaMouseXPos = GLFWWrap::deltaMouseYPos = 0.0f;

		// ゲームオーバー入力
		if ( glfwGetKey(GLFWWrap::window, GLFW_KEY_ENTER) == GLFW_PRESS &&
			glfwGetKey(GLFWWrap::window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ) {
			isGameOver = true;
		}

		if ( glfwGetKey(GLFWWrap::window, GLFW_KEY_ESCAPE) == GLFW_PRESS ) {
			glfwSetWindowShouldClose(GLFWWrap::window, true);
		}

		// ダブルバッファ切り替え
		glfwSwapBuffers(GLFWWrap::window);
	}

	//===== 終了処理 =====
	GLFWWrap::deleteWindow();
	return 0;
}