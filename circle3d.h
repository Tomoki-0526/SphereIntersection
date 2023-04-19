#ifndef CIRCLE3D_H
#define CIRCLE3D_H

#include <QVector3D>

class Circle3D
{
public:
    Circle3D();
    Circle3D(QVector3D c, QVector3D n, float r);
    Circle3D(float x, float y, float z, float nx, float ny, float nz, float r);
    ~Circle3D();

    QVector3D center() const;
    QVector3D normal() const;
    float radius() const;
    QVector3D localX() const;
    QVector3D localY() const;
    void getPlane(float& a, float& b, float& c, float& d) const;
    QVector3D getXYZByT(const float t) const;

private:
    void getLocalCoor(QVector3D& p, QVector3D& q) const;

private:
    QVector3D _c;
    QVector3D _n;
    float _r;
    QVector3D _a;
    QVector3D _b;
};

#endif // CIRCLE3D_H
