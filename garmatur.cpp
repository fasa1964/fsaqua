#include "garmatur.h"

#include <QPainter>
#include <QPen>

int GArmatur::type() const
{
    return Type;
}

GArmatur::GArmatur(GArmaturType TYPE, QGraphicsItem *parent):
    QGraphicsObject (parent)
{

    Type = TYPE;
    rect = QRectF(-16,-16,32,32);

    if(Type == KH)
    {
        svgItem = new QGraphicsSvgItem(":/valves/armKH.svg", this);
        setPixmap( QPixmap(":/valves/armKH.png").scaled(QSize(32,32), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        setName( tr("ball valve") );
        setKurzBezeichnung("KH");
        setSymbolFilename(":/valves/armKH.svg");
    }

    if(Type == SV)
    {
        svgItem = new QGraphicsSvgItem(":/valves/armSV.svg", this);
        setPixmap( QPixmap(":/valves/armSV.png").scaled(QSize(32,32), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        setName( tr("angle seat valve") );
        setKurzBezeichnung("SV");
        setSymbolFilename(":/valves/armSV.svg");
    }

       if(Type == GV)
       {
           svgItem = new QGraphicsSvgItem(":/valves/armGV.svg", this);
           setPixmap( QPixmap(":/valves/armGV.png").scaled(QSize(32,32), Qt::KeepAspectRatio, Qt::SmoothTransformation));
           setName( tr("straight seat valve") );
           setKurzBezeichnung("GV");
           setSymbolFilename(":/valves/armGV.svg");
       }

       if(Type == SVTE)
       {
           svgItem = new QGraphicsSvgItem(":/valves/armSVTE.svg", this);
           setPixmap( QPixmap(":/valves/armSVTE.png").scaled(QSize(32,32), Qt::KeepAspectRatio, Qt::SmoothTransformation));
           setName( tr("safety valve, temperature controlled") );
           setKurzBezeichnung("SVTE");
           setSymbolFilename(":/valves/armSVTE.svg");
       }

       if(Type == FIL)
       {
           svgItem = new QGraphicsSvgItem(":/valves/armFIL.svg", this);
           setPixmap( QPixmap(":/valves/armFIL.png").scaled(QSize(32,32), Qt::KeepAspectRatio, Qt::SmoothTransformation));
           setName( tr("mechanical waterfilter") );
           setKurzBezeichnung("FIL");
           setSymbolFilename(":/valves/armFIL.svg");
       }

       if(Type == DM)
       {
           svgItem = new QGraphicsSvgItem(":/valves/armDM.svg", this);
           setPixmap( QPixmap(":/valves/armDM.png").scaled(QSize(32,32), Qt::KeepAspectRatio, Qt::SmoothTransformation));
           setName( tr("pressure reduce valve") );
           setKurzBezeichnung("DM");
           setSymbolFilename(":/valves/armDM.svg");
       }

       if(Type == SVRV)
       {
           svgItem = new QGraphicsSvgItem(":/valves/armABRV.svg", this);
           setPixmap( QPixmap(":/valves/armABRV.png").scaled(QSize(32,32), Qt::KeepAspectRatio, Qt::SmoothTransformation));
           setName( tr("angle seat valve with backflow preventer") );
           setKurzBezeichnung("SVRV");
           setSymbolFilename(":/valves/armABRV.svg");
       }

       if(Type == RV)
       {
           svgItem = new QGraphicsSvgItem(":/valves/armRV.svg", this);
           setPixmap( QPixmap(":/valves/armRV.png").scaled(QSize(32,32), Qt::KeepAspectRatio, Qt::SmoothTransformation));
           setName( tr("backflow preventer valve") );
           setKurzBezeichnung("RV");
           setSymbolFilename(":/valves/armRV.svg");

       }

       if(Type == WZ)
       {
           svgItem = new QGraphicsSvgItem(":/valves/armWZ.svg", this);
           setPixmap( QPixmap(":/valves/armWZ.png").scaled(QSize(32,32), Qt::KeepAspectRatio, Qt::SmoothTransformation));
           setName( tr("water meter") );
           setKurzBezeichnung("WZ");
           setSymbolFilename(":/valves/armWZ.svg");
       }

       if(Type == SP)
       {
           svgItem = new QGraphicsSvgItem(":/valves/armProbe.svg", this);
           setPixmap( QPixmap(":/valves/armProbe.png").scaled(QSize(32,32), Qt::KeepAspectRatio, Qt::SmoothTransformation));
           setName( tr("sample point") );
           setKurzBezeichnung("SP");
           setSymbolFilename(":/valves/armProbe.svg");
       }

       if(Type == FSVE)
       {
           rect = QRectF(-17.5,-17.5 ,35,35);
           svgItem = new QGraphicsSvgItem(":/valves/armFSVE.svg", this);
           setPixmap( QPixmap(":/valves/armFSVE.png").scaled(QSize(32,32), Qt::KeepAspectRatio, Qt::SmoothTransformation) );
           QPointF p = svgItem->boundingRect().center();
           svgItem->setPos(mitte() - p);
           setName( tr("springloaded safety valve") );
           setKurzBezeichnung("FSV");
           setSymbolFilename(":/valves/armFSVE.svg");
       }

       if(Type == W_CHD)
       {
           rect = QRectF(-17.5,-17.5 ,35,35);
           svgItem = new QGraphicsSvgItem(":/valves/armDosier.svg", this);
           setPixmap( QPixmap(":/valves/armDosier.png").scaled(QSize(32,32), Qt::KeepAspectRatio, Qt::SmoothTransformation) );
           QPointF p = svgItem->boundingRect().center();
           svgItem->setPos(mitte() - p);
           setName( tr("dosing device") );
           setKurzBezeichnung("CHD");
           setSymbolFilename(":/valves/armDosier.svg");
       }

       if(Type == W_SOF)
       {
           rect = QRectF(-17.5,-17.5 ,35,35);
           svgItem = new QGraphicsSvgItem(":/valves/armEnthaertung.svg", this);
           setPixmap( QPixmap(":/valves/armEnthaertung.png").scaled(QSize(32,32), Qt::KeepAspectRatio, Qt::SmoothTransformation) );
           QPointF p = svgItem->boundingRect().center();
           svgItem->setPos(mitte() - p);
           setName( tr("softening device") );
           setKurzBezeichnung("SOF");
           setSymbolFilename(":/valves/armSOF.svg");
       }

       if(Type == W_ACF)
       {
           rect = QRectF(-17.5,-17.5 ,35,35);
           svgItem = new QGraphicsSvgItem(":/valves/armKohleFilter.svg", this);
           setPixmap( QPixmap(":/valves/armKohleFilter.png").scaled(QSize(32,32), Qt::KeepAspectRatio, Qt::SmoothTransformation) );
           QPointF p = svgItem->boundingRect().center();
           svgItem->setPos(mitte() - p);
           setName( tr("carbon filter") );
           setKurzBezeichnung("ACF");
           setSymbolFilename(":/valves/armKohleFilter.svg");
       }

       if(Type == W_EDS)
       {
           rect = QRectF(-17.5,-17.5 ,35,35);
           svgItem = new QGraphicsSvgItem(":/valves/armElektro.svg", this);
           setPixmap( QPixmap(":/valves/armElektro.png").scaled(QSize(32,32), Qt::KeepAspectRatio, Qt::SmoothTransformation) );
           QPointF p = svgItem->boundingRect().center();
           svgItem->setPos(mitte() - p);
           setName( tr("electrolytical device") );
           setKurzBezeichnung("EDS");
           setSymbolFilename(":/valves/armElektro.svg");
       }

       if(Type == W_UV)
       {
           rect = QRectF(-17.5,-17.5 ,35,35);
           svgItem = new QGraphicsSvgItem(":/valves/armDesinfektion.svg", this);
           setPixmap( QPixmap(":/valves/armDesinfektion.png").scaled(QSize(32,32), Qt::KeepAspectRatio, Qt::SmoothTransformation) );
           QPointF p = svgItem->boundingRect().center();
           svgItem->setPos(mitte() - p);
           setName( tr("disinfection device") );
           setKurzBezeichnung("UV");
           setSymbolFilename(":/valves/armDesinfektion.svg");
       }


       QPointF p = svgItem->boundingRect().center();
       svgItem->setPos(mitte()-p);
       setScale(0.8);
       setZValue(5);
       setToolTip( name() );

       // Values of armatur
       setPg(0.0);
       setVg(0.0);
       setFlowSpeed(0.0);
       setDichte(0.0);
       setDn(15);
       setShowStatus(true);
       setPessureLoss(0.0);
       setEntleerung(false);

       setFlag(QGraphicsItem::ItemIsSelectable);
       setAcceptedMouseButtons(Qt::LeftButton);

       QTransform trans;
       trans.rotate(180, Qt::XAxis);
       setTransform(trans);
}

QRectF GArmatur::boundingRect() const
{
    return rect;
}

void GArmatur::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing, true);

    QPen pen;
    pen.setColor(QColor(0,87,125));
    pen.setWidthF(1.0);
    painter->setPen(pen);

    if(isSelected())
        painter->drawRect(rect);

    if(showStatus() && Type != GArmaturType::FSVE){
        pen.setWidthF(3.8);
        pen.setColor( getStatusColor() );
        painter->setPen(pen);
        painter->drawRoundedRect(rect.bottomLeft().x()+4, rect.bottomLeft().y() - 6, 4,4,4,4);
    }

    if(getEntleerung() && Type != GArmaturType::FSVE){
        pen.setWidthF(1);
        pen.setColor( Qt::black );
        painter->setPen(pen);
        painter->drawPath(entleerung());
    }

}

QString GArmatur::kurzBezeichnung() const
{
    return m_kurzBezeichnung;
}

void GArmatur::setKurzBezeichnung(const QString &kurzBezeichnung)
{
    m_kurzBezeichnung = kurzBezeichnung;
}

QString GArmatur::name() const
{
    return m_name;
}

void GArmatur::setName(const QString &name)
{
    m_name = name;
}

QPointF GArmatur::sPos() const
{
    return m_sPos;
}

void GArmatur::setSPos(const QPointF &sPos)
{
    m_sPos = sPos;
}

QPixmap GArmatur::pixmap() const
{
    return m_pixmap;
}

void GArmatur::setPixmap(const QPixmap &pixmap)
{
    m_pixmap = pixmap;
}

QPointF GArmatur::mitte() const
{
    return mapToScene( rect.center() );
}

QString GArmatur::symbolFilename() const
{
    return m_symbolFilename;
}

void GArmatur::setSymbolFilename(const QString &symbolFilename)
{
    m_symbolFilename = symbolFilename;
}

int GArmatur::nr() const
{
    return m_nr;
}

void GArmatur::setNr(int nr)
{
    m_nr = nr;
}

void GArmatur::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        emit leftMouseButtonPressed(event->scenePos(), this);

    QGraphicsItem::mousePressEvent(event);
}

void GArmatur::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *)
{
    emit armaturInfo(this);
}

double GArmatur::getPeakFlow() const
{
    return m_peakFlow;
}

void GArmatur::setPeakFlow(double peakFlow)
{
    m_peakFlow = peakFlow;
}

double GArmatur::getZeta() const
{
    return m_zeta;
}

void GArmatur::setZeta(double zeta)
{
    m_zeta = zeta;
}

bool GArmatur::getEntleerung() const
{
    return m_entleerung;
}

void GArmatur::setEntleerung(bool entleerung)
{
    m_entleerung = entleerung;
}

double GArmatur::getPessureLoss() const
{
    return m_pessureLoss;
}

void GArmatur::setPessureLoss(double pessureLoss)
{
    m_pessureLoss = pessureLoss;
}

QColor GArmatur::getStatusColor()
{
    QColor color = Qt::red;

    if(getPessureLoss() > 0.0)
        color = Qt::green;

    return color;
}

QPainterPath GArmatur::entleerung()
{
    QPainterPath path;
    path.moveTo( rect.right()-2, rect.bottom()-6 );
    path.lineTo(rect.right()-2, rect.bottom());
    path.moveTo( rect.right()-3.5, rect.bottom()-3);
    path.lineTo(rect.right(), rect.bottom()-3);

    return path;
}

bool GArmatur::showStatus() const
{
    return m_showStatus;
}

void GArmatur::setShowStatus(bool showStatus)
{
    m_showStatus = showStatus;
}

int GArmatur::dn() const
{
    return m_dn;
}

void GArmatur::setDn(int dn)
{
    m_dn = dn;
}

double GArmatur::flowSpeed() const
{
    return m_flowSpeed;
}

void GArmatur::setFlowSpeed(const qreal &flowSpeed)
{
    m_flowSpeed = flowSpeed;
}

double GArmatur::dichte() const
{
    return m_dichte;
}

void GArmatur::setDichte(const qreal &dichte)
{
    m_dichte = dichte;
}

double GArmatur::vg() const
{
    return m_vg;
}

void GArmatur::setVg(const qreal &vg)
{
    m_vg = vg;
}

double GArmatur::pg() const
{
    return m_pg;
}

void GArmatur::setPg(const qreal &pg)
{
    m_pg = pg;
}

int GArmatur::pipeNr() const
{
    return m_pipeNr;
}

void GArmatur::setPipeNr(int pipeNr)
{
    m_pipeNr = pipeNr;
}

void GArmatur::infoAction()
{
    emit armaturInfo(this);
}

void GArmatur::deleteAction()
{
    emit armaturHasDeleted(this);
}

void GArmatur::updateBlinkingColor()
{

}

void GArmatur::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;

    QPixmap ipix = QPixmap(":/buttonicons/info.svg");
    QPixmap cpix = QPixmap(":/buttonicons/garbage.svg");

    QAction *infoAction = menu.addAction(QIcon(ipix),tr("info"));
    QAction *deleteAction = menu.addAction(QIcon(cpix), tr("delete"));
    connect(infoAction, &QAction::triggered, this, &GArmatur::infoAction);
    connect(deleteAction, &QAction::triggered, this, &GArmatur::deleteAction);

    menu.exec( event->screenPos() );
}
