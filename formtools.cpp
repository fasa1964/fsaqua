#include "formtools.h"
#include "ui_formtools.h"

#include <QTimer>
#include <QPalette>

#include <QDebug>

FormTools::FormTools(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormTools)
{
    ui->setupUi(this);
    setWindowTitle(tr("Tools"));

    setFloorCount(1);
    setfloorHeight(2.50);
    basement = false;

    m_colorBlack = ui->installButton->palette().background().color();
    m_colorRed = Qt::red;

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &FormTools::updateBlinkingButton);
}

FormTools::~FormTools()
{
    delete ui;
}


void FormTools::closeForm()
{
    close();
}

void FormTools::setRangeFloors(int min, int max)
{
    ui->floorBox->setRange(min, max);
}


void FormTools::on_wtButton_clicked(bool)
{
    emit dropObjectSelcted("wt");
}

void FormTools::on_wcButton_clicked(bool)
{
    emit dropObjectSelcted("wc");
}


void FormTools::on_wcdsButton_clicked(bool)
{
    emit dropObjectSelcted("wcdc");
}

void FormTools::on_duButton_clicked(bool)
{
    emit dropObjectSelcted("du");
}

void FormTools::on_waButton_clicked(bool)
{
    emit dropObjectSelcted("wa");
}

void FormTools::on_urButton_clicked(bool)
{
    emit dropObjectSelcted("ur");
}

void FormTools::on_swButton_clicked(bool)
{
    emit dropObjectSelcted("sw");
}

void FormTools::on_wmButton_clicked(bool)
{
    emit dropObjectSelcted("wm");
}

void FormTools::on_gsButton_clicked(bool)
{
    emit dropObjectSelcted("gs");
}

void FormTools::on_agavButton_clicked(bool)
{
    emit dropObjectSelcted("agav");
}

void FormTools::on_spButton_clicked(bool)
{
    emit dropObjectSelcted("sp");
}

void FormTools::on_dheButton_clicked(bool)
{
    emit dropObjectSelcted("dhe");
}

void FormTools::on_wsibButton_clicked(bool)
{
    emit dropObjectSelcted("wsib");
}

void FormTools::on_avButton_clicked(bool)
{
    emit dropObjectSelcted("av");
}

void FormTools::on_floorBox_valueChanged(int arg1)
{
    setFloorCount(arg1);
    emit floorCountChanged(arg1);
}

void FormTools::on_scaleSlider_sliderMoved(int position)
{
    emit scaleChanged(position);
}

void FormTools::on_highBox_valueChanged(double arg1)
{
    setfloorHeight(arg1);
    emit floorHeightChanged(arg1);
}

double FormTools::getfloorHeight() const
{
    return m_floorHeight;
}

void FormTools::setfloorHeight(double floorHeight)
{
    ui->highBox->setValue( floorHeight );
    m_floorHeight = floorHeight;
}

QString FormTools::getFloorText(int index)
{
    QString text;
    text = QString("%1 ETG").arg(index);

    if(ui->basementBox->isChecked() && index == 0)
        text = "KG";

    if(ui->basementBox->isChecked() && index == 1)
        text = "EG";

    if(!ui->basementBox->isChecked() && index == 0)
        text = "EG";

     if(ui->basementBox->isChecked() && index >= 2){
         index = index - 1;
         text = QString("%1 ETG").arg(index);
     }

     return text;
}

void FormTools::enableArmaturButtons(bool status)
{
    ui->tabArmatur->setEnabled(status);
}

void FormTools::enableObjectButtons(bool status)
{
    ui->urButton->setEnabled(status);
    ui->wtButton->setEnabled(status);
    ui->wcButton->setEnabled(status);
    ui->wcdsButton->setEnabled(status);
    ui->duButton->setEnabled(status);
    ui->waButton->setEnabled(status);
    ui->swButton->setEnabled(status);
    ui->gsButton->setEnabled(status);
    ui->wmButton->setEnabled(status);
    ui->avButton->setEnabled(status);
    ui->spButton->setEnabled(status);
    ui->wsibButton->setEnabled(status);
    ui->dheButton->setEnabled(status);
    ui->agavButton->setEnabled(status);
}

void FormTools::enableWSIBButton(bool status)
{
    ui->wsibButton->setEnabled(status);
}

int FormTools::getFloorCount() const
{
    return m_floorCount;
}

void FormTools::setFloorCount(int floorCount)
{
    ui->floorBox->setValue( floorCount );
    m_floorCount = floorCount;
}

void FormTools::on_basementBox_clicked(bool checked)
{
    if(checked)
        basement = true;
    else
        basement = false;

    emit basementClicked(checked);
}


void FormTools::on_strangButton_clicked(bool)
{
    emit setStrangMarker("STRANG");
}

bool FormTools::getBasement() const
{
    return basement;
}

void FormTools::setBasement(bool status)
{
    ui->basementBox->setChecked(status);
}

void FormTools::enableInstallButton(bool status)
{
    ui->installButton->setEnabled(status);
}

void FormTools::enableDeletePipeButton(bool status)
{
    ui->deletePipeButton->setEnabled(status);
}

void FormTools::enableTestInstallButton(bool status)
{
    ui->testInstallButton->setEnabled(status);
}

void FormTools::enableMainPipeButton(bool status)
{
    ui->mainPipeButton->setEnabled(status);
}

void FormTools::enableCalculateButton(bool status)
{
    ui->calculateButton->setEnabled(status);
}

void FormTools::setBlinkButton(bool status, const QString &button)
{
    buttonText = button;

    if(status)
       timer->start(1000);
    else{
        timer->stop();
        QPalette pal = ui->installButton->palette();
        pal.setColor(QPalette::Button, m_colorBlack);
        ui->installButton->setPalette(pal);
        update();
    }
}

void FormTools::on_installButton_clicked(bool /*checked*/)
{
    emit installButtonClicked();
}

void FormTools::updateBlinkingButton()
{
    if(buttonText == "install"){
        m_buttonState = ++m_buttonState % 2;
        QPalette pal = ui->installButton->palette();
        pal.setColor(QPalette::Button, (m_buttonState) ? m_colorBlack : m_colorRed);
        ui->installButton->setPalette(pal);
        update();
    }

    if(buttonText == "test"){
        m_buttonState = ++m_buttonState % 2;
        QPalette pal = ui->testInstallButton->palette();
        pal.setColor(QPalette::Button, (m_buttonState) ? m_colorBlack : m_colorRed);
        ui->testInstallButton->setPalette(pal);
        update();
    }
}



void FormTools::on_mainPipeButton_clicked(bool /*checked*/)
{
    emit mainPipeButtonClicked();
}

void FormTools::on_testInstallButton_clicked(bool /*checked*/)
{
    emit testInstallClicked();
}

void FormTools::on_abrvButton_clicked(bool /*checked*/)
{
    emit dropArmaturSelcted("svrv");
}

void FormTools::on_gvButton_clicked(bool /*checked*/)
{
    emit dropArmaturSelcted("gv");
}

void FormTools::on_rvButton_clicked(bool /*checked*/)
{
    emit dropArmaturSelcted("rv");
}

void FormTools::on_svButton_clicked(bool /*checked*/)
{
    emit dropArmaturSelcted("sv");
}

void FormTools::on_dmButton_clicked(bool /*checked*/)
{
     emit dropArmaturSelcted("dm");
}

void FormTools::on_wzButton_clicked(bool /*checked*/)
{
    emit dropArmaturSelcted("wz");
}

void FormTools::on_filButton_clicked(bool /*checked*/)
{
    emit dropArmaturSelcted("fil");
}

void FormTools::on_fsveButton_clicked(bool /*checked*/)
{
    emit dropArmaturSelcted("fsve");
}

void FormTools::on_khButton_clicked(bool /*checked*/)
{
    emit dropArmaturSelcted("kh");
}

void FormTools::on_svteButton_clicked(bool /*checked*/)
{
    emit dropArmaturSelcted("svte");
}

void FormTools::on_deletePipeButton_clicked(bool /*checked*/)
{
    emit deletePipeButtonClicked();
}

void FormTools::on_calculateButton_clicked(bool /*checked*/)
{
    emit calculatePipeClicked();
}
