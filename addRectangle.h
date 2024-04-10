#pragma once

#include <QMainWindow>
#include "ui_addRectangle.h"

class addRectangle : public QMainWindow
{
	Q_OBJECT

public:
	addRectangle(QMainWindow*parent);
	~addRectangle();
private:
	Ui::rectangle ui;

signals:
	void addrect_signal(double x, double y,double width,double height);

private slots:
	void addbtn_clicked();

};
