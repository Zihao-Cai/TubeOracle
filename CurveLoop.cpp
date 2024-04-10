#include "CurveLoop.h"

CurveLoop::CurveLoop(QList<int> loops, int tag, QList<double>outline)
{
    this->tag = tag;
    loop_path = loops;
	centerX = outline.at(0);
	centerY = outline.at(1);
	Top = outline.at(2);
	Left = outline.at(3);
	Bottom = outline.at(4);
	Right = outline.at(5);
}

QRectF CurveLoop::boundingRect() const
{
    return QRectF();
}

void CurveLoop::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
}
