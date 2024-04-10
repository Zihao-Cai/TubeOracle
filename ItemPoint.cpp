#include "ItemPoint.h"


ItemPoint::ItemPoint(double x, double y,int tag,QWidget* parent)
{
	px = x;
	py = y;
	this->tag = tag;
	par = parent;
	isselected = false;
	setAcceptDrops(true);
	setAcceptHoverEvents(true);
	connect(this, SIGNAL(pntselected(ItemPoint*)), (QObject*)(par), SLOT(on_addline(ItemPoint*)));
	connect(this, SIGNAL(pntselected(ItemPoint*)), (QObject*)(par), SLOT(on_addcirclearc(ItemPoint*)));

}

QRectF ItemPoint::boundingRect() const
{
	return QRectF(px * geoscale - pointsize, py * geoscale - pointsize, pointsize * 2, pointsize * 2);
}

void ItemPoint::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->setBrush(QColor(int(0.353 * 255), int(0.353 * 255), int(0.353 * 255)));
	painter->setPen(QPen(QColor(int(0.353 * 255), int(0.353 * 255), int(0.353 * 255)), pointsize));
	if (isselected) {
		painter->setPen(QPen(Qt::red, selectpointsize));
	}
	painter->drawPoint(px*geoscale, py*geoscale);
	
	
}

void ItemPoint::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
	setToolTip("Point(" + QString::number(tag) + "){" + QString::number(px) + "," + QString::number(py) + "}");
}

void ItemPoint::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
	setToolTip("");
}

void ItemPoint::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		if (boundingRect().contains(event->pos())) {
			emit pntselected(this);
		}
	}

}
