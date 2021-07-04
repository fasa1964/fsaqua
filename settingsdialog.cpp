#include "settingsdialog.h"
#include "ui_settingsdialog.h"


#include <fsaquawindow.h>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("Settings"));

    QStringList headers;
    headers = FSAquaWindow::availableMaterial("DIN 1988");
    ui->mainPipeMaterialColdBox->addItems(headers);
    ui->risingPipeMaterialColdBox->addItems(headers);
    ui->collectingPipeMaterialColdBox->addItems(headers);
    ui->connectingPipeMaterialColdBox->addItems(headers);
    ui->risingPipeMaterialHotBox->addItems(headers);
    ui->collectingPipeMaterialHotBox->addItems(headers);
    ui->connectingPipeMaterialHotBox->addItems(headers);

    QList<int> dnIntList;
    dnIntList = FSAquaWindow::dnList();
    QStringList dnList;
    foreach(int dn, dnIntList){
        QString t = QString("%1").arg(dn);
        dnList << t;
    }

    ui->mainPipeDNBox->addItems(dnList);
    ui->risingPipeDNBox->addItems(dnList);
    ui->collectingPipeDNBox->addItems(dnList);
    ui->connectingPipeDNBox->addItems(dnList);

}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::setPipeMap(const QMap<QString, QString> &map)
{
    pipeMap = map;

    ui->mainPipeDNBox->setCurrentText( map.value("maindn","25") );
    ui->risingPipeDNBox->setCurrentText( map.value("risingdn","20") );
    ui->collectingPipeDNBox->setCurrentText( map.value("collectingdn","15") );
    ui->connectingPipeDNBox->setCurrentText( map.value("connectingdn","12") );

    ui->mainPipeMaterialColdBox->setCurrentText(map.value("main","Kupfer"));
    ui->risingPipeMaterialColdBox->setCurrentText(map.value("risingcold","Kupfer"));
    ui->risingPipeMaterialHotBox->setCurrentText(map.value("risinghot","Kupfer"));
    ui->collectingPipeMaterialColdBox->setCurrentText(map.value("collectingcold","Kupfer"));
    ui->collectingPipeMaterialHotBox->setCurrentText(map.value("collectinghot","Kupfer"));
    ui->connectingPipeMaterialColdBox->setCurrentText(map.value("connectingcold","Kupfer"));
    ui->connectingPipeMaterialHotBox->setCurrentText(map.value("connectinghot","Kupfer"));

}

void SettingsDialog::on_cancelButton_clicked(bool /*checked*/)
{
    close();
}

void SettingsDialog::on_okButton_clicked(bool /*checked*/)
{
    QMap<QString, QString> map;
    map.insert("maindn",ui->mainPipeDNBox->currentText());
    map.insert("risingdn", ui->risingPipeDNBox->currentText());
    map.insert("collectingdn",ui->collectingPipeDNBox->currentText());
    map.insert("connectingdn",ui->connectingPipeDNBox->currentText());

    map.insert("main",ui->mainPipeMaterialColdBox->currentText());
    map.insert("risingcold",ui->risingPipeMaterialColdBox->currentText());
    map.insert("risinghot",ui->risingPipeMaterialHotBox->currentText());
    map.insert("collectingcold",ui->collectingPipeMaterialColdBox->currentText());
    map.insert("collectinghot",ui->collectingPipeMaterialHotBox->currentText());
    map.insert("connectingcold",ui->connectingPipeMaterialColdBox->currentText());
    map.insert("connectinghot",ui->connectingPipeMaterialHotBox->currentText());

    emit pipeMapChanged(map);
    close();
}














