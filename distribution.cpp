#include "distribution.h"
#include "ui_distribution.h"
#include <qvalidator.h>

distribution::distribution(QMainWindow*parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.dlineEdit->setValidator(new QIntValidator(2, 9999999, ui.dlineEdit));
}

distribution::~distribution()
{
}

QString distribution::getText()
{
	return ui.dlineEdit->text();
}
