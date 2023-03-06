#include "glwidget.h"

#include <QString>
#include <QDebug>

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget{parent}
{
    cam_pos = QVector3D(0.0, 0.0, 1.0);
    cam_front = QVector3D(0.0, 0.0, -1.0);
    cam_up = QVector3D(0.0, 1.0, 0.0);

    setMouseTracking(true);
    cam_view_size = 30.0f;
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
//    glPointSize(8.0f);
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(1.5f);

    glClearColor(0.83f, 0.83f, 0.83f, 1.0f);

    // 创建着色器
    program = new QOpenGLShaderProgram(this);
    program->addShaderFromSourceFile(QOpenGLShader::Vertex,
                                     QString("./shader/shader.vert"));
    program->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                     QString("./shader/shader.frag"));
    program->link();

    col_attr = program->uniformLocation("ColAttr");
    use_nor = program->uniformLocation("UseNor");
    proj_mat = program->uniformLocation("ProjectionMatrix");
    view_mat = program->uniformLocation("ViewMatrix");
    if (col_attr == -1 || use_nor == -1 || proj_mat == -1 || view_mat == -1) {
        qDebug() << "Error: Read shader failed!";
    }

    // 绑定VAO & VBO & VNO
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

    // 初始化坐标系
    initCoordinate();

    // 设置相机
    cam_proj_mat.setToIdentity();
    cam_proj_mat.ortho(-cam_view_size, cam_view_size, -float(height())/width()*cam_view_size, float(height())/width()*cam_view_size, -100.0f, 100.0f);
    cam_view_mat.lookAt(cam_pos, cam_front, cam_up);
}

void GLWidget::resizeGL(int w, int h)
{
    this->glViewport(0, 0, w, h);
}

void GLWidget::paintGL()
{
    this->glClear(GL_COLOR_BUFFER_BIT);
}

void GLWidget::mousePressEvent(QMouseEvent* event)
{

}

void GLWidget::mouseMoveEvent(QMouseEvent* event)
{

}

void GLWidget::mouseReleaseEvent(QMouseEvent* event)
{

}

void GLWidget::initCoordinate()
{

}

void GLWidget::drawSphere(Sphere sphere)
{

}
