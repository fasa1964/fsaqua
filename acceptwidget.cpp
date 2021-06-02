#include "acceptwidget.h"
#include "ui_acceptwidget.h"

AcceptWidget::AcceptWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AcceptWidget)
{
    ui->setupUi(this);
}

AcceptWidget::~AcceptWidget()
{
    delete ui;
}
