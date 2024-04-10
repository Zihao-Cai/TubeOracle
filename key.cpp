#include "key.h"
#include <qheaderview.h>


key::key(QMainWindow* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setWindowTitle("Keyboard Usage");
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

key::~key()
{}
