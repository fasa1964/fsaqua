#include "simulationsllider.h"
#include "ui_simulationsllider.h"

SimulationSllider::SimulationSllider(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimulationSllider)
{
    ui->setupUi(this);
}

SimulationSllider::~SimulationSllider()
{
    delete ui;
}

void SimulationSllider::setText(const QString &text, QColor color)
{
    QPalette pal = ui->message->palette();
    pal.setColor(QPalette::WindowText, color);
    ui->message->setPalette(pal);
    ui->message->setText(text);
}

void SimulationSllider::on_durationSlider_valueChanged(int value)
{
    emit durationChanged(value);
}
