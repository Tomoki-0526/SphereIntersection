#include "circle3d.h"

Circle3D::Circle3D()
    : _r(0)
{

}

Circle3D::Circle3D(QVector3D c, QVector3D n, float r)
    : _c(c)
    , _n(n.normalized())
    , _r(r)
{

}

Circle3D::Circle3D(float x, float y, float z, float nx, float ny, float nz, float r)
    : _c(QVector3D(x, y, z))
    , _n(QVector3D(nx, ny, nz).normalized())
    , _r(r)
{

}

Circle3D::~Circle3D()
{

}

QVector3D Circle3D::center() const
{
    return _c;
}

QVector3D Circle3D::normal() const
{
    return _n;
}

float Circle3D::radius() const
{
    return _r;
}
