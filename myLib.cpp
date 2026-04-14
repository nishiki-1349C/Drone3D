#include <iostream>
using namespace std;

float max3_myLib(float a, float b, float c) {
	return max(a, max(b, c));
}

float max4_myLib(float a, float b, float c, float d) {
	return max(max3_myLib(a, b, c), d);
}