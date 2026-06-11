#pragma once
#include "Renderer3DComponent.h"
#include "Object.h"
#include "Camera.h"
#include "VisTypes.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <algorithm>

class Renderer3D {
public:
	Renderer3D(Object* obj);
	~Renderer3D();

	static void initAllRenderers();
	static void drawAllRenderers();
	void init();

	static void drawAllShadowOn();
	void drawShadowOn(const Object* receiver);// 影をreceiverオブジェクトの上面に投影する
	const Object& getObj() const { return obj; }

	static inline glm::vec3 lightPos = glm::vec3(0, 50.0f, 0);

	static inline std::vector<Renderer3D*> allRenderers;

protected:
	// obj.visVertices を GPU に転送する（init前にvisVerticesを構築しておくこと）

	void draw();
	glm::mat4 calculateModelMatrix() const;
	bool isInsideReceiver(const glm::vec3& pos, const Object* receiver) const;
	Object& obj;
	GLuint VAO = 0;
	GLuint VBO = 0;
	GLuint shadowVAO = 0;
	GLuint shadowVBO = 0;

	GLuint shaderProgram = 0;
	GLuint shadowShader = 0;

	static inline GLuint sharedShaderProgram = 0;
	static inline GLuint sharedShadowShader  = 0;
	static inline int    shaderRefCount      = 0;
};