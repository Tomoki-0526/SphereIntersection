#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "paraminputdialog.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this, SIGNAL(sendDataToGLWidget(Sphere)), ui->openGLWidget, SLOT(drawSphere(Sphere)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawSphere(Sphere sphere)
{
    emit sendDataToGLWidget(sphere);
}

void MainWindow::on_actionDrawSphere_triggered()
{
    ParamInputDialog* dlg = new ParamInputDialog(this);
    connect(dlg, SIGNAL(sendDataToMainWindow(Sphere)), this, SLOT(drawSphere(Sphere)));
    dlg->setWindowTitle(QStringLiteral("输入球参数（球心坐标&半径）"));
    dlg->exec();
    delete dlg;
}
