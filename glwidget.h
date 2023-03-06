#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "sphere.h"

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include <QMouseEvent>
#include <QVector3D>
#include <QMatrix4x4>

class GLWidget : public QOpenGLWidget, public QOpenGLExtraFunctions
{
    Q_OBJECT

public:
    explicit GLWidget(QWidget *parent = nullptr);
    ~GLWidget();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

    void initCoordinate();

private slots:
    void drawSphere(Sphere sphere);

private:
    // 相机参数
    QVector3D cam_pos;
    QVector3D cam_front;
    QVector3D cam_up;
    QMatrix4x4 cam_proj_mat;
    QMatrix4x4 cam_view_mat;
    float cam_view_size;

    // 着色器
    QOpenGLShaderProgram* program;
    GLint col_attr = 0;
    GLint use_nor = 0;
    GLint proj_mat = 0;
    GLint view_mat = 0;

    // 缓冲区
    QOpenGLBuffer* vbo;
    QOpenGLVertexArrayObject* vao;
    QOpenGLBuffer* vno;
};

#endif // GLWIDGET_H
