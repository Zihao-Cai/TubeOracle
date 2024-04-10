#pragma once
#include <qfile.h>
#include <qfiledialog.h>
#include <qstring.h>
#include <QMainWindow>
#include "ui_newproPanel.h"

class newproPanel : public QMainWindow
{
	Q_OBJECT

public:
	newproPanel(QWidget *parent = nullptr);
	~newproPanel();

private:
	Ui::newproPanelClass ui;


private slots:
	void selectpath_clicked();
	void create_clicked();
private: signals:
	void createdsignal(bool iscr, QString prodir, QString proname);
};
