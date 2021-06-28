#include "pipelistwidget.h"
#include "ui_pipelistwidget.h"

#include <QDebug>

PipeListWidget::PipeListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PipeListWidget)
{
    ui->setupUi(this);
    setWindowTitle(tr("Sectionpipe info"));
    setupTableWidget();
    ui->tableWidget->setSortingEnabled(false);

    connect(ui->tableWidget, &QTableWidget::cellClicked, this, &PipeListWidget::cellClicked);
}

PipeListWidget::~PipeListWidget()
{
    delete ui;
}

void PipeListWidget::insertRow(const QList<GPipe *> &list, int nr, bool ok)
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    pipeMap.insert(nr, list);

    qreal lenght = 0.0;
    foreach(GPipe *p, list){
        lenght += p->getMeter();
    }

    QString cs = "ok";
    if(!ok)
        cs = "failed";

    QTableWidgetItem *itemNr = new QTableWidgetItem(QString("%1").arg(nr));
    QTableWidgetItem *itemConnect = new QTableWidgetItem(cs);
    QTableWidgetItem *itemLength = new QTableWidgetItem(QString("%1 m").arg(lenght));
    QTableWidgetItem *itemGeo = new QTableWidgetItem("");
    QTableWidgetItem *itemPmin = new QTableWidgetItem("");
    QTableWidgetItem *itemdPap = new QTableWidgetItem("");
    QTableWidgetItem *itemdRV = new QTableWidgetItem("");


    if(ok)
        itemConnect->setBackgroundColor(Qt::green);
     else
       itemConnect->setBackgroundColor(Qt::red);


    itemNr->setSelected(true);

    ui->tableWidget->setItem(row,0,itemNr);
    ui->tableWidget->setItem(row,1,itemConnect);
    ui->tableWidget->setItem(row,2,itemLength);
    ui->tableWidget->setItem(row,3,itemGeo);
    ui->tableWidget->setItem(row,4,itemPmin);
    ui->tableWidget->setItem(row,5,itemdPap);
    ui->tableWidget->setItem(row,6,itemdRV);

    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->update();

}

void PipeListWidget::clearList()
{
    ui->tableWidget->clear();
    setupTableWidget();
    ui->tableWidget->update();
}

void PipeListWidget::setLabelStatus(const QString &text)
{
    ui->statusLabel->setText(text);
}

void PipeListWidget::setText(int row, int column, const QString &text)
{
    QTableWidgetItem *item = ui->tableWidget->item(row, column);
    if(text == "OK")
        item->setBackground(Qt::green);
    else
        item->setBackground(Qt::red);

    QString nt = item->text() + " " +text;
    item->setText(nt);
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->update();
}

void PipeListWidget::setValue(int sectionPipeNr, int column, double value, const QString &einheit)
{

    QString nrs = QString("%1").arg(sectionPipeNr);
    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        QTableWidgetItem *item = ui->tableWidget->item(i, 0);
        if(item != nullptr)
        {
            if(item->text() == nrs)
            {
                QTableWidgetItem *itemValue = ui->tableWidget->item(i, column);
                if(itemValue != nullptr)
                {
                    itemValue->setText(QString("%1 ").arg(value)+einheit);
                    ui->tableWidget->resizeColumnsToContents();
                    ui->tableWidget->update();
                }
            }
        }
    }


//    QTableWidgetItem *item = ui->tableWidget->item(row, column);
//    if(item != nullptr)
//    {
//        item->setText(QString("%1 ").arg(value)+einheit);
//        ui->tableWidget->resizeColumnsToContents();
//        ui->tableWidget->update();
//    }
}


// Testing for saving and loading the project
//QMap<QString, QVariant> PipeListWidget::getPipeSectionInfos()
//{
//    QMap<QString, QVariant> map;
//    for (int r = 0; r < ui->tableWidget->rowCount(); r++)
//    {
//        QStringList textList;
//        for(int c = 0; c < ui->tableWidget->columnCount(); c++)
//        {
//            QString text = ui->tableWidget->item(r,c)->text();
//            textList << text;
//        }

//        map.insert( QString("%1").arg(r), textList);
//    }

//    return map;
//}

//void PipeListWidget::setPipeSectionInfos(QMap<int, QStringList> map)
//{

//}

void PipeListWidget::cellClicked(int row, int column)
{
    bool ok;
    int nr = ui->tableWidget->item(row, 0)->text().toInt(&ok);
    emit rowSelect(nr, column);
}

void PipeListWidget::setupTableWidget()
{
    ui->tableWidget->setColumnCount(7);

    QStringList header;
    header << "Nr" << "Connection" << "Length" << "dP(geo)" << "PminFl" << "dP(ap)" << "RV";

    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->setRowCount(0);
}
