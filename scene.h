#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QGraphicsObject>
#include <QGraphicsSceneEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>

class Scene : public QGraphicsScene
{
    Q_OBJECT

public:
    Scene(QObject *parent = 0);

    QRectF getSceneRect();

protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);


signals:
    void mousePos(QPointF pos);
    void leftMouseButtonClicked(QPointF pos);
    void leftMouseButtonReleased(QPointF pos);
    void rightMouseButtonClicked(QPointF pos);
    void middleMouseButtonClicked(QPointF pos);

private:


};

#endif // SCENE_H
