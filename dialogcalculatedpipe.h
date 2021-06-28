#ifndef DIALOGCALCULATEDPIPE_H
#define DIALOGCALCULATEDPIPE_H

#include <QDialog>
#include <QMap>

namespace Ui {
class DialogCalculatedPipe;
}

class DialogCalculatedPipe : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCalculatedPipe(QWidget *parent = nullptr);
    ~DialogCalculatedPipe();

    void setCaptionText(const QString &caption);
    void insertRow(int nr, int dn, double da, double di, const QString &material, double len, double vr,
                   double vs, int lu);

    void clearList();
signals:
    void calculatedPipeClicked(int pipeNr);

private slots:
    void cellClicked(int row, int);

private:
    Ui::DialogCalculatedPipe *ui;

    void updateTable();

};

#endif // DIALOGCALCULATEDPIPE_H
