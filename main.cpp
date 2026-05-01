//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "Blaster.h"
#include "MainDrone.h"
#include "GLFWWrap.h"
#include "Renderer3D.h"
#include "Camera.h"
#include "LineRenderer.h"
#include "FadeInOut.h"
#include "TimeMgr.h"
#include "Sequence.h"


int main() {
	Sequence::init();

	//===== メインループ =====
	while ( !glfwWindowShouldClose(GLFWWrap::window) ) {
		// 時間の更新
		TimeMgr::update();
		// イベント処理
		glfwPollEvents();
		// 画面のクリア (背景色と深度バッファ)
		GLFWWrap::clearWindowBuffer(glm::vec3(0.2f, 0.8f, 0.8f), 0.5f);

		//--- 物理更新（固定ステップ） ---
		while ( TimeMgr::shouldUpdate() ) {
			TimeMgr::consumeFixedStep();	// 固定ステップ分の時間を消費

			Sequence::update();				// 全オブジェクトの物理更新
			Sequence::checkAllCollision();	// AABB更新と衝突判定


			Blaster::shot();
			Blaster::checkDeadBullets();

			//カメラ更新
			Camera::cam->update();
		}




		//--- 描画セクション ---
		// オブジェクト描画　　全てのRenderer3Dインスタンスがここで一括描画される
		Renderer3D::drawAllRenderers();

		Renderer3D::drawAllShadowOn();// 影の描画

		// グリッド線描画
		// グリッド線描画レンダラー
		LineRenderer coordinateLineR(Camera::cam);
		coordinateLineR.init();
		float clNum = 50;
		float interval = 10;
		for ( float i = 0; i < clNum; i++ ) {
			float pos = interval * (i - clNum / 2);
			float limit = interval * clNum / 2.0f;

			// X軸方向の線
			coordinateLineR.drawLine(
				glm::vec3(-limit, 0.1f, pos),
				glm::vec3(limit, 0.1f, pos),
				glm::vec3(1, 0, 0)
			);
			// Z軸方向の線
			coordinateLineR.drawLine(
				glm::vec3(pos, 0.1f, -limit),
				glm::vec3(pos, 0.1f, limit),
				glm::vec3(1, 0, 0)
			);
		}

		// ===== 軸線の描画 =====
		LineRenderer axis(Camera::cam);
		axis.init();

		float axisLength = interval * clNum / 2.0f;
		float offset = 0.2f;

		// --- 線の太さを設定 (デフォルトは 1.0f) ---
		glLineWidth(5.0f);

		// --- X軸 (赤) ---
		axis.drawLine(glm::vec3(0.0f, offset, 0.0f), glm::vec3(axisLength, offset, 0.0f), glm::vec3(1, 0, 0));

		// --- Y軸 (緑) ---
		axis.drawLine(glm::vec3(0.0f, offset, 0.0f), glm::vec3(0.0f, axisLength + offset, 0.0f), glm::vec3(0, 1, 0));

		// --- Z軸 (青) ---
		axis.drawLine(glm::vec3(0.0f, offset, 0.0f), glm::vec3(0.0f, offset, axisLength), glm::vec3(0, 0, 1));

		// 他の描画に影響しないよう、最後に太さを 1.0f に戻しておく
		glLineWidth(1.0f);


		// デス時処理
		FadeInOut::fadeInOut(black, TimeMgr::getDeltaTime(), 2.0f);

		GLFWWrap::deltaMouseXPos = GLFWWrap::deltaMouseYPos = 0.0f;

		//マウス処理
		if ( glfwGetKey(GLFWWrap::window, GLFW_KEY_ENTER) == GLFW_PRESS &&
			glfwGetKey(GLFWWrap::window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ) {
			isGameOver = true;
		}

		if ( glfwGetKey(GLFWWrap::window, GLFW_KEY_ESCAPE) == GLFW_PRESS ) {
			glfwSetWindowShouldClose(GLFWWrap::window, true);
		}

		// ダブルバッファ入れ替え
		glfwSwapBuffers(GLFWWrap::window);


	}

	//===== 7. 終了処理 =====
	GLFWWrap::deleteWindow();
	return 0;
}