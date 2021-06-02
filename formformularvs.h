#ifndef FORMFORMULARVS_H
#define FORMFORMULARVS_H

#include <QWidget>

namespace Ui {
class FormFormularVs;
}

class FormFormularVs : public QWidget
{
    Q_OBJECT

public:
    explicit FormFormularVs(QWidget *parent = nullptr);
    ~FormFormularVs();

    void setVr(double vr);
    void setA(double a);
    void setB(double b);
    void setC(double c);
    void setArtLabel(const QString &text);

private slots:
    void on_closeButton_clicked(bool);

private:
    Ui::FormFormularVs *ui;

    double m_vr;
    double m_a;
    double m_b;
    double m_c;

};

#endif // FORMFORMULARVS_H
