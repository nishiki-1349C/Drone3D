#pragma once
#include <unordered_map>
#include <glm/glm.hpp>

enum class EnemyType {
	mob,
	tower,
	miniBoss,
	boss
};

struct EnemyData {
	int       hp;
	int       attack;
	glm::vec3 color;
	glm::vec3 size;
};

// データ参照する関数
inline const std::unordered_map<EnemyType, EnemyData>& getEnemyDataTable() {
	static const std::unordered_map<EnemyType, EnemyData> table = {
		{ EnemyType::mob,      { 100, 10, glm::vec3(1,0,0), glm::vec3(2,2,2) } },
		{ EnemyType::tower,    { 300, 20, glm::vec3(0,1,0), glm::vec3(4,3,4) } },
		{ EnemyType::miniBoss, { 300, 25, glm::vec3(0,0,1), glm::vec3(5,5,5) } },
		{ EnemyType::boss,     { 800, 50, glm::vec3(1,0,1), glm::vec3(6,6,6) } },
	};
	return table;
}