#ifndef FSAQUAWINDOW_H
#define FSAQUAWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsProxyWidget>

#include <formtools.h>
#include <gbadobject.h>
#include <gpipe.h>
#include <garmatur.h>
#include <scene.h>
#include <formformularvs.h>

#include <acceptwidget.h>
#include <pipelistwidget.h>
#include <simulationsllider.h>
#include <en806.h>

#define MAXFLOORS 15
#define MINSCENEHEIGHT 380
#define DISTANCEPIPE 20

namespace Ui {
class FSAquaWindow;
}

class FSAquaWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FSAquaWindow(QWidget *parent = nullptr);
    ~FSAquaWindow();

    virtual void resizeEvent(QResizeEvent *) Q_DECL_OVERRIDE;

    static QStringList gebaeudeArt(){
        QStringList list;
        list << "Wohngebäude";
        list << "Schule";
        list << "Verwaltungsgebäude";
        list << "Hotel";
        list << "Krankenhaus";
        list << "Pflegeheim";
        list << "Altersheim und betreutes Wohnen";
        return list;
    }

    static double getDefaultMeter(GBadObject::GBadObjectType type){
        double meter = 0.0;

        if(type == GBadObject::DU || type == GBadObject::WC)
            meter = 1.25;

        if(type == GBadObject::WA)
            meter = 0.65;

        if(type == GBadObject::AV)
            meter = 1.65;

        if(type == GBadObject::SP || type == GBadObject::GS || type == GBadObject::WT)
            meter = 0.55;

        if(type == GBadObject::WM)
            meter = 0.75;

        if(type == GBadObject::WSIB)
            meter = 1.75;

        if(type == GBadObject::WCDS || type == GBadObject::UR)
            meter = 1.2;

        if(type == GBadObject::SW || type == GBadObject::DHE )
            meter = 0.35;

        if(type == GBadObject::AGWB || type == GBadObject::AGAV )
            meter = 1.10;

        return meter;
    }

    static QStringList calculateDIN(){
        return QStringList()<< "DIN 1988" << "DIN EN 806";
    }

    static QStringList availableMaterial(const QString &dinTyp){

        QStringList list;

        if(dinTyp == "DIN EN 806")
            list << "Kupfer" << "Edelstahl" << "PEXAL Verbund" <<
                    "PE-X" << "PB" << "PP" << "PVC" << "verz. Stahl" ;

        if(dinTyp == "DIN 1988")
            list << "Kupfer" << "Edelstahl" << "PEXAL Verbund" <<
                    "PE-X" << "PB" << "PP" << "PVC" << "verz. Stahl" ;

        return list;
    }

    static QList<int> dnList(){
        QList<int>dn;
        dn << 10 << 12 << 15 << 20 << 25 << 32 << 40 << 50 << 60 << 65 << 80 << 100;
        return dn;
    }

    static double zeta(GArmatur::GArmaturType t, int dn){

        double z = 0.0;
        if(t == GArmatur::KH)
        {
           if(dn >= 12 && dn <= 50)
               z = 0.3;
        }

       if(t == GArmatur::GV)
       {
           if(dn >= 12 && dn <= 25)
               z = 7.0;
       }

       if(t == GArmatur::SV)
       {
           if(dn == 12)
               z = 2.5;

           if(dn > 12 && dn <= 32)
               z = 2.0;

           if(dn > 32 && dn < 50)
               z = 2.5;

           if(dn == 50)
               z = 2.5;

           if(dn > 50 && dn <= 65)
               z = 3.0;

           if(dn > 65 && dn <= 100)
               z = 3.5;

       }

       return z;
    }

    static double kWert(const QString &material){
        double k = 0.0;

        if(material == "Kupfer" || material == "Edelstahl")
            k = 0.0015;

        if(material == "PE-X" || material == "PB"  || material == "PP"  || material == "PVC"  ||
                material == "PEXAL Verbund")
            k = 0.007;

        if(material == "verz. Stahl")
            k = 0.15;

        return k;
    }

    static double durchmesser(double dn, const QString &material, bool da){

        double d = 0.0;
        if(material == "verz. Stahl")
        {
            if(da && dn == 10.0)
                d = 17.2;
            if(!da && dn == 10.0)
                d = 11.5;
            if(da && dn == 15.0)
                d = 21.2;
            if(!da && dn == 15.0)
                d = 15.2;
            if(da && dn == 20.0)
                d = 26.9;
            if(!da && dn == 20.0)
                d = 20.7;
            if(da && dn == 25.0)
                d = 33.7;
            if(!da && dn == 25.0)
                d = 26.1;
            if(da && dn == 32.0)
                d = 42.4;
            if(!da && dn == 32.0)
                d = 34.8;
            if(da && dn == 40.0)
                d = 48.3;
            if(!da && dn == 40.0)
                d = 40.7;
            if(da && dn == 50.0)
                d = 60.3;
            if(!da && dn == 50.0)
                d = 51.6;
            if(da && dn == 65.0)
                d = 76.1;
            if(!da && dn == 65.0)
                d = 67.2;
            if(da && dn == 80.0)
                d = 88.9;
            if(!da && dn == 80.0)
                d = 79.0;
            if(da && dn == 100.0)
                d = 114.3;
            if(!da && dn == 100.0)
                d = 103.0;
        }
        if(material == "Kupfer")
        {
            if(da && dn == 10.0)
                d = 12.0;
            if(!da && dn == 10.0)
                d = 10.0;
            if(da && dn == 12.0)
                d = 15.0;
            if(!da && dn == 12.0)
                d = 13.0;
            if(da && dn == 15.0)
                d = 18.0;
            if(!da && dn == 15.0)
                d = 16.0;
            if(da && dn == 20.0)
                d = 22.0;
            if(!da && dn == 20.0)
                d = 20.0;
            if(da && dn == 25.0)
                d = 28.0;
            if(!da && dn == 25.0)
                d = 25.0;
            if(da && dn == 32.0)
                d = 35.0;
            if(!da && dn == 32.0)
                d = 32.0;
            if(da && dn == 40.0)
                d = 42.0;
            if(!da && dn == 40.0)
                d = 39.0;
            if(da && dn == 50.0)
                d = 60.3;
            if(!da && dn == 50.0)
                d = 51.6;
            if(da && dn == 65.0)
                d = 76.1;
            if(!da && dn == 65.0)
                d = 67.2;
            if(da && dn == 80.0)
                d = 88.9;
            if(!da && dn == 80.0)
                d = 79.0;
            if(da && dn == 100.0)
                d = 114.3;
            if(!da && dn == 100.0)
                d = 103.0;
        }
        if(material == "Edelstahl")
        {
            if(da && dn == 10.0)
                d = 12.0;
            if(!da && dn == 10.0)
                d = 10.0;
            if(da && dn == 12.0)
                d = 15.0;
            if(!da && dn == 12.0)
                d = 13.0;
            if(da && dn == 15.0)
                d = 18.0;
            if(!da && dn == 15.0)
                d = 16.0;
            if(da && dn == 20.0)
                d = 22.0;
            if(!da && dn == 20.0)
                d = 19.6;
            if(da && dn == 25.0)
                d = 28.0;
            if(!da && dn == 25.0)
                d = 25.6;
            if(da && dn == 32.0)
                d = 35.0;
            if(!da && dn == 32.0)
                d = 32.0;
            if(da && dn == 40.0)
                d = 42.0;
            if(!da && dn == 40.0)
                d = 39.0;
            if(!da && dn == 50.0)
                d = 54.0;
            if(!da && dn == 50.0)
                d = 51.0;
            if(!da && dn == 60.0)
                d = 64.0;
            if(!da && dn == 60.0)
                d = 60.0;
            if(!da && dn == 65.0)
                d = 76.1;
            if(!da && dn == 65.0)
                d = 72.1;
            if(!da && dn == 80.0)
                d = 88.9;
            if(!da && dn == 80.0)
                d = 84.9;
            if(!da && dn == 100.0)
                d = 108.0;
            if(!da && dn == 100.0)
                d = 104.0;
        }
        if(material == "PP" || material == "PB" || material == "PVC")
        {
            if(da && dn == 10.0)
                d = 12.0;
            if(!da && dn == 10.0)
                d = 10.0;
            if(da && dn == 12.0)
                d = 16.0;
            if(!da && dn == 12.0)
                d = 11.6;
            if(da && dn == 15.0)
                d = 20.0;
            if(!da && dn == 15.0)
                d = 14.6;
            if(da && dn == 20.0)
                d = 25.0;
            if(!da && dn == 20.0)
                d = 18.0;
            if(da && dn == 25.0)
                d = 32.0;
            if(!da && dn == 25.0)
                d = 23.3;
            if(da && dn == 32.0)
                d = 40.0;
            if(!da && dn == 32.0)
                d = 29.0;
            if(da && dn == 40.0)
                d = 50.0;
            if(!da && dn == 40.0)
                d = 36.2;
            if(da && dn == 50.0)
                d = 63.0;
            if(!da && dn == 50.0)
                d = 45.8;
            if(da && dn == 65.0)
                d = 75.0;
            if(!da && dn == 65.0)
                d = 54.4;
            if(da && dn == 80.0)
                d = 90.0;
            if(!da && dn == 80.0)
                d = 65.4;
            if(da && dn == 100.0)
                d = 110.0;
            if(!da && dn == 100.0)
                d = 79.8;

        }
        if(material == "PEXAL Verbund")
        {
            if(da && dn == 10.0)
                d = 12.0;
            if(!da && dn == 10.0)
                d = 8.6;
            if(da && dn == 12.0)
                d = 16.0;
            if(!da && dn == 12.0)
                d = 11.0;
            if(da && dn == 15.0)
                d = 20.0;
            if(!da && dn == 15.0)
                d = 14.0;
            if(da && dn == 20.0)
                d = 26.0;
            if(!da && dn == 20.0)
                d = 17.6;
            if(da && dn == 25.0)
                d = 32.0;
            if(!da && dn == 25.0)
                d = 22.5;
            if(da && dn == 32.0)
                d = 40.0;
            if(!da && dn == 32.0)
                d = 28.0;
            if(da && dn == 40.0)
                d = 50.0;
            if(!da && dn == 40.0)
                d = 36.2;
            if(da && dn == 50.0)
                d = 63.0;
            if(!da && dn == 50.0)
                d = 45.8;
            if(da && dn == 65.0)
                d = 75.0;
            if(!da && dn == 65.0)
                d = 60.0;
            if(da && dn == 80.0)
                d = 90.0;
            if(!da && dn == 80.0)
                d = 73.0;
            if(da && dn == 100.0)
                d = 110.0;
            if(!da && dn == 100.0)
                d = 90.0;
        }
        return d;

    }

    static int getDN(double da, const QString &material)
    {
        int dn = 0;
        if(material == "Kupfer" || material == "Edelstahl")
        {
            if(da == 12)
                dn = 10;
            if(da == 15)
                dn = 12;
            if(da == 18)
                dn = 15;
            if(da == 22)
                dn = 20;
            if(da == 28)
                dn = 25;
            if(da == 35)
                dn = 32;
            if(da == 42)
                dn = 40;
            if(da == 54)
                dn = 50;
            if(da == 64)
                dn = 60;
            if(da == 76.1)
                dn = 65;
            if(da == 88.9)
                dn = 80;
            if(da == 108)
                dn = 100;
        }
        if(material == "PEXAL Verbund")
        {
            if(da == 12)
                dn = 10;
            if(da == 16)
                dn = 12;
            if(da == 18 || da == 20 )
                dn = 15;
            if(da == 25 || da == 26)
                dn = 20;
            if(da == 32)
                dn = 25;
            if(da == 40)
                dn = 32;
            if(da == 50)
                dn = 40;
            if(da == 63)
                dn = 50;
            if(da == 75)
                dn = 65;
            if(da == 90)
                dn = 80;
            if(da == 110)
                dn = 100;
        }
        if(material == "verz. Stahl")
        {
            if(da == 17.2)
                dn = 10;
            if(da == 21.3)
                dn = 15;
            if(da == 26.9)
                dn = 20;
            if(da == 33.7)
                dn = 25;
            if(da == 42.4)
                dn = 32;
            if(da == 48.3)
                dn = 40;
            if(da == 60.3)
                dn = 50;
            if(da == 76.1)
                dn = 65;
            if(da == 88.9)
                dn = 80;
            if(da == 114.3)
                dn = 100;
        }

        return dn;
    }


public slots:
    void pipeInfoObjectTableClicked(const QString &text);


private slots:
    void closeApplication();
    void infoApplication();
    void showToolsForm(bool status);
    void showObjectNr(bool status);
    void showFlowDirection(bool status);
    void showPipeNr(bool status);
    void showMousePosition(bool status);
    void showPipeSectionsInfo(bool status);
    void showFormVS();
    void showArmaturStatus(bool status);
    void buildingArtChanged(const QString);
    void scrollBarMoved(int);
    void on_spitzendurchflussBox_valueChanged(double /*arg1*/);

    void actionSaveProject();
    void actionLoadProject();
    void actionNewProject();
    void actionDeleteProject();
    void clearScene();

    // Tools slots
    void dropObjectSelcted(const QString &objectType);
    void dropArmaturSelcted(const QString &armaturType);
    void setStrangMarker(const QString &);
    void floorCountChanged(int floors);
    void scaleChanged(int value);
    void floorHeightChanged(double value);
    void installButtonClicked();
    void deletePipeButtonClicked();
    void mainPipeButtonClicked();
    void basementButtonClicked(bool status);
    void testInstallation();
    void calculateInstallation();


    // Scene
    void leftMouseButtonClicked(QPointF mpos);
    void leftMouseButtonReleased(QPointF);
    void mousePositionChanged(QPointF pos);
    QPointF getNextScenePos(QPointF mousePos);

    // GBadObject
    void objectHasDeleted(GBadObject *obj);
    void objectInfo(GBadObject *obj);
    void objectValueChanged(const QMap<QString, QVariant> &map);
    QList<GBadObject *> getObjectList(GBadObject::GBadObjectType type, GPipe *strang);
    void deleteObjects();

    // GPipe
    void pipeSelected(GPipe *pipe);
    void pipeDeleted(GPipe *pipe);
    void pipeMoving(GPipe *pipe);
    void pipeInfo(GPipe *pipe);
    void pipeConnecting(GPipe *pipe);
    void switchFlowDirectionPipe(GPipe *pipe);
    GPipe *pipe(int nr);

    // AcceptWidget
    void cancelConnecting();
    void acceptConnecting();
    void forwardConnectingChanged(bool status);


    void durationChanged(int value);

    // GArmatur
    void leftMouseButtonPressed(QPointF, GArmatur *armatur);
    void armaturInfo(GArmatur *arm);
    void armaturDeleted(GArmatur *arm);

    // Try to calculate the pressure loss
    double pressureLossArmatur(GArmatur *arm);

    // GArmaturInfoDialog
    void armaturDNHasBeenChanged(GArmatur *valve);

    // SectionPipeDialog
    void rowSelect(int row, int column);

private:
    Ui::FSAquaWindow *ui;

    double duration;   // For simulation
    bool positionStrangMarker;
    bool strangMarkerMove;
    bool dropObject;
    bool dropArmatur;
    bool armaturIsMoving;
    bool pipeInstalled;
    bool pipeCalculated;
    bool dropMainPipe;
    bool pipeTested;
    bool statusLoading;
    QMap<int, double> scaleMap;
    QMap<QString, QString> pipeMaterial;


    // the type of connecting between
    // two pipes
    bool forwardConnecting;
    void moveToRight(int strangIndex);

    // Scene
    Scene *scene;
    QGraphicsRectItem sceneFrame;
    void resizeScene();
    void resizeFrame();

    // GBadobject
    QList<GBadObject *> objectList;
    GBadObject *selectedObject;
    GBadObject *getGBadObject(int nr);
    bool collision(GBadObject *src);
    bool removeObjectFromStrang(GBadObject *obj);
    void renumberObjects();
    void updateObjects();


    // GPipe
    QList<GPipe *> pipeList;
    GPipe *selectedMarker;
    GPipe *selectedPipe;
    QList<GPipe *> getStrangMarkerList();
    void updateStrangMarker();
    bool installConectingPipe();
    bool installCollectingPipe();
    void installRisingPipe();
    bool pipeStartConnecting;
    QMap<int, GPipe *> connectingPipeMap;
    QMap<int, GPipe *> insertedPipeMap;
    void connectingPipeTogether();
    bool collisionPipe(GPipe *pipe);
    void connectingPipeFailed(const QString &message);
    int countConnectedPipe(QPointF pos);
    double getHeightDiff(QPointF pos1, QPointF pos2);

    double getPeakFlow(GPipe *pipe);
    void resetPipeColor();

    QList<GPipe *> previousPipe(GPipe *pipe);
    QList<GPipe *> previousCPipe(GPipe *pipe);
    QList<GPipe *> nextPipe(GPipe *pipe);
    bool hasPrevious(GPipe *pipe);
    bool hasNext(GPipe *pipe);

    // When testing the installation using this function
    QMap<int, QList<GPipe *>> teilstreckenMap;
    QList<GPipe *> getListConnection(GPipe *pipe);
    bool createSectionPipeMap();
    void setupPipeObjects( QMap<int, QList<GPipe *>>);
    bool containsMainPipe(QList<GPipe *> pipeList);
    bool objectListContains(const QList<GBadObject *> &list, GBadObject::GBadObjectType type);
    void updateHeightPressureDiff();
    void updatePressureLossArmatur();
    double getPipeListLength(QList<GPipe *> pList);
    double getDPgeo(QList<GPipe *> pList);  // returns the hDiff in hPa

    // When calculate the pipe
    int getPipeLU(GPipe *pipe);
    QMap<int, QList<int>> calculatedPipeMap;
    QMap<int, QList<int>> setupCalculatedMap();

    // Floors
    QList<QGraphicsRectItem*> floorList;
    int getFloorIndex(QPointF pos);

    // Measure
    QList<QGraphicsItem *> measureList;
    QGraphicsTextItem *measureText;
    QGraphicsLineItem *measureLine;

    int floorCount;
    int floorHeight;
    int floorThickness;
    void resizeFloors();
    void updateFloors();


    // Labels for foors
    QList<QGraphicsTextItem *> labelList;
    bool withBasement;
    void updateLabels();

    // Label in scene for mouse pos
    QGraphicsTextItem *mouseText;
    QGraphicsTextItem *statusText;
    void updateMouseText(QPointF pos);
    void updateStatusText(const QString text, QPointF pos);
    void removeStatusText();


    // Proxis widget
    QGraphicsProxyWidget *acceptProxy;
    AcceptWidget *acceptWidget;
    QGraphicsRectItem *acceptRect;
    QGraphicsRectItem *simulationRect;

    SimulationSllider *simulationWidget;
    void updateProxiWidgets();


    // Form Tools
    FormTools *tools;

    // Formular
    FormFormularVs *formvs;

    // PipeListWidget
    PipeListWidget *SectionPipeDialog;

    // GArmatur
    QList<GArmatur *> armaturList;
    GArmatur *selectedArmatur;

    void calculateDINEN806();
    void calculateDIN1988();
    void sendMessage(const QString &message);
    void hideMessage();
    void updateFlow();
    void updatePeakFlow(double pf);
    void readSettings();
    void saveSettings();
    void Sleep(int msec);
};

#endif // FSAQUAWINDOW_H
