#pragma once
#include <QMainWindow>
#include "ui_QualityView.h"
#include <qcustomplot.h>

class QualityView : public QMainWindow
{
	Q_OBJECT

public:
	QualityView(QWidget *parent = nullptr);
	~QualityView();
	void setData(QVector<qreal> quality);//setDataʵ������ΪBar��������

private:
	Ui::QualityViewClass ui;
	QCustomPlot* customplot = nullptr;
	QCPBars* dis_quality_num;
	QVector<qreal> ticks;

};
