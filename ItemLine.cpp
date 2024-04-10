#include "ItemLine.h"

using namespace std;
ItemLine::ItemLine(ItemPoint* start, ItemPoint* end, int tag, QWidget* parent)
{
	p1 = start;
	p2 = end;
	this->tag = tag;
	path.push_back(p1->tag);
	path.push_back(p2->tag);
	par = parent;
	top = qMin(p1->py, p2->py);
	bottom = qMax(p1->py, p2->py);
	left = qMin(p1->px, p2->px);
	right = qMax(p1->px, p2->px);
	isselected = false;
	setAcceptDrops(true);
	setAcceptHoverEvents(true); 
	connect(this, SIGNAL(lineselected(QGraphicsObject*)), (QObject*)(par), SLOT(on_recv_curves(QGraphicsObject*)));
	connect(this, SIGNAL(lineselected(QGraphicsObject*)), (QObject*)(par), SLOT(transfinite_get(QGraphicsObject*)));
}

QRectF ItemLine::boundingRect() const
{
	qreal minX = qMin(p1->px, p2->px) - curvewidth / 2;
	qreal minY = qMin(p1->py, p2->py) - curvewidth / 2;
	qreal maxX = qMax(p1->px, p2->px) + curvewidth / 2;
	qreal maxY = qMax(p1->py, p2->py) + curvewidth / 2;

	return QRectF(QPointF(minX * geoscale, minY * geoscale), QPointF(maxX * geoscale, maxY * geoscale));
}

QPainterPath ItemLine::shape() const
{
	// 创建路径，用于定义直线的精确形状
	QPainterPathStroker stroker;
	stroker.setWidth(2);
	QPainterPath path;
	path.moveTo(p1->px* geoscale, p1->py* geoscale);
	path.lineTo(p2->px* geoscale, p2->py* geoscale);
	return stroker.createStroke(path);
}

void ItemLine::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->setBrush(QColor(int(0 * 255), int(0 * 255), int(1 * 255)));
	painter->setPen(QPen(QColor(int(0 * 255), int(0 * 255), int(1 * 255)), curvewidth));
	if (isselected) {
		painter->setPen(QPen(Qt::red, selectcurvewidth));
	}
	painter->drawLine(p1->px* geoscale, p1->py* geoscale, p2->px* geoscale, p2->py* geoscale);
	
}

void ItemLine::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
	setToolTip("Line(" + QString::number(tag) + "){" + QString::number(p1->tag) + "," + QString::number(p2->tag) + "}");
}

void ItemLine::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
	setToolTip("");
}


void ItemLine::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		if (shape().contains(event->pos())) {
			emit lineselected(this);
		}
	}

}

double ItemLine::locateX(double y)
{
	if (y<top || y>bottom) return -999999;
	if (p1->px == p2->px)	return p1->px;
	if (p1->py == p2->py)	return (p1->px + p2->px) / 2;

	double a = (p2->py - p1->py) / (p2->px - p1->px);
	double b = p1->py - a * p1->px;

	return (y - b) / a;
}

double ItemLine::locateY(double x)
{
	if (x < left || x > right) return -999999;
	if (p1->px == p2->px)	return (p1->py + p2->py) / 2;
	if (p1->py == p2->py)	return p1->py;

	double a = (p2->py - p1->py) / (p2->px - p1->px);
	double b = p1->py - a * p1->px;
	
	return a * x + b;
}
