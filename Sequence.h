#pragma once
#include "Camera.h"
#include "MainDrone.h"

//初期化や物理の更新など実行される処理をまとめるクラス

class Sequence {
public:
	static void init();//初期化 メインループ前に一度だけ呼び出す

	static void mainLoop();//メインループ
	static void checkAllCollision();//全てのオブジェクトに対してAABB衝突判定を行う
	static bool checkCollision(Object* a, Object* b);//めり込み検出
	static bool checkTouching(Object* a, Object* b); //めり込みはなく表面の接触
	static void update();//全てのオブジェクトの物理的更新を行う
	//static void renderUpdate();//全てのオブジェクトの描画処理を行う


	//static Camera* cam;
	//static MainDrone* drone;

private:
	static void setInstance();//Camera,Object, Rendererの生成
	static void initRenderes();//描画用頂点の生成 
};