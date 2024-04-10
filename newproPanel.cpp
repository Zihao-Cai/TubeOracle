#include "newproPanel.h"


newproPanel::newproPanel(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.selectpath, SIGNAL(clicked()), this, SLOT(selectpath_clicked()));
	connect(ui.createbtn, SIGNAL(clicked()), this, SLOT(create_clicked()));
	QObject::connect(this, SIGNAL(createdsignal(bool, QString, QString)), parent, SLOT(on_creted_pro(bool, QString, QString)));
}

newproPanel::~newproPanel()
{}

void newproPanel::selectpath_clicked()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	ui.pathedit->setText(dir);

}

void newproPanel::create_clicked()
{
	//�ж���Ŀ���Ʒǿջ���δ�ظ����ж�path·����Ч
	QString proname = ui.nameedit->text();
	QString path = ui.pathedit->text();

	if (!QDir(path).exists())	return;
	if (proname == "")	return;
	if (path == "")	return;
	QString prodir = path + "/" + proname;
	if (QDir(prodir).exists()) {
		return;
	}
	else {
		QDir(prodir).mkdir(prodir);
	}
	QFile file(prodir + "/" + proname + ".geo");
	bool iscreate = file.open(QIODevice::WriteOnly);
	file.close();
	//��Ŀ�����ɹ����ͷ��ź�
	emit(createdsignal(true, prodir, proname));
	//
	this->close();
}
