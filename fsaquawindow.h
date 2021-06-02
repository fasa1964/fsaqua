#ifndef FSAQUAWINDOW_H
#define FSAQUAWINDOW_H

#include <QMainWindow>

namespace Ui {
class FSAquaWindow;
}

class FSAquaWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FSAquaWindow(QWidget *parent = nullptr);
    ~FSAquaWindow();

private:
    Ui::FSAquaWindow *ui;
};

#endif // FSAQUAWINDOW_H
