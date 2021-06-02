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
            list << "Kupfer" << "Edelstahl" << "verz. Stahl" << "PEXAL Verbund" <<
                    "PE-X" << "PB" << "PP" << "PVC";

        if(dinTyp == "DIN 1988")
            list << "Kupfer" << "Edelstahl" << "verz. Stahl" << "PEXAL Verbund" <<
                    "PE-X" << "PB" << "PP" << "PVC";



        return list;
    }

    static QList<int> dnList(){
        QList<int>dn;
        dn << 12 << 15 << 20 << 25 << 32 << 40 << 50 << 60 << 65 << 80 << 100;
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



private:
    Ui::FSAquaWindow *ui;

    double duration;   // For simulation
    bool positionStrangMarker;
    bool strangMarkerMove;
    bool dropObject;
    bool dropArmatur;
    bool armaturIsMoving;
    bool pipeInstalled;
    bool dropMainPipe;
    bool pipeTested;
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
    double getFlow(GPipe *pipe);
    double getPeakFlow(GPipe *pipe);

    QList<GPipe *> previousPipe(GPipe *pipe);
    bool hasPrevious(GPipe *pipe);
    QMap<int, QList<GPipe *>> teilstreckenMap;
    QList<GPipe *> testConnection(GPipe *pipe);
    bool testLength(QList<GPipe *> pipeList);
    bool containsMainPipe(QList<GPipe *> pipeList);

    // Floors
    QList<QGraphicsRectItem*> floorList;
    int getFloorIndex(QPointF pos);

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
    void updateMouseText(QPointF pos);

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
    PipeListWidget *formpipelist;

    // GArmatur
    QList<GArmatur *> armaturList;
    GArmatur *selectedArmatur;


    void updateFlow();
    void updatePeakFlow(double pf);
    void readSettings();
    void saveSettings();
    void Sleep(int msec);
};

#endif // FSAQUAWINDOW_H
