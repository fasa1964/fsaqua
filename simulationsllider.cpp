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
