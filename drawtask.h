#ifndef DRAWTASK_H
#define DRAWTASK_H

#include <QOpenGLFunctions>

class DrawTask
{
public:
    DrawTask();
    DrawTask(GLenum draw_mode, int num, GLfloat* vertices, GLfloat* colors, GLfloat* normals);
    ~DrawTask();

    GLenum getDrawMode() const;
    int getNum() const;
    GLfloat* getVertices() const;
    GLfloat* getColors() const;
    GLfloat* getNormals() const;

private:
    GLenum m_drawMode;
    int m_num;
    GLfloat* m_vertices;
    GLfloat* m_colors;
    GLfloat* m_normals;
};

#endif // DRAWTASK_H
