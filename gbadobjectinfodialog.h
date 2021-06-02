#ifndef GBADOBJECTINFODIALOG_H
#define GBADOBJECTINFODIALOG_H

#include <QDialog>

namespace Ui {
class GBadObjectInfoDialog;
}

class GBadObjectInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GBadObjectInfoDialog(QWidget *parent = nullptr);
    ~GBadObjectInfoDialog();

    double pww() const;
    void setPww(double pww);

    double pwc() const;
    void setPwc(double pwc);

    QString name() const;
    void setName(const QString &name);

    double md() const;
    void setMd(double md);

    QString fabrikat() const;
    void setFabrikat(const QString &fabrikat);

    int lu() const;
    void setLu(int lu);

    double kwh() const;
    void setKwh(double kwh);

    bool dauer() const;
    void setDauer(bool dauer);

    int pipeNr() const;
    void setPipeNr(int pipeNr);

    int nr() const;
    void setNr(int nr);

signals:
    void objectValueChanged(const QMap<QString, QVariant> &map);


private slots:
    void on_okButton_clicked(bool);
    void on_cancelButton_clicked(bool);

private:
    Ui::GBadObjectInfoDialog *ui;

    QString m_name;
    QString m_fabrikat;
    double m_pwc;
    double m_pww;
    double m_md;
    double m_kwh;
    int m_lu;
    bool m_dauer;
    int m_pipeNr;
    int m_nr;
};

#endif // GBADOBJECTINFODIALOG_H
