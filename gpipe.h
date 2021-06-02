#ifndef GPIPE_H
#define GPIPE_H

#include <QObject>
#include <QGraphicsObject>
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QGraphicsSvgItem>
#include <QPainterPath>

#include <QMenu>
#include <QAction>
#include <QPixmap>
#include <QTimer>

#include <gbadobject.h>
#include <garmatur.h>

class GPipe : public QGraphicsObject
{
    Q_OBJECT

public:
    int type() const Q_DECL_OVERRIDE;

    enum GPipeType
    {
        RGS =   1 + UserType,    // Rohr horizontal z. B. Stockwerksleitung
        RGA =   2 + UserType,    // Rohr vertikal z. B. Anschlussleitung
        ABZEIG = 3 + UserType,   // Rohr vertikal Abzweigleitung
        STRANG = 4 + UserType,   // Strangleitung
        ZIRK = 5   + UserType,   // Zirkulationsleitung
    };

    GPipe(GPipeType TYPE, bool COLD, double LENGTH, QGraphicsItem *parent = nullptr);

    virtual QRectF boundingRect() const Q_DECL_OVERRIDE;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget) Q_DECL_OVERRIDE;


    QRectF sceneRect();

    QColor color() const;
    void setColor(const QColor &color);

    double length() const;
    void setLength(double length);

    bool cold() const;
    void setCold(bool cold);

    bool circulation() const;
    void setCirculation(bool circulation);

    double density() const;
    void setDensity(double density);

    bool activ() const;
    void setActiv(bool activ);


    void appendGBadObject(GBadObject *o);
    bool deleteGBadObject(GBadObject *o);
    void setGBadObjectList(QList<GBadObject *> list);
    QList<GBadObject *> badObjectList();
    bool containsObjectType(const GBadObject::GBadObjectType &type);

    QString getTypeString();

    double getMeter() const;
    void setMeter(double meter);

    QPointF getSPos() const;
    void setSPos(const QPointF &sPos);

    int getNr() const;
    void setNr(int nr);


    QPointF getP1();
    QPointF getP2();

    QPointF getPipeEnd();
    QPointF getPipeStart();
    QPointF getPipeMitte();


    int getFloorIndex() const;
    void setFloorIndex(int floorIndex);

    QString getFlowDirection() const;
    void setFlowDirection(const QString &flowDirection);

    bool getShowArrow() const;
    void setShowArrow(bool showArrow);

    bool containsWSIB();

    bool getMarker() const;
    void setMarker(bool marker);

    qreal getBdKW() const;
    qreal getBdWW() const;

    bool getReadToConnect() const;
    void setReadToConnect(bool readToConnect);

    QList<int> getObjectNrList() const;
    void setObjectNrList(const QList<int> &value);

    QVariant region();

    int getStrangMarkerNr() const;
    void setStrangMarkerNr(int strangMarkerNr);

    bool getShowNr() const;
    void setShowNr(bool showNr);

    QString getMaterial() const;
    void setMaterial(const QString &material);

    double getFlowSpeed() const;
    void setFlowSpeed(double flowSpeed);

public slots:

    // for contex menu
    void infoAction();
    void deleteAction();
    void moveAction();
    void connectAction();

    void updateBlinkingColor();

signals:
    void pipeHasDeleted(GPipe *pipe);
    void pipeInfo(GPipe *pipe);
    void pipeMove(GPipe *pipe);
    void pipeIsSelected(GPipe *pipe);
    void pipeConnect(GPipe *pipe);


protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) Q_DECL_OVERRIDE;
    virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;

private:

    int Type;
    QRectF rect;                // bounding Rect
    QPointF m_sPos;             // Position in scene

    int m_nr;
    int m_strangMarkerNr;
    double m_meter;
    bool m_activ;
    bool m_cold;
    bool m_circulation;
    double m_density;
    double m_length;
    double m_flowSpeed;
    QColor m_color;
    QString m_material;

    int m_floorIndex;
    QString m_flowDirection;
    bool m_showArrow;
    bool m_marker;
    bool m_readToConnect;
    bool m_showNr;

    QTimer *timer;
    int m_blinkState;

    QPointF p1;     // Point 1 for drawing
    QPointF p2;     // Point 2 for drawing

    QList<GBadObject *> objectList;
    QList<int> objectNrList;

    QPainterPath arrow();


};

QDataStream & operator << (QDataStream& out, const GPipe& p);
QDataStream & operator >> (QDataStream& in, GPipe& p);

Q_DECLARE_METATYPE(GPipe *)
Q_DECLARE_OPAQUE_POINTER(GPipe *)

#endif // GPIPE_H
