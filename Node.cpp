#include "Node.h"


Node::Node(double x, double y, int tag, QWidget* parent)
{
	nx = x;
	ny = y;
	this->tag = tag;
	par = parent;
	setAcceptHoverEvents(true);
	connect(this, SIGNAL(nodeselected(Node*)), (QObject*)(par), SLOT(on_selNodes(Node*)));
}

QRectF Node::boundingRect() const
{
	return QRectF(nx * geoscale - nodesize, ny * geoscale - nodesize, nodesize * 2, nodesize * 2);
}

void Node::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{

	painter->setBrush(QColor(int(1 * 255), int(0.471 * 255), int(0 * 255)));
	painter->setPen(QPen(QColor(int(1 * 255), int(0.471 * 255), int(0 * 255)), nodesize));
	if (isselected) {
		painter->setPen(QPen(Qt::red, selectnodesize));
	}
	painter->drawPoint(nx* geoscale, ny* geoscale);
}

void Node::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
	setToolTip("Node(" + QString::number(tag) + "){" + QString::number(nx) + "," + QString::number(ny) + "}");
}

void Node::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
	setToolTip("");
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		if (boundingRect().contains(event->pos())) {
			emit nodeselected(this);
		}
	}
}
