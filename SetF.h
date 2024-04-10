#pragma once

#include <QMainWindow>
#include "ui_SetF.h"

class SetF : public QMainWindow
{
	Q_OBJECT

public:
	SetF(QWidget *parent = nullptr);
	~SetF();
	double getHF();
	double getVF();
	void appendNode(QString s);
	void clear();

private:
	Ui::SetFClass ui;
};
