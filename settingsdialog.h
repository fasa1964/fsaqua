#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QMap>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

    void setPipeMap(const QMap<QString, QString> &map);

signals:
    void pipeMapChanged(const QMap<QString, QString> &map);

private slots:
    void on_cancelButton_clicked(bool);
    void on_okButton_clicked(bool);

private:
    Ui::SettingsDialog *ui;

    QMap<QString, QString> pipeMap;

};

#endif // SETTINGSDIALOG_H
