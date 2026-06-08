#include "LineRenderer.h"
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

GLuint LineRenderer::lineShader = 0;
bool LineRenderer::shaderInitialized = false;
std::vector<LineRenderer*> LineRenderer::allLineRenderers;
Camera* LineRenderer::cam = nullptr;

LineRenderer::LineRenderer() {
	allLineRenderers.push_back(this);
}

LineRenderer::~LineRenderer() {
	allLineRenderers.erase(
		std::remove(allLineRenderers.begin(), allLineRenderers.end(), this),
		allLineRenderers.end()
	);
	if ( VAO ) glDeleteVertexArrays(1, &VAO);
	if ( VBO ) glDeleteBuffers(1, &VBO);
}

void LineRenderer::init() {
	if ( !shaderInitialized ) {
		compileLineShader();
		shaderInitialized = true;
	}
	if ( !VAO ) glGenVertexArrays(1, &VAO);
	if ( !VBO ) glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (void*)offsetof(LineVertex, position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (void*)offsetof(LineVertex, color));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (void*)offsetof(LineVertex, width));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	cam = Camera::cam;
}

void LineRenderer::drawLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& color, float width) {
	LineVertex verts [2] = {
		{p1, color, width},
		{p2, color, width}
	};

	glLineWidth(width);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);
	glUseProgram(lineShader);
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);

	glLineWidth(1.0f);
}

glm::vec3 LineRenderer::worldToNDC(const glm::vec3& worldPos, Camera* cam) {
	glm::vec4 clipPos = cam->proj * cam->view * glm::vec4(worldPos, 1.0f);
	if ( std::abs(clipPos.w) < 1e-6f ) return glm::vec3(0.0f);
	return glm::vec3(clipPos) / clipPos.w;
}

void LineRenderer::drawLine3D(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& color, float width, Camera* cam) {
	if ( !cam ) return;
	glm::vec3 ndcP1 = worldToNDC(p1, cam);
	glm::vec3 ndcP2 = worldToNDC(p2, cam);
	drawLine(ndcP1, ndcP2, color, width);
}

void LineRenderer::drawAllLineRenderers() {
	for ( LineRenderer* lr : allLineRenderers ) {
		lr->draw();
	}
}

void LineRenderer::draw() {}

void LineRenderer::compileLineShader() {
	const char* vsSrc = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec3 aColor;
        layout(location = 2) in float aWidth;
        out vec3 vColor;
        out float vWidth;
        void main() {
            gl_Position = vec4(aPos, 1.0);
            vColor = aColor;
            vWidth = aWidth;
        }
    )";

	const char* fsSrc = R"(
        #version 330 core
        in vec3 vColor;
        in float vWidth;
        out vec4 FragColor;
        void main() { FragColor = vec4(vColor, 1.0); }
    )";

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vsSrc, nullptr);
	glCompileShader(vs);

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fsSrc, nullptr);
	glCompileShader(fs);

	lineShader = glCreateProgram();
	glAttachShader(lineShader, vs);
	glAttachShader(lineShader, fs);
	glLinkProgram(lineShader);
	glDeleteShader(vs);
	glDeleteShader(fs);
}