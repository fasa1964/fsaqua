#include "fsaquawindow.h"
#include "ui_fsaquawindow.h"

#include <QSettings>

#include <QBrush>
#include <QFont>
#include <QCursor>
#include <QMessageBox>
#include <QtMath>
#include <QTime>
#include <QScrollBar>
#include <QFile>
#include <QFileDialog>

#include <gbadobjectinfodialog.h>
#include <gpipeinfodialog.h>
#include <garmaturinfodialog.h>

#include <QDebug>

FSAquaWindow::FSAquaWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FSAquaWindow)
{
    ui->setupUi(this);

    setWindowTitle("FSAqua");

    ui->placeHolder1Button->hide();
    ui->placeHolder2Button->hide();

    // Value settings
    duration = 4 * 150;
    positionStrangMarker = false;
    strangMarkerMove = false;
    pipeInstalled = false;
    dropObject = false;
    dropArmatur = false;
    armaturIsMoving = false;
    floorHeight = 250;
    floorCount = 1;
    floorThickness = 20;
    withBasement = false;
    pipeStartConnecting = false;
    dropMainPipe = false;
    forwardConnecting = true;
    pipeTested = false;

    selectedObject = nullptr;
    selectedMarker = nullptr;
    selectedPipe = nullptr;
    selectedArmatur = nullptr;

    ui->buildingBox->insertItems(0, FSAquaWindow::gebaeudeArt());
    ui->dinBox->insertItems(0, FSAquaWindow::calculateDIN());

    scaleMap.insert(1, 0.4);
    scaleMap.insert(3, 0.5);
    scaleMap.insert(4, 0.6);
    scaleMap.insert(5, 0.7);
    scaleMap.insert(6, 0.8);
    scaleMap.insert(7, 0.9);
    scaleMap.insert(8, 1.0);
    scaleMap.insert(9, 1.1);
    scaleMap.insert(10, 1.2);
    scaleMap.insert(11, 1.3);
    scaleMap.insert(12, 1.4);
    scaleMap.insert(13, 1.5);
    scaleMap.insert(14, 1.6);
    scaleMap.insert(15, 1.7);
    scaleMap.insert(16, 2.0);
    scaleMap.insert(17, 2.5);

    // Tools Form
    tools = new FormTools();
    tools->hide();
    tools->setRangeFloors(1, MAXFLOORS-1);
    tools->enableObjectButtons(false);
    tools->enableArmaturButtons(false);

    // Scene
    scene = new Scene();
    ui->editorView->setScene( scene );
    scene->setSceneRect(0,0,784,MINSCENEHEIGHT);

    sceneFrame.setRect(0,0,764,360);
    scene->addItem(&sceneFrame);
    sceneFrame.setPos(0,0);
    floorCountChanged(1);

    mouseText = new QGraphicsTextItem("Höhe: ");

    formvs = new FormFormularVs();
    formvs->hide();

    readSettings();

    // Main menu
    connect(ui->actionClose , &QAction::triggered, this, &FSAquaWindow::closeApplication);
    connect(ui->actionInfo , &QAction::triggered, this, &FSAquaWindow::infoApplication);
    connect(ui->actionSaveProject , &QAction::triggered, this, &FSAquaWindow::actionSaveProject);
    connect(ui->actionLoadProject , &QAction::triggered, this, &FSAquaWindow::actionLoadProject);
    connect(ui->actionNewProject , &QAction::triggered, this, &FSAquaWindow::actionNewProject);
    connect(ui->actionTools , &QAction::triggered, this, &FSAquaWindow::showToolsForm);
    connect(ui->actionShowObjectNr , &QAction::triggered, this, &FSAquaWindow::showObjectNr );
    connect(ui->actionShowFlowDirection , &QAction::triggered, this, &FSAquaWindow::showFlowDirection );
    connect(ui->actionShowPipeNr , &QAction::triggered, this, &FSAquaWindow::showPipeNr );
    connect(ui->actionShowMousePos, &QAction::triggered, this, &FSAquaWindow::showMousePosition );
    connect(ui->actionShowPipesectionsInfo, &QAction::triggered, this, &FSAquaWindow::showPipeSectionsInfo );
    connect(ui->actionShowArmaturStatus, &QAction::triggered, this, &FSAquaWindow::showArmaturStatus );
    connect(ui->formularFlowButton, &QToolButton::clicked, this, &FSAquaWindow::showFormVS );
    connect(ui->buildingBox, &QComboBox::currentTextChanged, this, &FSAquaWindow::buildingArtChanged );
    connect(ui->editorView->verticalScrollBar(), &QScrollBar::sliderMoved , this, &FSAquaWindow::scrollBarMoved );
    connect(ui->editorView->horizontalScrollBar(), &QScrollBar::sliderMoved , this, &FSAquaWindow::scrollBarMoved );


    // Tools
    connect(tools, &FormTools::dropObjectSelcted, this, &FSAquaWindow::dropObjectSelcted);
    connect(tools, &FormTools::dropArmaturSelcted, this, &FSAquaWindow::dropArmaturSelcted);
    connect(tools, &FormTools::setStrangMarker, this, &FSAquaWindow::setStrangMarker);
    connect(tools, &FormTools::floorCountChanged, this, &FSAquaWindow::floorCountChanged);
    connect(tools, &FormTools::scaleChanged, this, &FSAquaWindow::scaleChanged);
    connect(tools, &FormTools::floorHeightChanged, this, &FSAquaWindow::floorHeightChanged);
    connect(tools, &FormTools::installButtonClicked, this, &FSAquaWindow::installButtonClicked);
    connect(tools, &FormTools::deletePipeButtonClicked, this, &FSAquaWindow::deletePipeButtonClicked);
    connect(tools, &FormTools::mainPipeButtonClicked, this, &FSAquaWindow::mainPipeButtonClicked);
    connect(tools, &FormTools::basementClicked, this, &FSAquaWindow::basementButtonClicked);
    connect(tools, &FormTools::testInstallClicked, this, &FSAquaWindow::testInstallation);


    // Scene
    connect(scene, &Scene::leftMouseButtonReleased, this, &FSAquaWindow::leftMouseButtonReleased);
    connect(scene, &Scene::leftMouseButtonClicked, this, &FSAquaWindow::leftMouseButtonClicked);
    connect(scene, &Scene::mousePos, this, &FSAquaWindow::mousePositionChanged);


    // Rotate the scene by 180°
    QTransform trans;
    trans.rotate(180, Qt::XAxis);
    ui->editorView->setTransform( trans );
    ui->editorView->setMouseTracking( true );



    qreal w = geometry().width()+30;
    qreal h = geometry().height()+30;
    setGeometry(geometry().x(), geometry().y(), w, h);

    // Proxys
    acceptWidget = new AcceptWidget();
    acceptRect = scene->addRect(0, 0, acceptWidget->width(), 20, QPen(Qt::black), QBrush(Qt::darkGreen));
    acceptRect->setFlag(QGraphicsItem::ItemIsMovable, true);
    acceptRect->setFlag(QGraphicsItem::ItemIsSelectable, true);
    QGraphicsProxyWidget *proxy = scene->addWidget(acceptWidget);
    proxy->setTransform(trans);
    proxy->setParentItem(acceptRect);
    acceptRect->hide();

    simulationWidget = new SimulationSllider();
    simulationRect = scene->addRect(0, 0, simulationWidget->width(), 20, QPen(Qt::black), QBrush(Qt::darkGreen));

    simulationRect->setFlag(QGraphicsItem::ItemIsMovable, true);
    simulationRect->setFlag(QGraphicsItem::ItemIsSelectable, true);
    QGraphicsProxyWidget *proxySim = scene->addWidget(simulationWidget);
    proxySim->setTransform(trans);
    proxySim->setParentItem(simulationRect);
    simulationRect->hide();


    // Proxys
    connect(acceptWidget, &AcceptWidget::cancelConnecting, this, &FSAquaWindow::cancelConnecting);
    connect(acceptWidget, &AcceptWidget::acceptConnecting, this, &FSAquaWindow::acceptConnecting);
    connect(acceptWidget, &AcceptWidget::forwardConnecting, this, &FSAquaWindow::forwardConnectingChanged);

    connect(simulationWidget, &SimulationSllider::durationChanged, this, &FSAquaWindow::durationChanged);


    // PipeListWidget
    formpipelist = new PipeListWidget(0);
    formpipelist->hide();


    ui->editorView->update();
    scene->update();
}

FSAquaWindow::~FSAquaWindow()
{
    delete ui;
}

void FSAquaWindow::resizeEvent(QResizeEvent *)
{
    resizeScene();
    resizeFrame();
    resizeFloors();
    updateLabels();
}

void FSAquaWindow::pipeInfoObjectTableClicked(const QString &text)
{
    bool ok;
    int nr = text.toInt(&ok);
    foreach(GBadObject *o, objectList){
        if(o->getNr() != nr)
            getGBadObject(o->getNr())->setSelected(false);
        if(o->getNr() == nr)
            getGBadObject(o->getNr())->setSelected(true);

    }
}

void FSAquaWindow::closeApplication()
{
    saveSettings();

    tools->close();
    formpipelist->close();

    close();
}

void FSAquaWindow::infoApplication()
{

}

void FSAquaWindow::showToolsForm(bool status)
{
    if(status)
        tools->show();
    else
        tools->hide();
}

void FSAquaWindow::showObjectNr(bool status)
{
    foreach(GBadObject *o, objectList)
        o->setDisplayNr(status);

    scene->update();
}

void FSAquaWindow::showFlowDirection(bool status)
{
    foreach(GPipe *p, pipeList)
        p->setShowArrow(status);
    scene->update();
}

void FSAquaWindow::showPipeNr(bool status)
{
    foreach(GPipe *p, pipeList)
        p->setShowNr(status);
    scene->update();
}

void FSAquaWindow::showMousePosition(bool status)
{
    if(!status){
        if(mouseText != nullptr){
            scene->removeItem(mouseText);
            scene->update();
        }
    }
}

void FSAquaWindow::showPipeSectionsInfo(bool status)
{
    if(status)
        formpipelist->show();
    else
        formpipelist->hide();

    ui->actionShowPipesectionsInfo->setChecked(status);
}

void FSAquaWindow::showFormVS()
{
    formvs->show();
}

void FSAquaWindow::showArmaturStatus(bool status)
{
    foreach(GArmatur *a, armaturList)
        a->setShowStatus(status);
    scene->update();
}

void FSAquaWindow::buildingArtChanged(const QString /*&text*/)
{
    updateFlow();
}

void FSAquaWindow::scrollBarMoved(int)
{
    updateProxiWidgets();
}

void FSAquaWindow::on_spitzendurchflussBox_valueChanged(double)
{
    if(pipeTested){
        pipeTested = false;
        tools->setBlinkButton(true, "test");
    }
}

void FSAquaWindow::actionSaveProject()
{
    QString projectName = ui->projectNameEdit->text();

    if(projectName.isEmpty())
        return;

    QFile file(projectName+".prj");
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::information(this, tr("File error"), tr("Not able to write project file!Check your permission."));
        return;
    }
    else
    {

        QMap<QString, QVariant> map;


        map.insert("name", ui->projectNameEdit->text());
        map.insert("custom", "TestMich");
        map.insert("art", ui->buildingBox->currentText() );
        map.insert("floorcount", tools->getFloorCount());
        map.insert("floorheight", tools->getfloorHeight());
        map.insert("basement", tools->getBasement() );
        map.insert("din", ui->dinBox->currentText());
        map.insert("scenerect", scene->sceneRect());
        map.insert("objectcount", objectList.size());
        map.insert("pipecount", pipeList.size());
        map.insert("armaturcount", armaturList.size());
        map.insert("pipeinstalled", pipeInstalled);
        map.insert("pipetested", pipeTested);

        if(!teilstreckenMap.isEmpty())
        {
            QMap<QString, QVariant> vMap;
            QMapIterator<int, QList<GPipe *> > it(teilstreckenMap);
            while (it.hasNext()) {
                it.next();
                QString value = QString("%1").arg(it.key());
                QStringList nrList;
                for(int i = 0; i < it.value().size(); i++)
                    nrList << QString("%1").arg( it.value().at(i)->getNr() );
                vMap.insert(value, nrList);
            }

            map.insert("teilstreckenmap", vMap);

        }

        QDataStream out(&file);
        out << map;
        foreach(GBadObject *obj, objectList)
        {
            GBadObject::GBadObjectType t;
            (quint32&) t = obj->type();

            out << t << obj->scenePos() << obj->getNr() << obj->bezeichnung() << obj->fabrikat() <<
                obj->getFloorIndex() << obj->lu() << obj->bdKW() << obj->bdWW() << obj->md() << obj->kwh() <<
                obj->dn() << obj->getCold() << obj->getHot() << obj->getPipeNr() << obj->getDauerlaeufer();


        }

        foreach(GPipe *p, pipeList)
        {
            GPipe::GPipeType t;
            (quint32&) t = p->type();

            out << t << p->scenePos() << p->getNr() << p->getStrangMarkerNr() << p->getMeter() << p->activ() <<
                   p->cold() << p->circulation() << p->density() << p->length() << p->color() << p->getMaterial() <<
                   p->getFloorIndex() << p->getFlowDirection() << p->getMarker() << p->getObjectNrList();
        }


        foreach(GArmatur *a, armaturList)
        {
            GArmatur::GArmaturType t;
            (quint32&) t = a->type();

            out << t << a->scenePos() << a->nr() << a->pipeNr() << a->dn() << a->pg() << a->vg() << a->dichte() <<
                 a->flowSpeed() << a->rotation() << a->getEntleerung() << a->getPessureLoss() << a->getZeta() << a->getPeakFlow();
        }


        file.close();
        ui->statusBar->showMessage("Project: " + projectName + " successfully saved!", 5000);
    }
}

void FSAquaWindow::actionLoadProject()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Project"), QDir::currentPath(), tr("Project's (*.prj)"));

    if(filename.isEmpty())
        return;

    QMap<QString, QVariant> map;
    QList<GBadObject *> oList;
    QList<GPipe *> pList;
    QList<GArmatur *> aList;

    QFile file(filename);

    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(this, tr("File error"), tr("Not able to read project file!Check your permission."));
        return;
    }
    else
    {

        setCursor(Qt::IBeamCursor);

        clearScene();
        QDataStream in(&file);
        in >> map;

        bool ok;
        int countObject = map.value("objectcount").toInt(&ok);
        int countPipe = map.value("pipecount").toInt(&ok);
        int countArmatur = map.value("armaturcount").toInt(&ok);


        for(int i = 0; i < countObject; i++)
        {
            GBadObject::GBadObjectType t;
            int nr;
            QString bez;
            QString fab;
            QPointF sp;
            int findex;
            int lu;
            double kw;
            double ww;
            double md;
            double kwh;
            int dn;
            bool cold;
            bool hot;
            int pnr;
            bool pl;


            in >> (quint32&)t >> sp >> nr >> bez >> fab >> findex >> lu >> kw >> ww >> md >> kwh
               >> dn >> cold >> hot >> pnr >> pl;

            GBadObject *obj = new GBadObject(t,0);
            obj->setBezeichnung(bez);
            obj->setNr(nr);
            obj->setFabrikat(fab);
            obj->setSPos(sp);
            obj->setFloorIndex(findex);
            obj->setLu(lu);
            obj->setBdKW(kw);
            obj->setBdWW(ww);
            obj->setMd(md);
            obj->setKwh(kwh);
            obj->setDn(dn);
            obj->setCold(cold);
            obj->setHot(hot);
            obj->setPipeNr(pnr);
            obj->setDauerlaeufer(pl);


//            GBadObject obj;
//            in >> obj;

//            GBadObject *o = new GBadObject(obj);
            oList << obj;
        }

        for(int l = 0; l < countPipe; l++)
        {
            GPipe::GPipeType t;
            QPointF sp;
            int nr;
            int strangNr;
            double meter;
            bool activ;
            bool cold;
            bool circ;
            double density;
            double len;
            QColor col;
            QString mat;
            int findex;
            QString direction;
            bool marker;
            QList<int> nrList;


            in >> (quint32&)t >> sp >> nr >> strangNr >> meter >> activ >> cold >> circ >>
                  density >> len >> col >> mat >> findex >> direction >> marker >> nrList;

            GPipe *p = new GPipe(t,cold, len,0);
            p->setSPos(sp);
            p->setNr(nr);
            p->setStrangMarkerNr(strangNr);
            p->setMeter(meter);
            p->setActiv(activ);
            p->setCold(cold);
            p->setCirculation(circ);
            p->setDensity(density);
            p->setLength(len);
            p->setColor(col);
            p->setMaterial(mat);
            p->setFloorIndex(findex);
            p->setFlowDirection(direction);
            p->setMarker(marker);
            p->setObjectNrList( nrList );

            pList << p;
        }

        for(int a = 0; a < countArmatur; a++)
        {
            GArmatur::GArmaturType t;
            QPointF sp;
            int nr;
            int pnr;
            int dn;
            double pg;
            double vg;
            double dichte;
            double speed;
            double rot;
            bool entl;
            double hPa;
            double z;
            double pf;


            in >> (quint32&)t >> sp >> nr >> pnr >> dn >> pg >> vg >> dichte >> speed >> rot >> entl >>
                  hPa >> z >> pf;

            GArmatur *arm = new GArmatur(t,0);
            arm->setSPos(sp);
            arm->setNr(nr);
            arm->setPipeNr(pnr);
            arm->setDn(dn);
            arm->setPg(pg);
            arm->setVg(vg);
            arm->setDichte(dichte);
            arm->setFlowSpeed(speed);
            arm->setRotation(rot);
            arm->setEntleerung(entl);
            arm->setPessureLoss(hPa);
            arm->setZeta(z);
            arm->setPeakFlow(pf);

            aList << arm;
        }
    }

    if(!map.isEmpty())
    {
        bool ok;
        ui->projectNameEdit->setText( map.value("name").toString() );
        ui->customNameEdit->setText( map.value("custom").toString());
        ui->buildingBox->setCurrentText( map.value("art").toString() );
        tools->setFloorCount( map.value("floorcount").toInt(&ok));
        tools->setfloorHeight( map.value("floorheight").toDouble(&ok));
        tools->setBasement( map.value("basement").toBool() );
        scene->setSceneRect( map.value("scenerect").toRectF());
        pipeInstalled = map.value("pipeinstalled").toBool();
        pipeTested = map.value("pipetested").toBool();
        ui->dinBox->setCurrentText( map.value("din").toString());

        foreach(GBadObject *obj, oList)
        {
            scene->addItem(obj);
            obj->setPos( obj->sPos());

            objectList << obj;

            connect(obj, &GBadObject::objectHasDeleted, this, &FSAquaWindow::objectHasDeleted);
            connect(obj, &GBadObject::objectInfo, this, &FSAquaWindow::objectInfo);
        }

        foreach(GPipe *p, pList)
        {
            scene->addItem(p);
            p->setPos( p->getSPos() );

            pipeList << p;

            // For all kind of pipes
            connect(p, &GPipe::pipeInfo, this, &FSAquaWindow::pipeInfo);
            connect(p, &GPipe::pipeIsSelected, this, &FSAquaWindow::pipeSelected);
            connect(p, &GPipe::pipeHasDeleted,  this, &FSAquaWindow::pipeDeleted);

            if(!p->getMarker())
                connect(p, &GPipe::pipeConnect , this, &FSAquaWindow::pipeConnecting);


            if(p->getMarker())
                connect(p, &GPipe::pipeMove ,  this, &FSAquaWindow::pipeMoving);


            foreach(int nr, p->getObjectNrList())
                p->appendGBadObject( getGBadObject(nr) );

        }

        foreach(GArmatur *a, aList)
        {
            scene->addItem(a);
            a->setPos( a->sPos() );

            armaturList << a;
            connect(a, &GArmatur::leftMouseButtonPressed, this, &FSAquaWindow::leftMouseButtonPressed);
            connect(a, &GArmatur::armaturHasDeleted, this, &FSAquaWindow::armaturDeleted);
            connect(a, &GArmatur::armaturInfo, this, &FSAquaWindow::armaturInfo);
        }

        updateFlow();
        if(!pipeInstalled)
        {
            tools->enableMainPipeButton(false);
            tools->enableTestInstallButton(false);
            tools->enableDeletePipeButton(false);
            tools->enableArmaturButtons(false);

        }
        else
        {
            tools->enableMainPipeButton(true);
            tools->enableTestInstallButton(true);
            tools->enableDeletePipeButton(true);
            tools->enableArmaturButtons(true);
        }

        if(!objectList.isEmpty())
            tools->enableInstallButton(true);

        resizeScene();
        resizeFrame();
        resizeFloors();
        updateLabels();

        if(pipeTested)  // Pipe was tested
        {
            QMap<QString, QVariant> vMap;
            vMap = map.value("teilstreckenmap").toMap();
            QMapIterator<QString, QVariant> it(vMap);
            while (it.hasNext()) {
                it.next();

                bool ok;
                int nr = it.key().toInt(&ok);
                QStringList nrList = it.value().toStringList();
                QList<GPipe *> pipeList;
                for(int i = 0; i < nrList.size(); i++){
                    int pipeNr = nrList.at(i).toInt(&ok);
                    pipeList << pipe( pipeNr );
                }

//                qDebug() << "-----------------";
//                qDebug() << it.key();
//                qDebug() << it.value();
//                qDebug() << "-----------------";
                teilstreckenMap.insert(nr, pipeList);
                formpipelist->insertRow(pipeList, nr, true);

            }
        }
    }

}

void FSAquaWindow::actionNewProject()
{
    clearScene();
}

void FSAquaWindow::clearScene()
{
    // Clear the Formpipelist
    teilstreckenMap.clear();
    formpipelist->clearList();

    deletePipeButtonClicked();
    deleteObjects();

    // Remove all raising pipe marker
    foreach(GPipe *p, pipeList){
        if(pipeList.removeOne(p))
            scene->removeItem(p);
    }

    objectList.clear();
    pipeList.clear();
    armaturList.clear();
    pipeTested = false;

}



void FSAquaWindow::dropObjectSelcted(const QString &objectType)
{
    if( objectType == "wt")
        selectedObject = new GBadObject(GBadObject::WT);
    if( objectType == "wc")
        selectedObject = new GBadObject(GBadObject::WC);
    if( objectType == "wcdc")
        selectedObject = new GBadObject(GBadObject::WCDS);
    if( objectType == "wa")
        selectedObject = new GBadObject(GBadObject::WA);
    if( objectType == "du")
        selectedObject = new GBadObject(GBadObject::DU);
    if( objectType == "ur")
        selectedObject = new GBadObject(GBadObject::UR);
    if( objectType == "sw")
        selectedObject = new GBadObject(GBadObject::SW);
    if( objectType == "wm")
        selectedObject = new GBadObject(GBadObject::WM);
    if( objectType == "gs")
        selectedObject = new GBadObject(GBadObject::GS);
    if( objectType == "agav")
        selectedObject = new GBadObject(GBadObject::AGAV);
    if( objectType == "sp")
        selectedObject = new GBadObject(GBadObject::SP);
    if( objectType == "dhe")
        selectedObject = new GBadObject(GBadObject::DHE);
    if( objectType == "wsib")
        selectedObject = new GBadObject(GBadObject::WSIB);
    if( objectType == "av")
        selectedObject = new GBadObject(GBadObject::AV);

    dropObject = true;
    setCursor(QCursor(QPixmap(selectedObject->symbolFilename()).scaled(40,40,Qt::AspectRatioMode::KeepAspectRatio)));
}

void FSAquaWindow::dropArmaturSelcted(const QString &armaturType)
{
    if(armaturType == "rv")
        selectedArmatur = new GArmatur(GArmatur::RV);

    if(armaturType == "gv")
        selectedArmatur = new GArmatur(GArmatur::GV);

    if(armaturType == "svrv")
        selectedArmatur = new GArmatur(GArmatur::SVRV);

    if(armaturType == "wz")
        selectedArmatur = new GArmatur(GArmatur::WZ);

    if(armaturType == "sv")
        selectedArmatur = new GArmatur(GArmatur::SV);

    if(armaturType == "dm")
        selectedArmatur = new GArmatur(GArmatur::DM);

    if(armaturType == "fil")
        selectedArmatur = new GArmatur(GArmatur::FIL);

    if(armaturType == "fsve")
        selectedArmatur = new GArmatur(GArmatur::FSVE);

    if(armaturType == "kh")
        selectedArmatur = new GArmatur(GArmatur::KH);

    if(armaturType == "svte")
        selectedArmatur = new GArmatur(GArmatur::SVTE);

    dropArmatur = true;
    setCursor(QCursor(QPixmap(selectedArmatur->symbolFilename()).scaled(40,40,Qt::AspectRatioMode::KeepAspectRatio)));
}

void FSAquaWindow::setStrangMarker(const QString &)
{
    positionStrangMarker = true;
    setCursor(Qt::SizeVerCursor);

}

void FSAquaWindow::floorCountChanged(int floors)
{

    // Check if any objects inside deleted floors
    if(!objectList.isEmpty())
    {
        QList<GBadObject *> list;
        foreach(GBadObject *o, objectList)
        {
            if(o->getFloorIndex() >= floors)
            {
                list << o;
            }
        }

        if(!list.isEmpty())
        {
            int result = QMessageBox::question(this, tr("Etagen löschen"), tr("In der Etage befinden sich Objekte sollen diese gelöscht werden?"),
                                               QMessageBox::Cancel | QMessageBox::Yes);

            switch (result)
            {
                case QMessageBox::Cancel:{ tools->setFloorCount(floors+1); return; };
                case QMessageBox::Yes:
                {
                    foreach(GBadObject *o, list)
                    {

                        bool successfull = true;

                        if(!objectList.removeOne(o))
                            successfull = false;

                        if(!removeObjectFromStrang(o))
                            successfull = false;

                        if(successfull)
                            scene->removeItem(o);

                        if(!successfull)
                            QMessageBox::information(this, tr("Löschen"), tr("Das Löschen des Objektes hat leider nicht funktioniert!"));
                    }
                    renumberObjects();
                    updateFlow();
                };
            }
        }
    }

    floorCount = floors;
    updateFloors();
    resizeScene();
    resizeFrame();

    updateLabels();
    updateStrangMarker();
}

void FSAquaWindow::scaleChanged(int value)
{
    qreal x = scaleMap.value(value);
    QTransform tr = ui->editorView->transform();
    tr.setMatrix(x,tr.m12(),tr.m13(),tr.m21(),x,tr.m23(),tr.m31(),tr.m32(),tr.m33());
    tr.rotate(180, Qt::XAxis);

    ui->editorView->setTransform(tr);
}

void FSAquaWindow::floorHeightChanged(double value)
{
    floorHeight = value * 100;
    updateFloors();
    resizeScene();
    resizeFrame();

    updateLabels();
    updateStrangMarker();
    updateObjects();
}

void FSAquaWindow::installButtonClicked()
{

    connectingPipeMap.clear();

    // Remove all Armatur
    foreach(GArmatur *a, armaturList)
        scene->removeItem(a);
    armaturList.clear();

    // Remove all pipes
    foreach(GPipe *p, pipeList)
    {
        if(!p->getMarker())
        {
            pipeList.removeOne(p);
            scene->removeItem(p);
        }
    }



   installConectingPipe();
   installCollectingPipe();
   installRisingPipe();
   pipeInstalled = true;
   tools->setBlinkButton(false, "install");

   showFlowDirection(ui->actionShowFlowDirection->isChecked());
   scene->update();

   tools->enableMainPipeButton(true);
   tools->enableTestInstallButton(false);
   tools->enableDeletePipeButton(true);
   tools->enableArmaturButtons(true);

   // Clear the Formpipelist
   teilstreckenMap.clear();
   formpipelist->clearList();

}

void FSAquaWindow::deletePipeButtonClicked()
{

    foreach(GArmatur *arm, armaturList)
    {
        if(armaturList.removeOne(arm))
                scene->removeItem(arm);
            else
                QMessageBox::information(this, tr("Armatur löschen"), tr("Das Löschen der Armatur hat leider nicht funktioniert!"));

    }

    foreach(GPipe *p, pipeList)
    {
        if(!p->getMarker())
        {
            if(pipeList.removeOne(p))
                scene->removeItem(p);
            else
                QMessageBox::information(this, tr("Rohrleitung löschen"), tr("Das Löschen der Rohrleitung hat leider nicht funktioniert!"));

        }
    }

    tools->enableDeletePipeButton(false);
    tools->enableArmaturButtons(false);
    pipeInstalled = false;
}

void FSAquaWindow::mainPipeButtonClicked()
{
    dropMainPipe = true;
    QPixmap pix = QPixmap(":/buttonicons/pipe.png").scaled(40,40,Qt::AspectRatioMode::KeepAspectRatio);
    setCursor(QCursor(pix));
}

void FSAquaWindow::basementButtonClicked(bool status)
{
    withBasement = status;
    updateLabels();
}

void FSAquaWindow::testInstallation()
{
    simulationRect->show();
    updateProxiWidgets();

    showPipeSectionsInfo(true);

    teilstreckenMap.clear();
    formpipelist->clearList();

    formpipelist->setLabelStatus("Überprüfe Verbindungen!");
    ui->statusBar->showMessage("Überprüfe Verbindungen!");
    simulationWidget->setText("Überprüfe Verbindungen!", QColor(0,85,127));

    // Test connecting of pipe
    // --------------------
    QMap<int, QList<GPipe *>> failedMap;
    int index = 0;
    foreach(GPipe *p, pipeList)
    {
        QList<GPipe *> list;
        if(p->type() == GPipe::RGA)
        {
            if(!p->containsObjectType(GBadObject::WSIB) && !p->containsObjectType(GBadObject::DHE))
            {
                list = testConnection(p);

                if(containsMainPipe(list))
                {
                    teilstreckenMap.insert(index, list);
                    ui->statusBar->showMessage(QString("Teilstrecke %1 Verbindungen OK").arg(index));
                    simulationWidget->setText(QString("Teilstrecke %1 Verbindungen OK").arg(index), Qt::darkGreen);


                    formpipelist->insertRow(list, index, true);
                    formpipelist->setLabelStatus(QString("Teilstrecke %1 Verbindungen OK").arg(index));

                    Sleep(duration);
                }
                else
                {
                    ui->statusBar->showMessage(QString("Teilstrecke %1 fehlerhaft").arg(index));
                    simulationWidget->setText(QString("Teilstrecke %1 fehlerhaft").arg(index), Qt::red);
                    failedMap.insert(index,list);

                    formpipelist->insertRow(list, index, false);
                    formpipelist->setLabelStatus(QString("Teilstrecke %1 fehlerhaft").arg(index));
                }
                index++;

                foreach(GPipe *s, list)
                {
                    if(s->cold())
                        s->setColor(Qt::darkGreen);
                    else
                        s->setColor(Qt::red);
                    scene->update();
                }
            }
        }
    }

    formpipelist->setLabelStatus("Überprüfung der Verbindungen abgeschlossen!");
    ui->statusBar->showMessage("Überprüfung der Verbindungen abgeschlossen!",5000);
    simulationWidget->setText("Überprüfung der Verbindungen abgeschlossen!", QColor(0,85,127));
    Sleep(duration);

    // Switch to normal colors
    foreach(GPipe *p, pipeList){
        if(p->cold())
            p->setColor(Qt::darkGreen);
        else
            p->setColor(Qt::red);

    }
    scene->update();


    if(teilstreckenMap.isEmpty())
        return;


    // Test length of pipe
    // --------------------
    formpipelist->setLabelStatus("Überprüfe die Rohrleitungslängen!");
    ui->statusBar->showMessage("Überprüfe die Rohrleitungslängen!");
    simulationWidget->setText("Überprüfe die Rohrleitungslängen!", QColor(0,85,127));

    QMapIterator<int, QList<GPipe *>> it(teilstreckenMap);
    while (it.hasNext()) {
        it.next();
        if(testLength(it.value()))
            formpipelist->setText(it.key(), 2, "OK");
        else
            formpipelist->setText(it.key(), 2, "Failed");
        Sleep(duration);
    }

    formpipelist->setLabelStatus("Überprüfung der Rohrleitungslängen abgeschossen!");
    ui->statusBar->showMessage("Überprüfung der Rohrleitungslängen abgeschossen!",5000);
    simulationWidget->setText("Überprüfung der Rohrleitungslängen abgeschossen!", QColor(0,85,127));


    Sleep(duration);
    formpipelist->setLabelStatus("Beginne mit der Zuordnung der Objekte!");
    ui->statusBar->showMessage("Beginne mit der Zuordnung der Objekte!");
    simulationWidget->setText("Beginne mit der Zuordnung der Objekte!", QColor(0,85,127));

    // Setup flow and peak flow of each pipe
    QMapIterator<int, QList<GPipe *>> itc(teilstreckenMap);
    while (itc.hasNext())
    {
        itc.next();
        double sd = 0.0;
        int lu = 0;
        QList<GBadObject *> oList;
        foreach(GPipe *p, itc.value())
        {
            foreach(GBadObject *o, p->badObjectList())
            {
                if(!oList.contains(o))
                {
                    oList << o;

                    if(p->cold())
                        sd += o->bdKW();
                    else
                        sd += o->bdWW();
                    lu += o->lu();
                }
            }

            p->setColor(Qt::cyan);
            p->setGBadObjectList(oList);
            scene->update();
            Sleep(duration);
        }


        // Reset color and calculate the flow
        foreach(GPipe *p, itc.value())
        {
            if(p->cold())
                p->setColor(Qt::darkGreen);
            else
                 p->setColor(Qt::red);
            scene->update();
        }

        formpipelist->setFlow(itc.key(), 3  ,sd);
        formpipelist->setLu(itc.key(),4 ,lu);

        formpipelist->setLabelStatus(QString("Teilstrecke %1 OK").arg(itc.key()));
        simulationWidget->setText(QString("Teilstrecke %1 OK").arg(itc.key()), QColor(0,85,127));

    }

    formpipelist->setLabelStatus("Zuordnung der Objekte abgeschlossen!");
    ui->statusBar->showMessage("Zuordnung der Objekte abgeschlossen!");
    simulationWidget->setText("Zuordnung der Objekte abgeschlossen!", QColor(0,85,127));

    Sleep(5000);
    simulationRect->hide();
    pipeTested = true;
    tools->setBlinkButton(false, "test");
}

void FSAquaWindow::leftMouseButtonClicked(QPointF mpos)
{
    // -----------------------------
    // Droping object into the scene
    // -----------------------------
    if(selectedObject != nullptr && dropObject && selectedMarker != nullptr)
    {
        scene->addItem(selectedObject);

        QPointF np = getNextScenePos(mpos);
        if(np.x() == 0.0 || np.y() == 0.0)
        {
            QMessageBox::information(this, tr("Objects"), tr("Object droped outside the scene!"
            "\nPlease try again. Keep mouse inside the scene."));
            return;
        }

        selectedObject->setPos(np);
        objectList << selectedObject;

        selectedObject->setNr( objectList.size() );
        selectedObject->setFloorIndex( getFloorIndex( selectedObject->scenePos() ));

        selectedMarker->appendGBadObject(selectedObject);

        connect(selectedObject, &GBadObject::objectHasDeleted, this, &FSAquaWindow::objectHasDeleted);
        connect(selectedObject, &GBadObject::objectInfo, this, &FSAquaWindow::objectInfo);

        if(ui->actionShowObjectNr->isChecked())
            selectedObject->setDisplayNr(true);

        // check if any collision with other objects
        if( collision(selectedObject) )
        {
            int index = getStrangMarkerList().size()-1;
            moveToRight(index);
        }

        // check is strang marker inside the scene
        if( selectedObject->scenePos().x() >= scene->sceneRect().width()-168 )
        {
            QRectF srect = scene->getSceneRect();
            scene->setSceneRect(0,0,srect.width()+168, srect.height());
            resizeFrame();
            resizeFloors();
            scene->update();
        }

        // check if object droped to far left
        if( selectedObject->scenePos().x() <= 100 )
        {
            for(int i = 0; i < getStrangMarkerList().size(); i++)
                moveToRight(i);
        }

        dropObject = false;
        selectedObject = nullptr;
        setCursor(Qt::ArrowCursor);
        tools->enableInstallButton(true);
        updateFlow();

        if(pipeInstalled)
            tools->setBlinkButton(true, "install");

    }

    // ------------------------------
    // Droping armatur
    // ------------------------------
    if(dropArmatur && selectedArmatur != nullptr && selectedPipe != nullptr)
    {
        scene->addItem(selectedArmatur);

        if(selectedPipe->type() == GPipe::STRANG || selectedPipe->type() == GPipe::RGA)
        {
            if(selectedPipe->getFlowDirection() == "down" && selectedArmatur->type() == GArmatur::RV)
            {
                QTransform t = selectedArmatur->transform();
                t.rotate(90, Qt::ZAxis);
                t.rotate(180, Qt::YAxis);
                selectedArmatur->setTransform(t);
                selectedArmatur->setTransformOriginPoint( selectedArmatur->boundingRect().center());
            }
            else if(selectedPipe->getFlowDirection() == "up" && selectedArmatur->type() == GArmatur::SVRV)
            {
                QTransform t = selectedArmatur->transform();
                t.rotate(90, Qt::ZAxis);
                t.rotate(180, Qt::YAxis);
                selectedArmatur->setTransform(t);
                selectedArmatur->setTransformOriginPoint( selectedArmatur->boundingRect().center());
            }
            else if(selectedArmatur->type() != GArmatur::FSVE)
                selectedArmatur->setRotation(90);


            selectedArmatur->setPos( selectedPipe->scenePos().x(), selectedPipe->getPipeMitte().y() );

            if(selectedArmatur->type() == GArmatur::FSVE){
                selectedArmatur->setPos( selectedPipe->scenePos().x()-selectedArmatur->boundingRect().width()/2+4 , selectedPipe->getPipeMitte().y());
            }

        }

        if(selectedPipe->type() == GPipe::RGS )
        {

            if(selectedArmatur->type() == GArmatur::RV && selectedPipe->getFlowDirection() == "right")
            {
                selectedArmatur->setTransformOriginPoint( selectedArmatur->boundingRect().center());
                selectedArmatur->setRotation(180);
            }

            if(selectedArmatur->type() == GArmatur::SVRV && selectedPipe->getFlowDirection() == "left")
            {
                QTransform t = selectedArmatur->transform();
                t.rotate(180, Qt::YAxis);
                selectedArmatur->setTransform(t);
                selectedArmatur->setTransformOriginPoint( selectedArmatur->boundingRect().center());
            }

            selectedArmatur->setPos( selectedPipe->getPipeMitte().x(), selectedPipe->scenePos().y() );

            if(selectedArmatur->type() == GArmatur::FSVE)
                selectedArmatur->setPos( selectedPipe->getPipeMitte().x(), selectedPipe->scenePos().y() + selectedArmatur->boundingRect().height()/2 - 3 );
        }



        armaturList << selectedArmatur;
        selectedArmatur->setNr(armaturList.size());
        selectedArmatur->setPipeNr( selectedPipe->getNr());
        selectedArmatur->setDichte( selectedPipe->density() );
        double pf = getPeakFlow( selectedPipe ) * 3.600; // in m3/h
        selectedArmatur->setPeakFlow( pf );

        // try to calculate the pressure loss bei zeta
        double hPa = pressureLossArmatur(selectedArmatur);
        selectedArmatur->setPessureLoss(hPa);

        connect(selectedArmatur, &GArmatur::leftMouseButtonPressed, this, &FSAquaWindow::leftMouseButtonPressed);
        connect(selectedArmatur, &GArmatur::armaturHasDeleted, this, &FSAquaWindow::armaturDeleted);
        connect(selectedArmatur, &GArmatur::armaturInfo, this, &FSAquaWindow::armaturInfo);

        dropArmatur = false;
        selectedPipe = nullptr;
        setCursor(Qt::ArrowCursor);
    }

    // -----------------------------
    // Droping strang marker into the scene
    // -----------------------------
    if(positionStrangMarker)
    {

        // check first if any marker without object's
        foreach(GPipe *p, getStrangMarkerList())
        {
            if(p->badObjectList().isEmpty())
            {
                QMessageBox::information(this, tr("Strang"), tr("Rising pipe without object's detected."
                "\nPlease insert object's first!"));
                positionStrangMarker = false;
                setCursor(Qt::ArrowCursor);
                return;
            }
        }


        GPipe *strang = new GPipe(GPipe::STRANG, true, scene->getSceneRect().height());
        strang->setMarker(true);
        scene->addItem(strang);
        strang->setNr(pipeList.size()+1);
        strang->setStrangMarkerNr(pipeList.size()+1);

        if(objectList.isEmpty())
        {
            strang->setPos(scene->sceneRect().width()/2, 0);
        }
        else
        {
            qreal xpos = 0;
            foreach(GBadObject *o, objectList)
            {
                if(o->scenePos().x() > xpos)
                    xpos = o->scenePos().x();
            }

            strang->setPos(xpos + 50 + 100, 0);
        }

        pipeList << strang;
        connect(strang, &GPipe::pipeInfo , this, &FSAquaWindow::pipeInfo);
        connect(strang, &GPipe::pipeIsSelected, this, &FSAquaWindow::pipeSelected);
        connect(strang, &GPipe::pipeHasDeleted,  this, &FSAquaWindow::pipeDeleted);
        connect(strang, &GPipe::pipeMove ,  this, &FSAquaWindow::pipeMoving);

        // check is strang marker inside the scene
        if( strang->scenePos().x() >= scene->sceneRect().width()-168 )
        {
            QRectF srect = scene->getSceneRect();
            scene->setSceneRect(0,0,srect.width()+168, srect.height());
            resizeFrame();
            resizeFloors();
            scene->update();
        }

        positionStrangMarker = false;
        setCursor(Qt::ArrowCursor);
    }

    if(strangMarkerMove)
    {
        selectedMarker->setPos(mpos.x(), selectedMarker->scenePos().y());
        strangMarkerMove = false;
    }

    if(dropMainPipe)
    {
        GPipe *mainPipe = new GPipe(GPipe::RGS, true, 150);
        mainPipe->setFlag(QGraphicsItem::ItemIsMovable);
        scene->addItem(mainPipe);
        mainPipe->setPos(mpos);
        mainPipe->setFlowDirection("right");
        mainPipe->setNr( 0 );
        mainPipe->setFloorIndex( getFloorIndex(mpos) );
        mainPipe->setMeter(1.5);

        if(ui->actionShowFlowDirection->isChecked())
            mainPipe->setShowArrow(true);


        pipeList << mainPipe;

        connect(mainPipe, &GPipe::pipeInfo, this, &FSAquaWindow::pipeInfo);
        connect(mainPipe, &GPipe::pipeConnect , this, &FSAquaWindow::pipeConnecting);
        connect(mainPipe, &GPipe::pipeHasDeleted , this, &FSAquaWindow::pipeDeleted);
        connect(mainPipe, &GPipe::pipeIsSelected, this, &FSAquaWindow::pipeSelected);

        dropMainPipe = false;
        setCursor(Qt::ArrowCursor);
        tools->enableMainPipeButton(false);
    }

    if(pipeInstalled){
        foreach(GPipe *p, pipeList){
            if(p->cold())
                p->setColor(Qt::darkGreen);
            else
                p->setColor(Qt::red);

        }
        scene->update();
    }

}

void FSAquaWindow::leftMouseButtonReleased(QPointF /*mpos*/)
{
    if(armaturIsMoving)
        armaturIsMoving = false;
}

void FSAquaWindow::mousePositionChanged(QPointF pos)
{
    if(strangMarkerMove)
    {
        bool collisionDetected = false;
        QPointF oldspos = selectedMarker->scenePos();
        QPointF spos = selectedMarker->scenePos();
        qreal distance = 0;

        // Try to move objects
        foreach(GBadObject *o, selectedMarker->badObjectList())
        {

            QPointF oldPos = o->scenePos();
            QPointF obPos = o->scenePos();
            distance = spos.x() - obPos.x();

            obPos.setX( pos.x() - distance );
            o->setPos( obPos.toPoint() );
            if(collision(o)){
                o->setPos( oldPos );
                collisionDetected = true;
            }

            if(o->scenePos().x()+o->boundingRect().width()+100 > scene->sceneRect().width())
            {
                scene->setSceneRect(0,0,scene->sceneRect().width()+48, scene->sceneRect().height());
                resizeFrame();
                resizeFloors();
                scene->update();
            }

        }

        // check collision with other objects
        if(collisionDetected){
            strangMarkerMove = false;
            selectedMarker->setPos(oldspos);

        }else
            selectedMarker->setPos(pos.x(), selectedMarker->scenePos().y());

        if(pipeInstalled)
        {
            //qreal dist = 0.0;
            foreach(GPipe *p, pipeList)
            {
                if(!p->getMarker() && p->getStrangMarkerNr() == selectedMarker->getNr())
                {
                    QPointF oldPos = p->scenePos();
                    QPointF pPos = p->scenePos();
                    distance = spos.x() - pPos.x();

                    pPos.setX( pos.x() - distance );
                    p->setPos( pPos.toPoint() );

                    if(collisionDetected)
                    {
                        p->setPos(oldPos);
                    }
                }

                // try to resize length
                if(!p->getMarker() && p->getStrangMarkerNr() == -1)
                {
                    p->setReadToConnect(true);
                    if(p->type() == GPipe::STRANG)
                    {
                        QPointF oldPos = p->scenePos();
                        QPointF pPos = p->scenePos();
                        distance = spos.x() - pPos.x();

                        pPos.setX( pos.x() - distance );
                        p->setPos( pPos.toPoint() );

                        if(collisionDetected)
                        {
                            p->setPos(oldPos);
                        }
                    }
                }
            }

        }

    }

    if(ui->actionShowMousePos->isChecked()){
        updateMouseText(pos);
    }

    if(armaturIsMoving && selectedArmatur != nullptr)
    {
        selectedPipe = pipe( selectedArmatur->pipeNr() );

        if(selectedPipe->type() == GPipe::RGS)
        {

            if(selectedPipe->getFlowDirection() == "left")
            {
                if(selectedArmatur->scenePos().x() <= selectedPipe->getPipeEnd().x())
                {
                    selectedArmatur->setPos(pos.x()+selectedArmatur->boundingRect().width()+5, selectedPipe->scenePos().y());
                    armaturIsMoving = false;
                }
                else if( selectedArmatur->scenePos().x() >= selectedPipe->getPipeStart().x())
                {
                    selectedArmatur->setPos(pos.x() - selectedArmatur->boundingRect().width()-5 , selectedPipe->scenePos().y());
                    armaturIsMoving = false;
                }
                else
                    selectedArmatur->setPos(pos.x(), selectedPipe->scenePos().y());
            }

            if(selectedPipe->getFlowDirection() == "right")
            {
                if(selectedArmatur->scenePos().x() >= selectedPipe->getPipeEnd().x())
                {
                    selectedArmatur->setPos(pos.x()-selectedArmatur->boundingRect().width()-5 , selectedPipe->scenePos().y());
                    armaturIsMoving = false;
                }
                else if( selectedArmatur->scenePos().x() <= selectedPipe->getPipeStart().x())
                {
                    selectedArmatur->setPos(pos.x()+selectedArmatur->boundingRect().width()+5 , selectedPipe->scenePos().y());
                    armaturIsMoving = false;
                }
                else
                    selectedArmatur->setPos(pos.x(), selectedPipe->scenePos().y());
            }

            if(selectedArmatur->type() == GArmatur::FSVE)
                selectedArmatur->setPos( pos.x(), selectedPipe->scenePos().y() + selectedArmatur->boundingRect().height()/2 - 3 );

        }

        if(selectedPipe->type() == GPipe::STRANG || selectedPipe->type() == GPipe::RGA)
        {

            if(selectedPipe->getFlowDirection() == "up")
            {
                if(selectedArmatur->scenePos().y() >= selectedPipe->getPipeEnd().y())
                {
                    selectedArmatur->setPos(selectedPipe->scenePos().x(), selectedPipe->getPipeEnd().y()- selectedArmatur->boundingRect().width());
                    armaturIsMoving = false;
                }
                else if( selectedArmatur->scenePos().y() <= selectedPipe->getPipeStart().y())
                {
                    selectedArmatur->setPos(selectedPipe->scenePos().x(), selectedPipe->getPipeStart().y()+selectedArmatur->boundingRect().width()+5);
                    armaturIsMoving = false;
                }
                else
                    selectedArmatur->setPos(selectedPipe->scenePos().x(), pos.y());
            }

            if(selectedPipe->getFlowDirection() == "down")
            {
                if(selectedArmatur->scenePos().y() >= selectedPipe->getPipeStart().y())
                {                  
                   selectedArmatur->setPos(selectedPipe->scenePos().x(), selectedPipe->getPipeStart().y()-selectedArmatur->boundingRect().width()-5);
                   armaturIsMoving = false;
                }
                else if( selectedArmatur->scenePos().y() <= selectedPipe->getPipeEnd().y())
                {
                    selectedArmatur->setPos(selectedPipe->scenePos().x(), selectedPipe->scenePos().y() + selectedArmatur->boundingRect().width()+5);
                    armaturIsMoving = false;
                }
                else
                   selectedArmatur->setPos(selectedPipe->scenePos().x(), pos.y());
           }

           if(selectedArmatur->type() == GArmatur::FSVE)
                   selectedArmatur->setPos( selectedPipe->scenePos().x()-selectedArmatur->boundingRect().width()/2+4 , pos.y());
        }
    }
}

QPointF FSAquaWindow::getNextScenePos(QPointF mousePos)
{
    QPointF pos = QPointF(0.0,0.0);

    // Check if left side of rising pipe
    bool left = false;
    if(mousePos.x() < selectedMarker->scenePos().x())
        left = true;

    // check the floorIndex
    int floorIndex = getFloorIndex(mousePos);
    if(floorIndex == -1)
         return pos;

    // Get Y-Pos
    QGraphicsRectItem *floor = floorList.at(floorIndex);
    qreal ypos = floor->scenePos().y() + 10 + selectedObject->boundingRect().height() + floorThickness;
    pos.setY(ypos);

    // Get X-Pos
    qreal xpos = 0;
    QMap<qreal , GBadObject *> oMap;
    foreach(GBadObject *o, selectedMarker->badObjectList())
    {
        if(o->getFloorIndex() == floorIndex)
        {
            if(left)
            {
                if(o->scenePos().x() < selectedMarker->scenePos().x())
                {
                        oMap.insert(o->leftPos().x(), o);
                }
            }
            else
            {
                if(o->scenePos().x() > selectedMarker->scenePos().x())
                {
                    oMap.insert(o->rightPos().x(), o);
                }
            }
        }
    }

    if(left)
    {
        if(!oMap.isEmpty())
        {
            xpos = oMap.firstKey() - selectedObject->boundingRect().width() - 50;
        }
        else
        {
            xpos = selectedMarker->scenePos().x() - selectedObject->boundingRect().width() - 50;
        }
    }

    if(!left)
    {
        if(!oMap.isEmpty())
        {
            xpos = oMap.lastKey()  + 50;
        }
        else
        {
            xpos = selectedMarker->scenePos().x() + selectedObject->boundingRect().width() + 50;
        }
    }

    pos.setX(xpos);


    return pos;
}

void FSAquaWindow::objectHasDeleted(GBadObject *obj)
{
    if(objectList.removeOne(obj))
    {
        foreach(GPipe *str, getStrangMarkerList())
        {
            if(str->badObjectList().contains(obj))
                str->deleteGBadObject(obj);
        }

        scene->removeItem(obj);
        updateFlow();

        renumberObjects();
        scene->update();

        if(pipeInstalled)
            installButtonClicked();

    }else
    {
        QMessageBox::information(this, tr("Remove object"), tr("Removing object failed!"));
    }
}

void FSAquaWindow::objectInfo(GBadObject *obj)
{
    selectedObject = obj;
    GBadObjectInfoDialog *dlg = new GBadObjectInfoDialog(this);
    dlg->setPwc( obj->bdKW());
    dlg->setPww( obj->bdWW());
    dlg->setName( obj->bezeichnung());
    dlg->setFabrikat( obj->fabrikat());
    dlg->setMd( obj->md() );
    dlg->setLu( obj->lu() );
    dlg->setDauer( obj->getDauerlaeufer() );
    dlg->setKwh( obj->kwh());
    dlg->setPipeNr( obj->getPipeNr());
    dlg->setNr(obj->getNr());
    connect(dlg, &GBadObjectInfoDialog::objectValueChanged, this, &FSAquaWindow::objectValueChanged);
    dlg->exec();
}

void FSAquaWindow::objectValueChanged(const QMap<QString, QVariant> &map)
{
    if(selectedObject != nullptr)
    {
        selectedObject->setFabrikat( map.value("Fabrikat").toString());
        selectedObject->setBdKW( map.value("PWC").toDouble());
        selectedObject->setBdWW( map.value("PWW").toDouble());
        selectedObject->setMd( map.value("MD").toDouble());
        selectedObject->setLu( map.value("LU").toInt());
        selectedObject->setKwh( map.value("KWH").toDouble());
        selectedObject->setDauerlaeufer( map.value("DAUER").toBool());

        if(map.value("PWC").toDouble() == 0.0)
            selectedObject->setCold(false);
        else
            selectedObject->setCold(true);

        if(map.value("PWW").toDouble() == 0.0)
            selectedObject->setHot(false);
        else
            selectedObject->setHot(true);


        updateFlow();
    }
}

QList<GBadObject *> FSAquaWindow::getObjectList(GBadObject::GBadObjectType type, GPipe *strang)
{
    QList<GBadObject *>list;
    foreach(GBadObject *o, strang->badObjectList())
    {
        if(o->type() == type)
            list << o;
    }
    return list;
}

void FSAquaWindow::deleteObjects()
{
    foreach(GBadObject *o, objectList)
        objectHasDeleted(o);
}

// Signals from pipes
void FSAquaWindow::pipeSelected(GPipe *pipe)
{
    selectedPipe = pipe;
    if(pipe->getMarker())
    {
        selectedMarker = pipe;
        selectedMarker->setActiv(true);
        tools->enableObjectButtons(true);
        foreach(GPipe *p, pipeList){
            if(p != pipe && p->getMarker()){
                p->setSelected(false);
                p->setActiv(false);
            }
        }
    }
    scene->update();
}

void FSAquaWindow::pipeDeleted(GPipe *pipe)
{
    if(pipe->getMarker() && pipe->badObjectList().isEmpty()){
        if(pipeList.contains(pipe))
            pipeList.removeOne(pipe);
        tools->enableObjectButtons(false);
        scene->removeItem(pipe);
    }

    if(pipe->getMarker() && !pipe->badObjectList().isEmpty()){

        int ret = QMessageBox::information(this, tr("Delete Strang"), tr("All objects in the rising pipe will be delete!"),
                   QMessageBox::Abort | QMessageBox::Ok);
        if(ret == QMessageBox::Abort)
            return;

        foreach(GBadObject *o, pipe->badObjectList()){
            scene->removeItem(o);
            objectList.removeOne(o);
        }


        if(pipeList.contains(pipe))
        {

            pipeList.removeOne(pipe);

        }
        tools->enableObjectButtons(false);
        scene->removeItem(pipe);

        renumberObjects();
        updateFlow();
    }

    if(!pipe->getMarker())
    {
        if(pipeList.removeOne(pipe))
        {
            scene->removeItem(pipe);
            if(pipe->getNr() == 0){
                tools->enableMainPipeButton(true);
                tools->enableTestInstallButton(false);
            }

        }else
        {
            QMessageBox::information(this, tr("Delete pipe"), tr("Sorry deleting pipe failed!"));
        }
    }

    pipeTested = false;
}

void FSAquaWindow::pipeMoving(GPipe *pipe)
{
    selectedMarker = pipe;
    strangMarkerMove = true;
}

void FSAquaWindow::pipeInfo(GPipe *pipe)
{
    GPipeInfoDialog *pipeDlg = new GPipeInfoDialog(pipe, this);
    pipeDlg->setFloorText( tools->getFloorText( pipe->getFloorIndex() ));
    pipeDlg->setFlow( getFlow(pipe) );
    pipe->setReadToConnect(true);
    connect(pipeDlg, &GPipeInfoDialog::deletePipe, this, &FSAquaWindow::pipeDeleted);
    connect(pipeDlg, &GPipeInfoDialog::switchFlowDirection, this, &FSAquaWindow::switchFlowDirectionPipe);
    connect(pipeDlg, &GPipeInfoDialog::selectObjectNr, this, &FSAquaWindow::pipeInfoObjectTableClicked);
    pipeDlg->exec();
}

void FSAquaWindow::pipeConnecting(GPipe *pipe)
{
    pipeStartConnecting = true;
    connectingPipeMap.insert(connectingPipeMap.size(), pipe);
    pipe->setReadToConnect(true);

    if(connectingPipeMap.size() == 1)
    {
        if(countConnectedPipe(pipe->getPipeEnd()) == 3)
        {
            connectingPipeFailed("Connecting failed, because there is already two pipes connected");
            return;
        }

        if(pipe->type() == GPipe::RGA && pipe->getFlowDirection() == "down")
        {
            connectingPipeFailed("Connecting failed, because there is an object connected");
            return;
        }

        acceptRect->show();
        updateProxiWidgets();

    }

    if(connectingPipeMap.size() == 2)
    {
        if(connectingPipeMap.first() == connectingPipeMap.last())
        {
            connectingPipeFailed("It's not possible to connect the same pipe together!");
            return;
        }

        if((connectingPipeMap.first()->cold() && !connectingPipeMap.last()->cold()) ||
               (!connectingPipeMap.first()->cold() && connectingPipeMap.last()->cold()) )
        {

            connectingPipeFailed("Connecting cold with hot pipe does make not scence!");
            return;
        }

        // Check the connecting count of each pipe
        if(countConnectedPipe(connectingPipeMap.last()->getPipeStart()) == 3 )
        {
            connectingPipeFailed("Connecting failed, because there ist already two pipe connected!");
            return;
        }

        // Check flow direction of each pipe of each pipe
        if( connectingPipeMap.first()->type() == GPipe::RGS && connectingPipeMap.last()->type() == GPipe::RGS )
        {
            if(connectingPipeMap.first()->getFlowDirection() != connectingPipeMap.last()->getFlowDirection()){
                connectingPipeFailed("Can not connect pipe, make sure the flowdirection is correct!");
                return;
            }
        }

        connectingPipeTogether();
    }
}

void FSAquaWindow::switchFlowDirectionPipe(GPipe *pipe)
{
    if(pipe->type() == GPipe::STRANG || pipe->type() == GPipe::RGA)
    {
        if(pipe->getFlowDirection() == "up")
            pipe->setFlowDirection("down");
        else
            pipe->setFlowDirection("up");
    }

    if(pipe->type() == GPipe::RGS)
    {
        if(pipe->getFlowDirection() == "right")
            pipe->setFlowDirection("left");
        else
            pipe->setFlowDirection("right");
    }

    pipe->update();
    scene->update();
}

GPipe *FSAquaWindow::pipe(int nr)
{
    GPipe *sp = nullptr;
    foreach(GPipe *p, pipeList){
        if(p->getNr() == nr){
            sp = p;
            break;
        }
    }
    return sp;
}


void FSAquaWindow::cancelConnecting()
{

    foreach(GPipe *p, connectingPipeMap.values())
        p->setReadToConnect(false);

    foreach(GPipe *p, insertedPipeMap.values()){
        pipeList.removeOne(p);
        scene->removeItem(p);

    }

    insertedPipeMap.clear();
    connectingPipeMap.clear();
    acceptRect->hide();
    scene->update();
}

void FSAquaWindow::acceptConnecting()
{

    foreach(GPipe *p, connectingPipeMap.values()){
        p->setReadToConnect(false);
    }

    insertedPipeMap.clear();
    connectingPipeMap.clear();
    pipeStartConnecting = false;
    acceptRect->hide();
    scene->update();

}

void FSAquaWindow::forwardConnectingChanged(bool status)
{
    forwardConnecting = status;
}

void FSAquaWindow::durationChanged(int value)
{
    duration  = value * 150;
}

void FSAquaWindow::leftMouseButtonPressed(QPointF /*scenePos*/, GArmatur *armatur)
{
    selectedArmatur = armatur;
    armaturIsMoving = true;
}

void FSAquaWindow::armaturInfo(GArmatur *arm)
{
    GArmaturInfoDialog *dlg = new GArmaturInfoDialog(arm, this);
    dlg->exec();
}

void FSAquaWindow::armaturDeleted(GArmatur *arm)
{
    if(armaturList.contains(arm))
    {
        if(armaturList.removeOne(arm))
            scene->removeItem(arm);
    }
}

double FSAquaWindow::pressureLossArmatur(GArmatur *arm)
{
    GArmatur::GArmaturType t;
    (quint32&) t = arm->type();
    double z = FSAquaWindow::zeta(t, arm->dn());

    double hPa = 0.0;
    double v = pipe(arm->pipeNr())->getFlowSpeed();
    double d = pipe(arm->pipeNr())->density() ;
    arm->setFlowSpeed(v);
    hPa = z * (d/2) *  qPow(v, 2) / 100.00;
    return hPa;
}


void FSAquaWindow::moveToRight(int strangIndex)
{
    GPipe *strangMarker = getStrangMarkerList().at(strangIndex);

    strangMarker->moveBy(168,0);

    foreach(GBadObject *o, strangMarker->badObjectList())
        o->moveBy(168,0);

    QRectF srect = scene->getSceneRect();
    scene->setSceneRect(0,0,srect.width()+168, srect.height());
    resizeFrame();
    resizeFloors();
    scene->update();

}

void FSAquaWindow::resizeScene()
{

    QRect editorRect = ui->editorView->geometry();

    qreal height = (floorCount * floorHeight) + (floorThickness * floorCount) + 10;
    qreal width = editorRect.width();
    if(width < 620)
        width = 620;

    foreach(GBadObject *o, objectList){
        if(o->scenePos().x() + o->boundingRect().width() + 84 > width)
            width = o->scenePos().x() + o->boundingRect().width() + 84;
    }


    scene->setSceneRect(0,0,width, height);

    resizeFrame();
    resizeFloors();

}

void FSAquaWindow::resizeFrame()
{
    sceneFrame.setRect(0,0,scene->getSceneRect().width()-20,scene->getSceneRect().height()-20);
    sceneFrame.setPos(10,10);
}

GBadObject *FSAquaWindow::getGBadObject(int nr)
{
    GBadObject *obj = nullptr;
    foreach(GBadObject *o, objectList){
        if(o->getNr() == nr){
            obj = o;
            break;
        }
    }
    return obj;
}

bool FSAquaWindow::collision(GBadObject *src)
{
    bool status = false;

    QRegion srcRegion = QRegion( src->region().toRect(), QRegion::Rectangle);

    foreach(GBadObject *o, objectList)
    {

        if(o != src)
        {
            QRegion oRegion = QRegion( o->region().toRect(), QRegion::Rectangle);

            if( srcRegion.intersects(oRegion) )
            {
                status = true;
                break;
            }
        }
    }

    foreach(GPipe *p, getStrangMarkerList())
    {
        QRegion pipeRegion = QRegion( p->region().toRect(), QRegion::Rectangle);

        if( srcRegion.intersects(pipeRegion) )
        {
            status = true;
            break;
        }

    }


    return status;
}

bool FSAquaWindow::removeObjectFromStrang(GBadObject *obj)
{
    bool status = false;
    foreach(GPipe *p, getStrangMarkerList())
    {
        if(p->badObjectList().contains(obj)){
           if(p->deleteGBadObject(obj))
            status = true;
        }
    }
    return status;
}

void FSAquaWindow::renumberObjects()
{
    // Renumber all objects
    int nr = 1;
    foreach(GBadObject *o, objectList)
    {
        o->setNr(nr);
        nr++;
    }
    scene->update();
}

void FSAquaWindow::updateObjects()
{
    foreach(GBadObject *o, objectList)
    {
        QGraphicsRectItem *floor = floorList.at(o->getFloorIndex());
        if(floor != nullptr)
            o->setPos(  o->scenePos().x(), floor->scenePos().y()+10+o->boundingRect().height()+floorThickness);

    }
}

QList<GPipe *> FSAquaWindow::getStrangMarkerList()
{
    QList<GPipe *> list;
    foreach(GPipe *p, pipeList){
        if(p->getMarker())
            list << p;
    }

    return list;
}

void FSAquaWindow::updateStrangMarker()
{
    foreach(GPipe *p, getStrangMarkerList()){
        p->setLength(scene->getSceneRect().height());
    }

}

bool FSAquaWindow::installConectingPipe()
{
    bool status = true;
    foreach(GPipe *strang, getStrangMarkerList())
    {

        foreach(GBadObject *o, strang->badObjectList())
        {

            if(o->getCold() && o->type() != GBadObject::WSIB)
            {
                //qreal len = 75;
                qreal len = floorHeight - o->boundingRect().height() - 80;

                if(o->scenePos().x()  >  strang->scenePos().x())
                     len = floorHeight - o->boundingRect().height() - 50; //len = floorHeight - 130; //len = 95;

                GPipe *p = new GPipe(GPipe::RGA, true, len);
                scene->addItem(p);
                p->setPos( o->connectCold() );
                p->setNr( pipeList.size()+1 );
                p->setFlowDirection("down");
                p->setFloorIndex(o->getFloorIndex());
                p->appendGBadObject(o);
                p->setStrangMarkerNr( strang->getNr() );
                GBadObject::GBadObjectType t;
                (quint32&)t = o->type();
                p->setMeter( FSAquaWindow::getDefaultMeter(t) );

                p->setMaterial(pipeMaterial.value("AnschlussKalt","Kupfer"));

                pipeList << p;
                connect(p, &GPipe::pipeInfo, this, &FSAquaWindow::pipeInfo);
                connect(p, &GPipe::pipeConnect , this, &FSAquaWindow::pipeConnecting);
                connect(p, &GPipe::pipeIsSelected, this, &FSAquaWindow::pipeSelected);

            }

            if(o->getCold() && o->type() == GBadObject::WSIB)
            {

                qreal len = floorHeight - o->boundingRect().height() - 80;

                if(o->scenePos().x()  >  strang->scenePos().x())
                     len = floorHeight - o->boundingRect().height() - 50; //len = floorHeight - 130; //len = 95;

                GPipe *p = new GPipe(GPipe::RGA, true, len + 65);
                scene->addItem(p);
                p->setPos( o->connectCold() );
                p->setNr( pipeList.size()+1 );
                p->setFlowDirection("down");
                p->setStrangMarkerNr( strang->getNr() );
                GBadObject::GBadObjectType t;
                (quint32&)t = o->type();
                p->setMeter( FSAquaWindow::getDefaultMeter(t) );

                p->setMaterial(pipeMaterial.value("AnschlussKalt","Kupfer"));


                p->setFloorIndex(o->getFloorIndex());
                foreach(GBadObject *ob, objectList){
                    if(ob->bdWW() > 0.0)
                        p->appendGBadObject(ob);
                }

                pipeList << p;
                connect(p, &GPipe::pipeInfo, this, &FSAquaWindow::pipeInfo);
                connect(p, &GPipe::pipeConnect , this, &FSAquaWindow::pipeConnecting);
                connect(p, &GPipe::pipeIsSelected, this, &FSAquaWindow::pipeSelected);
            }

            if(o->getHot())
            {

                qreal len = floorHeight - o->boundingRect().height() - 110;

                if(o->scenePos().x()  >  strang->scenePos().x())
                     len = floorHeight - o->boundingRect().height() - 80;

                GPipe *p = new GPipe(GPipe::RGA, false, len);
                scene->addItem(p);
                p->setPos( o->connectHot() );
                p->setNr( pipeList.size()+1 );

                if(o->type() == GBadObject::WSIB || o->type() == GBadObject::DHE )
                    p->setFlowDirection("up");
                else
                    p->setFlowDirection("down");


                p->setFloorIndex(o->getFloorIndex());
                p->appendGBadObject(o);
                p->setStrangMarkerNr( strang->getNr() );
                GBadObject::GBadObjectType t;
                (quint32&)t = o->type();
                p->setMeter( FSAquaWindow::getDefaultMeter(t) );
                p->setMaterial(pipeMaterial.value("AnschlussWarm","Kupfer"));

                pipeList << p;
                connect(p, &GPipe::pipeInfo, this, &FSAquaWindow::pipeInfo);
                connect(p, &GPipe::pipeConnect , this, &FSAquaWindow::pipeConnecting);
                connect(p, &GPipe::pipeIsSelected, this, &FSAquaWindow::pipeSelected);
            }
        }

    }
    return status;
}

bool FSAquaWindow::installCollectingPipe()
{
    bool status = true;
    double dividor = 100.0;

    foreach(GPipe *str, getStrangMarkerList())
    {
        qreal x = str->scenePos().x();


        for(int i = 0; i < floorCount; i++)
        {
            QMap<qreal,GPipe *> mapLeftCold;
            QMap<qreal,GPipe *> mapRightCold;
            QMap<qreal,GPipe *> mapLeftHot;
            QMap<qreal,GPipe *> mapRightHot;
            foreach(GPipe *p, pipeList)
            {
                if(p->getStrangMarkerNr() == str->getStrangMarkerNr() && p->type() == GPipe::RGA && p->cold() && p->getFloorIndex() == i && p->scenePos().x() < x )
                    mapLeftCold.insert(p->scenePos().x(), p);

                if(p->getStrangMarkerNr() == str->getStrangMarkerNr() && p->type() == GPipe::RGA && p->cold() && p->getFloorIndex() == i && p->scenePos().x() > x )
                    mapRightCold.insert(p->scenePos().x(), p);

                if(p->getStrangMarkerNr() == str->getStrangMarkerNr() && p->type() == GPipe::RGA && !p->cold() && p->getFloorIndex() == i && p->scenePos().x() < x)
                    mapLeftHot.insert(p->scenePos().x(), p);

                if(p->getStrangMarkerNr() == str->getStrangMarkerNr() && p->type() == GPipe::RGA && !p->cold() && p->getFloorIndex() == i && p->scenePos().x() > x)
                    mapRightHot.insert(p->scenePos().x(), p);

            }

            QMapIterator<qreal, GPipe *> itcl(mapLeftCold);
            while (itcl.hasNext())
            {
                itcl.next();
                qreal length = 10;
                if(itcl.hasNext())
                    length = itcl.peekNext().value()->scenePos().x() - itcl.value()->x();
                else
                    length = x - itcl.value()->scenePos().x();

                GPipe *p = new GPipe(GPipe::RGS, true, length);
                scene->addItem(p);
                QPointF pos = itcl.value()->getP2();
                pos.setX( pos.x() + length);
                p->setPos(  pos );
                p->setMeter( length / dividor);
                p->setFloorIndex(i);
                p->setStrangMarkerNr(str->getStrangMarkerNr());
                p->setMaterial(pipeMaterial.value("StockwerksKalt","Kupfer"));

                p->setNr( pipeList.size()+1);
                pipeList << p;


                p->setFlowDirection("left");

                if(i == 0)
                    p->setFlowDirection("right");

                connect(p, &GPipe::pipeInfo, this, &FSAquaWindow::pipeInfo);
                connect(p, &GPipe::pipeConnect , this, &FSAquaWindow::pipeConnecting);
                connect(p, &GPipe::pipeHasDeleted , this, &FSAquaWindow::pipeDeleted);
                connect(p, &GPipe::pipeIsSelected, this, &FSAquaWindow::pipeSelected);
            }

            QMapIterator<qreal, GPipe *> itcr(mapRightCold);
            itcr.toBack();
            while (itcr.hasPrevious())
            {
                itcr.previous();
                qreal length = 10;
                if(itcr.hasPrevious())
                    length =  itcr.value()->x() - itcr.peekPrevious().value()->scenePos().x();
                else
                    length = itcr.value()->scenePos().x() - x;

                GPipe *p = new GPipe(GPipe::RGS, true, length);
                scene->addItem(p);
                p->setPos(  itcr.value()->getP2() );
                p->setFloorIndex(i);
                p->setStrangMarkerNr(str->getStrangMarkerNr());
                p->setNr( pipeList.size()+1);
                p->setFlowDirection("right");
                p->setMeter( length / dividor);
                p->setMaterial(pipeMaterial.value("StockwerksKalt","Kupfer"));
                pipeList << p;

                connect(p, &GPipe::pipeInfo, this, &FSAquaWindow::pipeInfo);
                connect(p, &GPipe::pipeConnect , this, &FSAquaWindow::pipeConnecting);
                connect(p, &GPipe::pipeHasDeleted , this, &FSAquaWindow::pipeDeleted);
                connect(p, &GPipe::pipeIsSelected, this, &FSAquaWindow::pipeSelected);
            }


            QMapIterator<qreal, GPipe *> ithl(mapLeftHot);
            while (ithl.hasNext())
            {
                ithl.next();
                qreal length = 10;
                if(ithl.hasNext())
                    length = ithl.peekNext().value()->scenePos().x() - ithl.value()->x();
                else
                    length = x - ithl.value()->scenePos().x() + DISTANCEPIPE;

                GPipe *p = new GPipe(GPipe::RGS, false, length);
                scene->addItem(p);
                QPointF pos = ithl.value()->getP2();
                pos.setX( pos.x() + length);
                p->setPos(  pos );

                p->setFloorIndex(i);
                p->setStrangMarkerNr(str->getStrangMarkerNr());
                p->setNr( pipeList.size()+1 );
                p->setFlowDirection("left");
                p->setMeter( length / dividor);
                p->setMaterial(pipeMaterial.value("StockwerksWarm","Kupfer"));
                pipeList << p;

                QList<GBadObject *>list = getObjectList(GBadObject::WSIB, str);
                if(!list.isEmpty())
                {
                    GBadObject *wsib = list.first();

                    if(wsib->getFloorIndex() == p->getFloorIndex())
                        p->setFlowDirection("right");

                    if(wsib->scenePos().x() > p->scenePos().x()-(length/2))
                         p->setFlowDirection("left");
                }

                if(ithl.value()->getFlowDirection() == "up" && ithl.value()->containsObjectType(GBadObject::DHE))
                    p->setFlowDirection("right");

                connect(p, &GPipe::pipeInfo, this, &FSAquaWindow::pipeInfo);
                connect(p, &GPipe::pipeConnect , this, &FSAquaWindow::pipeConnecting);
                connect(p, &GPipe::pipeHasDeleted , this, &FSAquaWindow::pipeDeleted);
                connect(p, &GPipe::pipeIsSelected, this, &FSAquaWindow::pipeSelected);
            }

            QMapIterator<qreal, GPipe *> ithr(mapRightHot);
            ithr.toBack();
            while (ithr.hasPrevious())
            {
                ithr.previous();
                qreal length = 10;
                if(ithr.hasPrevious())
                    length =  ithr.value()->x() - ithr.peekPrevious().value()->scenePos().x();
                else
                    length = ithr.value()->scenePos().x() - x - DISTANCEPIPE;

                GPipe *p = new GPipe(GPipe::RGS, false, length);
                scene->addItem(p);
                p->setPos(  ithr.value()->getP2() );
                p->setFloorIndex(i);
                p->setStrangMarkerNr(str->getStrangMarkerNr());
                p->setNr( pipeList.size() + 1 );
                p->setFlowDirection("right");
                p->setMeter( length / dividor);
                p->setMaterial(pipeMaterial.value("StockwerksWarm","Kupfer"));
                pipeList << p;

                if(str->containsWSIB())
                {
                    QList<GBadObject *>list;
                    list = getObjectList(GBadObject::WSIB, str);
                    if(!list.isEmpty())
                    {
                       GBadObject *sp = list.first();
                       if(str->scenePos().x() <= p->scenePos().x()-(length/2) &&
                               sp->scenePos().x() >=  p->scenePos().x()-(length/2) && p->getFloorIndex() == sp->getFloorIndex())
                            p->setFlowDirection("left");
                    }
                }

                if(ithr.value()->getFlowDirection() == "up" && ithr.value()->containsObjectType(GBadObject::DHE))
                    p->setFlowDirection("left");

                connect(p, &GPipe::pipeInfo, this, &FSAquaWindow::pipeInfo);
                connect(p, &GPipe::pipeConnect , this, &FSAquaWindow::pipeConnecting);
                connect(p, &GPipe::pipeHasDeleted , this, &FSAquaWindow::pipeDeleted);
                connect(p, &GPipe::pipeIsSelected, this, &FSAquaWindow::pipeSelected);
            }

        }
    }

    return status;
}

void FSAquaWindow::installRisingPipe()
{
    double dividor = 100.0;


    // Marker of rising pipe
    foreach(GPipe *str, getStrangMarkerList())
    {

        qreal x = str->scenePos().x() + DISTANCEPIPE; // Hot rising pipe
        qreal xc = str->scenePos().x();

        int floorIndexOfWSIB = -1;
        GBadObject *wsib = nullptr;


        if(str->containsWSIB())
        {
            foreach(GBadObject *obj, getObjectList(GBadObject::WSIB, str) )
            {
                floorIndexOfWSIB = obj->getFloorIndex();
                wsib = obj;
                break;
             }
        }

        // sort collecting pipe
        QMap<qreal, GPipe *> phMap;
        QMap<qreal, GPipe *> pcMap;
        foreach(GPipe *p, pipeList)
        {
            if(!p->cold() && p->type() == GPipe::RGS )
            {
                if(x == p->getPipeStart().x() || x == p->getPipeEnd().x() )
                {
                   phMap.insert(p->getPipeStart().y(), p);
                }
            }

            if(p->cold() && p->type() == GPipe::RGS )
            {
                if(xc == p->getPipeStart().x() || xc == p->getPipeEnd().x() )
                {
                    pcMap.insert(p->getPipeStart().y(), p);
                }
            }
        }


        QMapIterator<qreal, GPipe *> ith(phMap);
        ith.toBack();
        while (ith.hasPrevious())
        {
            ith.previous();
            qreal y1 = ith.key();
            qreal y2 = 0;
            if(ith.hasPrevious())
            {
                y2 = ith.peekPrevious().key();
            }
            qreal length = 20;
            if(y2 != 0)
            {
                length = y1 - y2;
                GPipe *s = new GPipe(GPipe::STRANG, false, length);
                scene->addItem(s);
                s->setPos(x, y2);
                s->setFlowDirection("up");
                s->setNr( pipeList.size()+1 );
                s->setStrangMarkerNr( str->getStrangMarkerNr());
                s->setFloorIndex( getFloorIndex( s->getPipeStart()));
                s->setMeter( length / dividor);
                s->setMaterial(pipeMaterial.value("StrangsWarm","Kupfer"));
                pipeList << s;

                // Check flow direction in case the WSIB is on right side
                // and objects with warm water are on left side
                if(ith.value()->getFloorIndex() == floorIndexOfWSIB)
                {
                    if(wsib != nullptr)
                    {
                        if(wsib->scenePos().x() > str->scenePos().x())
                        {
                            if(s->getPipeStart().y() >= y2)
                                s->setFlowDirection("down");
                        }
                    }
                }

                connect(s, &GPipe::pipeInfo, this, &FSAquaWindow::pipeInfo);
                connect(s, &GPipe::pipeConnect , this, &FSAquaWindow::pipeConnecting);
                connect(s, &GPipe::pipeHasDeleted , this, &FSAquaWindow::pipeDeleted);
                connect(s, &GPipe::pipeIsSelected, this, &FSAquaWindow::pipeSelected);
            }
        }

        QMapIterator<qreal, GPipe *> itc(pcMap);
        itc.toBack();
        while (itc.hasPrevious())
        {
            itc.previous();
            qreal y1 = itc.key();
            qreal y2 = 0;
            if(itc.hasPrevious())
            {
                y2 = itc.peekPrevious().key();
            }
            qreal length = 20;
            if(y2 != 0)
            {
                length = y1 - y2;
                GPipe *s = new GPipe(GPipe::STRANG, true, length);
                scene->addItem(s);
                s->setPos(xc, y2);
                s->setFlowDirection("up");
                s->setStrangMarkerNr( str->getStrangMarkerNr());
                s->setNr( pipeList.size()+1 );
                s->setFloorIndex( getFloorIndex( s->getPipeStart()));
                s->setMeter( length / dividor);
                s->setMaterial(pipeMaterial.value("StrangsKalt","Kupfer"));
                pipeList << s;

                connect(s, &GPipe::pipeInfo, this, &FSAquaWindow::pipeInfo);
                connect(s, &GPipe::pipeConnect , this, &FSAquaWindow::pipeConnecting);
                connect(s, &GPipe::pipeHasDeleted , this, &FSAquaWindow::pipeDeleted);
                connect(s, &GPipe::pipeIsSelected, this, &FSAquaWindow::pipeSelected);
            }
        }

    }

}

void FSAquaWindow::connectingPipeTogether()
{
    GPipe *src = connectingPipeMap.first();
    GPipe *dest = connectingPipeMap.last();

    if(src->getNr() == 0) // Main pipe
        src->setFlags(QGraphicsItem::ItemIsSelectable);


    if(src->getNr() == 0) // when main pipe
    {
        if(getHeightDiff(src->scenePos(), dest->scenePos()) <= 15 )
            src->setPos(src->scenePos().x(), dest->scenePos().y());

        tools->enableTestInstallButton(true);
    }

    qreal length = 0.0;
    qreal heigth = 0.0;
    qreal xSrc = src->getPipeEnd().x();
    qreal ySrc = src->getPipeEnd().y();
    qreal xDest = dest->getPipeStart().x();
    qreal yDest = dest->getPipeStart().y();

    // Calculate the length of connection with RGS pipe typ
    if(xSrc != xDest)
    {
        length =  xSrc > xDest ? xSrc - xDest : xDest - xSrc;
    }

    if(ySrc != yDest)
    {
        heigth =  ySrc > yDest ? ySrc - yDest : yDest - ySrc;
    }

    if(length > 0)
    {
        GPipe *rgs = new GPipe(GPipe::RGS, src->cold(), length);
        scene->addItem(rgs);
        insertedPipeMap.insert(insertedPipeMap.size()+1, rgs);

        if(src->type() == GPipe::RGS)
            rgs->setFlowDirection( src->getFlowDirection() );

        if(src->type() == GPipe::STRANG || src->type() == GPipe::RGA )
        {
            if(xSrc < xDest)
                rgs->setFlowDirection( "right" );

            if(xSrc > xDest)
                rgs->setFlowDirection( "left" );

        }


        rgs->setShowArrow(ui->actionShowFlowDirection->isChecked());
        rgs->setShowNr( ui->actionShowPipeNr->isChecked());
        rgs->setNr(pipeList.size()+1);
        rgs->setMeter(length / 100.0);
        if(src->cold())
            rgs->setMaterial(pipeMaterial.value("StockwerksKalt","Kupfer"));
        else
            rgs->setMaterial(pipeMaterial.value("StockwerksWarm","Kupfer"));
        pipeList << rgs;



        connect(rgs, &GPipe::pipeInfo, this, &FSAquaWindow::pipeInfo);
        connect(rgs, &GPipe::pipeConnect , this, &FSAquaWindow::pipeConnecting);
        connect(rgs, &GPipe::pipeHasDeleted , this, &FSAquaWindow::pipeDeleted);
        connect(rgs, &GPipe::pipeIsSelected, this, &FSAquaWindow::pipeSelected);

        if(forwardConnecting) // First connect to src pipe
        {
            if(src->getFlowDirection() == "left")
                rgs->setPos( src->getPipeEnd() );

            if(src->getFlowDirection() == "right")
                rgs->setPos( src->getPipeEnd().x() + length, src->getPipeEnd().y() );

            if(src->getFlowDirection() == "up"  && xSrc < xDest )
                rgs->setPos( dest->getPipeStart() );

            if(src->getFlowDirection() == "up"  && xSrc > xDest )
                rgs->setPos( dest->getPipeStart().x() + length, dest->getPipeStart().y() );

            if(src->getFlowDirection() == "down"  && xSrc < xDest )
                rgs->setPos( dest->getPipeStart() );

            if(src->getFlowDirection() == "down"  && xSrc > xDest )
                rgs->setPos( dest->getPipeStart().x() + length, dest->getPipeStart().y());


        }
        if(!forwardConnecting) // First connect to src pipe
        {
            rgs->setPos( dest->getPipeStart());

            if(src->getFlowDirection() == "left")
                rgs->setPos( dest->getPipeStart().x() + length , dest->getPipeStart().y() );

            if(src->getFlowDirection() == "up")
                rgs->setPos( src->getPipeEnd().x() + length , src->getPipeEnd().y() );

            if(src->getFlowDirection() == "up" && xSrc > xDest)
                rgs->setPos( src->getPipeEnd() );

        }

        //qDebug() << collisionPipe(rgs);
    }

    if(heigth > 0)
    {

        GPipe *strang = new GPipe(GPipe::STRANG, src->cold(), heigth);
        scene->addItem(strang);
        insertedPipeMap.insert(insertedPipeMap.size()+1, strang);

        if(ySrc < yDest)
            strang->setFlowDirection( "up" );
        else
            strang->setFlowDirection( "down" );

        strang->setShowArrow(ui->actionShowFlowDirection->isChecked());
        strang->setShowNr( ui->actionShowPipeNr->isChecked());
        strang->setNr(pipeList.size()+1);
        strang->setMeter(heigth / 100.0);

        if(src->cold())
            strang->setMaterial(pipeMaterial.value("StrangsKalt","Kupfer"));
        else
            strang->setMaterial(pipeMaterial.value("StrangsWarm","Kupfer"));

        pipeList << strang;

        connect(strang, &GPipe::pipeInfo, this, &FSAquaWindow::pipeInfo);
        connect(strang, &GPipe::pipeConnect , this, &FSAquaWindow::pipeConnecting);
        connect(strang, &GPipe::pipeHasDeleted , this, &FSAquaWindow::pipeDeleted);
        connect(strang, &GPipe::pipeIsSelected, this, &FSAquaWindow::pipeSelected);


        if(forwardConnecting) // First connect to src pipe
        {

            strang->setPos( src->getPipeStart());


            if(ySrc < yDest )
                strang->setPos( dest->getPipeStart().x(), dest->getPipeStart().y() - heigth );

            if(ySrc > yDest )
                strang->setPos( dest->getPipeStart());

            if(src->getFlowDirection() == "up" && ySrc < yDest)
                strang->setPos( src->getPipeEnd() );

            if(src->getFlowDirection() == "down" && ySrc > yDest)
                strang->setPos( src->getPipeEnd().x(), src->getPipeEnd().y() - heigth );
        }

        if(!forwardConnecting) // side connection
        {

            strang->setPos( src->getPipeEnd());

            if(ySrc > yDest)
                strang->setPos( src->getPipeEnd().x(), src->getPipeEnd().y() - heigth);

            if(src->getFlowDirection() == "up")
                strang->setPos( dest->getPipeStart().x(), dest->getPipeStart().y() - heigth);

        }

        //qDebug() << collisionPipe(strang);
    }

    ui->statusBar->showMessage("Connecting pipe sucessfully done!", 5000);
}

bool FSAquaWindow::collisionPipe(GPipe *pipe)
{
//    int count = 1;
    bool status = false;
    foreach(GPipe *p, pipeList)
    {
        if(pipe != p)
        {
            // Check if any contact with other pipe's
            if(p->sceneRect().intersects(pipe->sceneRect())) // This is working
            {

                if(!p->getMarker())
                {
//                    qDebug() << "------------------------";
//                    qDebug() << count;
//                    qDebug() << "Pipe : " << pipe->getNr();
//                    qDebug() << "Intersects with p:" << p->getNr();
//                    qDebug() << "------------------------";
//                    count++;

                    // Check if full intersected
                    QRectF ir = p->sceneRect().intersected(pipe->sceneRect());
                    if(ir.width() == p->sceneRect().width() && ir.height() == p->sceneRect().height())
                    {

//                        qDebug() << "------------------------";
//                        qDebug() << count;
//                        qDebug() << "Pipe : " << pipe->getNr();
//                        qDebug() << "Full covered with p:" << p->getNr();
//                        qDebug() << "------------------------";
                        status = true;
                        break;
                    }
                }

            }
//            if(pipe->boundingRect().intersects(p->boundingRect()))
//            {

//                QRectF ir = p->boundingRect().intersected(pipe->boundingRect());
//                if(!ir.isEmpty() )
//                {
//                    qDebug() << "pipe" << pipe->getNr();
//                    qDebug() << "p" << p->getNr();
//                    p->setReadToConnect(true);
//                    status = true;
//                    break;
//                }
//            }
        }
    }

    return status;
}

void FSAquaWindow::connectingPipeFailed(const QString &message)
{
    QMessageBox::information(this, tr("Connecting pipe"), message);

    foreach(GPipe *p, connectingPipeMap.values())
        p->setReadToConnect(false);

    connectingPipeMap.clear();
    acceptRect->hide();
    scene->update();
    pipeTested = false;
}

int FSAquaWindow::countConnectedPipe(QPointF pos)
{
    int count = 0;
    foreach(GPipe *p, pipeList){
        if(p->getPipeEnd() == pos || p->getPipeStart() == pos)
            count++;
    }

    return count;
}

double FSAquaWindow::getHeightDiff(QPointF pos1, QPointF pos2)
{
    double diff = 0.0;
    diff = pos1.y() > pos2.y() ? pos1.y() - pos2.y() : pos2.y() - pos1.y();
    return diff;
}

double FSAquaWindow::getFlow(GPipe *pipe)
{
    GBadObject *wsib = nullptr;

    // exist a wsib?
    if(pipe->containsWSIB()){
        foreach(GBadObject *o, pipe->badObjectList()){
            if(o->type() == GBadObject::WSIB){
                wsib = o;
            }
        }
    }

    double flow = 0.0;
    foreach(GBadObject *o, pipe->badObjectList()){

        if(pipe->cold() && pipe->containsWSIB())
        {
            flow += o->bdWW();

            if(pipe->badObjectList().last() != wsib)
                flow += o->bdKW();
        }

        if(pipe->cold() && !pipe->containsWSIB())
            flow += o->bdKW();

        if(!pipe->cold())
            flow += o->bdKW();

    }

    return flow;
}

double FSAquaWindow::getPeakFlow(GPipe *pipe)
{
    double peakflow = 0.00;
    double flow = getFlow(pipe);

    if(ui->buildingBox->currentText() == "Wohngebäude")
        peakflow = 1.48 * (qPow(flow,0.19)) - 0.94;


    if(ui->buildingBox->currentText() == "Schule")
        peakflow = 0.91 * (qPow(flow,0.31)) - 0.38;


    if(ui->buildingBox->currentText() == "Verwaltungsgebäude")
        peakflow = 0.91 * (qPow(flow,0.31)) - 0.38;

    if(ui->buildingBox->currentText() == "Krankenhaus")
        peakflow = 0.75 * (qPow(flow,0.44)) - 0.18;

    if(ui->buildingBox->currentText() == "Hotel")
        peakflow = 0.70 * (qPow(flow,0.48)) - 0.13;

    if(ui->buildingBox->currentText() == "Pflegeheim")
        peakflow = 1.40 * (qPow(flow,0.14)) - 0.92;


    if(ui->buildingBox->currentText() == "Altersheim und betreutes Wohnen")
        peakflow = 1.48 * (qPow(flow,0.19)) - 0.94;

    return peakflow;
}

QList<GPipe *> FSAquaWindow::previousPipe(GPipe *pipe)
{
    QList<GPipe *> list;
    foreach(GPipe *p, pipeList)
    {
        if(pipe->getPipeStart() == p->getPipeEnd())
            list << p;

        if(!pipe->cold() && p->cold() && pipe->type() == GPipe::RGA && pipe->getFlowDirection() == "up" &&
                p->getFloorIndex() == pipe->getFloorIndex() && p->getStrangMarkerNr() == pipe->getStrangMarkerNr())
        {
                if(pipe->containsObjectType(GBadObject::WSIB))
                {
                    if(pipe->scenePos().x() - 39.5 == p->scenePos().x())
                        list << p;
                }
                if(pipe->containsObjectType(GBadObject::DHE))
                {
                    if(pipe->scenePos().x() + 36 == p->scenePos().x())
                        list << p;
                }
        }

    }

    return list;
}

bool FSAquaWindow::hasPrevious(GPipe *pipe)
{
    bool status = false;
    foreach(GPipe *p, pipeList)
    {
        if(pipe->getPipeStart() == p->getPipeEnd())
            status = true;

        if(!pipe->cold() && p->cold() && pipe->type() == GPipe::RGA && pipe->getFlowDirection() == "up" &&
                p->getFloorIndex() == pipe->getFloorIndex() && p->getStrangMarkerNr() == pipe->getStrangMarkerNr())
        {
                if(pipe->containsObjectType(GBadObject::WSIB))
                {
                    if(pipe->scenePos().x() - 39.5 == p->scenePos().x())
                        status = true;
                }
                if(pipe->containsObjectType(GBadObject::DHE))
                {
                    if(pipe->scenePos().x() + 36 == p->scenePos().x())
                       status = true;
                }
        }
    }

    return status;
}

QList<GPipe *> FSAquaWindow::testConnection(GPipe *pipe)
{
    QList<GPipe *> list;
    int pnr = pipe->getNr();
    ui->statusBar->showMessage(QString("Überprüfe Rohrverbindungen Rohr:%1").arg(pnr));
    GPipe *nextPipe = pipe;
    list << nextPipe;
    nextPipe->setColor(Qt::cyan);

    scene->update();
    while (hasPrevious(nextPipe) )
    {
        QList<GPipe *> pl;
        pl = previousPipe(nextPipe);

        if(pl.isEmpty()){
            nextPipe->setColor(Qt::magenta);
            scene->update();
            Sleep(duration);
            return list;
        }

        nextPipe = pl.first();
        pnr = nextPipe->getNr();
        nextPipe->setColor(Qt::cyan);

        ui->statusBar->showMessage(QString("Überprüfe Rohrverbindungen Rohr:%1").arg(pnr));
        scene->update();
        Sleep(duration);
        list << nextPipe;
    }

    return list;

}

bool FSAquaWindow::testLength(QList<GPipe *> pList)
{

    bool status = true;
    foreach(GPipe *p, pList){
        if(p->getMeter() <= 0.0){
            status = false;
            p->setColor(Qt::magenta);
            scene->update();
        }
    }

    return status;
}

bool FSAquaWindow::containsMainPipe(QList<GPipe *> pipeList)
{
    bool status = false;
    foreach(GPipe *p, pipeList){
        if(p->getNr() == 0){ // Main pipe
            status = true;
            break;
        }
    }

    return status;
}

int FSAquaWindow::getFloorIndex(QPointF pos)
{
    int floorIndex = -1;
    QRegion reg = QRegion(pos.x(), pos.y(), 5,5);

    for(int i = 0;  i < floorList.size(); i++)
    {
        QGraphicsRectItem *f = floorList.at(i);
        QRegion region = QRegion(f->scenePos().x(), f->scenePos().y(), scene->sceneRect().width(), floorHeight);
        if(region.intersects(reg))
        {
            floorIndex = i;
            break;
        }
    }

    return floorIndex;
}

void FSAquaWindow::resizeFloors()
{
    foreach(QGraphicsRectItem *f, floorList)
    {
        f->setRect(0,0, sceneFrame.rect().width(), floorThickness);
    }
}

void FSAquaWindow::updateFloors()
{
    // Floor-Rectangle
    foreach(QGraphicsRectItem *r, floorList )
       scene->removeItem(r);

    floorList.clear();
    for(int i = 0; i < floorCount; i++)
    {
        // Floor-Rectangle
        QGraphicsRectItem *fitem = new QGraphicsRectItem(0);
        fitem->setRect(0,0, sceneFrame.boundingRect().width(), floorThickness);

        QBrush br = fitem->brush();
        br.setStyle(Qt::BDiagPattern);
        fitem->setBrush(br);

        scene->addItem(fitem);

        if(i == 0)
            fitem->setPos(10,10);
        else
            fitem->setPos(10, (i*floorHeight)+(i * floorThickness)+10);

        floorList << fitem;
    }
}

void FSAquaWindow::updateLabels()
{
    foreach(QGraphicsTextItem *text, labelList)
        scene->removeItem(text);

    labelList.clear();

    for(int i = 0; i < floorCount; i++)
    {

        QGraphicsTextItem *text = new QGraphicsTextItem(tools->getFloorText(i));
        text->setTransform( ui->editorView->transform() );
        scene->addItem(text);

        text->setPos(15, floorList.at(i)->scenePos().y()+floorThickness+text->boundingRect().height());
        text->setDefaultTextColor(QColor(0,87,127));

        QFont ft = text->font();
        ft.setBold(true);
        text->setFont(ft);

        labelList << text;
    }
    scene->update();
}

void FSAquaWindow::updateMouseText(QPointF pos)
{
    if(mouseText != nullptr)
        scene->removeItem(mouseText);
    if(ui->actionShowMousePos->isChecked()){
        mouseText = new QGraphicsTextItem(QString("Höhe: %1 m").arg(pos.y()/100 + floorThickness/100 - 0.3));
        scene->addItem(mouseText);
        mouseText->setTransform( ui->editorView->transform());
        //mouseText->setPos(scene->sceneRect().width()/2, scene->sceneRect().height()/2);
        mouseText->setPos(pos.x(), pos.y()+mouseText->boundingRect().height());
    }
}

void FSAquaWindow::updateProxiWidgets()
{
    QRect viewportRect(0, 0, ui->editorView->viewport()->width(), ui->editorView->viewport()->height());
    QRectF visibleSceneRect = ui->editorView->mapToScene(viewportRect).boundingRect();

    if(acceptRect->isVisible())
        acceptRect->setPos( visibleSceneRect.bottomLeft().x(), visibleSceneRect.bottomLeft().y() - 40 );

    if(simulationRect->isVisible())
        simulationRect->setPos( visibleSceneRect.width() / 2 - (simulationRect->boundingRect().width()/2) , visibleSceneRect.bottom() - 40 );
}

void FSAquaWindow::updateFlow()
{
    qreal pwc = 0.0;
    qreal pwh = 0.0;
    qreal dd = 0.0;
    qreal lu = 0.0;

    foreach(GBadObject *o, objectList)
    {
        if(!o->getDauerlaeufer())
        {
            pwc += o->bdKW();
            pwh += o->bdWW();
        }
        else
        {
            dd += o->bdKW();
            dd += o->bdWW();
        }

        lu += o->lu();
    }

    ui->summePWCBox->setValue(pwc);
    ui->summePWHBox->setValue(pwh);
    ui->summendurchflussBox->setValue(pwc+pwh+dd);
    ui->luBox->setValue(lu);
    updatePeakFlow(dd);

}

void FSAquaWindow::updatePeakFlow(double pf)
{
    qreal sd = ui->summendurchflussBox->value()-pf;
    qreal sp = 0.0;

    if(sd <= 0.2 || sd >= 500){
        ui->spitzendurchflussBox->setValue(sd);
        return;
    }

    if(ui->buildingBox->currentText() == "Wohngebäude"){
        sp = 1.48 * (qPow(sd,0.19)) - 0.94;
        formvs->setA(1.48);  formvs->setB(0.19);
        formvs->setC(0.94); formvs->setArtLabel("Wohngebäude");
    }

    if(ui->buildingBox->currentText() == "Schule"){
        sp = 0.91 * (qPow(sd,0.31)) - 0.38;
        formvs->setA(0.91);  formvs->setB(0.31);
        formvs->setC(0.38); formvs->setArtLabel("Schule");
    }

    if(ui->buildingBox->currentText() == "Verwaltungsgebäude"){
        sp = 0.91 * (qPow(sd,0.31)) - 0.38;
        formvs->setA(0.91);  formvs->setB(0.31);
        formvs->setC(0.38); formvs->setArtLabel("Verwaltungsgebäude");
    }

    if(ui->buildingBox->currentText() == "Krankenhaus"){
        sp = 0.75 * (qPow(sd,0.44)) - 0.18;
        formvs->setA(0.75);  formvs->setB(0.44);
        formvs->setC(0.18); formvs->setArtLabel("Krankenhaus");
    }

    if(ui->buildingBox->currentText() == "Hotel"){
        sp = 0.70 * (qPow(sd,0.48)) - 0.13;
        formvs->setA(0.70);  formvs->setB(0.48);
        formvs->setC(0.13); formvs->setArtLabel("Hotel");
    }

    if(ui->buildingBox->currentText() == "Pflegeheim"){
        sp = 1.40 * (qPow(sd,0.14)) - 0.92;
        formvs->setA(1.40);  formvs->setB(0.14);
        formvs->setC(0.92); formvs->setArtLabel("Pflegeheim");
    }

    if(ui->buildingBox->currentText() == "Altersheim und betreutes Wohnen"){
        sp = 1.48 * (qPow(sd,0.19)) - 0.94;
        formvs->setA(1.48);  formvs->setB(0.19);
        formvs->setC(0.94); formvs->setArtLabel("Altersheim und betreutes Wohnen");
    }

    formvs->setVr(sd);
    ui->spitzendurchflussBox->setValue(sp+pf);
}

void FSAquaWindow::readSettings()
{
    QSettings settings("FasaSoft", "AquaWindowMain");

    QRect rect = settings.value("geometrie", QRect(0,0,700,800)).toRect();
    setGeometry(rect.x(), rect.y(), rect.width(), rect.height());

    if(settings.value("toolsvisible").toBool()){
        tools->show();
        ui->actionTools->setChecked(true);
    }else{
        tools->hide();
        ui->actionTools->setChecked(false);
    }

    tools->setGeometry(settings.value("toolsgeometrie", QRect(0,0,300,500)).toRect());

    int fcount =  settings.value("floorcount", 1).toInt();
    floorCountChanged( fcount );
    tools->setFloorCount( fcount );
}

void FSAquaWindow::saveSettings()
{
    QSettings settings("FasaSoft", "AquaWindowMain");
    settings.setValue("geometrie", this->geometry());
    settings.setValue("editor", ui->editorView->rect());
    settings.setValue("toolsgeometrie", tools->geometry());
    settings.setValue("toolsvisible", tools->isVisible());
    settings.setValue("floorcount", floorCount);
}

void FSAquaWindow::Sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
        while( QTime::currentTime() < dieTime )
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

