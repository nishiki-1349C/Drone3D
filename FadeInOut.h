#pragma once
#include "Camera.h"
#include "Renderer3D.h"
#include <glm/glm.hpp>

// 前方宣言（循環参照防止）
class MainDrone;

static bool isGameOver = false;
static bool isRespawned = true;

extern const float wallHeight;

class FadeInOut {
public:
	// 引数 MainDrone* を追加 static
	static void init();
	static void fadeInOut(const glm::vec3& color, float deltaTime, float duration);

private:
	// static 関数から触るため static に変更
	static MainDrone* drone;
	static GLuint VAO;
	static GLuint VBO;
	static GLuint shaderProgram;
	static float alpha;
	static float fadeSpeed;

	static void FadeIn(); // 位置リセット用（必要なら）
	static void compileShader();
	static void initRenderData();
};