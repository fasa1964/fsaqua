#ifndef GARMATURINFODIALOG_H
#define GARMATURINFODIALOG_H

#include <QDialog>
#include <garmatur.h>

namespace Ui {
class GArmaturInfoDialog;
}

class GArmaturInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GArmaturInfoDialog(GArmatur *arm, QWidget *parent = nullptr);
    ~GArmaturInfoDialog();

signals:
    void armaturDNChanged(GArmatur *valve);

private slots:
    void on_okButton_clicked(bool);
    void on_cancelButton_clicked(bool);
    void on_dnBox_valueChanged(int value);
    void on_zetaBox_valueChanged(double);
    void on_calcButton_clicked(bool);

private:
    Ui::GArmaturInfoDialog *ui;
    int armaturDN;
    GArmatur *currentArmatur;
};

#endif // GARMATURINFODIALOG_H
