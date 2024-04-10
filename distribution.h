#pragma once

#include <QMainWindow>
#include "ui_distribution.h"

class distribution : public QMainWindow
{
	Q_OBJECT

public:
	distribution(QMainWindow*parent);
	~distribution();
	QString getText();
private:
	Ui::distribution ui;
};
