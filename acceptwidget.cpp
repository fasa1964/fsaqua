#include "acceptwidget.h"
#include "ui_acceptwidget.h"

AcceptWidget::AcceptWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AcceptWidget)
{
    ui->setupUi(this);
    setMouseTracking(true);
    setToolTip(tr("Accept here the connection"));
    setAutoFillBackground(true);
    setAttribute(Qt::WA_TranslucentBackground, false);
}

AcceptWidget::~AcceptWidget()
{
    delete ui;
}

void AcceptWidget::on_cancelButton_clicked(bool /*checked*/)
{
    emit cancelConnecting();
}

void AcceptWidget::on_acceptButton_clicked(bool /*checked*/)
{
    emit acceptConnecting();
}

void AcceptWidget::on_forwardConnectingButton_clicked(bool checked)
{
    if(checked)
        forwardConnecting(true);
}

void AcceptWidget::on_sideConnectingButton_clicked(bool checked)
{
    if(checked)
        forwardConnecting(false);
}
