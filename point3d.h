#ifndef POINT3D_H
#define POINT3D_H


// 三维点类
class Point3D
{
private:
    double _x;
    double _y;
    double _z;

public:
    // 构造与析构
    Point3D();
    Point3D(const double x, const double y, const double z);
    ~Point3D();

    // 获取参数
    double x() const;
    double y() const;
    double z() const;

    // 设置坐标
    void setX(const double x);
    void setY(const double y);
    void setZ(const double z);
};

#endif // POINT3D_H
