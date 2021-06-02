#ifndef PIPELISTWIDGET_H
#define PIPELISTWIDGET_H

#include <QWidget>

#include <gpipe.h>


namespace Ui {
class PipeListWidget;
}

class PipeListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PipeListWidget(QWidget *parent = nullptr);
    ~PipeListWidget();

    void insertRow(const QList<GPipe *> &list, int nr, bool ok);
    void clearList();
    void setLabelStatus(const QString &text);
    void setText(int row, int column, const QString &text);
    void setFlow(int row, int column, double value);
    void setLu(int row, int column, int value);

    // Testing for saving and loading the project
    QMap<QString, QVariant> getPipeSectionInfos();
    void setPipeSectionInfos(  QMap<int, QStringList>map );

private slots:
    void cellClicked(int row, int);

private:
    Ui::PipeListWidget *ui;
    void setupTableWidget();

    QMap<int, QList<GPipe *>>pipeMap;
};

#endif // PIPELISTWIDGET_H
