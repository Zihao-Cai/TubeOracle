#include "addPoint.h"
#include "ui_addPoint.h"

addPoint::addPoint(QMainWindow*parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setFixedSize(355, 157);
	connect(ui.addbtn, SIGNAL(clicked()), this, SLOT(addbtn_clicked()));
	QObject::connect(this, SIGNAL(addpnt_signal(double,double)), parent, SLOT(on_addpnt(double,double)));
}

addPoint::~addPoint()
{

}

void addPoint::addbtn_clicked()
{
	//判断坐标有效性 坐标有效则释放信号否则返回
	QRegExp regx("[0-9\.\-]+$");	//数字类型的lamda表达式
	if (regx.exactMatch(ui.inputx->text())&& regx.exactMatch(ui.inputy->text())) {
		emit(addpnt_signal(ui.inputx->text().toDouble(), ui.inputy->text().toDouble()));
	}
	else {
		return;
	}
}
