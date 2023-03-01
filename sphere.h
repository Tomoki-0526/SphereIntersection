#ifndef SPHERE_H
#define SPHERE_H
#include "Point2D.h"
#include "Point3D.h"


// 球面类
class Sphere
{
private:
    Point3D _c;
    double _r;

public:
    // 构造与析构
    Sphere();
    Sphere(const Point3D& c, const double r);
    Sphere(const double x, const double y, const double z, const double r);
    ~Sphere();

    // 获取参数
    Point3D center() const;
    double radius() const;

    // 设置参数
    void setCenter(const Point3D& c);
    void setCenter(const double x, const double y, const double z);
    void setRadius(const double r);

    // uv坐标
    Point2D getUV(const Point3D& p);	// 计算球面上一点的uv坐标
};


#endif // SPHERE_H
