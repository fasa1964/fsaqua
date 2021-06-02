#include "pipelistwidget.h"
#include "ui_pipelistwidget.h"



PipeListWidget::PipeListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PipeListWidget)
{
    ui->setupUi(this);
    setWindowTitle("Teilstrecken Infos");
    setupTableWidget();

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
    QTableWidgetItem *itemFlow = new QTableWidgetItem("");
    QTableWidgetItem *itemLu = new QTableWidgetItem("");


    if(ok)
        itemConnect->setBackgroundColor(Qt::green);
     else
       itemConnect->setBackgroundColor(Qt::red);


    itemNr->setSelected(true);

    ui->tableWidget->setItem(row,0,itemNr);
    ui->tableWidget->setItem(row,1,itemConnect);
    ui->tableWidget->setItem(row,2,itemLength);
    ui->tableWidget->setItem(row,3,itemFlow);
    ui->tableWidget->setItem(row,4,itemLu);



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

void PipeListWidget::setFlow(int row, int column, double value)
{
    QTableWidgetItem *item = ui->tableWidget->item(row, column);
    if(item != nullptr)
    {
        item->setBackground(Qt::green);
        item->setText(QString("%1 l/s").arg(value));
        ui->tableWidget->resizeColumnsToContents();
        ui->tableWidget->update();
    }
}

void PipeListWidget::setLu(int row, int column, int value)
{
    QTableWidgetItem *item = ui->tableWidget->item(row, column);
    if(item != nullptr)
    {
        item->setBackground(Qt::green);
        item->setText(QString("%1").arg(value));
        ui->tableWidget->resizeColumnsToContents();
        ui->tableWidget->update();
    }
}

// Testing for saving and loading the project
QMap<QString, QVariant> PipeListWidget::getPipeSectionInfos()
{
    QMap<QString, QVariant> map;
    for (int r = 0; r < ui->tableWidget->rowCount(); r++)
    {
        QStringList textList;
        for(int c = 0; c < ui->tableWidget->columnCount(); c++)
        {
            QString text = ui->tableWidget->item(r,c)->text();
            textList << text;
        }

        map.insert( QString("%1").arg(r), textList);
    }

    return map;
}

void PipeListWidget::setPipeSectionInfos(QMap<int, QStringList> map)
{

}

void PipeListWidget::cellClicked(int row, int /*column*/)
{

    QMapIterator<int, QList<GPipe *>> it(pipeMap);
    while (it.hasNext()) {
        it.next();

        if(it.key() != row){
            foreach(GPipe *p, pipeMap.value(it.key()) ){
                if(p->cold())
                    p->setColor(Qt::darkGreen);
                else
                    p->setColor(Qt::red);
                p->update();
            }
        }
    }

    foreach(GPipe *p, pipeMap.value(row) ){
        p->setColor(Qt::cyan);
        p->update();
    }
}

void PipeListWidget::setupTableWidget()
{
    ui->tableWidget->setColumnCount(5);

    QStringList header;
    header << "Nr" << "Connection" << "Length" << "Flow" << "LU";

    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->setRowCount(0);
}
