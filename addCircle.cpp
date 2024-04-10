#include "addCircle.h"
#include "ui_addCircle.h"


addCircle::addCircle(QMainWindow*parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setFixedSize(360, 162);
	connect(ui.addbtn, SIGNAL(clicked()), this, SLOT(addbtn_clicked()));
	QObject::connect(this, SIGNAL(addcircle_signal(double, double, double)), parent, SLOT(on_addcircle(double, double, double)));
}

addCircle::~addCircle()
{
}

void addCircle::addbtn_clicked()
{
	//�ж�������Ч�� ������Ч���ͷ��źŷ��򷵻�
	QRegExp regx("[0-9\.\-]+$");	//�������͵�lamda���ʽ
	if (regx.exactMatch(ui.cxedit->text()) && regx.exactMatch(ui.cyedit->text()) && regx.exactMatch(ui.redit->text())) {
		if (ui.redit->text().toDouble() == 0)	return;
		emit(addcircle_signal(ui.cxedit->text().toDouble(), ui.cyedit->text().toDouble(), ui.redit->text().toDouble()));
	}
	else {
		return;
	}
}
