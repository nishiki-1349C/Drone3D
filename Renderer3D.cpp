#include "Renderer3D.h"

// --- 静的メンバの定義 ---
std::vector<Renderer3D*> Renderer3D::allRenderers;
glm::vec3 Renderer3D::lightPos = glm::vec3(0, 50.0f, 0);
GLuint    Renderer3D::sharedShaderProgram = 0;
GLuint    Renderer3D::sharedShadowShader = 0;
int       Renderer3D::shaderRefCount = 0;

// 色定数の定義（Renderer3DComponent.h で extern しているので外部から参照可能）
glm::vec3 white = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 black = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 red = glm::vec3(1.0f, 0.2f, 0.2f);
glm::vec3 green = glm::vec3(0.2f, 1.0f, 0.2f);
glm::vec3 blue = glm::vec3(0.2f, 0.6f, 1.0f);
glm::vec3 skyBlue = glm::vec3(0.2f, 1.0f, 1.0f);
glm::vec3 yellow = glm::vec3(1.0f, 1.0f, 0.2f);

Renderer3D::Renderer3D(Object* obj)
	: obj(*obj) {
	if ( shaderRefCount == 0 ) { Renderer3DComponent::initShaders(sharedShaderProgram, sharedShadowShader); }
	shaderRefCount++;
	shaderProgram = sharedShaderProgram;
	shadowShader = sharedShadowShader;
	allRenderers.push_back(this);
}

Renderer3D::~Renderer3D() {
	allRenderers.erase(
		std::remove(allRenderers.begin(), allRenderers.end(), this),
		allRenderers.end()
	);
	Renderer3DComponent::cleanupMesh(VAO, VBO, shadowVAO, shadowVBO);
	shaderRefCount--;
	if ( shaderRefCount == 0 ) {
		glDeleteProgram(sharedShaderProgram);
		glDeleteProgram(sharedShadowShader);
		sharedShaderProgram = 0;
		sharedShadowShader = 0;
	}
}

void Renderer3D::init() {
	// obj.visVertices を GPU に転送（静的）
	Renderer3DComponent::setupMainMesh(VAO, VBO, &obj);
	Renderer3DComponent::setupShadowMesh(shadowVAO, shadowVBO);
}

// Object::getModelMatrix() に委譲
glm::mat4 Renderer3D::calculateModelMatrix() const {
	return obj.getModelMatrix();
}

void Renderer3D::draw() {
	glUseProgram(shaderProgram);
	glUniform3fv(
		glGetUniformLocation(shaderProgram, "uLightPos"),
		1,
		glm::value_ptr(lightPos)
	);
	Renderer3DComponent::drawMain(
		shaderProgram, VAO,
		calculateModelMatrix(), Camera::cam->view, Camera::cam->proj,
		obj.visVertices.size()
	);
}

void Renderer3D::drawAllShadowOn() {
	for ( Renderer3D* r : Renderer3D::allRenderers ) {
		if ( r->getObj().getObjType() == ObjectType::dynamic ) {
			for ( Object* env : Object::allEnvironments ) {
				r->drawShadowOn(env);
			}
		}
	}
}

void Renderer3D::drawShadowOn(const Object* receiver) {
	glm::vec3 lightDirW = glm::normalize(lightPos - obj.currentPos);
	glm::vec3 planePoint = receiver->currentPos + glm::vec3(0, receiver->size.y * 0.5f, 0);
	glm::mat4 modelMat = calculateModelMatrix();

	std::vector<glm::vec3> shadowVerts;
	shadowVerts.reserve(obj.visVertices.size());

	for ( const auto& vert : obj.visVertices ) {
		glm::vec3 worldPos = glm::vec3(modelMat * glm::vec4(vert.pos, 1.0f));
		float denom = glm::dot(lightDirW, glm::vec3(0, 1, 0));
		if ( std::abs(denom) < 1e-6f ) {
			shadowVerts.push_back(glm::vec3(0, -1000, 0));
			continue;
		}
		float t = glm::dot(planePoint - worldPos, glm::vec3(0, 1, 0)) / denom;
		glm::vec3 projPos = worldPos + lightDirW * t;

		if ( isInsideReceiver(projPos, receiver) ) {
			projPos.y += 0.003f; // Zファイティング防止
			shadowVerts.push_back(projPos);
		}
		else {
			shadowVerts.push_back(glm::vec3(0, -1000, 0)); // 範囲外は画面外に退避
		}
	}

	Renderer3DComponent::drawShadow(
		shadowShader, shadowVAO, shadowVBO,
		shadowVerts, Camera::cam->view, Camera::cam->proj
	);
}

// ゲッター経由でAABBoxにアクセス
bool Renderer3D::isInsideReceiver(const glm::vec3& pos, const Object* receiver) const {
	constexpr float margin = 0.01f;
	return (pos.x >= receiver->getAABBoxMin().x - margin &&
		pos.x <= receiver->getAABBoxMax().x + margin &&
		pos.z >= receiver->getAABBoxMin().z - margin &&
		pos.z <= receiver->getAABBoxMax().z + margin);
}

void Renderer3D::initAllRenderers() {
	for ( Renderer3D* r : allRenderers ) r->init();
}

void Renderer3D::drawAllRenderers() {
	for ( Renderer3D* r : allRenderers ) r->draw();
}