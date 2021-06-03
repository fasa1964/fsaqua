#ifndef FORMTOOLS_H
#define FORMTOOLS_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QCloseEvent>


#include <gbadobject.h>
#include <gpipe.h>

namespace Ui {
class FormTools;
}

class FormTools : public QWidget
{
    Q_OBJECT

public:
    explicit FormTools(QWidget *parent = nullptr);
    ~FormTools();

    void closeForm();
    void setRangeFloors(int min, int max);

    int getFloorCount() const;
    void setFloorCount(int floorCount);

    double getfloorHeight() const;
    void setfloorHeight(double floorHeight);

    QString getFloorText(int index);

    void enableArmaturButtons(bool status);
    void enableObjectButtons(bool status);
    void enableWSIBButton(bool status);

    bool getBasement() const;
    void setBasement(bool status);

    void enableInstallButton(bool status);
    void enableDeletePipeButton(bool status);
    void enableTestInstallButton(bool status);
    void enableMainPipeButton(bool status);
    void enableCalculateButton(bool status);


    void setBlinkButton(bool status, const QString &button);

signals:
    void dropObjectSelcted(const QString &objectType);
    void dropArmaturSelcted(const QString &armaturType);
    void setStrangMarker(const QString &strang);
    void floorCountChanged(int floors);
    void scaleChanged(int value);
    void floorHeightChanged(double value);
    void installButtonClicked();
    void deletePipeButtonClicked();
    void mainPipeButtonClicked();
    void basementClicked(bool status);
    void testInstallClicked();
    void calculatePipeClicked();

private slots:
    void on_wcButton_clicked(bool);
    void on_wtButton_clicked(bool);
    void on_wcdsButton_clicked(bool);
    void on_duButton_clicked(bool);
    void on_waButton_clicked(bool);
    void on_urButton_clicked(bool);
    void on_swButton_clicked(bool);
    void on_wmButton_clicked(bool);
    void on_gsButton_clicked(bool);
    void on_agavButton_clicked(bool);
    void on_spButton_clicked(bool);
    void on_dheButton_clicked(bool);
    void on_wsibButton_clicked(bool);
    void on_avButton_clicked(bool);
    void on_floorBox_valueChanged(int arg1);
    void on_scaleSlider_sliderMoved(int position);

    void on_highBox_valueChanged(double arg1);
    void on_basementBox_clicked(bool checked);


    // Pipe
    void on_strangButton_clicked(bool);
    void on_installButton_clicked(bool);
    void on_mainPipeButton_clicked(bool);
    void on_testInstallButton_clicked(bool checked);
    void on_deletePipeButton_clicked(bool);


    void updateBlinkingButton();

    // GArmatur
    void on_abrvButton_clicked(bool);
    void on_gvButton_clicked(bool);
    void on_rvButton_clicked(bool);
    void on_svButton_clicked(bool);
    void on_dmButton_clicked(bool);
    void on_wzButton_clicked(bool);
    void on_filButton_clicked(bool);
    void on_fsveButton_clicked(bool);
    void on_khButton_clicked(bool);
    void on_svteButton_clicked(bool);


    void on_calculateButton_clicked(bool);

private:
    Ui::FormTools *ui;

    int m_floorCount;
    double m_floorHeight;
    bool basement;

    QTimer *timer;
    int m_buttonState;
    QColor m_colorRed;
    QColor m_colorBlack;
    QString buttonText;

};

#endif // FORMTOOLS_H
