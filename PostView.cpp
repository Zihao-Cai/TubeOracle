#include "PostView.h"


PostView::PostView(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//初始化customplot
	customplot1 = new QCustomPlot(ui.ax);
	customplot1->setGeometry(0, 0, ui.ax->width(), ui.ax->height());
	customplot2 = new QCustomPlot(ui.sx);
	customplot2->setGeometry(0, 0, ui.sx->width(), ui.sx->height());
	customplot3 = new QCustomPlot(ui.ay);
	customplot3->setGeometry(0, 0, ui.ay->width(), ui.ay->height());
	customplot4 = new QCustomPlot(ui.sy);
	customplot4->setGeometry(0, 0, ui.sy->width(), ui.sy->height());
	customplot5 = new QCustomPlot(ui.at);
	customplot5->setGeometry(0, 0, ui.at->width(), ui.at->height());
	customplot6 = new QCustomPlot(ui.st);
	customplot6->setGeometry(0, 0, ui.st->width(), ui.st->height());

	customplot1->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	customplot2->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	customplot3->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	customplot4->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	customplot5->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	customplot6->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	// 设置标签
	customplot1->xAxis2->setLabel("X");
	customplot1->yAxis->setLabel("Y");
	customplot2->xAxis2->setLabel("X");
	customplot2->yAxis->setLabel("Y");
	customplot3->xAxis2->setLabel("X");
	customplot3->yAxis->setLabel("Y");
	customplot4->xAxis2->setLabel("X");
	customplot4->yAxis->setLabel("Y");
	customplot5->xAxis2->setLabel("X");
	customplot5->yAxis->setLabel("Y");
	customplot6->xAxis2->setLabel("X");
	customplot6->yAxis->setLabel("Y");
	//初始化云图坐标轴
	colorMap1 = new QCPColorMap(customplot1->xAxis2, customplot1->yAxis);
	colorMap2 = new QCPColorMap(customplot2->xAxis2, customplot2->yAxis);
	colorMap3 = new QCPColorMap(customplot3->xAxis2, customplot3->yAxis);
	colorMap4 = new QCPColorMap(customplot4->xAxis2, customplot4->yAxis);
	colorMap5 = new QCPColorMap(customplot5->xAxis2, customplot5->yAxis);
	colorMap6 = new QCPColorMap(customplot6->xAxis2, customplot6->yAxis);
	customplot1->xAxis2->setVisible(true);
	customplot1->xAxis->setVisible(false);
	customplot1->yAxis->setRangeReversed(true);
	customplot2->xAxis2->setVisible(true);
	customplot2->xAxis->setVisible(false);
	customplot2->yAxis->setRangeReversed(true);
	customplot3->xAxis2->setVisible(true);
	customplot3->xAxis->setVisible(false);
	customplot3->yAxis->setRangeReversed(true);
	customplot4->xAxis2->setVisible(true);
	customplot4->xAxis->setVisible(false);
	customplot4->yAxis->setRangeReversed(true);
	customplot5->xAxis2->setVisible(true);
	customplot5->xAxis->setVisible(false);
	customplot5->yAxis->setRangeReversed(true);
	customplot6->xAxis2->setVisible(true);
	customplot6->xAxis->setVisible(false);
	customplot6->yAxis->setRangeReversed(true);

	connect(ui.save, SIGNAL(triggered()), this, SLOT(save_img()));
}

PostView::~PostView()
{
	delete customplot1;
	delete customplot2;
	delete customplot3;
	delete customplot4;
	delete customplot5;
	delete customplot6;
}

void PostView::setData(std::unordered_map<size_t, Node*> NodeMap, std::unordered_map<size_t, Triangle*> Triangles, std::vector<double> choords)
{
	//绘制应力应变
	//假若坐标分辨率0.5, 每个填充点坐标需要除以0.5
	
	double resol = choords[0];
	int nx = (choords[2] - choords[1]) / resol + 1;
	int ny = (choords[4] - choords[3]) / resol + 1;
	double x_offset, y_offset;
	choords[1] < 0 ? x_offset = -choords[1] : x_offset = 0;
	choords[3] < 0 ? y_offset = -choords[3] : y_offset = 0;
	colorMap1->data()->setSize(nx, ny);
	colorMap1->data()->setRange(QCPRange(choords[1], choords[2]), QCPRange(choords[3], choords[4]));
	colorMap2->data()->setSize(nx, ny);
	colorMap2->data()->setRange(QCPRange(choords[1], choords[2]), QCPRange(choords[3], choords[4]));
	colorMap3->data()->setSize(nx, ny);
	colorMap3->data()->setRange(QCPRange(choords[1], choords[2]), QCPRange(choords[3], choords[4]));
	colorMap4->data()->setSize(nx, ny);
	colorMap4->data()->setRange(QCPRange(choords[1], choords[2]), QCPRange(choords[3], choords[4]));
	colorMap5->data()->setSize(nx, ny);
	colorMap5->data()->setRange(QCPRange(choords[1], choords[2]), QCPRange(choords[3], choords[4]));
	colorMap6->data()->setSize(nx, ny);
	colorMap6->data()->setRange(QCPRange(choords[1], choords[2]), QCPRange(choords[3], choords[4]));


	//颜色数据布置
	for (auto it = Triangles.begin(); it != Triangles.end(); ++it) {
		colorMap1->data()->setCell((NodeMap[it->second->nodesIndex[0] + 1]->nx + x_offset) / resol, (NodeMap[it->second->nodesIndex[0] + 1]->ny + y_offset) / resol, it->second->strain[0]);
		colorMap1->data()->setCell((NodeMap[it->second->nodesIndex[1] + 1]->nx + x_offset) / resol, (NodeMap[it->second->nodesIndex[1] + 1]->ny + y_offset) / resol, it->second->strain[0]);
		colorMap1->data()->setCell((NodeMap[it->second->nodesIndex[2] + 1]->nx + x_offset) / resol, (NodeMap[it->second->nodesIndex[2] + 1]->ny + y_offset) / resol, it->second->strain[0]);

		colorMap2->data()->setCell((NodeMap[it->second->nodesIndex[0] + 1]->nx + x_offset) / resol, (NodeMap[it->second->nodesIndex[0] + 1]->ny + y_offset) / resol, it->second->stress[0]);
		colorMap2->data()->setCell((NodeMap[it->second->nodesIndex[1] + 1]->nx + x_offset) / resol, (NodeMap[it->second->nodesIndex[1] + 1]->ny + y_offset) / resol, it->second->stress[0]);
		colorMap2->data()->setCell((NodeMap[it->second->nodesIndex[2] + 1]->nx + x_offset) / resol, (NodeMap[it->second->nodesIndex[2] + 1]->ny + y_offset) / resol, it->second->stress[0]);

		colorMap3->data()->setCell((NodeMap[it->second->nodesIndex[0] + 1]->nx + x_offset) / resol, (NodeMap[it->second->nodesIndex[0] + 1]->ny + y_offset) / resol, it->second->strain[1]);
		colorMap3->data()->setCell((NodeMap[it->second->nodesIndex[1] + 1]->nx + x_offset) / resol, (NodeMap[it->second->nodesIndex[1] + 1]->ny + y_offset) / resol, it->second->strain[1]);
		colorMap3->data()->setCell((NodeMap[it->second->nodesIndex[2] + 1]->nx + x_offset) / resol, (NodeMap[it->second->nodesIndex[2] + 1]->ny + y_offset) / resol, it->second->strain[1]);

		colorMap4->data()->setCell((NodeMap[it->second->nodesIndex[0] + 1]->nx + x_offset) / resol, (NodeMap[it->second->nodesIndex[0] + 1]->ny + y_offset) / resol, it->second->stress[1]);
		colorMap4->data()->setCell((NodeMap[it->second->nodesIndex[1] + 1]->nx + x_offset) / resol, (NodeMap[it->second->nodesIndex[1] + 1]->ny + y_offset) / resol, it->second->stress[1]);
		colorMap4->data()->setCell((NodeMap[it->second->nodesIndex[2] + 1]->nx + x_offset) / resol, (NodeMap[it->second->nodesIndex[2] + 1]->ny + y_offset) / resol, it->second->stress[1]);

		colorMap5->data()->setCell((NodeMap[it->second->nodesIndex[0] + 1]->nx + x_offset) / resol, (NodeMap[it->second->nodesIndex[0] + 1]->ny + y_offset) / resol, it->second->strain[2]);
		colorMap5->data()->setCell((NodeMap[it->second->nodesIndex[1] + 1]->nx + x_offset) / resol, (NodeMap[it->second->nodesIndex[1] + 1]->ny + y_offset) / resol, it->second->strain[2]);
		colorMap5->data()->setCell((NodeMap[it->second->nodesIndex[2] + 1]->nx + x_offset) / resol, (NodeMap[it->second->nodesIndex[2] + 1]->ny + y_offset) / resol, it->second->strain[2]);

		colorMap6->data()->setCell((NodeMap[it->second->nodesIndex[0] + 1]->nx + x_offset) / resol, (NodeMap[it->second->nodesIndex[0] + 1]->ny + y_offset) / resol, it->second->stress[2]);
		colorMap6->data()->setCell((NodeMap[it->second->nodesIndex[1] + 1]->nx + x_offset) / resol, (NodeMap[it->second->nodesIndex[1] + 1]->ny + y_offset) / resol, it->second->stress[2]);
		colorMap6->data()->setCell((NodeMap[it->second->nodesIndex[2] + 1]->nx + x_offset) / resol, (NodeMap[it->second->nodesIndex[2] + 1]->ny + y_offset) / resol, it->second->stress[2]);

	}//在三角形顶点位置填充
	
	 //添加色条
	try {
		customplot1->plotLayout()->removeAt(1);
		customplot2->plotLayout()->removeAt(1);
		customplot3->plotLayout()->removeAt(1);
		customplot4->plotLayout()->removeAt(1);
		customplot5->plotLayout()->removeAt(1);
		customplot6->plotLayout()->removeAt(1);
	}
	catch (std::exception e) {

	}
	QCPColorScale* colorscale1 = new QCPColorScale(customplot1);
	QCPColorScale* colorscale2 = new QCPColorScale(customplot2);
	QCPColorScale* colorscale3 = new QCPColorScale(customplot3);
	QCPColorScale* colorscale4 = new QCPColorScale(customplot4);
	QCPColorScale* colorscale5 = new QCPColorScale(customplot5);
	QCPColorScale* colorscale6 = new QCPColorScale(customplot6);
	customplot1->plotLayout()->addElement(0, 1, colorscale1);
	customplot2->plotLayout()->addElement(0, 1, colorscale2);
	customplot3->plotLayout()->addElement(0, 1, colorscale3);
	customplot4->plotLayout()->addElement(0, 1, colorscale4);
	customplot5->plotLayout()->addElement(0, 1, colorscale5);
	customplot6->plotLayout()->addElement(0, 1, colorscale6);
	colorscale1->setType(QCPAxis::atRight);
	colorscale1->axis()->setLabel("Strain for X");
	colorMap1->setColorScale(colorscale1);
	colorscale2->setType(QCPAxis::atRight);
	colorscale2->axis()->setLabel("Stress for X");
	colorMap2->setColorScale(colorscale2);
	colorscale3->setType(QCPAxis::atRight);
	colorscale3->axis()->setLabel("Strain for Y");
	colorMap3->setColorScale(colorscale3);
	colorscale4->setType(QCPAxis::atRight);
	colorscale4->axis()->setLabel("Stress for Y");
	colorMap4->setColorScale(colorscale4);
	colorscale5->setType(QCPAxis::atRight);
	colorscale5->axis()->setLabel("Strain for shear");
	colorMap5->setColorScale(colorscale5);
	colorscale6->setType(QCPAxis::atRight);
	colorscale6->axis()->setLabel("Stress for shear");
	colorMap6->setColorScale(colorscale6);
	//渐变效果
	colorMap1->setGradient(QCPColorGradient::gpJet);
	colorMap2->setGradient(QCPColorGradient::gpJet);
	colorMap3->setGradient(QCPColorGradient::gpJet);
	colorMap4->setGradient(QCPColorGradient::gpJet);
	colorMap5->setGradient(QCPColorGradient::gpJet);
	colorMap6->setGradient(QCPColorGradient::gpJet);
	colorMap1->rescaleDataRange();
	colorMap2->rescaleDataRange();
	colorMap3->rescaleDataRange();
	colorMap4->rescaleDataRange();
	colorMap5->rescaleDataRange();
	colorMap6->rescaleDataRange();
	
	customplot1->rescaleAxes();
	customplot2->rescaleAxes();
	customplot3->rescaleAxes();
	customplot4->rescaleAxes();
	customplot5->rescaleAxes();
	customplot6->rescaleAxes();
	customplot1->replot();
	customplot2->replot();
	customplot3->replot();
	customplot4->replot();
	customplot5->replot();
	customplot6->replot();
}

void PostView::keyPressEvent(QKeyEvent* event)
{
	if (event->modifiers() == Qt::ControlModifier) {
		if (event->key() == Qt::Key_S) {
			save_img();
			return;
		}
	}
}

void PostView::save_img()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	
	try {
		customplot1->savePng(dir + "/strainForX.png", 800, 600);
		customplot2->savePng(dir + "/stressForX.png", 800, 600);
		customplot3->savePng(dir + "/strainForY.png", 800, 600);
		customplot4->savePng(dir + "/stressForY.png", 800, 600);
		customplot5->savePng(dir + "/strainForShear.png", 800, 600);
		customplot6->savePng(dir + "/stressForShear.png", 800, 600);
	}
	catch (std::exception e) {

	}

}

