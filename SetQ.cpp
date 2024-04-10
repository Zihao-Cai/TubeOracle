#include "SetQ.h"
#include <qvalidator.h>

SetQ::SetQ(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.qedit->setValidator(new QDoubleValidator(-999999999.999999, 999999999.999999, 6, this));
	ui.n1->setEnabled(false);
	ui.n2->setEnabled(false);
}

SetQ::~SetQ()
{}

void SetQ::setn1(QString s)
{
	ui.n1->setText(s);
}

void SetQ::setn2(QString s)
{
	ui.n2->setText(s);
}

double SetQ::getq()
{
	return ui.qedit->text().toDouble();
}