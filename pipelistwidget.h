#ifndef PIPELISTWIDGET_H
#define PIPELISTWIDGET_H

#include <QWidget>

namespace Ui {
class PipeListWidget;
}

class PipeListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PipeListWidget(QWidget *parent = nullptr);
    ~PipeListWidget();

private:
    Ui::PipeListWidget *ui;
};

#endif // PIPELISTWIDGET_H
