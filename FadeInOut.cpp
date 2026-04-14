#include "FadeInOut.h"
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

// Renderer3D.cpp で定義されているグローバルカラーを使用
extern glm::vec3 white, black, blue;
// 静的メンバの実体定義
MainDrone* FadeInOut::drone = MainDrone::mainDrone;

// シェーダー・描画系
GLuint FadeInOut::VAO = 0, FadeInOut::VBO = 0, FadeInOut::shaderProgram = 0;
float FadeInOut::alpha = 1.0f;
float FadeInOut::fadeSpeed = 0.5f;

void FadeInOut::init() {
	initRenderData();
	compileShader();
}

void FadeInOut::fadeInOut(const glm::vec3& color, float deltaTime, float duration) {
	// 状態更新の簡略化
	float step = deltaTime / duration;

	if ( isRespawned ) {
		alpha = std::max(0.0f, alpha - step);
		if ( alpha <= 0.0f ) isGameOver = isRespawned = false;
	}
	else if ( isGameOver ) {
		alpha = std::min(1.0f, alpha + step);
		if ( alpha >= 1.0f ) FadeIn();
	}

	if ( alpha <= 0.0f ) return;

	// 描画設定（Renderer3Dのdrawと同様のパイプライン）
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	glUseProgram(shaderProgram);
	glUniform1f(glGetUniformLocation(shaderProgram, "uAlpha"), alpha);
	glUniform3fv(glGetUniformLocation(shaderProgram, "uColor"), 1, glm::value_ptr(color));

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void FadeInOut::FadeIn() {
	if ( drone ) {
		drone->currentPos = drone->initPos;
		drone->droneVelocity = glm::vec3(0);
	}
	isRespawned = true;
	isGameOver = false;
	alpha = 1.0f; // 真っ暗な状態からスタート

}

void FadeInOut::initRenderData() {
	float vertices [] = {
		-1.0f,  1.0f,  -1.0f, -1.0f,   1.0f, -1.0f,
		-1.0f,  1.0f,   1.0f, -1.0f,   1.0f,  1.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void FadeInOut::compileShader() {
	const char* vsSrc = R"(
        #version 330 core
        layout(location=0) in vec2 aPos;
        void main() { gl_Position = vec4(aPos, 0.0, 1.0); }
    )";

	const char* fsSrc = R"(
        #version 330 core
        out vec4 FragColor;
        uniform float uAlpha;
        uniform vec3 uColor;
        void main() { FragColor = vec4(uColor, uAlpha); }
    )";

	// Renderer3Dと同様のリンク手順
	auto compile = [] (GLenum type, const char* src) {
		GLuint s = glCreateShader(type);
		glShaderSource(s, 1, &src, nullptr);
		glCompileShader(s);
		return s;
		};

	GLuint vs = compile(GL_VERTEX_SHADER, vsSrc);
	GLuint fs = compile(GL_FRAGMENT_SHADER, fsSrc);
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vs);
	glAttachShader(shaderProgram, fs);
	glLinkProgram(shaderProgram);
	glDeleteShader(vs); glDeleteShader(fs);
}