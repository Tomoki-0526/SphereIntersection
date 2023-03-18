#include "glwidget.h"

#include <cmath>

#include <QString>
#include <QDebug>

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget{parent}
{
    cam_pos = QVector3D(0.0, 0.0, 1.0);
    cam_front = QVector3D(0.0, 0.0, -1.0);
    cam_up = QVector3D(0.0, 1.0, 0.0);

    setMouseTracking(true);
    view_size = 20.0f;

    rotate_with_center = true;
    rotate_flag = false;
    move_flag = false;
}

GLWidget::~GLWidget()
{
}

void GLWidget::initializeGL()
{
    this->initializeOpenGLFunctions();

    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(10.0f);
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(2.0f);

    glClearColor(0.83f, 0.83f, 0.83f, 1.0f);

    shader_program = new QOpenGLShaderProgram(this);
    shader_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "C:\\Users\\Tomoki\\OneDrive\\Homework\\CAD\\SphereIntersection\\shader\\v1.vert");
    shader_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "C:\\Users\\Tomoki\\OneDrive\\Homework\\CAD\\SphereIntersection\\shader\\v1.frag");
    shader_program->link();

    col_attr = shader_program->uniformLocation("ColAttr");
    use_nor = shader_program->uniformLocation("UseNor");
    projection_matrix = shader_program->uniformLocation("ProjectionMatrix");
    view_matrix = shader_program->uniformLocation("ViewMatrix");
    if (col_attr == -1 || use_nor == -1 || projection_matrix == -1 || view_matrix == -1) {
        qDebug() << "read shader error";
    }

    vao = new QOpenGLVertexArrayObject();
    vao->create();
    vao->bind();

    vbo = new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer);
    vbo->create();
    vbo->bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    vbo->release();

    vno = new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer);
    vno->create();
    vno->bind();
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    GLfloat nor_data;
    vno->allocate(&nor_data, sizeof(GLfloat));
    vno->release();

    vao->release();

    cam_proj_mat.setToIdentity();
    cam_proj_mat.ortho(-view_size, view_size, -float(height()) / width() * view_size, float(height()) / width() * view_size, -100.0f, 100.0f);
    cam_view_mat.lookAt(cam_pos, cam_pos + cam_front, cam_up);
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    shader_program->bind();
    shader_program->setUniformValue(projection_matrix, cam_proj_mat);
    shader_program->setUniformValue(view_matrix, cam_view_mat);

    // TODO: 绘制

    vao->release();
    shader_program->release();
}

void GLWidget::mousePressEvent(QMouseEvent* event)
{
    // 左键移动视角
    if (event->button() == Qt::LeftButton) {
        move_flag = true;
        last_mouse_pos = event->pos();
    }
    // 右键旋转视角
    if (event->button() == Qt::RightButton) {
        rotate_flag = true;
        last_mouse_pos = event->pos();
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
    // 移动
    if (move_flag) {
        QVector3D right_offset = QVector3D::crossProduct(cam_front, cam_up);
        QVector3D up_offset = QVector3D::crossProduct(right_offset, cam_front);

        float dx = (event->pos().x() - last_mouse_pos.x());
        float dy = (last_mouse_pos.y() - event->pos().y());
        QVector3D offset = dx * right_offset + dy * up_offset;

        cam_pos -= offset * 0.01f;
        cam_view_mat.translate(offset * 0.01f);

        update();
        last_mouse_pos = event->pos();
    }
    // 旋转
    else if (rotate_flag) {
        yaw += (event->pos().x() - last_mouse_pos.x()) * 0.05;
        pitch += (last_mouse_pos.y() - event->pos().y()) * 0.05;
        pitch = pitch > 89.0f ? 89.0f : pitch;
        pitch = pitch < -89.0f ? -89.0f : pitch;

        float yaw_r = yaw * (3.14159f / 180);
        float pitch_r = pitch * (3.14159f / 180);

        if (rotate_with_center) {
            QVector3D center = cam_pos + cam_front * 3;
            cam_front[0] = -cos(yaw_r) * cos(-pitch_r);
            cam_front[1] = sin(-pitch_r);
            cam_front[2] = -sin(yaw_r) * cos(-pitch_r);
            cam_front = -cam_front;
            cam_pos = center - cam_front * 3;

            cam_view_mat.setToIdentity();
            cam_view_mat.lookAt(cam_pos, cam_pos + cam_front, cam_up);
        } else {
            cam_front[0] = cos(yaw_r) * cos(pitch_r);
            cam_front[1] = sin(pitch_r);
            cam_front[2] = sin(yaw_r) * cos(pitch_r);

            cam_view_mat.setToIdentity();
            cam_view_mat.lookAt(cam_pos, cam_pos + cam_front, cam_up);
        }

        update();
        last_mouse_pos = event->pos();
    }
}

void GLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    // 移动
    if (move_flag) {
        move_flag = false;
    }

    // 旋转
    if (rotate_flag) {
        rotate_flag = false;
    }
}

void GLWidget::wheelEvent(QWheelEvent* event)
{
    float delta = event->angleDelta().y();
    view_size -= delta * 0.002f;
    if (view_size < 1.0f || view_size > 50.0f){
        view_size += delta * 0.002f;
        return;
    }
    cam_proj_mat.setToIdentity();
    cam_proj_mat.ortho(-view_size, view_size, -float(height()) / width() * view_size, float(height()) / width() * view_size, -100.0f, 100.0f);

    update();
}

void GLWidget::drawSphere(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLint M, GLint N)
{
    float step_z = M_PI / M;
    float step_xy = 2 * M_PI / N;
    float xx[4], yy[4], zz[4];

    float angle_z = 0.0;
    float angle_xy = 0.0;
    int i = 0, j = 0;
    glBegin(GL_QUADS);
    for(i = 0; i < M; i++)
    {
        angle_z = i * step_z;

        for(j = 0; j < N; j++)
        {
            angle_xy = j * step_xy;

            xx[0] = r * sin(angle_z) * cos(angle_xy);
            yy[0] = r * sin(angle_z) * sin(angle_xy);
            zz[0] = r * cos(angle_z);

            xx[1] = r * sin(angle_z + step_z) * cos(angle_xy);
            yy[1] = r * sin(angle_z + step_z) * sin(angle_xy);
            zz[1] = r * cos(angle_z + step_z);

            xx[2] = r * sin(angle_z + step_z) * cos(angle_xy + step_xy);
            yy[2] = r * sin(angle_z + step_z) * sin(angle_xy + step_xy);
            zz[2] = r * cos(angle_z + step_z);

            xx[3] = r * sin(angle_z) * cos(angle_xy + step_xy);
            yy[3] = r * sin(angle_z) * sin(angle_xy + step_xy);
            zz[3] = r * cos(angle_z);

            for(int k = 0; k < 4; k++)
            {
                glColor3f(0.30f, 0.66f, 0.26f);
                glVertex3f(x + xx[k], y + yy[k], z + zz[k]);
            }
        }
    }
    glEnd();
}
