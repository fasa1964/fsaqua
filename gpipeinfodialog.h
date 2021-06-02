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

    void setFloorText(const QString &floorText);
    void setFlow(double value);
    void setDN(int value);

signals:
    void deletePipe(GPipe *pipe);
    void switchFlowDirection(GPipe *pipe);
    void selectObjectNr(const QString &text);


private slots:
    void on_cancelButton_clicked(bool);
    void on_okButton_clicked(bool);
    void on_deleteButton_clicked(bool);
    void on_switchButton_clicked(bool);
    void cellClicked(int row, int column);

private:
    Ui::GPipeInfoDialog *ui;

    GPipe *currentPipe;
    QString m_floorText;

    void updateTable(const QList<GBadObject *> &list);

};

#endif // GPIPEINFODIALOG_H
