#pragma once
#include <QMainWindow>
#include "ui_key.h"

class key  : public QMainWindow
{
	Q_OBJECT

public:
	key(QMainWindow* parent);
	~key();
private:
	Ui::mykey ui;
};
