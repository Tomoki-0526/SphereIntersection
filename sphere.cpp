#define _USE_MATH_DEFINES

#include "sphere.h"
#include "mathutils.h"

#include <cmath>

Sphere::Sphere()
    : _r(0)
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
    float u = isNonNegative(p.y() - _c.y()) ? atan2f(p.y() - _c.y(), p.x() - _c.x()) : atan2f(p.y() - _c.y(), p.x() - _c.x()) + 2 * M_PI;
    float v = asinf((p.z() - _c.z()) / _r);

    return QVector2D(u, v);
}
