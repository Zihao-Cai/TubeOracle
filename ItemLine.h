#pragma once
#include "ItemPoint.h"
#include "config.h"

class ItemLine : public QGraphicsObject
{
    Q_OBJECT
public:
    int tag;
    bool isselected;
    ItemPoint* p1 = nullptr;
    ItemPoint* p2 = nullptr;
    QList<int> path;
    QWidget* par = nullptr;
    double top, left, bottom, right;
    ItemLine(ItemPoint* start, ItemPoint* end, int tag, QWidget* parent);
    QRectF boundingRect() const override;
    QPainterPath shape()const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    double locateX(double y);
    double locateY(double x);

signals:
    void lineselected(QGraphicsObject* line);
private:


};

