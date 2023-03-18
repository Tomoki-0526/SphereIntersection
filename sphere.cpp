#define _USE_MATH_DEFINES

#include "sphere.h"

#include <cmath>

Sphere::Sphere()
    : _c(QVector3D(0, 0, 0))
    , _r(0)
{
}

Sphere::Sphere(const QVector3D& c, const float r)
    : _c(c)
    , _r(r)
{
}

Sphere::Sphere(const float x, const float y, const float z, const float r)
    : _c(QVector3D(x, y, z))
    , _r(r)
{
}

Sphere::~Sphere()
{
}

QVector3D Sphere::center() const
{
    return _c;
}

float Sphere::radius() const
{
    return _r;
}

void Sphere::center(const QVector3D& c)
{
    _c.setX(c.x());
    _c.setY(c.y());
    _c.setZ(c.z());
}

void Sphere::center(const float x, const float y, const float z)
{
    _c.setX(x);
    _c.setY(y);
    _c.setZ(z);
}

void Sphere::radius(const float r)
{
    _r = r;
}

QVector2D Sphere::getUV(const QVector3D& p)
{
    float u = atan2(p.y(), p.x()) + M_PI;
    float v = asin(p.z() / _r);

    return QVector2D(u, v);
}
