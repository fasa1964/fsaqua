#include "gbadobject.h"

#include <QPainter>
#include <QMessageBox>

#include <QSvgRenderer>
#include <QTransform>

#include <QDebug>

int GBadObject::type() const
{
    return Type;
}

GBadObject::GBadObject(GBadObjectType TYPE, QGraphicsItem *parent) :
    QGraphicsObject (parent)
{
    Type = TYPE;

    if(Type == WT)
    {
       svgItem = new QGraphicsSvgItem(":/objects/wt.svg", this);
       setSymbolFilename(":/objects/wt.svg");
       setBezeichnung(tr("WT-Waschtisch"));
       setFabrikat(tr(""));


       // Flowmeter
       setEn_lu(1);         // Belastungswert nach DIN EN 806-3
       setLu(1);            // Belastungswert LU
       setBdKW(0.07);       // Berechnungsdurchfluss Kaltwasser
       setDin_bdKW(0.07);   // Berechnungsdurchfluss Kaltwasser nach DIN
       setBdWW(0.07);       // Berechnungsdurchfluss Warmwasser
       setDin_bdWW(0.07);   // Berechnungsdurchfluss Warmwasser nach DIN

       // minimum Pressure
       setMd(1000.0);         // Mindestfließdruck in hPa
       setDin_md(1000.0);     // Mindestfließdruck in hPa

       setKwh(0.7);         // Wärmebedarf nach DIN 4708
       setDn(15);           // Anschlussnennweite DN

       setCold(true);       // Has cold water connection
       setHot(true);        // Has hot water connection

       setAnschlussHoehe(0.55); // Anschlusshöhe in Meter

    }

    if(Type == WC)
    {
        svgItem = new QGraphicsSvgItem(":/objects/wc.svg", this);
        setSymbolFilename(":/objects/wc.svg");
        setBezeichnung(tr("WC-Becken"));

        // Flowmeter
        setEn_lu(1);         // Belastungswert nach DIN EN 806-3
        setLu(1);            // Belastungswert LU
        setBdKW(0.13);       // Berechnungsdurchfluss Kaltwasser
        setDin_bdKW(0.13);    // Berechnungsdurchfluss Kaltwasser nach DIN
        setBdWW(0.0);        // Berechnungsdurchfluss Warmwasser
        setDin_bdWW(0.0);    // Berechnungsdurchfluss Warmwasser nach DIN

        // minimum Pressure
        setMd(500.0);         // Mindestfließdruck in hPa
        setDin_md(500.0);     // Mindestfließdruck in hPa

        setKwh(0.0);         // Wärmebedarf nach DIN 4708
        setDn(15);           // Anschlussnennweite DN
        setCold(true);       // Has cold water connection
        setHot(false);        // Has hot water connection

        setAnschlussHoehe(1.20); // Anschlusshöhe in Meter
    }

    if(Type == WCDS)
    {
        svgItem = new QGraphicsSvgItem(":/objects/wcds.svg", this);
        setSymbolFilename(":/objects/wcds.svg");
        setBezeichnung(tr("WC-Becken mit Druckspüler"));

        // Flowmeter
        setEn_lu(15);        // Belastungswert nach DIN EN 806-3
        setLu(15);           // Belastungswert LU
        setBdKW(1.0);        // Berechnungsdurchfluss Kaltwasser
        setDin_bdKW(1.0);    // Berechnungsdurchfluss Kaltwasser nach DIN
        setBdWW(0.0);        // Berechnungsdurchfluss Warmwasser
        setDin_bdWW(0.0);    // Berechnungsdurchfluss Warmwasser nach DIN

        // minimum Pressure
        setMd(1200.0);         // Mindestfließdruck in hPa
        setDin_md(1200.0);     // Mindestfließdruck in hPa

        setKwh(0.0);         // Wärmebedarf nach DIN 4708
        setDn(20);           // Anschlussnennweite DN
        setCold(true);       // Has cold water connection
        setHot(false);        // Has hot water connection

        setAnschlussHoehe(1.2); // Anschlusshöhe in Meter

    }

    if(Type == WA)
    {
        svgItem = new QGraphicsSvgItem(":/objects/wa.svg", this);
        setSymbolFilename(":/objects/wa.svg");
        setBezeichnung(tr("WA-Badewanne"));

        // Flowmeter
        setEn_lu(4);         // Belastungswert nach DIN EN 806-3
        setLu(4);            // Belastungswert LU
        setBdKW(0.15);       // Berechnungsdurchfluss Kaltwasser
        setDin_bdKW(0.15);   // Berechnungsdurchfluss Kaltwasser nach DIN
        setBdWW(0.15);       // Berechnungsdurchfluss Warmwasser
        setDin_bdWW(0.15);   // Berechnungsdurchfluss Warmwasser nach DIN

        // minimum Pressure
        setMd(1000.0);         // Mindestfließdruck in hPa
        setDin_md(1000.0);     // Mindestfließdruck in hPa

        setKwh(5.82);        // Wärmebedarf nach DIN 4708
        setDn(15);           // Anschlussnennweite DN
        setCold(true);       // Has cold water connection
        setHot(true);        // Has hot water connection

        setAnschlussHoehe(0.75); // Anschlusshöhe in Meter


    }

    if(Type == DU)
    {
        svgItem = new QGraphicsSvgItem(":/objects/du.svg", this);
        setSymbolFilename(":/objects/du.svg");
        setBezeichnung(tr("DU-Duschwanne"));

        // Flowmeter
        setEn_lu(2);         // Belastungswert nach DIN EN 806-3
        setLu(2);            // Belastungswert LU
        setBdKW(0.15);       // Berechnungsdurchfluss Kaltwasser
        setDin_bdKW(0.15);   // Berechnungsdurchfluss Kaltwasser nach DIN
        setBdWW(0.15);       // Berechnungsdurchfluss Warmwasser
        setDin_bdWW(0.15);   // Berechnungsdurchfluss Warmwasser nach DIN

        // minimum Pressure
        setMd(1000.0);         // Mindestfließdruck in hPa
        setDin_md(1000.0);     // Mindestfließdruck in hPa

        setKwh(1.63);        // Wärmebedarf nach DIN 4708
        setDn(15);           // Anschlussnennweite DN
        setCold(true);       // Has cold water connection
        setHot(true);        // Has hot water connection

        setAnschlussHoehe(1.25); // Anschlusshöhe in Meter
    }

    if(Type == SW)
    {
        svgItem = new QGraphicsSvgItem(":/objects/sw.svg", this);
        setSymbolFilename(":/objects/sw.svg");
        setBezeichnung(tr("SW-Sitzwaschbecken"));

        // Flowmeter
        setEn_lu(1);         // Belastungswert nach DIN EN 806-3
        setLu(1);            // Belastungswert LU
        setBdKW(0.07);       // Berechnungsdurchfluss Kaltwasser
        setDin_bdKW(0.07);   // Berechnungsdurchfluss Kaltwasser nach DIN
        setBdWW(0.07);       // Berechnungsdurchfluss Warmwasser
        setDin_bdWW(0.07);   // Berechnungsdurchfluss Warmwasser nach DIN

        // minimum Pressure
        setMd(1000.0);         // Mindestfließdruck in hPa
        setDin_md(1000.0);     // Mindestfließdruck in hPa

        setKwh(0.81);        // Wärmebedarf nach DIN 4708
        setDn(15);           // Anschlussnennweite DN
        setCold(true);       // Has cold water connection
        setHot(true);        // Has hot water connection

        setAnschlussHoehe(0.35); // Anschlusshöhe in Meter
    }

    if(Type == WM)
    {
        svgItem = new QGraphicsSvgItem(":/objects/wm.svg", this);
        setSymbolFilename(":/objects/wm.svg");
        setBezeichnung(tr("WM-Waschmaschine"));

        // Flowmeter
        setEn_lu(2);         // Belastungswert nach DIN EN 806-3
        setLu(2);            // Belastungswert LU
        setBdKW(0.15);       // Berechnungsdurchfluss Kaltwasser
        setDin_bdKW(0.15);   // Berechnungsdurchfluss Kaltwasser nach DIN
        setBdWW(0.0);        // Berechnungsdurchfluss Warmwasser
        setDin_bdWW(0.0);    // Berechnungsdurchfluss Warmwasser nach DIN

        // minimum Pressure
        setMd(500.0);          // Mindestfließdruck in hPa
        setDin_md(500.0);      // Mindestfließdruck in hPa

        setKwh(0.0);         // Wärmebedarf nach DIN 4708
        setDn(15);           // Anschlussnennweite DN
        setCold(true);       // Has cold water connection
        setHot(false);        // Has hot water connection

        setAnschlussHoehe(1.10); // Anschlusshöhe in Meter
    }

    if(Type == GS)
    {
        svgItem = new QGraphicsSvgItem(":/objects/gs.svg", this);
        setSymbolFilename(":/objects/gs.svg");
        setBezeichnung(tr("GS-Geschirrspüler"));

        // Flowmeter
        setEn_lu(2);         // Belastungswert nach DIN EN 806-3
        setLu(2);            // Belastungswert LU
        setBdKW(0.07);       // Berechnungsdurchfluss Kaltwasser
        setDin_bdKW(0.07);   // Berechnungsdurchfluss Kaltwasser nach DIN
        setBdWW(0.0);        // Berechnungsdurchfluss Warmwasser
        setDin_bdWW(0.0);    // Berechnungsdurchfluss Warmwasser nach DIN

        // minimum Pressure
        setMd(500.0);          // Mindestfließdruck in hPa
        setDin_md(500.0);      // Mindestfließdruck in hPa

        setKwh(0.0);         // Wärmebedarf nach DIN 4708
        setDn(15);           // Anschlussnennweite DN
        setCold(true);       // Has cold water connection
        setHot(false);        // Has hot water connection

        setAnschlussHoehe(0.55); // Anschlusshöhe in Meter
    }

    if(Type == UR)
    {
        svgItem = new QGraphicsSvgItem(":/objects/ur.svg", this);
        setSymbolFilename(":/objects/ur.svg");
        setBezeichnung(tr("UR-Urinalbecken"));

        // Flowmeter
        setEn_lu(3);         // Belastungswert nach DIN EN 806-3
        setLu(3);            // Belastungswert LU
        setBdKW(0.3);        // Berechnungsdurchfluss Kaltwasser
        setDin_bdKW(0.3);    // Berechnungsdurchfluss Kaltwasser nach DIN
        setBdWW(0.0);        // Berechnungsdurchfluss Warmwasser
        setDin_bdWW(0.0);    // Berechnungsdurchfluss Warmwasser nach DIN

        // minimum Pressure
        setMd(1000.0);         // Mindestfließdruck in hPa
        setDin_md(1000.0);     // Mindestfließdruck in hPa

        setKwh(0.0);         // Wärmebedarf nach DIN 4708
        setDn(15);           // Anschlussnennweite DN
        setCold(true);       // Has cold water connection
        setHot(false);        // Has hot water connection

        setAnschlussHoehe(1.2); // Anschlusshöhe in Meter
    }

    if(Type == AGAV)
    {
        svgItem = new QGraphicsSvgItem(":/objects/agav.svg", this);
        setSymbolFilename(":/objects/agav.svg");
        setBezeichnung(tr("AGAV-Ausgußbecken mit Auslaufventil"));

        // Flowmeter
        setEn_lu(2);         // Belastungswert nach DIN EN 806-3
        setLu(2);            // Belastungswert LU
        setBdKW(0.15);       // Berechnungsdurchfluss Kaltwasser
        setDin_bdKW(0.15);   // Berechnungsdurchfluss Kaltwasser nach DIN
        setBdWW(0.0);        // Berechnungsdurchfluss Warmwasser
        setDin_bdWW(0.0);    // Berechnungsdurchfluss Warmwasser nach DIN

        // minimum Pressure
        setMd(1000.0);          // Mindestfließdruck in hPa
        setDin_md(1000.0);      // Mindestfließdruck in hPa

        setKwh(0.0);         // Wärmebedarf nach DIN 4708
        setDn(15);           // Anschlussnennweite DN
        setCold(true);       // Has cold water connection
        setHot(false);        // Has hot water connection

        setAnschlussHoehe(1.15); // Anschlusshöhe in Meter
    }

    if(Type == SP)
    {
        svgItem = new QGraphicsSvgItem(":/objects/sp.svg", this);
        setSymbolFilename(":/objects/sp.svg");
        setBezeichnung(tr("SP-Spülbecken"));

        // Flowmeter
        setEn_lu(2);         // Belastungswert nach DIN EN 806-3
        setLu(2);            // Belastungswert LU
        setBdKW(0.07);       // Berechnungsdurchfluss Kaltwasser
        setDin_bdKW(0.07);   // Berechnungsdurchfluss Kaltwasser nach DIN
        setBdWW(0.07);       // Berechnungsdurchfluss Warmwasser
        setDin_bdWW(0.07);   // Berechnungsdurchfluss Warmwasser nach DIN

        // minimum Pressure
        setMd(1000.0);          // Mindestfließdruck in hPa
        setDin_md(1000.0);      // Mindestfließdruck in hPa

        setKwh(1.16);        // Wärmebedarf nach DIN 4708
        setDn(15);           // Anschlussnennweite DN
        setCold(true);       // Has cold water connection
        setHot(true);        // Has hot water connection

        setAnschlussHoehe(0.55); // Anschlusshöhe in Meter
    }

    if(Type == AV)
    {
        svgItem = new QGraphicsSvgItem(":/objects/av.svg", this);
        setSymbolFilename(":/objects/av.svg");
        setBezeichnung(tr("Auslaufventil"));

        // Flowmeter
        setEn_lu(5);         // Belastungswert nach DIN EN 806-3
        setLu(5);            // Belastungswert LU
        setBdKW(0.30);       // Berechnungsdurchfluss Kaltwasser
        setDin_bdKW(0.30);   // Berechnungsdurchfluss Kaltwasser nach DIN
        setBdWW(0.00);       // Berechnungsdurchfluss Warmwasser
        setDin_bdWW(0.00);   // Berechnungsdurchfluss Warmwasser nach DIN

        // minimum Pressure
        setMd(500.0);          // Mindestfließdruck in hPa
        setDin_md(500.0);      // Mindestfließdruck in hPa

        setKwh(0.00);        // Wärmebedarf nach DIN 4708
        setDn(15);           // Anschlussnennweite DN
        setCold(true);       // Has cold water connection
        setHot(false);        // Has hot water connection

        setAnschlussHoehe(0.95); // Anschlusshöhe in Meter
    }

    if(Type == DHE)
    {
        svgItem = new QGraphicsSvgItem(":/objects/dhe.svg", this);
        setSymbolFilename(":/objects/dhe.svg");
        setBezeichnung(tr("DHE-Durschlauferhitzer"));

        // Flowmeter
        setEn_lu(0);         // Belastungswert nach DIN EN 806-3
        setLu(0);            // Belastungswert LU
        setBdKW(0.0);        // Berechnungsdurchfluss Kaltwasser
        setDin_bdKW(0.0);    // Berechnungsdurchfluss Kaltwasser nach DIN
        setBdWW(0.0);        // Berechnungsdurchfluss Warmwasser
        setDin_bdWW(0.0);    // Berechnungsdurchfluss Warmwasser nach DIN

        // minimum Pressure
        setMd(0.0);            // Mindestfließdruck in hPa
        setDin_md(0.0);        // Mindestfließdruck in hPa

        setKwh(0.0);         // Wärmebedarf nach DIN 4708
        setDn(15);           // Anschlussnennweite DN
        setCold(true);       // Has cold water connection
        setHot(true);        // Has hot water connection

        setAnschlussHoehe(0.35); // Anschlusshöhe in Meter
    }

    if(Type == WSIB)
    {
        svgItem = new QGraphicsSvgItem(":/objects/wsib.svg", this);
        setSymbolFilename(":/objects/wsib.svg");
        setBezeichnung(tr("WISB-Trinkwasserspeicher indirekt beheizt"));

        // Flowmeter
        setEn_lu(0);         // Belastungswert nach DIN EN 806-3
        setLu(0);            // Belastungswert LU
        setBdKW(0.0);        // Berechnungsdurchfluss Kaltwasser
        setDin_bdKW(0.0);    // Berechnungsdurchfluss Kaltwasser nach DIN
        setBdWW(0.0);        // Berechnungsdurchfluss Warmwasser
        setDin_bdWW(0.0);    // Berechnungsdurchfluss Warmwasser nach DIN

        // minimum Pressure
        setMd(0.0);            // Mindestfließdruck in hPa
        setDin_md(0.0);        // Mindestfließdruck in hPa

        setKwh(0.0);         // Wärmebedarf nach DIN 4708
        setDn(15);           // Anschlussnennweite DN
        setCold(true);       // Has cold water connection
        setHot(true);        // Has hot water connection

        setAnschlussHoehe(1.8); // Anschlusshöhe in Meter
    }

    if(svgItem != nullptr){
        svgItem->setScale(0.7);
        rect = QRectF(0,0, svgItem->boundingRect().width()*0.7, svgItem->boundingRect().height()*0.7);
    }

    setDauerlaeufer(false);
    setDisplayNr(false);
    setPipeNr(0);

    setAcceptDrops(true);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptHoverEvents(true);
    setToolTip(bezeichnung());


    QTransform tr;
    tr.rotate(180, Qt::XAxis);
    setTransform(tr);

}

GBadObject::GBadObject(QGraphicsItem *parent) :
    QGraphicsObject (parent)
{

}

GBadObject::GBadObject(const GBadObject &other, QGraphicsItem *parent) :
    QGraphicsObject (parent)
{
    //Type = other.type();
    rect = other.boundingRect();                    // bounding Rect
    svgItem = other.getSvgItem();                   // Grafisches Symbol svg
    m_symbolFilename = other.symbolFilename();      // path of image
    m_sPos = other.sPos();                          // Position in scene

    // Values for object
    m_nr = other.getNr();
    m_pipeNr = other.getPipeNr();

    m_din_bdKW = other.din_bdKW();                  // Berechnungsdurchfluss Kaltwasser nach DIN
    m_din_bdWW = other.din_bdWW();                  // Berechnungsdurchfluss Warmwasser nach DIN
    m_din_md = other.din_md();                      // Mindestfließdruck nach DIN

    m_bdKW = other.bdKW();                          // Berechnungsdurchfluss Kaltwasser
    m_bdWW = other.bdWW();                          // Berechnungsdurchfluss Warmwasser
    m_md = other.md();                              // Mindestfließdruck in hPa
    m_kwh = other.kwh();                            // Wärmebedarf

    m_en_lu = other.en_lu();                        // Belastungswert nach EN806
    m_lu = other.lu();                              // Belastungswert

    m_dn = other.dn();                              // Anschluss-Nennweite

    m_bezeichnung = other.bezeichnung();            // Description
    m_fabrikat = other.fabrikat();                  // Type, Manufakturing
    m_dauerlaeufer = other.getDauerlaeufer();
    m_floorIndex = other.getFloorIndex();

    m_cold = other.getCold();
    m_hot = other.getHot();
}

QRectF GBadObject::boundingRect() const
{
    return rect;
}

void GBadObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QPen pen;
    pen.setColor(Qt::blue);
    pen.setWidthF(1.0);
    painter->setPen(pen);

    if(isSelected())
        painter->drawRect(rect);

    if(displayNr()){

        if(isSelected()){
            pen.setColor(Qt::magenta);
            pen.setWidthF(0.5);
            painter->setPen(pen);
        }

        painter->drawText(5,rect.height()-2, QString("%1").arg(getNr()));
    }
}

QString GBadObject::bezeichnung() const
{
    return m_bezeichnung;
}

void GBadObject::setBezeichnung(const QString &bezeichnung)
{
    m_bezeichnung = bezeichnung;
}

QString GBadObject::symbolFilename() const
{
    return m_symbolFilename;
}

void GBadObject::setSymbolFilename(const QString &symbolFilename)
{
    m_symbolFilename = symbolFilename;
}

qreal GBadObject::din_bdKW() const
{
    return m_din_bdKW;
}

void GBadObject::setDin_bdKW(const qreal &din_bdKW)
{
    m_din_bdKW = din_bdKW;
}

qreal GBadObject::din_bdWW() const
{
    return m_din_bdWW;
}

void GBadObject::setDin_bdWW(const qreal &din_bdWW)
{
    m_din_bdWW = din_bdWW;
}

qreal GBadObject::bdKW() const
{
    return m_bdKW;
}

void GBadObject::setBdKW(const qreal &bdKW)
{
    m_bdKW = bdKW;
}

qreal GBadObject::bdWW() const
{
    return m_bdWW;
}

void GBadObject::setBdWW(const qreal &bdWW)
{
    m_bdWW = bdWW;
}

qreal GBadObject::din_md() const
{
    return m_din_md;
}

void GBadObject::setDin_md(const qreal &din_md)
{
    m_din_md = din_md;
}

qreal GBadObject::md() const
{
    return m_md;
}

void GBadObject::setMd(const qreal &md)
{
    m_md = md;
}

int GBadObject::en_lu() const
{
    return m_en_lu;
}

void GBadObject::setEn_lu(int en_lu)
{
    m_en_lu = en_lu;
}

int GBadObject::lu() const
{
    return m_lu;
}

void GBadObject::setLu(int lu)
{
    m_lu = lu;
}

int GBadObject::dn() const
{
    return m_dn;
}

void GBadObject::setDn(int dn)
{
    m_dn = dn;
}

QString GBadObject::fabrikat() const
{
    return m_fabrikat;
}

void GBadObject::setFabrikat(const QString &fabrikat)
{
    m_fabrikat = fabrikat;
}

QPointF GBadObject::sPos() const
{
    return m_sPos;
}

void GBadObject::setSPos(const QPointF &sPos)
{
    m_sPos = sPos;
}

qreal GBadObject::kwh() const
{
    return m_kwh;
}

void GBadObject::setKwh(const qreal &kwh)
{
    m_kwh = kwh;
}


void GBadObject::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;

    QPixmap ipix = QPixmap(":/buttonicons/info.svg");
    QPixmap cpix = QPixmap(":/buttonicons/garbage.svg");

    QAction *infoAction = menu.addAction(QIcon(ipix),tr("info"));
    QAction *deleteAction = menu.addAction(QIcon(cpix), tr("delete"));
    connect(infoAction, &QAction::triggered, this, &GBadObject::infoAction);
    connect(deleteAction, &QAction::triggered, this, &GBadObject::deleteAction);

    menu.exec( event->screenPos() );
}

void GBadObject::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *)
{
    emit objectInfo( this );
}

qreal GBadObject::getAnschlussHoehe() const
{
    return m_anschlussHoehe;
}

void GBadObject::setAnschlussHoehe(const qreal &anschlussHoehe)
{
    m_anschlussHoehe = anschlussHoehe;
}

bool GBadObject::displayNr() const
{
    return m_displayNr;
}

void GBadObject::setDisplayNr(bool displayNr)
{
    m_displayNr = displayNr;
}

QPointF GBadObject::leftPos()
{
    return mapToScene( boundingRect().bottomLeft() );
}

QPointF GBadObject::rightPos()
{
    return mapToScene( boundingRect().bottomRight() );
}

int GBadObject::getNr() const
{
    return m_nr;
}

void GBadObject::setNr(int nr)
{
    m_nr = nr;
}

bool GBadObject::getHot() const
{
    return m_hot;
}

void GBadObject::setHot(bool hot)
{
    m_hot = hot;
}

bool GBadObject::getCold() const
{
    return m_cold;
}

void GBadObject::setCold(bool cold)
{
    m_cold = cold;
}

int GBadObject::getPipeNr() const
{
    return m_pipeNr;
}

void GBadObject::setPipeNr(int pipeNr)
{
    m_pipeNr = pipeNr;
}

bool GBadObject::getDauerlaeufer() const
{
    return m_dauerlaeufer;
}

void GBadObject::setDauerlaeufer(bool dauerlaeufer)
{
    m_dauerlaeufer = dauerlaeufer;
}

QPointF GBadObject::connectCold()
{
    QPointF point = mitte();

    if(type() == GBadObject::AV || type() == GBadObject::WM || type() == GBadObject::UR
            || type() == GBadObject::AGAV || type() == GBadObject::WCDS)
        point = mitte();

    if(type() == GBadObject::WT || type() == GBadObject::DU || type() == GBadObject::WA
        || type() == GBadObject::SP || type() == GBadObject::SW)
        point.setX( point.x() + 7);

    if(type() == GBadObject::DHE){
        point.setX( point.x() + 18);
    }

    if(type() == GBadObject::WSIB){
        point.setX( 0 );
        point.setY( 67 );
    }

    return mapToScene( point );
}

QPointF GBadObject::connectHot()
{
     QPointF point = mitte();

     if(type() == GBadObject::WT || type() == GBadObject::DU || type() == GBadObject::WA
         || type() == GBadObject::SP || type() == GBadObject::SW)
         point.setX( point.x() - 7);

     if(type() == GBadObject::WSIB){
         point.setX( point.x() + 8);

     }

     if(type() == GBadObject::DHE){
         point.setX( point.x() - 18);
     }

     return mapToScene(point);
}

QPointF GBadObject::getMitte()
{
    QPointF point = mitte();
    return mapToScene(point);
}

QPointF GBadObject::mitte()
{
    QPointF pos = scenePos();
    pos.setX( boundingRect().width() / 2 );
    pos.setY( boundingRect().height()-boundingRect().height() + 2 );
    return pos;
}

QVariant GBadObject::region()
{
    QRectF sr = mapRectToScene(rect.x(), rect.y(), rect.width(), rect.height());
    return sr;
}

int GBadObject::getFloorIndex() const
{
    return m_floorIndex;
}

void GBadObject::setFloorIndex(int foorIndex)
{
    m_floorIndex = foorIndex;
}

QGraphicsSvgItem *GBadObject::getSvgItem() const
{
    return svgItem;
}

void GBadObject::setSvgItem(QGraphicsSvgItem *value)
{
    svgItem = value;
}

void GBadObject::deleteAction()
{
    emit objectHasDeleted( this );
}

void GBadObject::infoAction()
{
    emit objectInfo( this );
}

QDataStream & operator << (QDataStream& out, const GBadObject& obj)
{
    GBadObject::GBadObjectType t;
    (quint32&) t = obj.type();

    out << t << obj.getNr() << obj.bezeichnung() << obj.fabrikat() << obj.scenePos() << obj.lu() <<
           obj.bdKW() << obj.bdWW() << obj.md() << obj.kwh() << obj.dn() << obj.getDauerlaeufer() <<
           obj.getFloorIndex() << obj.dn() << obj.getCold() << obj.getHot() << obj.getPipeNr();

    return out;
}

QDataStream & operator >> (QDataStream& in, GBadObject& obj)
{
    GBadObject::GBadObjectType t;
    int nr;
    QString bez;
    QString fab;
    QPointF sp;
    int lu;
    double kw;
    double ww;
    int md;
    double kwh;
    int dn;
    bool cold;
    bool hot;
    int pnr;


    in >> (quint32&)t >> nr >> bez >> fab >> sp >> lu >> kw >> ww >> md >> kwh >> dn >> cold >> hot >> pnr;


    obj.setBezeichnung(bez);
    obj.setNr(nr);
    obj.setFabrikat(fab);
    obj.setSPos(sp);
    obj.setLu(lu);
    obj.setBdKW(kw);
    obj.setBdWW(ww);
    obj.setMd(md);
    obj.setKwh(kwh);
    obj.setDn(dn);
    obj.setCold(cold);
    obj.setHot(hot);
    obj.setPipeNr(pnr);

    return in;
}
