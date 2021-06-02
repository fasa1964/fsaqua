#ifndef SIMULATIONSLLIDER_H
#define SIMULATIONSLLIDER_H

#include <QWidget>

namespace Ui {
class SimulationSllider;
}

class SimulationSllider : public QWidget
{
    Q_OBJECT

public:
    explicit SimulationSllider(QWidget *parent = nullptr);
    ~SimulationSllider();

private:
    Ui::SimulationSllider *ui;
};

#endif // SIMULATIONSLLIDER_H
