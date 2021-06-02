#include "pipelistwidget.h"
#include "ui_pipelistwidget.h"

PipeListWidget::PipeListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PipeListWidget)
{
    ui->setupUi(this);
}

PipeListWidget::~PipeListWidget()
{
    delete ui;
}
