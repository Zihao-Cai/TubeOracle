#pragma once

#include <QMainWindow>
#include "ui_SetQ.h"

class SetQ : public QMainWindow
{
	Q_OBJECT

public:
	SetQ(QWidget *parent = nullptr);
	~SetQ();
	void setn1(QString s);
	void setn2(QString s);
	double getq();

private:
	Ui::SetQClass ui;
};
