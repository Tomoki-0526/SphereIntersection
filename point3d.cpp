#include "point3d.h"

Point3D::Point3D():
    _x(0),
    _y(0),
    _z(0)
{
}

Point3D::Point3D(const double x, const double y, const double z):
    _x(x),
    _y(y),
    _z(z)
{
}

Point3D::~Point3D()
{
}

double Point3D::x() const
{
    return _x;
}

double Point3D::y() const
{
    return _y;
}

double Point3D::z() const
{
    return _z;
}

void Point3D::setX(const double x)
{
    _x = x;
}

void Point3D::setY(const double y)
{
    _y = y;
}

void Point3D::setZ(const double z)
{
    _z = z;
}
