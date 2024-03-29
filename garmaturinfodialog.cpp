#include "garmaturinfodialog.h"
#include "ui_garmaturinfodialog.h"

#include <fsaquawindow.h>
#include <QtMath>

#include <QDebug>

GArmaturInfoDialog::GArmaturInfoDialog(GArmatur *arm, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GArmaturInfoDialog)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(QPixmap(":/icons/appIcon")));
    setWindowTitle(tr("Info Armatur"));

    currentArmatur = arm;
    armaturDN = currentArmatur->dn();

    ui->pressureLossBox->setValue( currentArmatur->getPessureLoss() );

    ui->dnBox->setValue(currentArmatur->dn());
    ui->dnBox->setMaximum( FSAquaWindow::dnList().last());
    ui->dnBox->setMinimum( FSAquaWindow::dnList().first());

    ui->dichteBox->setValue( currentArmatur->dichte());
    ui->flowVelocityBox->setValue( currentArmatur->flowSpeed());
    ui->vsBox->setValue( currentArmatur->getPeakFlow() );
    ui->vsBox2->setValue( currentArmatur->getPeakFlow() );

    double hPa = currentArmatur->getPessureLoss();
    ui->pressureLossBox->setValue( hPa );

    if(currentArmatur->getZeta() > 0.0)
        ui->ePressureLossBox->setValue( hPa );
    else
        ui->pAPBox->setValue( hPa );



    GArmatur::GArmaturType type;
    (quint32&) type = currentArmatur->type();
    double zeta = FSAquaWindow::zeta( type,  currentArmatur->dn() );

    ui->zetaBox->setValue( zeta );
    ui->zetaBox2->setValue( zeta );
    ui->aramturLabel->setText( arm->name() );
    ui->entleerungBox->setChecked( currentArmatur->getEntleerung());
}

GArmaturInfoDialog::~GArmaturInfoDialog()
{
    delete ui;
}

void GArmaturInfoDialog::on_okButton_clicked(bool /*checked*/)
{
    currentArmatur->setDn( ui->dnBox->value() );
    currentArmatur->setPessureLoss( ui->pressureLossBox->value() );
    currentArmatur->setEntleerung( ui->entleerungBox->isChecked() );
    currentArmatur->setZeta( ui->zetaBox->value() );
    currentArmatur->setPg( ui->pgBox->value() );
    currentArmatur->setVg( ui->vgBox->value() );

    if(ui->pAPBox->value() > 0.0)
        currentArmatur->setPessureLoss( ui->pAPBox->value() );

    currentArmatur->update();

    close();
}

void GArmaturInfoDialog::on_cancelButton_clicked(bool /*checked*/)
{
    close();
}


void GArmaturInfoDialog::on_dnBox_valueChanged(int value)
{

    int dn = value;
    while (!FSAquaWindow::dnList().contains(dn))
    {
        if(armaturDN < value)
            dn++;

        if(armaturDN >= value)
            dn--;

        if(dn < ui->dnBox->minimum())
            ui->dnBox->setValue( ui->dnBox->minimum());

        if(dn > ui->dnBox->maximum())
            ui->dnBox->setValue( ui->dnBox->maximum());
    }

    armaturDN = dn;
    ui->dnBox->setValue( dn );

    // Set drag coefficient
    GArmatur::GArmaturType type;
    (quint32&) type = currentArmatur->type();
    double zeta =  FSAquaWindow::zeta( type, dn);
    ui->zetaBox->setValue( zeta );
    on_zetaBox_valueChanged( zeta );
}

void GArmaturInfoDialog::on_zetaBox_valueChanged(double /*value*/)
{
    double z = ui->zetaBox->value();
    double v = ui->flowVelocityBox->value();
    double d = ui->dichteBox->value();

    double hPa = z * (d/2) *  qPow(v, 2) / 100.00;
    ui->pressureLossBox->setValue(hPa);
}

void GArmaturInfoDialog::on_calcButton_clicked(bool /*checked*/)
{
    double pg = ui->pgBox->value();
    double vs = ui->vsBox2->value();
    double vg = ui->vgBox->value();

    double pap = pg * qPow((vs/vg),2);
    ui->pAPBox->setValue(pap);
}
