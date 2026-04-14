#include <cmath>
#include "Vector3.h"
//三次元ベクトル

//漬物石

//演算子オーバーロード Vector3(a,b,c)+ Vector3(d,e,f)
Vector Vector::operator+(const Vector& other) const { return Vector(x + other.x, y + other.y, z + other.z); }
Vector Vector::operator-(const Vector& other) const { return Vector(x - other.x, y - other.y, z - other.z); }
Vector Vector::operator*(float scalar) const { return Vector(x * scalar, y * scalar, z * scalar); }
Vector Vector::operator/(float scalar) const { return Vector(x / scalar, y / scalar, z / scalar); }

//代入演算子 Vector3(a,b,c) += Vector3(d,e,f)
Vector& Vector::operator+=(const Vector& other) { x += other.x; y += other.y; z += other.z; return *this; }
Vector& Vector::operator-=(const Vector& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
Vector& Vector::operator*=(float scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
Vector& Vector::operator/=(float scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; }

//ベクトル大きさ
float Vector::magnitude(const Vector& a, const Vector& b) { return sqrt(x * x + y * y + z * z); }
//内積 a・b
float Vector::dot(const Vector& a, const Vector& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
//外積 a×b
Vector Vector::cross(const Vector& a, const Vector& b) {
	return Vector(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	);
}

