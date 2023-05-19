#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "paraminputdialog.h"
#include "mathutils.h"

#include <chrono>

#include <QMessageBox>
#include <QDebug>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , s1(nullptr)
    , s2(nullptr)
    , list_model(nullptr)
{
    ui->setupUi(this);

    connect(this, SIGNAL(sendSphere2GLWidget(Sphere)), ui->openGLWidget, SLOT(addDrawTask(Sphere)));
    connect(this, SIGNAL(sendCircle3D2GLWidget(Circle3D)), ui->openGLWidget, SLOT(addDrawTask(Circle3D)));
    connect(this, SIGNAL(sendPoint2GLWidget(QVector3D)), ui->openGLWidget, SLOT(addDrawTask(QVector3D)));
    connect(this, SIGNAL(clear()), ui->openGLWidget, SLOT(removeAllDrawTask()));
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
        if (list_model) {
            delete list_model;
            list_model = nullptr;
        }

        auto begin = std::chrono::high_resolution_clock::now();

        vector<float> res;
        int type = intersectSphereSphere(res, s1, s2);

        QString c1 = QString::fromStdString("    球心: (" + to_string(s1->center().x()) + ", " + to_string(s1->center().y()) + ", " + to_string(s1->center().z()) + ")");
        QString r1 = QString::fromStdString("    半径: " + to_string(s1->radius()));
        QString c2 = QString::fromStdString("    球心: (" + to_string(s2->center().x()) + ", " + to_string(s2->center().y()) + ", " + to_string(s2->center().z()) + ")");
        QString r2 = QString::fromStdString("    半径: " + to_string(s2->radius()));

        QStringList string_list;
        string_list.append("球面参数");
        string_list.append("· 球面1");
        string_list.append(c1);
        string_list.append(r1);
        string_list.append("· 球面2");
        string_list.append(c2);
        string_list.append(r2);
        string_list.append("===============================");

        switch (type) {
        case 0: {
            string_list.append("求交结果: 两球不相交");
        } break;
        case 1: {
            QVector3D pt(res[0], res[1], res[2]);
            emit sendPoint2GLWidget(pt);

            QVector2D uv1 = s1->getPointUV(pt);
            QVector2D uv2 = s2->getPointUV(pt);

            string_list.append("求交结果: 两球交于一点");
            string_list.append(QString::fromStdString("· 坐标: (" + to_string(pt.x()) + ", " + to_string(pt.y()) + ", " + to_string(pt.z()) + ")"));
            string_list.append("· UV坐标");
            string_list.append(QString::fromStdString("    球1: (" + to_string(uv1.x()) + ", " + to_string(uv1.y()) + ")"));
            string_list.append(QString::fromStdString("    球2: (" + to_string(uv2.x()) + ", " + to_string(uv2.y()) + ")"));
        } break;
        case 2: {
            Circle3D circ(res[0], res[1], res[2], res[3], res[4], res[5], res[6]);
            emit sendCircle3D2GLWidget(circ);

            QVector3D o = circ.center();
            float r = circ.radius();
            float a, b, c, d;
            circ.getPlane(a, b, c, d);
            QVector3D local_x = circ.localX();
            QVector3D local_y = circ.localY();
            float k1, γ1, d1, k2, γ2, d2;
            bool is_vertical1 = s1->getCircleUV(circ, k1, γ1, d1);
            bool is_vertical2 = s2->getCircleUV(circ, k2, γ2, d2);

            string_list.append("求交结果: 两球交于一圆");
            string_list.append("· 基准平面");
            string plane_eq("    " + to_string(a) + "x");
            if (!isZero(b)) {
                if (isPositive(b)) {
                    plane_eq += "+";
                }
                plane_eq += to_string(b) + "y";
            }
            if (!isZero(c)) {
                if (isPositive(c)) {
                    plane_eq += "+";
                }
                plane_eq += to_string(c) + "z";
            }
            if (!isZero(d)) {
                if (isPositive(d)) {
                    plane_eq += "+";
                }
                plane_eq += to_string(d);
            }
            plane_eq += "\n    =0";
            string_list.append(QString::fromStdString(plane_eq));
            string_list.append("· 局部坐标系");
            string_list.append(QString::fromStdString("    原点: (" + to_string(o.x()) + ", " + to_string(o.y()) + ", " + to_string(o.z()) + ")"));
            string_list.append(QString::fromStdString("    x轴: (" + to_string(local_x.x()) + ", " + to_string(local_x.y()) + ", " + to_string(local_x.z()) + ")"));
            string_list.append(QString::fromStdString("    y轴: (" + to_string(local_y.x()) + ", " + to_string(local_y.y()) + ", " + to_string(local_y.z()) + ")"));
            string_list.append(QString::fromStdString("· 局部方程: x^2+y^2=" + to_string(r * r)));
            string_list.append("· 空间参数方程");
            string xt("    x(t)=" + to_string(o.x()));
            if (!isZero(r * local_x.x())) {
                if (isPositive(r * local_x.x())) {
                    xt += "+";
                }
                xt += to_string(r * local_x.x()) + "cos(t)";
            }
            if (!isZero(r * local_y.x())) {
                if (isPositive(r * local_y.x())) {
                    xt += "+";
                }
                xt += to_string(r * local_y.x()) + "sin(t)";
            }
            string yt("    y(t)=" + to_string(o.y()));
            if (!isZero(r * local_x.y())) {
                if (isPositive(r * local_x.y())) {
                    yt += "+";
                }
                yt += to_string(r * local_x.y()) + "cos(t)";
            }
            if (!isZero(r * local_y.y())) {
                if (isPositive(r * local_y.y())) {
                    yt += "+";
                }
                yt += to_string(r * local_y.y()) + "sin(t)";
            }
            string zt("    z(t)=" + to_string(o.z()));
            if (!isZero(r * local_x.z())) {
                if (isPositive(r * local_x.z())) {
                    zt += "+";
                }
                zt += to_string(r * local_x.z()) + "cos(t)";
            }
            if (!isZero(r * local_y.z())) {
                if (isPositive(r * local_y.z())) {
                    zt += "+";
                }
                zt += to_string(r * local_y.z()) + "sin(t)";
            }
            string_list.append(QString::fromStdString(xt));
            string_list.append(QString::fromStdString(yt));
            string_list.append(QString::fromStdString(zt));
            string_list.append("· UV参数曲线");
            string uv1 = "    球1: ";
            if (!is_vertical1) {
                if (!isZero(k1)) {
                    uv1 += to_string(k1 * s1->radius());
                }
                uv1 += "cos(v)sin(u";
                if (!isZero(γ1)) {
                    if (isPositive(γ1)) {
                        uv1 += "+";
                    }
                    uv1 += to_string(γ1);
                }
                uv1 += ")\n        -" + to_string(s1->radius()) + "sin(v)";
                if (!isZero(d1 * sqrtf(1 + k1 * k1))) {
                    if (isPositive(d1 * sqrtf(1 + k1 * k1))) {
                        uv1 += "+";
                    }
                    uv1 += to_string(d1 * sqrtf(1 + k1 * k1));
                }
                uv1 += "=0";
            }
            else {
                uv1 += to_string(s1->radius()) + "cos(v)sin(u";
                if (!isZero(γ1)) {
                    if (isPositive(γ1)) {
                        uv1 += "+";
                    }
                    uv1 += to_string(γ1);
                }
                uv1 += ")\n        +" + to_string(d1) + "=0";
            }
            string uv2 = "    球2: ";
            if (!is_vertical2) {
                if (!isZero(k2)) {
                    uv2 += to_string(k2 * s2->radius());
                }
                uv2 += "cos(v)sin(u";
                if (!isZero(γ2)) {
                    if (isPositive(γ2)) {
                        uv2 += "+";
                    }
                    uv2 += to_string(γ2);
                }
                uv2 += ")\n        -" + to_string(s2->radius()) + "sin(v)";
                if (!isZero(d2 * sqrtf(1 + k2 * k2))) {
                    if (isPositive(d2 * sqrtf(1 + k2 * k2))) {
                        uv2 += "+";
                    }
                    uv2 += to_string(d2 * sqrtf(1 + k2 * k2));
                }
                uv2 += "=0";
            }
            else {
                uv2 += to_string(s2->radius()) + "cos(v)sin(u";
                if (!isZero(γ2)) {
                    if (isPositive(γ2)) {
                        uv2 += "+";
                    }
                    uv2 += to_string(γ2);
                }
                uv2 += ")\n        +" + to_string(d2) + "=0";
            }
            string_list.append(QString::fromStdString(uv1));
            string_list.append(QString::fromStdString(uv2));
        } break;
        case 3: {
            string_list.append("求交结果：两球重合");
        } break;
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
        QString time_str = QString::fromStdString("求交耗时(ms): " + to_string(time.count() * 1e-6));
        string_list.append(time_str);

        list_model = new QStringListModel(string_list);
        ui->listView->setModel(list_model);
    }
    else {
        QMessageBox::warning(this,
                             tr("提示"),
                             tr("不足两个球面，无法求交"));
    }

    ui->openGLWidget->update();
}

void MainWindow::on_actionClear_triggered()
{
    if (s1) {
        delete s1;
        s1 = nullptr;
    }
    if (s2) {
        delete s2;
        s2 = nullptr;
    }
    if (list_model) {
        delete list_model;
        list_model = nullptr;
    }
    emit clear();
}
