#pragma once

#include <QMainWindow>
#include "ui_elementsColor.h"

class elementsColor  : public QMainWindow
{
	Q_OBJECT

public:
	elementsColor(QMainWindow *parent);
	~elementsColor();
private:
	Ui::color ui;	
};
