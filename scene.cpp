#include "scene.h"

#include <QPainter>

#include <QDebug>

Scene::Scene(QObject *parent) :
    QGraphicsScene(parent)
{


}

QRectF Scene::getSceneRect()
{
    return  sceneRect();
}


void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    emit mousePos(event->scenePos());
    QGraphicsScene::mouseMoveEvent(event);
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        emit leftMouseButtonClicked(event->scenePos());

    if(event->button() == Qt::RightButton)
        emit rightMouseButtonClicked(event->scenePos());

    if(event->button() == Qt::MidButton)
        emit middleMouseButtonClicked(event->scenePos());

    QGraphicsScene::mousePressEvent(event);

}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        emit leftMouseButtonReleased(event->screenPos());

    QGraphicsScene::mouseReleaseEvent(event);

}

void Scene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        leftDoubleMouseButtonClicked(event->scenePos());

    QGraphicsScene::mouseDoubleClickEvent(event);
}

