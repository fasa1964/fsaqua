#include "dialogcalculatedpipe.h"
#include "ui_dialogcalculatedpipe.h"

DialogCalculatedPipe::DialogCalculatedPipe(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCalculatedPipe)
{
    ui->setupUi(this);
}

DialogCalculatedPipe::~DialogCalculatedPipe()
{
    delete ui;
}
