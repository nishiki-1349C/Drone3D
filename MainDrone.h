#pragma once
#include "Object.h"
#include "GLFWWrap.h"
#include "TimeMgr.h"
#include <glm/glm.hpp>

//���C���@��
class MainDrone : public Object {
public:
	//�R���X�g���N�^
	MainDrone(ShapeType shape, ObjectTag tag, ObjectType act, glm::vec3 color, glm::vec3 size, glm::vec3 pos);
	~MainDrone() override final;

	void update();//�������Z
	void onCollision(Object* other) override final;
	static MainDrone* mainDrone;

	glm::vec3 lastPos,
		droneVelocity = glm::vec3(0, 0, 0);

	static glm::vec3 initPos;

private:
	void move();			//�ړ�����
	void operate();			//������͏���
	//void calcDroneVelocity();//���t���[�����x�v�Z

	static glm::vec3 forward;
	static glm::vec3 right;


	bool isOverSpeed;
	float acceleration = 30.0f;
	float upAcceleration = 2.0f;
	float drag = 0.75f;												//�����ʏ�̌���
	float horizontalSpeedLimit = 30.0f;
	float fallSpeedLimit = 30.0f;
	float mainDroneSpeedMin = 0.1f;
	float crashSpeedThreshold = 15.0f;
	const glm::vec3 gravity = glm::vec3(0, -9.8, 0);
	float gravityScale;
};


