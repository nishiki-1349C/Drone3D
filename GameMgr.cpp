#include "GameMgr.h"
#include "Blaster.h"
#include "Camera.h"
#include "Sequence.h"

// 静的メンバの実体定義
gameState GameMgr::state = gameState::loading;
float GameMgr::loadTime = 1.0f;
float GameMgr::loadTimer = GameMgr::loadTime;
bool GameMgr::isGameOver = false;

void GameMgr::doPhase() {
	switch ( state ) {
		case gameState::loading:
			//now loading...の表示

			// ゲームの初期化処理（オブジェクトの生成やリソースの読み込みなど）をここで行う

			loadTimer -= TimeMgr::getDeltaTime();	// ローディング処理

			loadTime -= TimeMgr::getDeltaTime();//リアルタイムでloadtimeだけ待機
			if ( loadTimer >= loadTime ) {
				loadTimer = 0.0f;
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


				Camera::cam->update();			//カメラ更新
			}
			if ( isGameOver ) {				// ゲームオーバーに遷移
				state = gameState::gameOver;
				isGameOver = false;
			}
			break;
		case gameState::gameOver:
			//半透明の黒でresult表示
			break;
		default:
			break;
	}
}

void GameMgr::resetGame() {

}
