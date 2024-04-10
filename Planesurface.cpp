#include "Planesurface.h"

Planesurface::Planesurface(QList<CurveLoop*> bounding, int tag)
{
	this->tag = tag;
	this->boundaries = bounding;
	updateoutline();

}

QRectF Planesurface::boundingRect() const
{
	return QRectF();
}

void Planesurface::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	
	painter->setBrush(QColor(int(0.502 * 255), int(0.502 * 255), int(0.502 * 255)));
	painter->setPen(QPen(QColor(int(0.502 * 255), int(0.502 * 255), int(0.502 * 255)), dashlinewidth,Qt::DashLine));
	for (const auto &line : this->outline) {
		painter->drawLine(line);
	}
}

void Planesurface::updateoutline()
{
	this->outline.clear();

	//先只做外边界轮廓
	CurveLoop* loop = this->boundaries.at(0);
	QLineF line1(loop->centerX * geoscale, loop->centerY * geoscale, loop->centerX * geoscale, loop->Top * geoscale);
	outline.push_back(line1);
	QLineF line2(loop->centerX * geoscale, loop->centerY * geoscale, loop->centerX * geoscale, loop->Bottom * geoscale);
	outline.push_back(line2);
	QLineF line3(loop->centerX * geoscale, loop->centerY * geoscale, loop->Left * geoscale, loop->centerY * geoscale);
	outline.push_back(line3);
	QLineF line4(loop->centerX * geoscale, loop->centerY * geoscale, loop->Right * geoscale, loop->centerY * geoscale);
	outline.push_back(line4);

}

