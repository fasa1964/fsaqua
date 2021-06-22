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
    void setValue(int sectionPipeNr, int column, double value, const QString &einheit);

    // Testing for saving and loading the project
//    QMap<QString, QVariant> getPipeSectionInfos();
//    void setPipeSectionInfos(  QMap<int, QStringList>map );

signals:
    void rowSelect(int row, int column);

private slots:
    void cellClicked(int row, int column);

private:
    Ui::PipeListWidget *ui;
    void setupTableWidget();

    QMap<int, QList<GPipe *>>pipeMap;
};

#endif // PIPELISTWIDGET_H
