#ifndef MATHUTILS_H
#define MATHUTILS_H

#include "sphere.h"

#include <vector>

using namespace std;

int intersectSphereSphere(vector<float>& res, const Sphere* s1, const Sphere* s2);
int floatCmp(const float& a, const float& b);
bool isZero(const float& a);
bool isPositive(const float& a);
bool isNegative(const float& a);
bool isNonNegative(const float& a);
bool isNonPositive(const float& a);
float areaTriangle(const float& a, const float& b, const float& c);

#endif // MATHUTILS_H
