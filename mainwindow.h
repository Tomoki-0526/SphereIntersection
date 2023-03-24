#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "sphere.h"
#include "circle3d.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void sendSphere2GLWidget(Sphere sphere);
    void sendCircle3D2GLWidget(Circle3D circle);
    void sendPoint2GLWidget(QVector3D point);

public slots:
    void drawSphereByGLWidget(Sphere sphere);

private slots:
    void on_actionDrawSphere_triggered();
    void on_actionIntersection_triggered();

private:
    Ui::MainWindow *ui;

    Sphere* s1;
    Sphere* s2;
};
#endif // MAINWINDOW_H
