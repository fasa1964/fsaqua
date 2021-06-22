#ifndef GBADOBJECT_H
#define GBADOBJECT_H

#include <QObject>
#include <QGraphicsObject>
#include <QGraphicsItem>
#include <QGraphicsSvgItem>
#include <QDataStream>
#include <QAction>
#include <QMenu>
#include <QRegion>


#include <QGraphicsSceneContextMenuEvent>


class GBadObject : public QGraphicsObject
{
    Q_OBJECT
    Q_ENUMS(GBadObjectType)

public:
    int type() const    Q_DECL_OVERRIDE;

    enum GBadObjectType
    {
        WT = 1 + UserType,          // Waschtisch
        WC = 2 + UserType,          // WC
        DU = 3 + UserType,          // Dusche
        WA = 4 + UserType,          // Badewanne
        SP = 5 + UserType,          // Küchenspüle
        WM = 6 + UserType,          // Waschmaschine
        GS = 7 + UserType,          // Geschirrspülmaschine
        WCDS = 8 + UserType,        // WC mit Druckspüler
        UR = 10 + UserType,         // Druckspüler für Urinalbecken
        SW = 11 + UserType,         // Sitzwaschbecken
        AGWB = 12 + UserType,       // Ausgussbecken mit Wandmischbatterie
        AGAV = 13 + UserType,       // Ausgussbecken mit Auslaufventil DN 15
        AV = 14 + UserType,         // Wandauslaufventil DN ?

        WSIB = 9 + UserType,        // Warmwasserspeicher indirekt beheizt
        DHE = 15 + UserType,        // Durchlauferhitzer

    };

    GBadObject(GBadObjectType TYPE, QGraphicsItem *parent = nullptr);
    GBadObject(const QString &typstr, QGraphicsItem *parent = nullptr);
    GBadObject(QGraphicsItem *parent = nullptr);
    GBadObject(const GBadObject &other, QGraphicsItem *parent = nullptr );


    virtual QRectF boundingRect() const Q_DECL_OVERRIDE;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget) Q_DECL_OVERRIDE;



    QString bezeichnung() const;
    void setBezeichnung(const QString &bezeichnung);

    QString symbolFilename() const;
    void setSymbolFilename(const QString &symbolFilename);

    qreal din_bdKW() const;
    void setDin_bdKW(const qreal &din_bdKW);

    qreal din_bdWW() const;
    void setDin_bdWW(const qreal &din_bdWW);

    qreal bdKW() const;
    void setBdKW(const qreal &bdKW);

    qreal bdWW() const;
    void setBdWW(const qreal &bdWW);

    qreal din_md() const;
    void setDin_md(const qreal &din_md);

    qreal md() const;
    void setMd(const qreal &md);

    int en_lu() const;
    void setEn_lu(int en_lu);

    int lu() const;
    void setLu(int lu);

    int dn() const;
    void setDn(int dn);

    QString fabrikat() const;
    void setFabrikat(const QString &fabrikat);

    QPointF sPos() const;
    void setSPos(const QPointF &sPos);

    qreal kwh() const;
    void setKwh(const qreal &kwh);

    QGraphicsSvgItem *getSvgItem() const;
    void setSvgItem(QGraphicsSvgItem *value);

    int getFloorIndex() const;
    void setFloorIndex(int foorIndex);

    bool getDauerlaeufer() const;
    void setDauerlaeufer(bool dauerlaeufer);

    QPointF connectCold();
    QPointF connectHot();

    QPointF getMitte();
    QPointF mitte();
    QVariant region();

    int getPipeNr() const;
    void setPipeNr(int pipeNr);

    bool getCold() const;
    void setCold(bool cold);

    bool getHot() const;
    void setHot(bool hot);

    int getNr() const;
    void setNr(int nr);

    bool displayNr() const;
    void setDisplayNr(bool displayNr);

    QPointF leftPos();
    QPointF rightPos();

    qreal getAnschlussHoehe() const;
    void setAnschlussHoehe(const qreal &anschlussHoehe);

signals:
    void objectHasDeleted(GBadObject *obj);
    void objectInfo(GBadObject *obj);

public slots:
    // for contex menu
    void infoAction();
    void deleteAction();


protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) Q_DECL_OVERRIDE;

private:
    int Type;

    QRectF rect;                // bounding Rect
    QGraphicsSvgItem *svgItem;  // Grafisches Symbol svg
    QString m_symbolFilename;   // path of image
    QPointF m_sPos;             // Position in scene

    // Values for object
    qreal m_din_bdKW;           // Berechnungsdurchfluss Kaltwasser nach DIN
    qreal m_din_bdWW;           // Berechnungsdurchfluss Warmwasser nach DIN
    qreal m_din_md;             // Mindestfließdruck nach DIN
    qreal m_anschlussHoehe;     // Anschlusshöhe in Meter

    qreal m_bdKW;               // Berechnungsdurchfluss Kaltwasser
    qreal m_bdWW;               // Berechnungsdurchfluss Warmwasser
    qreal m_md;                 // Mindestfließdruck in hPa
    qreal m_kwh;                // Wärmebedarf

    int m_en_lu;                // Belastungswert nach EN806
    int m_lu;                   // Belastungswert

    int m_dn;                   // Anschluss-Nennweite
    int m_floorIndex;           // Index of floor
    bool m_dauerlaeufer;        // Dauerläufer
    bool m_cold;                // Has cold water connection
    bool m_hot;                 // Has hot water connection

    QString m_bezeichnung;      // Description
    QString m_fabrikat;         // Type, Manufakturing


    bool m_displayNr;           // Status for showing the nr of object

    int m_pipeNr;               // The nr of connecting pipe
    int m_nr;                   // Object nr

};

QDataStream & operator << (QDataStream& out, const GBadObject& obj);
QDataStream & operator >> (QDataStream& in, GBadObject& obj);
//QDataStream & operator << (QDataStream& out, const QList<GBadObject *> &objList);
//QDataStream & operator >> (QDataStream& in, QList<GBadObject *> &objList);

Q_DECLARE_METATYPE(GBadObject *)
Q_DECLARE_OPAQUE_POINTER(GBadObject *)

#endif // GOBJECT_H
