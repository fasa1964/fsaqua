#ifndef GARMATURINFODIALOG_H
#define GARMATURINFODIALOG_H

#include <QDialog>

namespace Ui {
class GArmaturInfoDialog;
}

class GArmaturInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GArmaturInfoDialog(QWidget *parent = nullptr);
    ~GArmaturInfoDialog();

private:
    Ui::GArmaturInfoDialog *ui;
};

#endif // GARMATURINFODIALOG_H
