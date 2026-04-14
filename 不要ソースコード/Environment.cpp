/*#include "Environment.h"
using namespace std;

// static ÉÅÉìÉoíËã`
std::vector<Environment*> Environment::allEnvironments;

Environment::Environment(ShapeType type,
	glm::vec3 color,
	glm::vec3& size,
	glm::vec3& pos)
	: Object(type, color, size, pos) {
	allEnvironments.push_back(this);
}

Environment::Environment(ShapeType type,
	std::string name,
	glm::vec3 color,
	glm::vec3& size,
	glm::vec3& pos)
	: Object(type, name, color, size, pos) {
	allEnvironments.push_back(this);
}

Environment::~Environment() {
	auto it = std::find(allEnvironments.begin(), allEnvironments.end(), this);
	if ( it != allEnvironments.end() ) {
		allEnvironments.erase(it);
	}
}
*/