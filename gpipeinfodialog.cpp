#include "gpipeinfodialog.h"
#include "ui_gpipeinfodialog.h"

#include <QTableWidget>
#include <QString>
#include <QPalette>

#include <fsaquawindow.h>

#include <QDebug>

GPipeInfoDialog::GPipeInfoDialog(GPipe *pipe, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GPipeInfoDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("Info Pipe"));

    currentPipe = pipe;

    QPalette pal = ui->nrBox->palette();
    if(currentPipe->cold())
        pal.setColor(QPalette::Base, Qt::darkGreen);
    else
        pal.setColor(QPalette::Base, Qt::red);
    ui->nrBox->setPalette(pal);


    ui->nrBox->setForegroundRole(QPalette::Text);


    ui->pipeTypeLabel->setText( pipe->getTypeString() );
    ui->meterBox->setValue( pipe->getMeter());
    ui->nrBox->setValue( pipe->getNr() );
    ui->strangNrBox->setValue(pipe->getStrangMarkerNr());
    ui->flowSpeedBox->setValue( pipe->getFlowSpeed());

    updateTable(currentPipe->badObjectList());

    ui->materialBox->insertItems(0,  FSAquaWindow::availableMaterial("DIN 1988"));
    ui->materialBox->setCurrentText( pipe->getMaterial() );

}

GPipeInfoDialog::~GPipeInfoDialog()
{
    delete ui;
}

void GPipeInfoDialog::on_cancelButton_clicked(bool /*checked*/)
{
    currentPipe->setReadToConnect(false);
    close();
}

void GPipeInfoDialog::on_okButton_clicked(bool /*checked*/)
{
    currentPipe->setReadToConnect(false);
    double meter = ui->meterBox->value();
    currentPipe->setMeter(meter);
    currentPipe->setMaterial( ui->materialBox->currentText() );
    currentPipe->update();
    close();
}

void GPipeInfoDialog::updateTable(const QList<GBadObject *> &list)
{
    ui->objectsTable->setRowCount(list.size());
    ui->objectsTable->setColumnCount(5);

    QStringList headers;
    headers << "Art" << "Obj.-Nr" << "PWC" << "PWH" << "LU";
    ui->objectsTable->setHorizontalHeaderLabels(headers);

    int row = 0;
    foreach(GBadObject *o, list)
    {
        QTableWidgetItem *itemArt = new QTableWidgetItem(o->bezeichnung());
        QTableWidgetItem *itemNr = new QTableWidgetItem(QString("%1").arg( o->getNr() ));
        QTableWidgetItem *itemPWC = new QTableWidgetItem( QString("%1").arg( o->bdKW()) );
        QTableWidgetItem *itemPWH = new QTableWidgetItem( QString("%1").arg(o->bdWW()) );
        QTableWidgetItem *itemLU = new QTableWidgetItem( QString("%1").arg(o->lu()) );


        ui->objectsTable->setItem(row,0, itemArt);
        ui->objectsTable->setItem(row,1, itemNr);
        ui->objectsTable->setItem(row,2, itemPWC);
        ui->objectsTable->setItem(row,3, itemPWH);
        ui->objectsTable->setItem(row,4, itemLU);

        QBrush br;
        br.setColor(QColor(0,85,127));
        itemArt->setFlags(Qt::ItemIsSelectable);
        itemArt->setForeground(br);

        br.setColor(Qt::darkGreen);
        itemPWC->setForeground(br);

        br.setColor(Qt::red);
        itemPWH->setForeground(br);


        itemPWC->setFlags(Qt::ItemIsSelectable);
        itemPWH->setFlags(Qt::ItemIsSelectable);

        row++;
    }

    connect(ui->objectsTable, &QTableWidget::cellClicked, this, &GPipeInfoDialog::cellClicked);

    ui->objectsTable->resizeColumnsToContents();
}

void GPipeInfoDialog::on_deleteButton_clicked(bool /*checked*/)
{
    emit deletePipe(currentPipe);
    close();
}

void GPipeInfoDialog::on_switchButton_clicked(bool /*checked*/)
{
    emit switchFlowDirection(currentPipe);
    currentPipe->setReadToConnect(false);
    close();
}

void GPipeInfoDialog::cellClicked(int row, int column)
{
    if(column == 1)
    {
        QString text = ui->objectsTable->item(row, column)->text();
        emit selectObjectNr(text);
    }
}

void GPipeInfoDialog::setFloorText(const QString &floorText)
{
    ui->foorEdit->setText( floorText );
    m_floorText = floorText;
}

void GPipeInfoDialog::setFlow(double value)
{
    ui->flowBox->setValue( value );
}

void GPipeInfoDialog::setDN(int value)
{
    ui->dnBox->setValue( value );
}
