#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "paraminputdialog.h"
#include "mathutils.h"
#include "circle3d.h"

#include <windows.h>

#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , s1(nullptr)
    , s2(nullptr)
{
    ui->setupUi(this);

    connect(this, SIGNAL(sendDataToGLWidget(Sphere)), ui->openGLWidget, SLOT(addDrawTask(Sphere)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawSphereByGLWidget(Sphere sphere)
{
    if (!s1) {
        s1 = new Sphere(sphere);
    }
    else if (!s2) {
        s2 = new Sphere(sphere);
    }
    emit sendDataToGLWidget(sphere);
}

void MainWindow::on_actionDrawSphere_triggered()
{
    if (!s1 || !s2) {
        ParamInputDialog* dlg = new ParamInputDialog(this);

        dlg->setWindowTitle(QStringLiteral("输入球参数（球心坐标&半径）"));
        dlg->exec();
        delete dlg;
    }
    else {
        QMessageBox::warning(this,
                             tr("提示"),
                             tr("最多输入两个球面"));
    }
}

void MainWindow::on_actionIntersection_triggered()
{
    if (s1 && s2) {
        vector<float> res;

        LARGE_INTEGER start_time, end_time, frequency;
        QueryPerformanceFrequency(&frequency);
        QueryPerformanceFrequency(&start_time);

        int type = intersectSphereSphere(res, s1, s2);

        QueryPerformanceCounter(&end_time);
        double time = 0.001 * (double)(end_time.QuadPart - start_time.QuadPart) / (double)frequency.QuadPart;

        qDebug() << "求交耗时(ms): " << time;

        switch (type) {
        case 0: {
            qDebug() << "两球不相交";
        } break;
        case 1: {
            QVector3D p(res[0], res[1], res[2]);
            qDebug() << "两球交于一点";
            qDebug() << "交点: (" << p.x() << "," << p.y() << "," << p.z() << ")";
        } break;
        case 2: {
            Circle3D c(res[0], res[1], res[2], res[3], res[4], res[5], res[6]);
            qDebug() << "两球交于一圆";
            qDebug() << "圆心: (" << c.center().x() << "," << c.center().y() << "," << c.center().z() << ")";
            qDebug() << "法向: (" << c.normal().x() << "," << c.normal().y() << "," << c.normal().z() << ")";
            qDebug() << "半径: " << c.radius();
        } break;
        case 3: {
            qDebug() << "两球重合";
        } break;
        }
    }
    else {
        QMessageBox::warning(this,
                             tr("提示"),
                             tr("不足两个球面，无法求交"));
    }
}

