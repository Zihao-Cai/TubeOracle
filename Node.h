#pragma once
#include <qgraphicsitem.h>
#include <qpainter.h>
#include <qpainterpath.h>
#include <qgraphicssceneevent.h>
#include <qtmath>
#include "config.h"

class Node : public QGraphicsObject
{
	Q_OBJECT
public:
    size_t tag;
    double nx;
    double ny;
    bool isselected = false;
    QWidget* par = nullptr;
    Node(double x, double y, int tag, QWidget* parent);
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event);

signals:
    void nodeselected(Node* node);


};

