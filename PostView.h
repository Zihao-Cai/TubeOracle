#pragma once
#include <QMainWindow>
#include "ui_PostView.h"
#include <qcustomplot.h>
#include <unordered_map>
#include "Node.h"
#include "Triangle.h"
#include <vector>
#include <qfiledialog.h>

class PostView : public QMainWindow
{
	Q_OBJECT

public:
	PostView(QWidget *parent = nullptr);
	~PostView();
	void setData(std::unordered_map<size_t, Node*> NodeMap, std::unordered_map<size_t, Triangle*> Triangles, std::vector<double> choords);
	void keyPressEvent(QKeyEvent* event);

private:
	Ui::PostViewClass ui;
	QCustomPlot* customplot1 = nullptr;
	QCustomPlot* customplot2 = nullptr;
	QCustomPlot* customplot3 = nullptr;
	QCustomPlot* customplot4 = nullptr;
	QCustomPlot* customplot5 = nullptr;
	QCustomPlot* customplot6 = nullptr;
	QCPColorMap* colorMap1;
	QCPColorMap* colorMap2;
	QCPColorMap* colorMap3;
	QCPColorMap* colorMap4;
	QCPColorMap* colorMap5;
	QCPColorMap* colorMap6;

private slots:
	void save_img();
	
};
