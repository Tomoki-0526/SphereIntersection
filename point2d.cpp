#include "point2d.h"

Point2D::Point2D():
    _u(0),
    _v(0)
{
}

Point2D::Point2D(const double u, const double v):
    _u(u),
    _v(v)
{
}

Point2D::~Point2D()
{
}

double Point2D::u() const
{
    return _u;
}

double Point2D::v() const
{
    return _v;
}

void Point2D::setU(const double u)
{
    _u = u;
}

void Point2D::setV(const double v)
{
    _v = v;
}
