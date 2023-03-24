#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "paraminputdialog.h"
#include "mathutils.h"

#include <windows.h>

#include <QMessageBox>
#include <QDebug>
#include <QStringList>
#include <QStringListModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , s1(nullptr)
    , s2(nullptr)
{
    ui->setupUi(this);

    connect(this, SIGNAL(sendSphere2GLWidget(Sphere)), ui->openGLWidget, SLOT(addDrawTask(Sphere)));
    connect(this, SIGNAL(sendCircle3D2GLWidget(Circle3D)), ui->openGLWidget, SLOT(addDrawTask(Circle3D)));
    connect(this, SIGNAL(sendPoint2GLWidget(QVector3D)), ui->openGLWidget, SLOT(addDrawTask(QVector3D)));
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
    emit sendSphere2GLWidget(sphere);
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

        QStringList string_list;
        QString time_str = QString::fromStdString("求交耗时(ms): " + to_string(time));
        string_list.append(time_str);
        string_list.append("---------------------------------------------------------");
        string_list.append("求交结果");

        switch (type) {
        case 0: {
            string_list.append("两球不相交");

            QStringListModel* list_model = new QStringListModel(string_list);
            ui->listView->setModel(list_model);
        } break;
        case 1: {
            QVector3D p(res[0], res[1], res[2]);
            emit sendPoint2GLWidget(p);

            string_list.append("两球交于一点");
            QString coordinate = QString::fromStdString("坐标: (" + to_string(p.x()) + ", " + to_string(p.y()) + ", " + to_string(p.z()) + ")");
            string_list.append(coordinate);

            QStringListModel* list_model = new QStringListModel(string_list);
            ui->listView->setModel(list_model);
        } break;
        case 2: {
            Circle3D c(res[0], res[1], res[2], res[3], res[4], res[5], res[6]);
            emit sendCircle3D2GLWidget(c);

            string_list.append("两球交于一圆");
            QString center = QString::fromStdString("圆心: (" + to_string(c.center().x()) + ", " + to_string(c.center().y()) + ", " + to_string(c.center().z()) + ")");
            QString normal = QString::fromStdString("法向: (" + to_string(c.normal().x()) + ", " + to_string(c.normal().y()) + ", " + to_string(c.normal().z()) + ")");
            QString radius = QString::fromStdString("半径: " + to_string(c.radius()));
            string_list.append(center);
            string_list.append(normal);
            string_list.append(radius);

            QStringListModel* list_model = new QStringListModel(string_list);
            ui->listView->setModel(list_model);
        } break;
        case 3: {
            string_list.append("两球重合");

            QStringListModel* list_model = new QStringListModel(string_list);
            ui->listView->setModel(list_model);
        } break;
        }
    }
    else {
        QMessageBox::warning(this,
                             tr("提示"),
                             tr("不足两个球面，无法求交"));
    }
}

