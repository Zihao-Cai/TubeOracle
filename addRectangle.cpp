#include "addRectangle.h"
#include "ui_addRectangle.h"

addRectangle::addRectangle(QMainWindow*parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setFixedSize(364, 160);
	connect(ui.addbtn, SIGNAL(clicked()), this, SLOT(addbtn_clicked()));
	QObject::connect(this, SIGNAL(addrect_signal(double, double, double, double)), parent, SLOT(on_addrect(double, double, double, double)));
}

addRectangle::~addRectangle()
{
}

void addRectangle::addbtn_clicked()
{
	//�ж�������Ч�� ������Ч���ͷ��źŷ��򷵻�
	QRegExp regx("[0-9\.\-]+$");	//�������͵�lamda���ʽ
	if (regx.exactMatch(ui.xedit->text()) && regx.exactMatch(ui.yedit->text())&& regx.exactMatch(ui.wedit->text()) && regx.exactMatch(ui.hedit->text())) {
		emit(addrect_signal(ui.xedit->text().toDouble(), ui.yedit->text().toDouble(), ui.wedit->text().toDouble(), ui.hedit->text().toDouble()));
	}
	else {
		return;
	}
}
