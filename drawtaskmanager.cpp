#include "drawtaskmanager.h"

vector<DrawTask> DrawTaskManager::draw_tasks = vector<DrawTask>();

DrawTaskManager::DrawTaskManager()
{

}

DrawTaskManager::~DrawTaskManager()
{

}

vector<DrawTask>& DrawTaskManager::getDrawTasks()
{
    return draw_tasks;
}

void DrawTaskManager::addDrawTask(DrawTask task)
{
    draw_tasks.push_back(task);
}

void DrawTaskManager::addDrawTask(GLenum draw_mode, int num, GLfloat* vertices, GLfloat* colors, GLfloat* normals)
{
    Q_ASSERT(vertices != nullptr);

    DrawTask task(draw_mode, num, vertices, colors, normals);
    draw_tasks.push_back(task);
}

void DrawTaskManager::removeDrawTask(int index)
{
    Q_ASSERT(index >= 0 && index <= draw_tasks.size());

    draw_tasks.erase(begin(draw_tasks) + index);
}
