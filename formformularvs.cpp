#include "formformularvs.h"
#include "ui_formformularvs.h"

FormFormularVs::FormFormularVs(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormFormularVs)
{
    ui->setupUi(this);
    setWindowTitle(tr("Formel Spitzendurchfluss"));
    setWindowFlag(Qt::Popup);

}

FormFormularVs::~FormFormularVs()
{
    delete ui;
}

void FormFormularVs::on_closeButton_clicked(bool)
{
    this->hide();
}

void FormFormularVs::setC(double c)
{
    ui->cSpinBox->setValue(c);
    m_c = c;
}

void FormFormularVs::setArtLabel(const QString &text)
{
    ui->artLabel->setText(text);
}

void FormFormularVs::setB(double b)
{
    ui->bSpinBox->setValue(b);
    m_b = b;
}

void FormFormularVs::setA(double a)
{
    ui->aSpinBox->setValue(a);
    m_a = a;
}

void FormFormularVs::setVr(double vr)
{
    ui->vrSpinBox->setValue(vr);
    m_vr = vr;
}
