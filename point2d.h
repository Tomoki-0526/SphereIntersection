#ifndef POINT2D_H
#define POINT2D_H


// 二维点类（主要用于uv坐标）
class Point2D
{
private:
    double _u;
    double _v;

public:
    // 构造与析构
    Point2D();
    Point2D(const double u, const double v);
    ~Point2D();

    // 获取参数
    double u() const;
    double v() const;

    // 设置参数
    void setU(const double u);
    void setV(const double v);
};



#endif // POINT2D_H
