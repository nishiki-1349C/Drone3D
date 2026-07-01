#include "GameMgr.h"
#include "Blaster.h"
#include "Camera.h"
#include "EnemyMgr.h"
#include "FadeInOut.h"
#include "Player.h"
#include "Sequence.h"
#include "LineRenderer.h"

using namespace std;

// 現在のgameStateを出力する（doPhaseのロジックから独立）
void GameMgr::printState() {
	switch ( state ) {
		case gameState::none:		cout << "gameState: none" << endl;		break;
		case gameState::ready:		cout << "gameState: ready" << endl;		break;
		case gameState::loading:	cout << "gameState: loading" << endl;	break;
		case gameState::ingame:		cout << "gameState: ingame" << endl;	break;
		case gameState::gameOver:	cout << "gameState: gameOver" << endl;	break;
		default:													break;
	}
}

void GameMgr::doPhysicalPhase() {
	//printState();	// 現在の状態を出力
	//int n = EnemyMgr::allEnemies.size();

	cout << Player::player->hp << endl;

	switch ( state ) {
		case gameState::loading:
			//now loading...の表示

			// ゲームの初期化処理（オブジェクトの生成やリソースの読み込みなど）をここで行う

			loadTimer -= TimeMgr::getDeltaTime();	// ローディング処理

			if ( shouldGameReset ) {
				resetGame();
				shouldGameReset = false;	// リセットは loading 中に一度のみ
				isGameOver = false;
			}

			if ( loadTimer <= 0 ) {
				state = gameState::ingame;
			}
			break;
		case gameState::ingame:
			// ゲームプレイ中の処理
			while ( TimeMgr::shouldUpdate() ) {
				TimeMgr::consumeFixedStep();	// 固定ステップ分の時間を消費

				Sequence::update();				// 全オブジェクトの物理更新
				Sequence::checkAllCollision();	// AABB更新と衝突判定


				Blaster::shot();				// 弾の発射
				Blaster::checkDeadBullets();	//弾の削除
				EnemyMgr::checkDeadEnemy();		//死亡した敵の削除


				Camera::cam->update();			//カメラ更新

				if ( Player::player->hp <= 0 ) { state = gameState::gameOver; }
				if ( glfwGetKey(GLFWWrap::window, GLFW_KEY_G) == GLFW_PRESS && glfwGetKey(GLFWWrap::window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ) { state = gameState::gameOver; }
			}
			break;
		case gameState::gameOver:
			if ( glfwGetKey(GLFWWrap::window, GLFW_KEY_ENTER) == GLFW_PRESS ) {
				state = gameState::loading;
				shouldGameReset = true;
			}

			break;
		default:
			break;
	}
}

void GameMgr::doVisualPhase() {
	switch ( state ) {
		case gameState::loading:
			//now loading...の表示
			FadeInOut::fill(black, 1.0f);	// 完全に塗りつぶして画面を隠す
			break;
		case gameState::ingame:	break;
		case gameState::gameOver:
			FadeInOut::fill(black, 0.6f);	// 物理停止状態で薄い黒を重ねる
			break;
		default:
			break;
	}
}

void GameMgr::resetGame() {
	loadTimer = loadTime;	// loading開始時にローディング時間をリセット
	Player::player->init();
	MainDrone::mainDrone->currentPos = MainDrone::initPos;
	EnemyMgr::deleteAllEnemies();
	EnemyMgr::setInstance(1);
	Camera::cam->Reset();	//カメラを初期状態へ（ドローンを初期位置へ戻した後）
}