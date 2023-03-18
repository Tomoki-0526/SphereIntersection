#ifndef SPHERE_H
#define SPHERE_H

#include <QVector3D>
#include <QVector2D>

// 球面类
class Sphere
{
private:
    QVector3D _c;
    float _r;

public:
    // 构造与析构
    Sphere();
    Sphere(const QVector3D& c, const float r);
    Sphere(const float x, const float y, const float z, const float r);
    ~Sphere();

    // 获取参数
    QVector3D center() const;
    float radius() const;

    // 设置参数
    void center(const QVector3D& c);
    void center(const float x, const float y, const float z);
    void radius(const float r);

    // uv坐标
    QVector2D getUV(const QVector3D& p);	// 计算球面上一点的uv坐标
};


#endif // SPHERE_H