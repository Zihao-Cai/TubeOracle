#pragma once
#include <qgraphicsitem.h>
#include <qpainter.h>
#include <qpainterpath.h>
#include <qgraphicssceneevent.h>
#include <qtmath>
#include "config.h"


class ItemPoint : public QGraphicsObject
{
    Q_OBJECT
public:
    int tag;
    double px;
    double py;
    bool isselected = false;
    QWidget* par = nullptr;
    ItemPoint(double x, double y, int tag,QWidget* parent);
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
signals:
    void pntselected(ItemPoint* point);
private:


};


