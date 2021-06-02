#include "gpipeinfodialog.h"
#include "ui_gpipeinfodialog.h"

#include <QTableWidget>
#include <QString>

GPipeInfoDialog::GPipeInfoDialog(GPipe *pipe, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GPipeInfoDialog)
{
    ui->setupUi(this);
    currentPipe = pipe;

    ui->pipeTypeLabel->setText( pipe->getTypeString() );
    ui->meterBox->setValue( pipe->getMeter());
    ui->nrBox->setValue( pipe->getNr() );
    ui->strangNrBox->setValue( pipe->getStrangNr() );

    updateTable(currentPipe->badObjectList());

}

GPipeInfoDialog::~GPipeInfoDialog()
{
    delete ui;
}

void GPipeInfoDialog::on_cancelButton_clicked(bool /*checked*/)
{
    close();
}

void GPipeInfoDialog::on_okButton_clicked(bool /*checked*/)
{
    close();
}

void GPipeInfoDialog::updateTable(const QList<GBadObject *> &list)
{
    ui->objectsTable->setRowCount(list.size());
    ui->objectsTable->setColumnCount(4);

    QStringList headers;
    headers << "Art" << "Obj.-Nr" << "PWC" << "PWH";
    ui->objectsTable->setHorizontalHeaderLabels(headers);

    int row = 0;
    foreach(GBadObject *o, list)
    {
        QTableWidgetItem *itemArt = new QTableWidgetItem(o->bezeichnung());
        QTableWidgetItem *itemNr = new QTableWidgetItem(QString("%1").arg( o->getNr() ));
        QTableWidgetItem *itemPWC = new QTableWidgetItem( QString("%1").arg( o->bdKW()) );
        QTableWidgetItem *itemPWH = new QTableWidgetItem( QString("%1").arg(o->bdWW()) );


        ui->objectsTable->setItem(row,0, itemArt);
        ui->objectsTable->setItem(row,1, itemNr);
        ui->objectsTable->setItem(row,2, itemPWC);
        ui->objectsTable->setItem(row,3, itemPWH);

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
    close();
}
