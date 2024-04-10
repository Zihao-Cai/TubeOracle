#pragma once
#include "CurveLoop.h"

class Planesurface : public QGraphicsObject
{
	Q_OBJECT
public:
	int tag;
	QList<CurveLoop*> boundaries;
	Planesurface(QList<CurveLoop*> bounding, int tag);
	QRectF boundingRect() const override;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
	QList<QLineF> outline;
	void updateoutline();
private:
	


};

