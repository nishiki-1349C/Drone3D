#include "LineRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

GLuint LineRenderer::lineShader = 0;
bool LineRenderer::shaderInitialized = false;

LineRenderer::LineRenderer(Camera* cam) : cam(cam) {}

LineRenderer::~LineRenderer() {
	if ( VAO ) glDeleteVertexArrays(1, &VAO);
	if ( VBO ) glDeleteBuffers(1, &VBO);
}

void LineRenderer::init() {
	// シェーダー共有
	if ( !shaderInitialized ) {
		compileLineShader();
		shaderInitialized = true;
	}

	// VAO/VBOの初期化
	if ( !VAO ) glGenVertexArrays(1, &VAO);
	if ( !VBO ) glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// 頂点属性LineVertexの設定
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (void*)offsetof(LineVertex, position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (void*)offsetof(LineVertex, color));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void LineRenderer::drawLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& color) {
	if ( !cam ) return;

	LineVertex verts [2] = {
		{p1, color},
		{p2, color}
	};
	drawLineInternal(verts, 2);
}

void LineRenderer::drawLineInternal(const LineVertex* verts, int count) {
	if ( !cam ) return;

	// バッファデータの更新
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(LineVertex) * count, verts, GL_DYNAMIC_DRAW);

	// シェーダーとVAOのバインド
	glUseProgram(lineShader);
	glBindVertexArray(VAO);

	// ユニフォーム変数の設定
	glUniformMatrix4fv(glGetUniformLocation(lineShader, "uView"), 1, GL_FALSE, glm::value_ptr(cam->view));
	glUniformMatrix4fv(glGetUniformLocation(lineShader, "uProj"), 1, GL_FALSE, glm::value_ptr(cam->proj));

	// 描画
	glDrawArrays(GL_LINES, 0, count);

	glBindVertexArray(0);
}

void LineRenderer::compileLineShader() {
	const char* vertexShaderSource = R"(
		#version 330 core
		layout(location = 0) in vec3 aPos;
		layout(location = 1) in vec3 aColor;
		
		uniform mat4 uView;
		uniform mat4 uProj;
		
		out vec3 vColor;
		
		void main() {
			gl_Position = uProj * uView * vec4(aPos, 1.0);
			vColor = aColor;
		}
	)";

	const char* fragmentShaderSource = R"(
		#version 330 core
		in vec3 vColor;
		
		out vec4 FragColor;
		
		void main() {
			FragColor = vec4(vColor, 1.0);
		}
	)";

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	lineShader = glCreateProgram();
	glAttachShader(lineShader, vertexShader);
	glAttachShader(lineShader, fragmentShader);
	glLinkProgram(lineShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}