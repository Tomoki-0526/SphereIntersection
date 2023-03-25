#include "mathutils.h"
#include <cmath>

const float eps = 1e-7;

int intersectSphereSphere(vector<float>& res, const Sphere* s1, const Sphere* s2)
{
    QVector3D c1 = s1->center();
    float r1 = s1->radius();
    QVector3D c2 = s2->center();
    float r2 = s2->radius();

    float d = (c1 - c2).length();
    if (isZero(d) && floatCmp(r1, r2) == 0) {
        return 3;
    }

    if (floatCmp(d, r1 + r2) > 0) {
        return 0;
    }
    else if (floatCmp(d, r1 + r2) == 0) {
        QVector3D n = c2 - c1;
        n.normalize();

        QVector3D p1 = c1 + r1 * n;
        QVector3D p2 = c2 - r2 * n;
        QVector3D p = (p1 + p2) / 2;

        res.push_back(p.x());
        res.push_back(p.y());
        res.push_back(p.z());
        return 1;
    }
    else if (floatCmp(d, r1 + r2) < 0 && floatCmp(d, fabs(r1 - r2)) > 0) {
        float cosθ1 = (r1 * r1 + d * d - r2 * r2) / (2 * r1 * d);
        float cosθ2 = (r2 * r2 + d * d - r1 * r1) / (2 * r2 * d);

        float d1 = r1 * cosθ1;
        float d2 = r2 * cosθ2;

        QVector3D n = c2 - c1;
        n.normalize();

        QVector3D p1 = c1 + d1 * n;
        QVector3D p2 = c2 - d2 * n;
        QVector3D p = (p1 + p2) / 2;

        float h = 2 * areaTriangle(r1, r2, d) / d;

        res.push_back(p.x());
        res.push_back(p.y());
        res.push_back(p.z());
        res.push_back(n.x());
        res.push_back(n.y());
        res.push_back(n.z());
        res.push_back(h);

        return 2;
    }
    else if (floatCmp(d, fabs(r1 - r2)) == 0) {
        QVector3D n = c1 - c2;
        if (floatCmp(r1, r2) > 0) {
            n = c2 - c1;
        }
        n.normalize();

        QVector3D p1 = c1 + r1 * n;
        QVector3D p2 = c2 + r2 * n;
        QVector3D p = (p1 + p2) / 2;

        res.push_back(p.x());
        res.push_back(p.y());
        res.push_back(p.z());
        return 1;
    }
    else {
        return 0;
    }
}

int floatCmp(const float& a, const float& b)
{
    if (fabs(a - b) <= eps) {
        return 0;
    }
    else if (a - b > eps) {
        return 1;
    }
    else {
        return -1;
    }
}

bool isZero(const float& a)
{
    return floatCmp(a, 0) == 0;
}

bool isNonNegative(const float& a)
{
    return floatCmp(a, 0) >= 0;
}

bool isNonPositive(const float& a)
{
    return floatCmp(a, 0) <= 0;
}

float areaTriangle(const float& a, const float& b, const float& c)
{
    float s = 0.5 * (a + b + c);
    return sqrt(s * (s - a) * (s - b) * (s - c));
}
