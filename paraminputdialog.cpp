#include "paraminputdialog.h"
#include "ui_paraminputdialog.h"
#include <QMessageBox>

ParamInputDialog::ParamInputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParamInputDialog)
{
    ui->setupUi(this);

    connect(this, SIGNAL(sendDataToMainWindow(Sphere)), parent, SLOT(drawSphereByGLWidget(Sphere)));
}

ParamInputDialog::~ParamInputDialog()
{
    delete ui;
}

void ParamInputDialog::on_button_cancel_clicked()
{
    this->close();
}

void ParamInputDialog::on_button_ok_clicked()
{
    bool ok_x, ok_y, ok_z, ok_r;
    ok_x = ok_y = ok_z = ok_r = false;
    double x = ui->lineEdit_x->text().toDouble(&ok_x);
    double y = ui->lineEdit_y->text().toDouble(&ok_y);
    double z = ui->lineEdit_z->text().toDouble(&ok_z);
    double r = ui->lineEdit_r->text().toDouble(&ok_r);

    if (ok_x & ok_y & ok_z & ok_r) {
        Sphere sphere(x, y, z, r);
        emit sendDataToMainWindow(sphere);

        this->close();
        return;
    }

    QMessageBox::warning(this,
                         tr("提示"),
                         tr("请输入正确的数值"));
}
