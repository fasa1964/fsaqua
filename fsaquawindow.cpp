#include "fsaquawindow.h"
#include "ui_fsaquawindow.h"

FSAquaWindow::FSAquaWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FSAquaWindow)
{
    ui->setupUi(this);
}

FSAquaWindow::~FSAquaWindow()
{
    delete ui;
}
