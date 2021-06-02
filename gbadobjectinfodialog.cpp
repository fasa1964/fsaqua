#include "gbadobjectinfodialog.h"
#include "ui_gbadobjectinfodialog.h"

GBadObjectInfoDialog::GBadObjectInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GBadObjectInfoDialog)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(QPixmap(":/icons/appIcon")));
    setWindowTitle("Object info");
}

GBadObjectInfoDialog::~GBadObjectInfoDialog()
{
    delete ui;
}

void GBadObjectInfoDialog::on_okButton_clicked(bool /*checked*/)
{
    QMap<QString, QVariant> map;
    map.insert("Fabrikat", ui->fabrikatEdit->text());
    map.insert("PWC", ui->pwcSpinBox->value());
    map.insert("PWW", ui->pwwSpinBox->value());
    map.insert("MD", ui->mdBox->value());
    map.insert("LU", ui->luBox->value());
    map.insert("KWH", ui->kwhBox->value());

    if(ui->dauerBox->checkState() == Qt::Unchecked)
        map.insert("DAUER", false);
    else
        map.insert("DAUER", true);

    emit objectValueChanged(map);
    close();
}

void GBadObjectInfoDialog::on_cancelButton_clicked(bool /*checked*/)
{
    close();
}

int GBadObjectInfoDialog::nr() const
{
    return m_nr;
}

void GBadObjectInfoDialog::setNr(int nr)
{
    ui->objectNrBox->setValue(nr);
    m_nr = nr;
}

int GBadObjectInfoDialog::pipeNr() const
{
    return m_pipeNr;
}

void GBadObjectInfoDialog::setPipeNr(int pipeNr)
{
    ui->pipeNrBox->setValue(pipeNr);
    m_pipeNr = pipeNr;
}

bool GBadObjectInfoDialog::dauer() const
{
    return m_dauer;
}

void GBadObjectInfoDialog::setDauer(bool dauer)
{
    ui->dauerBox->setChecked(dauer);
    m_dauer = dauer;
}

double GBadObjectInfoDialog::kwh() const
{
    return m_kwh;
}

void GBadObjectInfoDialog::setKwh(double kwh)
{
    ui->kwhBox->setValue(kwh);
    m_kwh = kwh;
}

int GBadObjectInfoDialog::lu() const
{
    return m_lu;
}

void GBadObjectInfoDialog::setLu(int lu)
{
    ui->luBox->setValue(lu);
    m_lu = lu;
}

QString GBadObjectInfoDialog::fabrikat() const
{
    return m_fabrikat;
}

void GBadObjectInfoDialog::setFabrikat(const QString &fabrikat)
{
    ui->fabrikatEdit->setText(fabrikat);
    m_fabrikat = fabrikat;
}

double GBadObjectInfoDialog::md() const
{
    return m_md;
}

void GBadObjectInfoDialog::setMd(double md)
{
    ui->mdBox->setValue(md);
    m_md = md;
}

QString GBadObjectInfoDialog::name() const
{
    return m_name;
}

void GBadObjectInfoDialog::setName(const QString &name)
{
    ui->labelObjectName->setText(name);
    m_name = name;
}

double GBadObjectInfoDialog::pwc() const
{
    return m_pwc;
}

void GBadObjectInfoDialog::setPwc(double pwc)
{
    ui->pwcSpinBox->setValue(pwc);
    m_pwc = pwc;
}

double GBadObjectInfoDialog::pww() const
{
    return m_pww;
}

void GBadObjectInfoDialog::setPww(double pww)
{
    ui->pwwSpinBox->setValue(pww);
    m_pww = pww;
}
