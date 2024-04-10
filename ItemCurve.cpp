#include "ItemCurve.h"
using namespace std;

ItemCurve::ItemCurve(ItemPoint* start, ItemPoint* center, ItemPoint* end, int tag, QWidget* parent)
{
	ps = start;
	pc = center;
	pe = end;
	this->tag = tag;
	path.push_back(ps->tag);
	path.push_back(pc->tag);
	path.push_back(pe->tag);
	par = parent;
	isselected = false;
	calculate_path();
	setAcceptDrops(true);
	setAcceptHoverEvents(true);
	connect(this, SIGNAL(curveselected(QGraphicsObject*)), (QObject*)(par), SLOT(on_recv_curves(QGraphicsObject*)));
	connect(this, SIGNAL(curveselected(QGraphicsObject*)), (QObject*)(par), SLOT(transfinite_get(QGraphicsObject*)));
}

QRectF ItemCurve::boundingRect() const
{
	//这里的矩形为整圆的外界矩形
	QRectF rect;
	rect = QRectF((pc->px - radius)* geoscale, (pc->py - radius)* geoscale, 2 * radius* geoscale, 2 * radius* geoscale);
	rect = rect.normalized();
	return rect;
}

void ItemCurve::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->setBrush(QColor(int(0 * 255), int(0 * 255), int(1 * 255)));
	painter->setPen(QPen(QColor(int(0 * 255), int(0 * 255), int(1 * 255)), curvewidth));
	if (isselected) {
		painter->setPen(QPen(Qt::red, selectcurvewidth));
	}
	painter->drawArc((pc->px - radius)* geoscale , (pc->py - radius)* geoscale, 2 * radius* geoscale, 2 * radius* geoscale, startAngle * 16, span * 16);
}

void ItemCurve::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
	setToolTip("Circle(" + QString::number(tag) + "){" + QString::number(ps->tag) + "," + QString::number(pc->tag) + "," + QString::number(pe->tag) + "}");
}

void ItemCurve::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
	setToolTip("");
}

void ItemCurve::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		if (shape().contains(event->pos())) {
			emit curveselected(this);
		}
	}

}

QPainterPath ItemCurve::shape() const
{
	// 创建路径，用于定义圆弧的精确形状
	QPainterPathStroker stroker;
	stroker.setWidth(curvewidth);
	QPainterPath path;
	path.moveTo((pc->px + radius * qCos(qDegreesToRadians(startAngle)))* geoscale, (pc->py - radius * qSin(qDegreesToRadians(startAngle)))* geoscale);
	path.arcTo((pc->px - radius)* geoscale, (pc->py - radius)* geoscale, 2 * radius* geoscale, 2 * radius* geoscale, startAngle, span);
	return stroker.createStroke(path);
}

QList<double> ItemCurve::locateX(double y)
{
	QList<double> allX;
	if (y<top || y>bottom) return QList<double>();

	qreal theta = qAsin((pc->py - y) / radius);
	qreal theta_ = theta + M_PI;
	double x1 = pc->px + radius * qCos(theta);
	double x2 = pc->px + radius * qCos(theta_);
	if (x1 >= left && x1 <= right)	allX.push_back(x1);
	if (x2 >= left && x2 <= right)	allX.push_back(x2);
	return allX;
}

QList<double> ItemCurve::locateY(double x)
{
	QList<double> allY;
	if (x < left || x > right) return QList<double>();

	qreal theta = qAcos((x - pc->px) / radius);
	qreal theta_ = theta + M_PI;
	double y1 = pc->py - radius * qSin(theta);
	double y2= pc->py - radius * qSin(theta_);
	if (y1 >= top && y1 <= bottom)	allY.push_back(y1);
	if (y2 >= top && y2 <= bottom)	allY.push_back(y2);
	return allY;
}

void ItemCurve::calculate_path()
{
	// Calculate the distances
	radius = QLineF(pc->px,pc->py,ps->px,ps->py).length();
	qreal angle = QLineF(pc->px, pc->py, ps->px, ps->py).angleTo(QLineF(pc->px, pc->py, pe->px, pe->py));

	// Calculate the start angle and span
	startAngle = QLineF(pc->px, pc->py, ps->px, ps->py).angle();
	span = angle;

	// Choose the shorter arc
	if (span > 180) {
		startAngle = startAngle + span;
		span = 360 - span;
	}
	if (startAngle >= 360)	startAngle -= 360;

	left = right = pc->px + radius * qCos(qDegreesToRadians(startAngle));
	top = bottom = pc->py - radius * qSin(qDegreesToRadians(startAngle));
	for (qreal angle = startAngle; angle <= startAngle + span; angle++) {
		if (pc->px + radius * qCos(qDegreesToRadians(angle)) < left)	left = pc->px + radius * qCos(qDegreesToRadians(angle));
		if (pc->px + radius * qCos(qDegreesToRadians(angle)) > right)	right = pc->px + radius * qCos(qDegreesToRadians(angle));
		if (pc->py - radius * qSin(qDegreesToRadians(angle)) < top)		top = pc->py - radius * qSin(qDegreesToRadians(angle));
		if (pc->py - radius * qSin(qDegreesToRadians(angle)) > bottom)	bottom = pc->py - radius * qSin(qDegreesToRadians(angle));
	}
}
