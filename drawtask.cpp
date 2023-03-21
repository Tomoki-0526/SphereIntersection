#include "drawtask.h"

DrawTask::DrawTask()
    : m_drawMode(GL_TRIANGLES)
    , m_num(0)
    , m_vertices(nullptr)
    , m_colors(nullptr)
    , m_normals(nullptr)
{

}

DrawTask::DrawTask(GLenum draw_mode, int num, GLfloat* vertices, GLfloat* colors, GLfloat* normals)
    : m_drawMode(draw_mode)
    , m_num(num)
{
    if (num == 0) {
        m_vertices = nullptr;
        m_colors = nullptr;
        m_normals = nullptr;

        return;
    }

    m_vertices = new GLfloat[num * 3];
    for (int i = 0; i < num * 3; ++i) {
        m_vertices[i] = vertices[i];
    }

    if (colors) {
        m_colors = new GLfloat[num * 4];
        for (int i = 0; i < num * 4; ++i) {
            m_colors[i] = colors[i];
        }
    }
    else {
        m_colors = nullptr;
    }

    if (normals) {
        m_normals = new GLfloat[num * 3];
        for (int i = 0; i < num * 3; ++i) {
            m_normals[i] = normals[i];
        }
    }
    else {
        m_normals = nullptr;
    }
}

DrawTask::~DrawTask()
{
    if (m_vertices) {
        delete[] m_vertices;
        m_vertices = nullptr;
    }
    if (m_colors) {
        delete[] m_colors;
        m_colors = nullptr;
    }
    if (m_normals) {
        delete[] m_normals;
        m_normals = nullptr;
    }
}

GLenum DrawTask::getDrawMode() const
{
    return m_drawMode;
}

int DrawTask::getNum() const
{
    return m_num;
}

GLfloat* DrawTask::getVertices() const
{
    return m_vertices;
}

GLfloat* DrawTask::getColors() const
{
    return m_colors;
}

GLfloat* DrawTask::getNormals() const
{
    return m_normals;
}
