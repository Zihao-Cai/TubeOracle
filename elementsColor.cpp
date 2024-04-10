#include "elementsColor.h"

elementsColor::elementsColor(QMainWindow *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

elementsColor::~elementsColor()
{}
