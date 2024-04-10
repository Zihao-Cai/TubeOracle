#pragma once
#include "Node.h"
#include "config.h"

class Edge : public QGraphicsObject
{
	Q_OBJECT
public:
    Node* n1 = nullptr;
    Node* n2 = nullptr;
    QWidget* par = nullptr;
    Edge(Node* n1, Node* n2, QWidget* parent);
    QRectF boundingRect() const override;
    QPainterPath shape()const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

};

