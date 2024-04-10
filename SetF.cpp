#include "SetF.h"
#include <qvalidator.h>

SetF::SetF(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.HF->setValidator(new QDoubleValidator(-999999999.999999, 999999999.999999, 6, this));
	ui.VF->setValidator(new QDoubleValidator(-999999999.999999, 999999999.999999, 6, this));
	ui.textBrowser->setEnabled(false);
}

SetF::~SetF()
{}

double SetF::getHF()
{
	return ui.HF->text().toDouble();
}

double SetF::getVF()
{
	return ui.VF->text().toDouble();
}

void SetF::appendNode(QString s)
{
	ui.textBrowser->append(s);
}

void SetF::clear()
{
	ui.textBrowser->clear();
}
