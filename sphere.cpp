#define _USE_MATH_DEFINES

#include "sphere.h"

#include <cmath>

Sphere::Sphere():
    _c(Point3D(0, 0, 0)),
    _r(0)
{
}

Sphere::Sphere(const Point3D& c, const double r):
    _c(c),
    _r(r)
{
}

Sphere::Sphere(const double x, const double y, const double z, const double r):
    _c(Point3D(x, y, z)),
    _r(r)
{
}

Sphere::~Sphere()
{
}

Point3D Sphere::center() const
{
    return _c;
}

double Sphere::radius() const
{
    return _r;
}

void Sphere::setCenter(const Point3D& c)
{
    _c.setX(c.x());
    _c.setY(c.y());
    _c.setZ(c.z());
}

void Sphere::setCenter(const double x, const double y, const double z)
{
    _c.setX(x);
    _c.setY(y);
    _c.setZ(z);
}

void Sphere::setRadius(const double r)
{
    _r = r;
}

Point2D Sphere::getUV(const Point3D& p)
{
    double u = atan2(p.y(), p.x()) + M_PI;
    double v = asin(p.z() / _r);

    return Point2D(u, v);
}
