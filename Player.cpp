#include "Player.h"

Player* Player::player = nullptr;

Player::Player(int hp, int score, int attack)
	: hp(hp)
	, score(score)
	, attack(initAttack) {
	player = this;
}



void Player::init() {
	score = 0;
	attack = initAttack;
}