#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include <QMouseEvent>
#include <QWheelEvent>
#include <QVector3D>
#include <QMatrix4x4>
#include <QTimer>
#include <QPoint>

#include <vector>

using namespace std;

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

    // 鼠标事件
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;

    // 绘制
    void drawSphere(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLint M = 100, GLint N = 100);

private:
    // 相机
    QVector3D cam_pos;
    QVector3D cam_front;
    QVector3D cam_up;
    QMatrix4x4 cam_proj_mat;
    QMatrix4x4 cam_view_mat;
    float view_size;

    // 鼠标控制
    bool rotate_with_center;
    bool rotate_flag;
    bool move_flag;
    float yaw;
    float pitch;
    QPoint last_mouse_pos;

    // 光照


    // 着色器
    QOpenGLShaderProgram* shader_program;

    QOpenGLBuffer* vbo;
    QOpenGLVertexArrayObject* vao;
    QOpenGLBuffer* vno;

    GLint col_attr = 0;
    GLint use_nor = 0;
    GLint projection_matrix = 0;
    GLint view_matrix = 0;
};

#endif // GLWIDGET_H
