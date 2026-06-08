#pragma once
//#include "FadeInOut.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

// ウィンドウ生成のラップ関数
class GLFWWrap {
public:
	static GLFWwindow* window;

	// ウィンドウ作成
	static int createWindow(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share);
	// 背景色と深度バッファのクリア
	static void clearWindowBuffer(glm::vec3 BGColor, float alpha);
	// ウィンドウのバッファ更新
	static void refreshWindowBuffer();
	// ウィンドウを閉じる
	static void deleteWindow();
	// 深度テスト・透過度の初期設定
	static void init();
	// マウス操作
	static void senseMouseMove(GLFWwindow* window, double xPos, double yPos);

	// ウィンドウの縦横
	static const int winWidth = 1200;
	static const int winHeight = 800;

	// マウスの移動量
	static float deltaMouseXPos;
	static float deltaMouseYPos;

private:
	//マウス操作
	static bool  initMouse;
	static float lastXPos;
	static float lastYPos;
};