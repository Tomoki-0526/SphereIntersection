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

void Circle3D::getGenaralEquationParams(std::vector<float>& params) const
{
    float nx = _n.x(), ny = _n.y(), nz = _n.z();
    float a = _c.x(), b = _c.y(), c = _c.z();
    float r = _r;

    params.push_back(nx - 2 * a);
    params.push_back(ny - 2 * b);
    params.push_back(nz - 2 * c);
    params.push_back(a * a + b * b + c * c - r * r - a * nx - b * ny - c * nz);
}
