#ifndef DIALOGCALCULATEDPIPE_H
#define DIALOGCALCULATEDPIPE_H

#include <QDialog>

namespace Ui {
class DialogCalculatedPipe;
}

class DialogCalculatedPipe : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCalculatedPipe(QWidget *parent = nullptr);
    ~DialogCalculatedPipe();

private:
    Ui::DialogCalculatedPipe *ui;
};

#endif // DIALOGCALCULATEDPIPE_H
