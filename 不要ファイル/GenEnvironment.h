#pragma once
/*
inline static void GenEncironment() {


	// 地面groundオブジェクト
	//(window, pos ,size)
	Environment ground(mainWin.window, "ground", glm::vec3(0, -50, 0), glm::vec3(1000.0f, 100.0f, 1000.0f));
	ground.addCubeVertices();
	Renderer3D groundR(&ground, &mainCam);
	groundR.addCubeVertices(groundColor);

	// 壁オブジェクト
	glm::vec3 wallColor = groundColor;

	// 前壁 (+Z方向)
	Environment wallFront(mainWin.window, "wall", glm::vec3(0, wallHeight / 2.0f, 200), glm::vec3(100.0f, wallHeight, 20.0f));
	wallFront.addCubeVertices();
	Renderer3D wallFrontR(&wallFront, &mainCam);
	wallFrontR.addCubeVertices(wallColor);

	// 後壁 (-Z方向)
	Environment wallBack(mainWin.window, "wall", glm::vec3(0, wallHeight / 2.0f, -30), glm::vec3(100.0f, wallHeight, 20.0f));
	wallBack.addCubeVertices();
	Renderer3D wallBackR(&wallBack, &mainCam);
	wallBackR.addCubeVertices(wallColor);

	// 左壁 (-X方向)
	Environment wallLeft(mainWin.window, "wall", glm::vec3(-50, wallHeight / 2.0f, 85), glm::vec3(20.0f, wallHeight, 230.0f));
	wallLeft.addCubeVertices();
	Renderer3D wallLeftR(&wallLeft, &mainCam);
	wallLeftR.addCubeVertices(wallColor);

	// 右壁 (+X方向)
	Environment wallRight(mainWin.window, "wall", glm::vec3(50, wallHeight / 2.0f, 85), glm::vec3(20.0f, wallHeight, 230.0f));
	wallRight.addCubeVertices();
	Renderer3D wallRightR(&wallRight, &mainCam);
	wallRightR.addCubeVertices(wallColor);



	glm::vec3 initPlColor = glm::vec3(0.8f, 0.8f, 0.8f);
	// 柱1
	Environment pillar1(mainWin.window, "pillar", glm::vec3(35, 7.5f, 60), glm::vec3(10.0f, 15.0f, 10.0f));
	pillar1.addCubeVertices();
	Renderer3D pillar1R(&pillar1, &mainCam);
	pillar1R.addCubeVertices(initPlColor);
	// 柱2
	Environment pillar2(mainWin.window, "pillar", glm::vec3(-35, 7.5f, 120), glm::vec3(10.0f, 15.0f, 10.0f));
	pillar2.addCubeVertices();
	Renderer3D pillar2R(&pillar2, &mainCam);
	pillar2R.addCubeVertices(initPlColor); // 鮮やかな赤


}*/