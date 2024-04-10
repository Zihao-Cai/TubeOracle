#pragma once
#include "ItemPoint.h"
class CurveLoop : public QGraphicsObject
{
	Q_OBJECT
public:
	int tag;
	QList<int> loop_path;
	double Top, Bottom, Left, Right = 0;
	double centerX, centerY = 0;

	CurveLoop(QList<int> loops, int tag, QList<double>outline);
	QRectF boundingRect() const override;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
};

