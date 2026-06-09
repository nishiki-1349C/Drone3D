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
	lineVertices.push_back({ vec3(p1, 0.0f), vec3(p2, 0.0f), color, width, false });
}

void LineRenderer::addLine3D(const vec3& p1, const vec3& p2, const vec3& color, float width) {
	lineVertices.push_back({ p1, p2, color, width, true });
}

void LineRenderer::draw() {
	if ( lineVertices.empty() ) return;

	if ( !depthTest ) glDisable(GL_DEPTH_TEST);

	glUseProgram(lineShader);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	for ( auto& lv : lineVertices ) {
		// 3Dラインはdraw()時にワールド座標→NDCへ変換（カメラ移動に追従）
		vec3 s = lv.is3D ? worldToNDC(lv.start, cam) : lv.start;
		vec3 e = lv.is3D ? worldToNDC(lv.end,   cam) : lv.end;

		float verts[] = {
			s.x, s.y, s.z, lv.color.r, lv.color.g, lv.color.b,
			e.x, e.y, e.z, lv.color.r, lv.color.g, lv.color.b
		};

		glLineWidth(lv.width);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_LINES, 0, 2);
	}

	glBindVertexArray(0);
	glLineWidth(1.0f);

	if ( !depthTest ) glEnable(GL_DEPTH_TEST);

	lineVertices.clear();
}

vec3 LineRenderer::worldToNDC(const vec3& worldPos, Camera* cam) {
	vec4 clipPos = cam->proj * cam->view * vec4(worldPos, 1.0f);
	if ( abs(clipPos.w) < 1e-6f ) return vec3(0.0f);
	return vec3(clipPos) / clipPos.w;
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
        out vec3 vColor;
        void main() {
            gl_Position = vec4(aPos, 1.0);
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
