#include "glwidget.h"
#include "drawtaskmanager.h"
#include "mathutils.h"

#include <cmath>

#include <QString>
#include <QDebug>
#include <QMatrix4x4>
#include <QDir>

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget{parent}
    , cam_pos(QVector3D(0.0, 0.0, 1.0))
    , cam_front(QVector3D(0.0, 0.0, -1.0))
    , cam_up(QVector3D(0.0, 1.0, 0.0))
    , view_size(20.0f)
    , rotate_with_center(true)
    , rotate_flag(false)
    , move_flag(false)
    , yaw(-90.0f)
    , pitch(0.0f)
{
    setMouseTracking(true);
}

GLWidget::~GLWidget()
{
    if (shader_program) {
        delete shader_program;
        shader_program = nullptr;
    }
    if (vao) {
        delete vao;
        vao = nullptr;
    }
    if (vbo) {
        delete vbo;
        vbo = nullptr;
    }
    if (vno) {
        delete vno;
        vno = nullptr;
    }
}

void GLWidget::initializeGL()
{
    this->initializeOpenGLFunctions();

    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(10.0f);
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(6.0f);

    glClearColor(0.83f, 0.83f, 0.83f, 1.0f);

//    qDebug() << QDir::currentPath();
//    printf("%s\n", glGetString(GL_VERSION));

    shader_program = new QOpenGLShaderProgram(this);
#ifdef QT_DEBUG
    shader_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "./debug/shader/v1.vert");
    shader_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "./debug/shader/v1.frag");
#else
    shader_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "./shader/v1.vert");
    shader_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "./shader/v1.frag");
#endif
//    shader_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "C:\\Users\\Tomoki\\OneDrive\\Homework\\CAD\\SphereIntersection\\shader\\v1.vert");
//    shader_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "C:\\Users\\Tomoki\\OneDrive\\Homework\\CAD\\SphereIntersection\\shader\\v1.frag");
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

    vector<DrawTask> draw_tasks = DrawTaskManager::getDrawTasks();
    for (auto& task : draw_tasks) {
        draw(task.getDrawMode(), task.getNum(), task.getVertices(), task.getColors(), task.getNormals());
    }

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
    else if (event->button() == Qt::RightButton) {
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

        float yaw_r = yaw * (M_PI / 180);
        float pitch_r = pitch * (M_PI / 180);

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
        view_size += delta * 0.004f;
        return;
    }
    cam_proj_mat.setToIdentity();
    cam_proj_mat.ortho(-view_size, view_size, -float(height()) / width() * view_size, float(height()) / width() * view_size, -100.0f, 100.0f);

    update();
}

void GLWidget::draw(GLenum draw_mode, int num, GLfloat* vertices, GLfloat* colors, GLfloat* normals)
{
    if (colors) {
        shader_program->setUniformValue(col_attr, QVector4D(colors[0], colors[1], colors[2], 0.5));
    }
    else {
        GLfloat* color = new GLfloat[4];
        for (int i = 0; i < 4; ++i) {
            color[i] = 0.2f;
        }
        shader_program->setUniformValue(col_attr, QVector4D(color[0], color[1], color[2], 0.5));
        delete[] color;
        color = nullptr;
    }

    vao->bind();
    if (normals) {
        vbo->bind();
        vbo->allocate(vertices, num * 3 * sizeof(GLfloat));

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

        shader_program->setUniformValue(use_nor, true);

        vno->bind();
        vno->allocate(normals, num * 3 * sizeof(GLfloat));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    }
    else {
        vbo->bind();
        vbo->allocate(vertices, num * 3 * sizeof(GLfloat));

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    }

    glDrawArrays(draw_mode, 0, num);

    if (normals) {
        shader_program->setUniformValue(use_nor, false);
        vno->release();
    }
    vbo->release();
    vao->release();
}

void GLWidget::addDrawTask(Sphere sphere)
{
    QVector3D center = sphere.center();
    float radius = sphere.radius();
    int resolution = 100;

    int num_vert = resolution * resolution;
    float* vert = new float[num_vert * 3];
    float* nor = new float[num_vert * 3];

    for (int col = 0; col < resolution; col++) {
        for (int row = 0; row < resolution; row++) {
            QMatrix4x4 rot_mat;
            rot_mat.rotate(float(col) / resolution * 360, QVector3D(0.0, 0.0, 1.0));
            rot_mat.rotate(float(row) / resolution * 360, QVector3D(0.0, 1.0, 0.0));
            QVector3D ret = rot_mat.map(QVector3D(1.0, 0.0, 0.0)).normalized();
            vert[(col * resolution + row) * 3] = center.x() + ret.x() * radius;
            vert[(col * resolution + row) * 3 + 1] = center.y() + ret.y() * radius;
            vert[(col * resolution + row) * 3 + 2] = center.z() + ret.z() * radius;

            nor[(col * resolution + row) * 3] = ret.x();
            nor[(col * resolution + row) * 3 + 1] = ret.y();
            nor[(col * resolution + row) * 3 + 2] = ret.z();
        }
    }

    int num_tri = 2 * resolution * resolution;

    int num_indices = num_tri * 3;
    float* vertices = new float[num_indices * 3];
    int* indices = new int[num_indices];

    float* normals = new float[num_indices * 3];

    for (int col = 0; col < resolution - 1; col++) {
        for (int row = 0; row < resolution - 1; row++) {
            indices[(col * resolution + row) * 6] = col * resolution + row;
            indices[(col * resolution + row) * 6 + 1] = col * resolution + row + 1;
            indices[(col * resolution + row) * 6 + 2] = col * resolution + row + resolution;
            indices[(col * resolution + row) * 6 + 3] = col * resolution + row + 1;
            indices[(col * resolution + row) * 6 + 4] = col * resolution + row + resolution + 1;
            indices[(col * resolution + row) * 6 + 5] = col * resolution + row + resolution;

            for (int k = 0; k < 3; k++) {
                vertices[((col * resolution + row) * 6) * 3 + k] = vert[(col * resolution + row) * 3 + k];
                vertices[((col * resolution + row) * 6 + 1) * 3 + k] = vert[(col * resolution + row + 1) * 3 + k];
                vertices[((col * resolution + row) * 6 + 2) * 3 + k] = vert[(col * resolution + row + resolution) * 3 + k];
                vertices[((col * resolution + row) * 6 + 3) * 3 + k] = vert[(col * resolution + row + 1) * 3 + k];
                vertices[((col * resolution + row) * 6 + 4) * 3 + k] = vert[(col * resolution + row + resolution + 1) * 3 + k];
                vertices[((col * resolution + row) * 6 + 5) * 3 + k] = vert[(col * resolution + row + resolution) * 3 + k];

                normals[((col * resolution + row) * 6) * 3 + k] = nor[(col * resolution + row) * 3 + k];
                normals[((col * resolution + row) * 6 + 1) * 3 + k] = nor[(col * resolution + row + 1) * 3 + k];
                normals[((col * resolution + row) * 6 + 2) * 3 + k] = nor[(col * resolution + row + resolution) * 3 + k];
                normals[((col * resolution + row) * 6 + 3) * 3 + k] = nor[(col * resolution + row + 1) * 3 + k];
                normals[((col * resolution + row) * 6 + 4) * 3 + k] = nor[(col * resolution + row + resolution + 1) * 3 + k];
                normals[((col * resolution + row) * 6 + 5) * 3 + k] = nor[(col * resolution + row + resolution) * 3 + k];
            }
        }
    }

    for (int col = 0; col < resolution - 1; col++) {
        indices[(col * resolution + resolution - 1) * 6] = col * resolution + resolution - 1;
        indices[(col * resolution + resolution - 1) * 6 + 1] = col * resolution;
        indices[(col * resolution + resolution - 1) * 6 + 2] = col * resolution + resolution - 1 + resolution;

        indices[(col * resolution + resolution - 1) * 6 + 3] = col * resolution;
        indices[(col * resolution + resolution - 1) * 6 + 4] = (col + 1) * resolution;
        indices[(col * resolution + resolution - 1) * 6 + 5] = col * resolution + resolution - 1 + resolution;

        for (int k = 0; k < 3; k++) {
            vertices[((col * resolution + resolution - 1) * 6) * 3 + k] = vert[(col * resolution + resolution - 1) * 3 + k];
            vertices[((col * resolution + resolution - 1) * 6 + 1) * 3 + k] = vert[(col * resolution) * 3 + k];
            vertices[((col * resolution + resolution - 1) * 6 + 2) * 3 + k] = vert[(col * resolution + resolution - 1 + resolution) * 3 + k];
            vertices[((col * resolution + resolution - 1) * 6 + 3) * 3 + k] = vert[(col * resolution) * 3 + k];
            vertices[((col * resolution + resolution - 1) * 6 + 4) * 3 + k] = vert[((col + 1) * resolution) * 3 + k];
            vertices[((col * resolution + resolution - 1) * 6 + 5) * 3 + k] = vert[(col * resolution + resolution - 1 + resolution) * 3 + k];

            normals[((col * resolution + resolution - 1) * 6) * 3 + k] = nor[(col * resolution + resolution - 1) * 3 + k];
            normals[((col * resolution + resolution - 1) * 6 + 1) * 3 + k] = nor[(col * resolution) * 3 + k];
            normals[((col * resolution + resolution - 1) * 6 + 2) * 3 + k] = nor[(col * resolution + resolution - 1 + resolution) * 3 + k];
            normals[((col * resolution + resolution - 1) * 6 + 3) * 3 + k] = nor[(col * resolution) * 3 + k];
            normals[((col * resolution + resolution - 1) * 6 + 4) * 3 + k] = nor[((col + 1) * resolution) * 3 + k];
            normals[((col * resolution + resolution - 1) * 6 + 5) * 3 + k] = nor[(col * resolution + resolution - 1 + resolution) * 3 + k];
        }
    }

    for (int row = 0; row < resolution - 1; row++) {
        indices[((resolution - 1) * resolution + row) * 6] = (resolution - 1) * resolution + row;
        indices[((resolution - 1) * resolution + row) * 6 + 1] = (resolution - 1) * resolution + row + 1;
        indices[((resolution - 1) * resolution + row) * 6 + 2] = row;

        indices[((resolution - 1) * resolution + row) * 6 + 3] = (resolution - 1) * resolution + row + 1;
        indices[((resolution - 1) * resolution + row) * 6 + 4] = row + 1;
        indices[((resolution - 1) * resolution + row) * 6 + 5] = row;

        for (int k = 0; k < 3; k++) {
            vertices[(((resolution - 1) * resolution + row) * 6) * 3 + k] = vert[((resolution - 1) * resolution + row) * 3 + k];
            vertices[(((resolution - 1) * resolution + row) * 6 + 1) * 3 + k] = vert[((resolution - 1) * resolution + row + 1) * 3 + k];
            vertices[(((resolution - 1) * resolution + row) * 6 + 2) * 3 + k] = vert[row * 3 + k];
            vertices[(((resolution - 1) * resolution + row) * 6 + 3) * 3 + k] = vert[((resolution - 1) * resolution + row + 1) * 3 + k];
            vertices[(((resolution - 1) * resolution + row) * 6 + 4) * 3 + k] = vert[(row + 1) * 3 + k];
            vertices[(((resolution - 1) * resolution + row) * 6 + 5) * 3 + k] = vert[(row)*3 + k];

            normals[(((resolution - 1) * resolution + row) * 6) * 3 + k] = nor[((resolution - 1) * resolution + row) * 3 + k];
            normals[(((resolution - 1) * resolution + row) * 6 + 1) * 3 + k] = nor[((resolution - 1) * resolution + row + 1) * 3 + k];
            normals[(((resolution - 1) * resolution + row) * 6 + 2) * 3 + k] = nor[row * 3 + k];
            normals[(((resolution - 1) * resolution + row) * 6 + 3) * 3 + k] = nor[((resolution - 1) * resolution + row + 1) * 3 + k];
            normals[(((resolution - 1) * resolution + row) * 6 + 4) * 3 + k] = nor[(row + 1) * 3 + k];
            normals[(((resolution - 1) * resolution + row) * 6 + 5) * 3 + k] = nor[(row)*3 + k];
        }
    }

    for (int k = 0; k < 3; k++) {
        vertices[((resolution * resolution - 1) * 6) * 3 + k] = vert[(resolution * resolution - 1) * 3 + k];
        vertices[(((resolution - 1) * resolution + resolution - 1) * 6 + 1) * 3 + k] = vert[((resolution - 1) * resolution) * 3 + k];
        vertices[(((resolution - 1) * resolution + resolution - 1) * 6 + 2) * 3 + k] = vert[(resolution - 1) * 3 + k];
        vertices[(((resolution - 1) * resolution + resolution - 1) * 6 + 3) * 3 + k] = vert[((resolution - 1) * resolution) * 3 + k];
        vertices[(((resolution - 1) * resolution + resolution - 1) * 6 + 4) * 3 + k] = vert[k];
        vertices[(((resolution - 1) * resolution + resolution - 1) * 6 + 5) * 3 + k] = vert[(resolution - 1) * 3 + k];

        normals[((resolution * resolution - 1) * 6) * 3 + k] = nor[(resolution * resolution - 1) * 3 + k];
        normals[(((resolution - 1) * resolution + resolution - 1) * 6 + 1) * 3 + k] = nor[((resolution - 1) * resolution) * 3 + k];
        normals[(((resolution - 1) * resolution + resolution - 1) * 6 + 2) * 3 + k] = nor[(resolution - 1) * 3 + k];
        normals[(((resolution - 1) * resolution + resolution - 1) * 6 + 3) * 3 + k] = nor[((resolution - 1) * resolution) * 3 + k];
        normals[(((resolution - 1) * resolution + resolution - 1) * 6 + 4) * 3 + k] = nor[k];
        normals[(((resolution - 1) * resolution + resolution - 1) * 6 + 5) * 3 + k] = nor[(resolution - 1) * 3 + k];
    }

    float colors[4] = { 0.54f, 0.65f, 1.0f, 0.5f };
    DrawTaskManager::addDrawTask(GL_TRIANGLES, num_indices, vertices, colors, normals);

    delete[] nor;
    nor = nullptr;
    delete[] vert;
    vert = nullptr;
    delete[] vertices;
    vertices = nullptr;
    delete[] normals;
    normals = nullptr;
    delete[] indices;
    indices = nullptr;
}

void GLWidget::addDrawTask(Circle3D circle)
{
    QVector3D center = circle.center();
    QVector3D normal = circle.normal();
    float radius = circle.radius();
    float R = radius + 0.01;

    QVector3D dir;
    if (isZero(normal.y())) {
        dir = QVector3D(0, 1, 0);
    }
    else {
        dir = QVector3D(1, -normal.x() / normal.y(), 0).normalized();
    }

    int resolution = 180;
    float* vertices = new float[resolution * 3];
    for (int i = 0; i < resolution; ++i) {
        QMatrix4x4 rot_mat;
        rot_mat.rotate(float(i) / resolution * 360, normal);
        QVector3D v = rot_mat.map(dir).normalized();

        vertices[i * 3] = center.x() + v.x() * R;
        vertices[i * 3 + 1] = center.y() + v.y() * R;
        vertices[i * 3 + 2] = center.z() + v.z() * R;
    }

    DrawTaskManager::addDrawTask(GL_LINE_LOOP, resolution, vertices);
}

void GLWidget::addDrawTask(QVector3D point)
{
    float* vertices = new float[3];
    for (int i = 0; i < 3; ++i) {
        vertices[i] = point[i];
    }

    DrawTaskManager::addDrawTask(GL_POINTS, 1, vertices);
}

void GLWidget::removeAllDrawTask()
{
    while (!DrawTaskManager::empty()) {
        DrawTaskManager::removeDrawTask(0);
    }

    update();
}
