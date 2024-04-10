#pragma once
#include "ItemPoint.h"
#include "config.h"

class ItemCurve : public QGraphicsObject
{
    Q_OBJECT
public:
    int tag;
    QList<int> path;
    ItemPoint* ps = nullptr;
    ItemPoint* pc = nullptr;
    ItemPoint* pe = nullptr;
    QWidget* par = nullptr;
    qreal radius;
    qreal startAngle;
    qreal span;
    double top, left, bottom, right;
    bool isselected;
    ItemCurve(ItemPoint* start, ItemPoint* center, ItemPoint* end, int tag, QWidget* parent);
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    QPainterPath shape()const override;
    QList<double> locateX(double y);
    QList<double> locateY(double x);

signals:
    void curveselected(QGraphicsObject* curve);
private:
    void calculate_path();

};

