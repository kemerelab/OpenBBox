#include "controlwindow.h"
#include "ui_controlwindow.h"

ControlWindow::ControlWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ControlWindow)
{
    ui->setupUi(this);
}

ControlWindow::~ControlWindow()
{
    delete ui;
}
