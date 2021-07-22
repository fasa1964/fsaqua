#include "gpipe.h"

#include <QPen>
#include <QPainter>
#include <QFont>
#include <QFontMetrics>

#include <scene.h>


#include <QDebug>

int GPipe::type() const
{
    return Type;
}


GPipe::GPipe(GPipeType TYPE, bool COLD, double LENGTH, QGraphicsItem *parent) :
    QGraphicsObject (parent)
{
    Type = TYPE;
    m_cold = COLD;
    m_length = LENGTH;


    if(cold()){
        m_density = 999.7;
        m_color = Qt::darkGreen;
    }else{
        m_density = 983.2;
        m_color = Qt::red;
    }


    // Anschlussleitung
    if(Type == RGA)
    {
       rect = QRectF(-10, -5, 20, m_length+10);
       p1 = QPointF(0,0);
       p2 = QPointF(0, m_length);
    }

    // Stockwerksleitung
    if(Type == RGS)
    {
        rect = QRectF(-5,-10, m_length+10, 20);
        p1 = QPointF(0,0);
        p2 = QPointF(m_length,0);
    }

    // Strangleitung
    if(Type == STRANG)
    {
        rect = QRectF(-10, -5, 20, m_length+10);
        p1 = QPointF(0,0);
        p2 = QPointF(0, m_length);

        setMeter( m_length/100.0);
    }

    // Strangleitung
    if(Type == ZIRK)
    {
        rect = QRectF(-10, -5, 20, m_length+10);
        m_color = QColor(238,130,238); // violet
    }


    QTransform trans;
    trans.rotate(180, Qt::YAxis);
    //trans.rotate(180, Qt::XAxis);
    setTransform(trans);

    setAcceptDrops(true);
    setFlag(QGraphicsItem::ItemIsSelectable);
    //setFlag(QGraphicsItem::ItemIsMovable);
    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptHoverEvents(true);


    // Settings
    setActiv(false);
    setMarker(false);
    setShowArrow(false);
    setNr(0);
    setFloorIndex(0);
    setSelected(false);
    setShowNr(false);
    setMaterial("");
    setFlowSpeed(2.0);
    setTotalResistance(45.0);
    setDn(0);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GPipe::updateBlinkingColor);
}

QRectF GPipe::boundingRect() const
{
    return rect;
}

void GPipe::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QBrush br;
    br.setStyle(Qt::NoBrush);

    QPen pen;
    pen.setColor( color() );

    pen.setWidthF(2);
    if(getMarker())
        pen.setStyle(Qt::DashDotLine);
    else
        pen.setStyle(Qt::SolidLine);
    painter->setPen( pen );
    painter->setBrush(br);


    painter->drawLine(p1,p2);

    if(getShowArrow()){
        pen.setColor( Qt::blue );
        pen.setWidthF(1);
        painter->setPen( pen );
        painter->drawPath(arrow());
    }

    if(getShowNr()){
         //= painter->fontMetrics();
        QFont font = painter->font();
        QFontMetrics fm(font);
        int w = fm.width(QString("%1").arg( getNr()));
        pen.setColor( Qt::blue );
        pen.setWidthF(1);
        painter->setPen( pen );

        painter->rotate(180);
        if(type() == GPipe::RGS)
            painter->drawText(-30-w,-2,QString("%1").arg( getNr()) );
        if(type() == GPipe::RGA && cold())
            painter->drawText(2,-15,QString("%1").arg( getNr()) );
        if(type() == GPipe::RGA && !cold())
            painter->drawText(-2 - w, -15,QString("%1").arg( getNr()) );
        if(type() == GPipe::STRANG && cold())
            painter->drawText(-15,-length()/2,QString("%1").arg( getNr()) );
        if(type() == GPipe::STRANG && !cold())
            painter->drawText(15 - w, -length()/2 ,QString("%1").arg( getNr()) );

        painter->rotate(180);
    }

    // Test for showing p1 and p2
    pen.setColor( Qt::blue );
    pen.setWidthF(3);
    painter->setPen( pen );
    painter->drawPoint(p1);

    pen.setColor( Qt::magenta );
    pen.setWidthF(3);
    painter->setPen( pen );
    painter->drawPoint(p2);

    if(isSelected()){
        pen.setColor( QColor(15,87,125) );
        pen.setWidthF(1.0);
        painter->setPen( pen );
        painter->drawRect(rect);
    }

    if(activ() &&  this->type() == GPipe::STRANG && getMarker()){
        br.setStyle(Qt::SolidPattern);
        br.setColor(Qt::magenta);
        painter->setBrush(br);
        pen.setColor( Qt::magenta );
        pen.setWidthF(0.5);
        painter->setPen( pen );
        painter->drawEllipse(5,5,5,5);

    }
}

QRectF GPipe::sceneRect()
{
    return mapRectToScene(boundingRect());
}


QVariant GPipe::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
 {

    if(change == QGraphicsItem::ItemSelectedChange){
         if(value == true){
             setActiv(true);
             emit pipeIsSelected(this);
         }
     }
    return QGraphicsItem::itemChange(change, value);
}

int GPipe::getDn() const
{
    return m_dn;
}

void GPipe::setDn(int dn)
{
    m_dn = dn;
}

double GPipe::getTotalResistance() const
{
    return m_totalResistance;
}

void GPipe::setTotalResistance(double totalResistance)
{
    m_totalResistance = totalResistance;
}

double GPipe::getFlowSpeed() const
{
    return m_flowSpeed;
}

void GPipe::setFlowSpeed(double flowSpeed)
{
    m_flowSpeed = flowSpeed;
}

QString GPipe::getMaterial() const
{
    return m_material;
}

void GPipe::setMaterial(const QString &material)
{
    m_material = material;
}

bool GPipe::getShowNr() const
{
    return m_showNr;
}

void GPipe::setShowNr(bool showNr)
{
    m_showNr = showNr;
}

int GPipe::getStrangMarkerNr() const
{
    return m_strangMarkerNr;
}

void GPipe::setStrangMarkerNr(int strangMarkerNr)
{
    m_strangMarkerNr = strangMarkerNr;
}

QList<int> GPipe::getObjectNrList() const
{
    return objectNrList;
}

void GPipe::setObjectNrList(const QList<int> &value)
{
    objectNrList = value;
}

QVariant GPipe::region()
{
    QRectF sr = mapRectToScene(rect.x(), rect.y(), rect.width(), rect.height());
    return sr;
}

bool GPipe::getReadToConnect() const
{
    return m_readToConnect;
}

void GPipe::setReadToConnect(bool readToConnect)
{
    if(readToConnect)
    {
        timer->start(500);
    }
    else
    {
        if(timer != nullptr)
        {
            timer->stop();
            if(cold())
                setColor(Qt::darkGreen);
            if(!cold())
                setColor(Qt::red);
            update();
        }
    }

    m_readToConnect = readToConnect;
}

qreal GPipe::getBdWW() const
{
    double ww = 0.0;

    foreach(GBadObject *o, objectList){
        ww += o->bdWW();
    }

    return ww;
}

qreal GPipe::getBdKW() const
{
    double kw = 0.0;

    foreach(GBadObject *o, objectList){
        kw += o->bdKW();
    }

    return kw;
}

bool GPipe::getMarker() const
{
    return m_marker;
}

void GPipe::setMarker(bool marker)
{
    m_marker = marker;
}

bool GPipe::getShowArrow() const
{
    return m_showArrow;
}

void GPipe::setShowArrow(bool showArrow)
{
    m_showArrow = showArrow;
}

bool GPipe::containsWSIB()
{
    bool status = false;

    foreach(GBadObject *o, objectList){
        if(o->type() == GBadObject::WSIB){
            status = true;
            break;
        }
    }

    return status;
}

QString GPipe::getFlowDirection() const
{
    return m_flowDirection;
}

void GPipe::setFlowDirection(const QString &flowDirection)
{
    m_flowDirection = flowDirection;
}

QPainterPath GPipe::arrow()
{
    QPainterPath path;
    qreal len = 25;
    if(length() < 25)
        len = 10;


    if((type() == GPipe::RGA || type() == GPipe::STRANG) && getFlowDirection() == "up")
    {
        // Flowdirection up to p2
        path.setFillRule(Qt::WindingFill);
        path.moveTo(rect.width()/2-3.5, rect.height()/2);
        path.lineTo(rect.width()/2 , rect.height()/2 - 9.5);
        path.lineTo(rect.width()/2-7 , rect.height()/2-9.5 );
        path.lineTo(rect.width()/2-3.5, rect.height()/2);
        path.closeSubpath();
        path.moveTo(rect.width()/2-3.5, rect.height()/2-9.5);
        path.lineTo(rect.width()/2-3.5, rect.height()/2-len);
    }

    if((type() == GPipe::RGA || type() == GPipe::STRANG) && getFlowDirection() == "down")
    {
        // Flowdirection down to p1
        path.setFillRule(Qt::WindingFill);
        path.moveTo(rect.width()/2-1, rect.height()/2);
        path.lineTo(rect.width()/2-7 , rect.height()/2 );
        path.lineTo(rect.width()/2-3.5 , rect.height()/2-9.5 );
        path.lineTo(rect.width()/2-1, rect.height()/2);
        path.moveTo(rect.width()/2-3.5, rect.height()/2);
        path.lineTo(rect.width()/2-3.5, rect.height()/2+len);
    }

    if(type() == GPipe::RGS && getFlowDirection() == "left")
    {
        // Flowdirection left to p2
        path.setFillRule(Qt::WindingFill);
        path.moveTo(rect.width()/2, rect.height()/2-1);
        path.lineTo(rect.width()/2 , rect.height()/2-7);
        path.lineTo(rect.width()/2+9.5 , rect.height()/2-3.5);
        path.lineTo(rect.width()/2, rect.height()/2-1);
        path.moveTo(rect.width()/2, rect.height()/2-3.5);
        path.lineTo(rect.width()/2-len, rect.height()/2-3.5);
    }

    if(type() == GPipe::RGS && getFlowDirection() == "right")
    {
        // Flowdirection right to p1
        path.setFillRule(Qt::WindingFill);
        path.moveTo(rect.width()/2, rect.height()/2-1);
        path.lineTo(rect.width()/2 , rect.height()/2-7);
        path.lineTo(rect.width()/2-9.5 , rect.height()/2-3.5);
        path.lineTo(rect.width()/2, rect.height()/2-1);
        path.moveTo(rect.width()/2, rect.height()/2-3.5);
        path.lineTo(rect.width()/2+len, rect.height()/2-3.5);
    }

    return path;
}

int GPipe::getFloorIndex() const
{
    return m_floorIndex;
}

void GPipe::setFloorIndex(int floorIndex)
{
    m_floorIndex = floorIndex;
}


QPointF GPipe::getP1()
{
    return mapToScene(p1);
}

QPointF GPipe::getP2()
{
    return mapToScene(p2);
}

QPointF GPipe::getPipeEnd()
{
    QPointF pos;

    if(type() == GPipe::RGA && getFlowDirection() == "up")
        pos = p2;

    if(type() == GPipe::RGA && getFlowDirection() == "down")
        pos = p1;

    if(type() == GPipe::RGS && getFlowDirection() == "left")
        pos = p2;

    if(type() == GPipe::RGS && getFlowDirection() == "right")
        pos = p1;

    if(type() == GPipe::STRANG && getFlowDirection() == "up")
        pos = p2;

    if(type() == GPipe::STRANG && getFlowDirection() == "down")
        pos = p1;

    return mapToScene( pos );
}

QPointF GPipe::getPipeStart()
{
    QPointF pos;

    if(type() == GPipe::RGA && getFlowDirection() == "up")
        pos = p1;

    if(type() == GPipe::RGA && getFlowDirection() == "down")
        pos = p2;

    if(type() == GPipe::RGS && getFlowDirection() == "left")
        pos = p1;

    if(type() == GPipe::RGS && getFlowDirection() == "right")
        pos = p2;

    if(type() == GPipe::STRANG && getFlowDirection() == "up")
        pos = p1;

    if(type() == GPipe::STRANG && getFlowDirection() == "down")
        pos = p2;

    return mapToScene( pos );
}

QPointF GPipe::getPipeMitte()
{
    return mapToScene( rect.center() );
}

int GPipe::getNr() const
{
    return m_nr;
}

void GPipe::setNr(int nr)
{
    m_nr = nr;
}

QPointF GPipe::getSPos() const
{
    return m_sPos;
}

void GPipe::setSPos(const QPointF &sPos)
{
    m_sPos = sPos;
}

double GPipe::getMeter() const
{
    return m_meter;
}

void GPipe::setMeter(double meter)
{
    m_meter = meter;
}

bool GPipe::activ() const
{
    return m_activ;
}

void GPipe::setActiv(bool activ)
{
    prepareGeometryChange();
    m_activ = activ;
}

void GPipe::appendGBadObject(GBadObject *o)
{
    // Only append when object not exist
    if(!objectList.contains(o))
        objectList.append(o);

    if(!objectNrList.contains(o->getNr()))
        objectNrList << o->getNr();
}

bool GPipe::deleteGBadObject(GBadObject *o)
{
    if(objectList.removeOne(o)){
        objectNrList.removeOne(o->getNr());
        return true;
    }
    else {
        return false;
    }
}

void GPipe::setGBadObjectList(QList<GBadObject *> list)
{
    objectList = list;
    foreach(GBadObject *o, list){
        if(!objectNrList.contains(o->getNr()))
            objectNrList << o->getNr();
    }
}

QList<GBadObject *> GPipe::badObjectList()
{
    return objectList;
}

bool GPipe::containsObjectType(const GBadObject::GBadObjectType &type)
{
    bool status = false;
    foreach(GBadObject *o, objectList){
        if(o->type() == type){
            status = true;
            break;
        }
    }

    return status;
}

int GPipe::getLu(int objNr)
{
    int lu = 0;
    foreach(GBadObject *o, objectList){
        if(o->getNr() == objNr){
            lu = o->lu();
            break;
        }
    }
    return lu;
}

void GPipe::insertFlow(int nr, double cold, double hot)
{
    QPair<double, double> p;
    p.first = cold;
    p.second = hot;
    objectFlowMap.insert(nr, p);
}

QMap<int, QPair<double, double> > GPipe::getObjectFlowMap()
{
    return objectFlowMap;
}

void GPipe::setObjectFlowMap(QMap<int, QPair<double, double> >map)
{
    objectFlowMap = map;
}

double GPipe::getFlow()
{
    double bd = 0.0;
    QMapIterator<int, QPair<double, double>>it(objectFlowMap);
    while (it.hasNext()) {
        it.next();
        bd += it.value().first;
        bd += it.value().second;
    }

    return bd;
}

QString GPipe::getTypeString()
{
    QString t = "";

    if(type() == GPipe::RGA)
        t = "Anschlussleitung";

    if(type() == GPipe::ABZEIG)
        t = "Abzweigleitung";

    if(type() == GPipe::RGS)
        t = "Stockwerksleitung";

    if(type() == GPipe::STRANG)
        t = "Strangleitung";

    if(type() == GPipe::ZIRK)
        t = "Zirkulationsleitung";

    return t;
}

QColor GPipe::color() const
{
    return m_color;
}

void GPipe::setColor(const QColor &color)
{
    m_color = color;
}

double GPipe::length() const
{
    return m_length;
}

void GPipe::setLength(double length)
{
    if(length != m_length){
        prepareGeometryChange();
        m_length = length;
        rect = QRectF(-10, -5, 20, m_length+10);
        p1 = QPointF(0,0);
        p2 = QPointF(0, m_length);
    }
}

bool GPipe::cold() const
{
    return m_cold;
}

void GPipe::setCold(bool cold)
{
    m_cold = cold;
}

bool GPipe::circulation() const
{
    return m_circulation;
}

void GPipe::setCirculation(bool circulation)
{
    m_circulation = circulation;
}

double GPipe::density() const
{
    return m_density;
}

void GPipe::setDensity(double density)
{
    m_density = density;
}

void GPipe::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;

    QPixmap ipix = QPixmap(":/buttonicons/info.svg");
    QPixmap cpix = QPixmap(":/buttonicons/garbage.svg");
    QPixmap mpix = QPixmap(":/buttonicons/move.png");
    QPixmap connectpix = QPixmap(":/buttonicons/pipe.svg");

    QAction *infoAction = menu.addAction(QIcon(ipix),tr("info"));
    QAction *deleteAction = menu.addAction(QIcon(cpix), tr("delete"));
    connect(infoAction, &QAction::triggered, this, &GPipe::infoAction);
    connect(deleteAction, &QAction::triggered, this, &GPipe::deleteAction);

    if(!getMarker()){
        QAction *connectAction = menu.addAction(QIcon(connectpix),tr("connect"));
        connect(connectAction, &QAction::triggered, this, &GPipe::connectAction);
    }

    if(getMarker()){
        QAction *moveAction = menu.addAction(QIcon(mpix),tr("move"));
        connect(moveAction, &QAction::triggered, this, &GPipe::moveAction);

    }

    menu.exec( event->screenPos() );
}

void GPipe::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *)
{
     emit pipeInfo(this);
}

void GPipe::infoAction()
{
    emit pipeInfo(this);
}

void GPipe::deleteAction()
{
    emit pipeHasDeleted(this);
}

void GPipe::moveAction()
{
    emit pipeMove(this);
}

void GPipe::connectAction()
{
    emit pipeConnect(this);
}

void GPipe::updateBlinkingColor()
{
    m_blinkState = ++m_blinkState % 2;
    if(cold())
        setColor( (m_blinkState) ? Qt::darkGreen : Qt::cyan );
    if(!cold())
        setColor( (m_blinkState) ? Qt::red : Qt::cyan );

    update();
}


QDataStream &operator <<(QDataStream &out, const GPipe &p)
{
    out << p.type() << p.scenePos() << p.getNr() << p.getStrangMarkerNr() << p.getMeter() << p.activ() <<
           p.cold() << p.circulation() << p.density() << p.length() << p.color() << p.getMaterial() <<
           p.getFloorIndex() << p.getFlowDirection() << p.getMarker() << p.getObjectNrList();

    return out;
}

QDataStream &operator >>(QDataStream &in, GPipe &p)
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

    p.setSPos(sp);
    p.setNr(nr);
    p.setStrangMarkerNr(strangNr);
    p.setMeter(meter);
    p.setActiv(activ);
    p.setCold(cold);
    p.setCirculation(circ);
    p.setDensity(density);
    p.setLength(len);
    p.setColor(col);
    p.setMaterial(mat);
    p.setFloorIndex(findex);
    p.setFlowDirection(direction);
    p.setMarker(marker);
    p.setObjectNrList( nrList );

    return in;

}
