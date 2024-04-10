#pragma once

#include <QMainWindow>
#include "ui_addPoint.h"

class addPoint : public QMainWindow
{
	Q_OBJECT

public:
	addPoint(QMainWindow*parent);
	~addPoint();
private:
	Ui::point ui;

signals:
	void addpnt_signal(double pointx,double pointy);

private slots:
	void addbtn_clicked();


};
