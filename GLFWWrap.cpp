#include "GLFWWrap.h"
using namespace std;

// 静的メンバの実体定義
GLFWwindow* GLFWWrap::window = nullptr;
float GLFWWrap::deltaMouseXPos = 0.0f;
float GLFWWrap::deltaMouseYPos = 0.0f;
bool  GLFWWrap::initMouse = true;
float GLFWWrap::lastXPos = 0.0f;
float GLFWWrap::lastYPos = 0.0f;

// ウィンドウ生成
int GLFWWrap::createWindow(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share) {
	if ( !glfwInit() ) {
		return -1;
	}
	GLFWWrap::window = glfwCreateWindow(width, height, title, monitor, share);
	if ( !window ) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	return 0;
}

// バッファの初期化
void GLFWWrap::clearWindowBuffer(glm::vec3 BGColor, float alpha) {
	glClearColor(BGColor.x, BGColor.y, BGColor.z, alpha);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// ウィンドウバッファ更新
void GLFWWrap::refreshWindowBuffer() {
	glfwSwapBuffers(window);
}

// ウィンドウ破棄
void GLFWWrap::deleteWindow() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

// 深度テスト・透過度の初期設定
void GLFWWrap::init() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// マウスの移動量を取得する
void GLFWWrap::senseMouseMove(GLFWwindow*, double xPos, double yPos) {

	//if ( isGameOver || isRespawned ) { return; }
	float x = static_cast<float>(xPos);
	float y = -static_cast<float>(yPos);
	// これがないと初手で膨大な数値になりカメラが暴走する
	if ( initMouse ) {
		lastXPos = x;
		lastYPos = y;
		initMouse = false;
	}

	deltaMouseXPos += x - lastXPos;
	deltaMouseYPos += y - lastYPos;
	lastXPos = x;
	lastYPos = y;
	//cout << "(x,y)" << deltaMouseXPos << ", " << deltaMouseYPos << endl;
}