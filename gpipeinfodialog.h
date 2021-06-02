#ifndef GPIPEINFODIALOG_H
#define GPIPEINFODIALOG_H

#include <QDialog>


#include <gpipe.h>
#include <gbadobject.h>

namespace Ui {
class GPipeInfoDialog;
}

class GPipeInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GPipeInfoDialog(GPipe *pipe, QWidget *parent = nullptr);
    ~GPipeInfoDialog();

signals:
    void deletePipe(GPipe *pipe);
    void switchFlowDirection(GPipe *pipe);


private slots:
    void on_cancelButton_clicked(bool);
    void on_okButton_clicked(bool);
    void on_deleteButton_clicked(bool);
    void on_switchButton_clicked(bool);

private:
    Ui::GPipeInfoDialog *ui;

    GPipe *currentPipe;

    void updateTable(const QList<GBadObject *> &list);

};

#endif // GPIPEINFODIALOG_H
