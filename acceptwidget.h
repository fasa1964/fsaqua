#ifndef ACCEPTWIDGET_H
#define ACCEPTWIDGET_H

#include <QWidget>

namespace Ui {
class AcceptWidget;
}

class AcceptWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AcceptWidget(QWidget *parent = nullptr);
    ~AcceptWidget();

signals:
    void cancelConnecting();
    void acceptConnecting();
    void forwardConnecting(bool status);

private slots:
    void on_cancelButton_clicked(bool);
    void on_acceptButton_clicked(bool);

    void on_forwardConnectingButton_clicked(bool checked);
    void on_sideConnectingButton_clicked(bool checked);

private:
    Ui::AcceptWidget *ui;
};

#endif // ACCEPTWIDGET_H
