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

    void setText(const QString &text, QColor color);

signals:
    void durationChanged(int value);

private slots:
    void on_durationSlider_valueChanged(int value);

private:
    Ui::SimulationSllider *ui;
};

#endif // SIMULATIONSLLIDER_H
