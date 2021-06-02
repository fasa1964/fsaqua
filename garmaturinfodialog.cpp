#include "garmaturinfodialog.h"
#include "ui_garmaturinfodialog.h"

GArmaturInfoDialog::GArmaturInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GArmaturInfoDialog)
{
    ui->setupUi(this);
}

GArmaturInfoDialog::~GArmaturInfoDialog()
{
    delete ui;
}
