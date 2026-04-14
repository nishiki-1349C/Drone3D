#include "CalcVertices.h"
#include "Object.h"
#include <cmath>
using namespace glm;

// 三角形 1 枚分の Vertex 3 個を追加（法線を外積で計算）
void CalcVertices::pushTriangle(Object* obj,
	const  vec3& v0,
	const  vec3& v1,
	const  vec3& v2,
	const  vec3& color) {
	vec3 A = v2 - v0;
	vec3 B = v1 - v0;

	//法線は半時計周りの頂点に対する外積　（外積： 左手系（x+右、y+上、z+奥）なのでA×Bに対して左ねじの方向）
	vec3 normal = normalize(cross(A, B));
	obj->visVertices.push_back({v0, normal, color});
	obj->visVertices.push_back({v1, normal, color});
	obj->visVertices.push_back({v2, normal, color});
}

// 四角形一枚を三角形二枚で表現
void CalcVertices::pushQuad(Object* obj,
	const  vec3& v0,
	const  vec3& v1,
	const  vec3& v2,
	const  vec3& v3,
	const  vec3& color) {
	CalcVertices::pushTriangle(obj, v0, v1, v2, color);
	CalcVertices::pushTriangle(obj, v0, v2, v3, color);
}

// Box の 8 頂点（ローカル座標、中心が原点）
std::vector<vec3> CalcVertices::calcVertices_box(vec3 size) {
	std::vector<vec3> temp;
	float width = size.x * 0.5f;
	float height = size.y * 0.5f;
	float depth = size.z * 0.5f;

	//上面　y軸周りに第一から四象限の順
	temp.push_back(vec3(width, height, depth)); // v0
	temp.push_back(vec3(-width, height, depth)); // v1
	temp.push_back(vec3(-width, height, -depth)); // v2
	temp.push_back(vec3(width, height, -depth)); // v3

	//下面　y軸周りに第一から四象限の順
	temp.push_back(vec3(width, -height, depth)); // v4
	temp.push_back(vec3(-width, -height, depth)); // v5
	temp.push_back(vec3(-width, -height, -depth)); // v6
	temp.push_back(vec3(width, -height, -depth)); // v7
	return temp;
}

// 球の頂点計算用
std::vector<vec3> CalcVertices::calcVertices_sphere(float radius, int verNum) {
	float dTheta = PI / verNum;
	float dPhi = PI / verNum;

	std::vector<vec3> temp;


	//緯線上に頂点は 2*verNum
	for ( float theta = 0; theta < 2.0f * PI; theta += dTheta ) {
		// 経線上に頂点は verNum -1  +2 （北極と南極）
		for ( float phi = dPhi; phi < PI; phi += dPhi ) {
			// 三次元極座標からデカルト座標への変換
			float x = radius * std::sin(phi) * std::cos(theta);
			float y = radius * std::cos(phi);
			float z = radius * std::sin(phi) * std::sin(theta);

			temp.push_back(vec3(x, y, z));
		}
	}

	//temp[n-2] = 北極  temp[n-1] = 南極
	temp.push_back(vec3(0, radius, 0));   // 北極
	temp.push_back(vec3(0, -radius, 0));  // 南極
	return temp;
}

// Box の 6 面を visVertices に追加
void CalcVertices::addVisVertices_box(Object* obj) {
	std::vector<vec3> tempVertices = calcVertices_box(obj->size);
	vec3 color = obj->color;

	// 上面 (Y+)
	pushQuad(obj, tempVertices [0], tempVertices [1], tempVertices [2], tempVertices [3], color);
	// 下面 (Y-)
	pushQuad(obj, tempVertices [4], tempVertices [7], tempVertices [6], tempVertices [5], color);
	// 背面 (Z+)
	pushQuad(obj, tempVertices [2], tempVertices [1], tempVertices [5], tempVertices [6], color);
	// 前面 (Z-)
	pushQuad(obj, tempVertices [0], tempVertices [3], tempVertices [7], tempVertices [4], color);
	// 右面 (X+)
	pushQuad(obj, tempVertices [1], tempVertices [0], tempVertices [4], tempVertices [5], color);
	// 左面 (X-)
	pushQuad(obj, tempVertices [3], tempVertices [2], tempVertices [6], tempVertices [7], color);
}

// 球の visVertices を追加
void CalcVertices::addVisVertices_sphere(Object* obj, int verNum) {
	std::vector<vec3> temp = calcVertices_sphere(obj->size.x * 0.5f, verNum);
	vec3 color(obj->color);

	int phiNum = verNum - 1; //極点除いた頂点数は経線上で verNum-1 個
	int thetaNum = 2 * verNum; //緯線上の頂点数は 2*verNum 個
	int north = (int)temp.size() - 2;
	int south = (int)temp.size() - 1;


	//θ緯線
	for ( int i = 0; i < thetaNum; i++ ) {
		//次の緯線が最終週でout of rangeになる対策
		int inext = (i + 1) % thetaNum;

		//最初の一回は北極からリングの頂点を三角形で結ぶ
		pushTriangle(obj,
			temp [north],
			temp [i * phiNum],
			temp [inext * phiNum],
			color);

		//φ経線
		//最初の頂点、最後の頂点
		for ( int j = 0; j < phiNum - 1; j++ ) {
			pushQuad(obj,
				temp [i * phiNum + j],
				temp [i * phiNum + j + 1],
				temp [inext * phiNum + j + 1],
				temp [inext * phiNum + j], color);
		}

		//最後の一回は南極からリングの頂点を三角形で結ぶ
		pushTriangle(obj,
			temp [south],
			temp [inext * phiNum + phiNum - 1],
			temp [i * phiNum + phiNum - 1],
			color);
	}
}
