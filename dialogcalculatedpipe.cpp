#include "dialogcalculatedpipe.h"
#include "ui_dialogcalculatedpipe.h"

DialogCalculatedPipe::DialogCalculatedPipe(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCalculatedPipe)
{
    ui->setupUi(this);
    setWindowTitle(tr("Calculated pipe info"));
    updateTable();
    connect(ui->pipeTableWidget, &QTableWidget::cellClicked, this, &DialogCalculatedPipe::cellClicked);
}

DialogCalculatedPipe::~DialogCalculatedPipe()
{
    delete ui;
}

void DialogCalculatedPipe::setCaptionText(const QString &caption)
{
    ui->captionLabel->setText(caption);
}

void DialogCalculatedPipe::insertRow(int nr, int dn, double da, double di, const QString &material, double len, double vr, double vs, int lu)
{
    int row = ui->pipeTableWidget->rowCount();
    ui->pipeTableWidget->insertRow(row);

    QTableWidgetItem *itemNr = new QTableWidgetItem(QString("%1").arg(nr));
    QTableWidgetItem *itemDN = new QTableWidgetItem(QString("%1").arg(dn));
    QTableWidgetItem *itemDA = new QTableWidgetItem(QString("%1").arg(da));
    QTableWidgetItem *itemDI = new QTableWidgetItem(QString("%1").arg(di));
    QTableWidgetItem *itemMat = new QTableWidgetItem(material);
    QTableWidgetItem *itemLength = new QTableWidgetItem(QString("%1 m").arg(len));
    QTableWidgetItem *itemVR = new QTableWidgetItem(QString("%1").arg(vr));
    QTableWidgetItem *itemVS = new QTableWidgetItem(QString("%1").arg(vs));
    QTableWidgetItem *itemLU = new QTableWidgetItem(QString("%1").arg(lu));

    ui->pipeTableWidget->setItem(row, 0, itemNr);
    ui->pipeTableWidget->setItem(row, 1, itemDN);
    ui->pipeTableWidget->setItem(row, 2, itemDA);
    ui->pipeTableWidget->setItem(row, 3, itemDI);
    ui->pipeTableWidget->setItem(row, 4, itemMat);
    ui->pipeTableWidget->setItem(row, 5, itemLength);
    ui->pipeTableWidget->setItem(row, 6, itemVR);
    ui->pipeTableWidget->setItem(row, 7, itemVS);
    ui->pipeTableWidget->setItem(row, 8, itemLU);

    ui->pipeTableWidget->resizeColumnsToContents();
    ui->pipeTableWidget->update();
}

void DialogCalculatedPipe::clearList()
{
    ui->pipeTableWidget->clear();
    updateTable();
}

void DialogCalculatedPipe::cellClicked(int row, int /*column*/)
{
    bool ok;
    QString text = ui->pipeTableWidget->item(row,0)->text();

    if(!text.isEmpty()){
        int pnr = text.toInt(&ok); Q_ASSERT(ok);
        if(ok)
            calculatedPipeClicked(pnr);
    }
}

void DialogCalculatedPipe::updateTable()
{

    ui->pipeTableWidget->setRowCount(0);
    ui->pipeTableWidget->setColumnCount(9);
    QStringList headerlabel;
    headerlabel << "Nr." << "DN" << "Da" << "Di" << "Material" << "Length" << "Vr" << "Vs" << "LU";
    ui->pipeTableWidget->setHorizontalHeaderLabels(headerlabel);
    ui->pipeTableWidget->resizeColumnsToContents();
}




























