#pragma once
#include "Object.h"
//#include "Bullet.h"
#include "Camera.h"
#include "FadeInOut.h"
#include "GLFWWrap.h"
#include "TimeMgr.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

//メイン機体
class MainDrone : public Object {
public:
	//コンストラクタ
	MainDrone(ShapeType shape, ObjectTag tag, ObjectType act, glm::vec3 color, glm::vec3 size, glm::vec3 pos);
	~MainDrone() override final;

	void update();//物理演算
	void onCollision(Object* other) override final;
	static MainDrone* mainDrone;

	glm::vec3 lastPos,
		droneVelocity = glm::vec3(0, 0, 0);

	static glm::vec3 initPos;

private:
	void move();			//移動処理
	void operate();			//操作入力処理
	//void calcDroneVelocity();//毎フレーム速度計算

	static glm::vec3 forward;
	static glm::vec3 right;


	bool isOverSpeed;
	float acceleration = 30.0f;
	float upAcceleration = 2.0f;
	float drag = 0.75f;												//水平面上の減速
	float horizontalSpeedLimit = 30.0f;
	float fallSpeedLimit = 30.0f;
	float mainDroneSpeedMin = 0.1f;
	float crashSpeedThreshold = 15.0f;
	const glm::vec3 gravity = glm::vec3(0, -9.8, 0);
	float gravityScale;
};


