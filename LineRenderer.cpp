#include "LineRenderer.h"
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
using namespace std;
using namespace glm;

GLuint LineRenderer::lineShader = 0;
bool LineRenderer::shaderInitialized = false;
vector<LineRenderer*> LineRenderer::allLineRenderers;
Camera* LineRenderer::cam = nullptr;

LineRenderer::LineRenderer() {
	allLineRenderers.push_back(this);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// GPUへ渡す頂点レイアウト: position(vec3) + color(vec3) = 6floats
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

LineRenderer::LineRenderer(bool st) {
	allLineRenderers.push_back(this);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// GPUへ渡す頂点レイアウト: position(vec3) + color(vec3) = 6floats
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	isDynamic = st;
}

LineRenderer::~LineRenderer() {
	EraseLR();
	allLineRenderers.erase(
		remove(allLineRenderers.begin(), allLineRenderers.end(), this),
		allLineRenderers.end()
	);
}

void LineRenderer::EraseLR() {
	if ( VAO ) glDeleteVertexArrays(1, &VAO);
	if ( VBO ) glDeleteBuffers(1, &VBO);
	VAO = 0;
	VBO = 0;
}

void LineRenderer::init() {
	if ( !shaderInitialized ) {
		compileLineShader();
		shaderInitialized = true;
	}
	cam = Camera::cam;
}

void LineRenderer::addLine(const vec2& p1, const vec2& p2, const vec3& color, float width) {
	lineVertices.push_back({vec3(p1, 0.0f), vec3(p2, 0.0f), color, width, false});
}

void LineRenderer::addLine3D(const vec3& p1, const vec3& p2, const vec3& color, float width) {
	lineVertices.push_back({p1, p2, color, width, true});
}

void LineRenderer::draw() {
	if ( lineVertices.empty() ) return;

	if ( !depthTest ) glDisable(GL_DEPTH_TEST);

	glUseProgram(lineShader);

	// VP行列をシェーダーに渡す
	mat4 vp = cam->proj * cam->view;
	glUniformMatrix4fv(glGetUniformLocation(lineShader, "uVP"), 1, GL_FALSE, &vp [0][0]);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	for ( auto& lv : lineVertices ) {
		glUniform1i(glGetUniformLocation(lineShader, "uIs3D"), lv.is3D ? 1 : 0);

		float verts [] = {
			lv.start.x, lv.start.y, lv.start.z, lv.color.r, lv.color.g, lv.color.b,
			lv.end.x,   lv.end.y,   lv.end.z,   lv.color.r, lv.color.g, lv.color.b
		};

		glLineWidth(lv.width);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_LINES, 0, 2);
	}

	glBindVertexArray(0);
	glLineWidth(1.0f);

	if ( !depthTest ) glEnable(GL_DEPTH_TEST);

	if ( isDynamic ) lineVertices.clear();		// 動的な場合
}


void LineRenderer::drawAllLineRenderers() {
	for ( LineRenderer* lr : allLineRenderers ) {
		lr->draw();
	}
}

void LineRenderer::compileLineShader() {
	const char* vsSrc = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec3 aColor;
        uniform mat4 uVP;
        uniform bool uIs3D;
        out vec3 vColor;
        void main() {
            gl_Position = uIs3D ? uVP * vec4(aPos, 1.0) : vec4(aPos, 1.0);
            vColor = aColor;
        }
    )";

	const char* fsSrc = R"(
        #version 330 core
        in vec3 vColor;
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

void LineRenderer::setInstance() {
	static LineRenderer grid;
	const float haba = 10.0f;
	const int lineNum = 30.0f;
	const float half = (lineNum - 1) * haba / 2.0f;
	const vec3 color(1.0f, 0.0f, 0.0f);
	const float y = 0.01f;

	for ( int i = 0; i < lineNum; i++ ) {
		float pos = i * haba - half;
		// X軸に平行な線
		grid.addLine3D(vec3(-half, y, pos), vec3(half, y, pos), color, 1.0f);
		// Z軸に平行な線
		grid.addLine3D(vec3(pos, y, -half), vec3(pos, y, half), color, 1.0f);
	}
}

void LineRenderer::update() {};