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
    getLocalCoor(_a, _b);
}

Circle3D::Circle3D(float x, float y, float z, float nx, float ny, float nz, float r)
    : _c(QVector3D(x, y, z))
    , _n(QVector3D(nx, ny, nz).normalized())
    , _r(r)
{
    getLocalCoor(_a, _b);
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

QVector3D Circle3D::localX() const
{
    return _a;
}

QVector3D Circle3D::localY() const
{
    return _b;
}

void Circle3D::getPlane(float& a, float& b, float& c, float& d) const
{
    a = _n.x();
    b = _n.y();
    c = _n.z();

    float x = _c.x();
    float y = _c.y();
    float z = _c.z();

    d = -(a * x + b * y + c * z);
}

void Circle3D::getLocalCoor(QVector3D& p, QVector3D& q) const
{
    QVector3D i(1, 0, 0);
    p = QVector3D::crossProduct(_n, i).normalized();
    if (p.isNull()) {
        QVector3D j(0, 1, 0);
        p = QVector3D::crossProduct(_n, j).normalized();
    }
    q = QVector3D::crossProduct(_n, p).normalized();
}

QVector3D Circle3D::getXYZByT(const float t) const
{
    float x = _c.x() + _r * _a.x() * cos(t) + _r * _b.x() * sin(t);
    float y = _c.y() + _r * _a.y() * cos(t) + _r * _b.y() * sin(t);
    float z = _c.z() + _r * _a.z() * cos(t) + _r * _b.z() * sin(t);
    return QVector3D(x, y, z);
}
