#pragma once

#include <QMainWindow>
#include "ui_addCircle.h"

class addCircle : public QMainWindow
{
	Q_OBJECT

public:
	addCircle(QMainWindow*parent);
	~addCircle();
private:
	Ui::circle ui;

signals:
	void addcircle_signal(double cx, double cy, double radius);

private slots:
	void addbtn_clicked();


};
