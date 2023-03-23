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

private:
    QVector3D _c;
    QVector3D _n;
    float _r;
};

#endif // CIRCLE3D_H
