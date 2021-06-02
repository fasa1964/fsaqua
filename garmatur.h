#ifndef GARMATUR_H
#define GARMATUR_H

#include <QObject>
#include <QGraphicsObject>
#include <QGraphicsItem>
#include <QGraphicsSvgItem>
#include <QDataStream>
#include <QAction>
#include <QMenu>
#include <QRegion>
#include <QTimer>

#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneMouseEvent>



class GArmatur : public QGraphicsObject
{

    Q_OBJECT
    Q_ENUMS(GAramaturType)

public:

    int type() const   Q_DECL_OVERRIDE;

    enum GArmaturType
    {
        SV      = 20 + UserType,    // Schrägsitzventil
        KH      = 21 + UserType,    // Kugelhahn
        GV      = 22 + UserType,    // Geradsitzventil
        SVRV    = 23 + UserType,    // Schrägsitzventil m. Rückflussverhinderer
        DM      = 24 + UserType,    // Druckminderer
        FIL     = 25 + UserType,    // Filter
        RV      = 26 + UserType,    // Rückflussverhinderer
        WZ      = 27 + UserType,    // Wasserzähler
        FSVE    = 28 + UserType,    // Federbelastetes Sicherheitsventil
        SP      = 29 + UserType,    // Probeentnahmestelle
        SVTE    = 35 + UserType,    // Sicherheitsventil, Temperaturablassventil

        W_CHD   = 30 + UserType,    // Dosiergerät
        W_SOF   = 31 + UserType,    // Enthärtungsanlage
        W_ACF   = 32 + UserType,    // Aktivkohlefilter
        W_EDS   = 33 + UserType,    // Elektrolytisches Gerät
        W_UV    = 34 + UserType,    // Desinfiktionsanlage UV
    };

    GArmatur(GArmaturType TYPE, QGraphicsItem *parent = nullptr);

    virtual QRectF boundingRect() const Q_DECL_OVERRIDE;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget) Q_DECL_OVERRIDE;


    QString kurzBezeichnung() const;
    void setKurzBezeichnung(const QString &kurzBezeichnung);

    QString name() const;
    void setName(const QString &name);

    QPointF sPos() const;
    void setSPos(const QPointF &sPos);

    QPixmap pixmap() const;
    void setPixmap(const QPixmap &pixmap);

    QPointF mitte() const;

    QString symbolFilename() const;
    void setSymbolFilename(const QString &symbolFilename);

    int nr() const;
    void setNr(int nr);

    int pipeNr() const;
    void setPipeNr(int pipeNr);

    double pg() const;
    void setPg(const qreal &pg);

    double vg() const;
    void setVg(const qreal &vg);

    double dichte() const;
    void setDichte(const qreal &dichte);

    double flowSpeed() const;
    void setFlowSpeed(const qreal &flowSpeed);

    int dn() const;
    void setDn(int dn);

    bool showStatus() const;
    void setShowStatus(bool showStatus);

    double getPessureLoss() const;
    void setPessureLoss(double pessureLoss);

    bool getEntleerung() const;
    void setEntleerung(bool entleerung);

    double getZeta() const;
    void setZeta(double zeta);

    double getPeakFlow() const;
    void setPeakFlow(double peakFlow);

public slots:
    void infoAction();
    void deleteAction();

    void updateBlinkingColor();

protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;


signals:
    void leftMouseButtonPressed(QPointF scenePos, GArmatur *arm);
    void armaturInfo(GArmatur *arm);
    void armaturHasDeleted(GArmatur *arm);

private:
    int Type;

    QRectF rect;
    QString m_symbolFilename;           // path of image

    QGraphicsTextItem *dnText;          // DN im Text
    QGraphicsSvgItem *svgItem;          // Grafisches Symbol svg
    QGraphicsSvgItem *svgEntleerung;    // Grafisches Symbol svg
    QPixmap m_pixmap;                   // Grafisches Symbol png
    QPointF m_sPos;                     // Scene Position

    int m_nr;                           // Number of armatur
    int m_pipeNr;                       // Installed pipe nr
    QString m_name;                     // Ventil/Armatur Name
    QString m_kurzBezeichnung;          // Kurzbezeichnung

    int m_dn;                           // DN of armatur
    double m_pessureLoss;               // Druckverlust an der Armatur
    double m_pg;                        // Hersteller Druckverlust bei Betriebspunkt in hPa
    double m_vg;                        // Duchfluss für einen Bestimmten Betriebsbspunkt
    double m_dichte;                    // Dichte des Mediums
    double m_flowSpeed;                 // aktuelle Strömungsgeschwindigkeit
    bool m_entleerung;
    double m_zeta;
    double m_peakFlow;


    QTime *timer;
    int m_blinkState;
    bool m_showStatus;
    QColor getStatusColor();
    QPainterPath entleerung();

};

Q_DECLARE_METATYPE(GArmatur *)
Q_DECLARE_OPAQUE_POINTER(GArmatur *)

#endif // GARMATUR_H
