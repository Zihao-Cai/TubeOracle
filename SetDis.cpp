#include "SetDis.h"

SetDis::SetDis(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.textBrowser->setEnabled(false);
}

SetDis::~SetDis()
{}

void SetDis::appendNodes(QString s)
{
	ui.textBrowser->append(s);
}

void SetDis::clear()
{
	ui.textBrowser->clear();
}
