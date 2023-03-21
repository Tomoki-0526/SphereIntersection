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
    static void addDrawTask(GLenum draw_mode, int num, GLfloat* vertices, GLfloat* colors, GLfloat* normals);
    static void removeDrawTask(int index);

private:
    static vector<DrawTask> draw_tasks;
};

#endif // DRAWTASKMANAGER_H
