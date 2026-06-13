#pragma once
#include "Object.h"
#include "GLFWWrap.h"
#include "TimeMgr.h"
#include <glm/glm.hpp>

// メインドローン
class MainDrone : public Object {
public:
	// コンストラクタ
	MainDrone(ShapeType shape, ObjectTag tag, ObjectType act, glm::vec3 color, glm::vec3 size, glm::vec3 pos);
	~MainDrone() override final;

	void update();
	void onCollision(Object* other) override final;
	static inline MainDrone* mainDrone = nullptr;

	glm::vec3 lastPos,
		droneVelocity = glm::vec3(0, 0, 0);

	static inline glm::vec3 initPos = glm::vec3(0, 40, 0);

private:
	void move();
	void operate();

	static inline glm::vec3 forward = glm::vec3(0, 0, 1);
	static inline glm::vec3 right   = glm::vec3(1, 0, 0);

	bool isOverSpeed;
	float acceleration = 30.0f;
	float upAcceleration = 2.0f;
	float drag = 0.75f;
	float horizontalSpeedLimit = 30.0f;
	float fallSpeedLimit = 30.0f;
	float mainDroneSpeedMin = 0.1f;
	float crashSpeedThreshold = 15.0f;
	const glm::vec3 gravity = glm::vec3(0, -9.8, 0);
	float gravityScale;
};

