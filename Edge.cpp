#include "Edge.h"

Edge::Edge(Node* n1, Node* n2, QWidget* parent)
{
	this->n1 = n1;
	this->n2 = n2;
	par = parent;
	setAcceptHoverEvents(true);
}


QRectF Edge::boundingRect() const
{
	return QRectF();
}

QPainterPath Edge::shape() const
{
	QPainterPathStroker stroker;
	stroker.setWidth(edgewidth);
	QPainterPath path;
	path.moveTo(n1->nx* geoscale, n1->ny* geoscale);
	path.lineTo(n2->nx* geoscale, n2->ny* geoscale);
	return stroker.createStroke(path);
}

void Edge::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->setPen(QPen(QColor(0, 255, 192), edgewidth));
	painter->drawLine(n1->nx* geoscale, n1->ny* geoscale, n2->nx* geoscale, n2->ny* geoscale);
}

void Edge::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
	setToolTip("Edge{" + QString::number(n1->tag) + "," + QString::number(n2->tag) + "}");
}

void Edge::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
	setToolTip("");
}
