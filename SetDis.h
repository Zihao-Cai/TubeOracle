#pragma once

#include <QMainWindow>
#include "ui_SetDis.h"

class SetDis : public QMainWindow
{
	Q_OBJECT

public:
	SetDis(QWidget *parent = nullptr);
	~SetDis();
	void appendNodes(QString s);
	void clear();

private:
	Ui::SetDisClass ui;
};
