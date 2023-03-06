#ifndef PARAMINPUTDIALOG_H
#define PARAMINPUTDIALOG_H

#include "sphere.h"
#include <QDialog>

namespace Ui {
class ParamInputDialog;
}

class ParamInputDialog : public QDialog
{
    Q_OBJECT

signals:
    void sendDataToMainWindow(Sphere sphere);

public:
    explicit ParamInputDialog(QWidget *parent = nullptr);
    ~ParamInputDialog();

private slots:
    void on_button_cancel_clicked();
    void on_button_ok_clicked();

private:
    Ui::ParamInputDialog *ui;
};

#endif // PARAMINPUTDIALOG_H
