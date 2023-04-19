#ifndef DRAWTASKMANAGER_H
#define DRAWTASKMANAGER_H

#include "drawtask.h"
#include <vector>

using namespace std;

class DrawTaskManager
{
public:
    DrawTaskManager();
    ~DrawTaskManager();

    static vector<DrawTask>& getDrawTasks();
    static void addDrawTask(DrawTask task);
    static void addDrawTask(GLenum draw_mode, int num, GLfloat* vertices, GLfloat* colors = nullptr, GLfloat* normals = nullptr);
    static void removeDrawTask(int index);
    static size_t size();
    static bool empty();

private:
    static vector<DrawTask> draw_tasks;
};

#endif // DRAWTASKMANAGER_H
