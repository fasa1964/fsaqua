#ifndef ACCEPTWIDGET_H
#define ACCEPTWIDGET_H

#include <QWidget>

namespace Ui {
class AcceptWidget;
}

class AcceptWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AcceptWidget(QWidget *parent = nullptr);
    ~AcceptWidget();

private:
    Ui::AcceptWidget *ui;
};

#endif // ACCEPTWIDGET_H
