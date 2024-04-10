#include "options.h"

#define windowHeight 400
#define windowWidth 550
options::options(QMainWindow *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setWindowTitle("选项");
	//设置窗口大小
	this->setFixedHeight(windowHeight);
	this->setFixedWidth(windowWidth);
	//设置左边白色部分的位置和大小
	ui.frameBank->setGeometry(0, 0, windowWidth/5, windowHeight);

	//设置{geometry，mesh，solver}面板的位置和大小
	ui.frameOptions->setGeometry(0, 0, windowWidth / 5, windowHeight/4);


	//设置geometry的面板 geometrytab
	ui.geometrytab->setGeometry(windowWidth / 5, 0, windowWidth - (windowWidth / 5), windowHeight);
	ui.geometrytab->addTab(ui.gaspectWidget, "Aspect");
	ui.geometrytab->addTab(ui.gvisibilityWidge, "Visibility");
	//geometry的aspect面板的设计
	//先简单的添加曲面效果(cross和solid)
	QStringList strList;
	strList << "cross" << "solid";
	ui.sufaceseffect->addItems(strList);
	

	//geometry的visibility面板的设计
	

	//mesh的面板，meshtab设计
	ui.meshtab->setGeometry(windowWidth / 5, 0, windowWidth - (windowWidth / 5), windowHeight);
	ui.meshtab->addTab(ui.generalwidge, "General");
	
	ui.meshtab->addTab(ui.maspectWidget, "Aspect");
	ui.meshtab->addTab(ui.mvisibilityWidget, "Visibility");



	
	//最后添加切换逻辑，动画
	ui.meshtab->hide();
	ui.geometrytab->hide();
	//按钮
	

	connect(ui.geobtn, SIGNAL(clicked()), this, SLOT(op_geo_clicked()));
	connect(ui.meshbtn, SIGNAL(clicked()), this, SLOT(op_mesh_clicked()));
	//实时数据传输options
	//Size config
	connect(ui.pointSize, SIGNAL(valueChanged(double)), parent, SLOT(globalconfig()));
	connect(ui.selectPointSize, SIGNAL(valueChanged(double)), parent, SLOT(globalconfig()));
	connect(ui.curveWidth, SIGNAL(valueChanged(double)), parent, SLOT(globalconfig()));
	connect(ui.selectCurve, SIGNAL(valueChanged(double)), parent, SLOT(globalconfig()));

	connect(ui.pointSize_2, SIGNAL(valueChanged(double)), parent, SLOT(globalconfig()));
	connect(ui.lineWidth, SIGNAL(valueChanged(double)), parent, SLOT(globalconfig()));
	
	connect(ui.geoscale, SIGNAL(valueChanged(double)), parent, SLOT(globalconfig()));

	//Label visibility
	connect(ui.pointLabels, SIGNAL(stateChanged(int)), parent, SLOT(label_filtrate()));
	connect(ui.curvelabels, SIGNAL(stateChanged(int)), parent, SLOT(label_filtrate()));
	connect(ui.surfacelabels, SIGNAL(stateChanged(int)), parent, SLOT(label_filtrate()));

	connect(ui.twoDEL, SIGNAL(stateChanged(int)), parent, SLOT(label_filtrate()));
	connect(ui.nodeLables, SIGNAL(stateChanged(int)), parent, SLOT(label_filtrate()));
	connect(ui.twoDEF, SIGNAL(stateChanged(int)), parent, SLOT(label_filtrate()));

	//Surface effect switch
	connect(ui.sufaceseffect, SIGNAL(currentIndexChanged(QString)), parent, SLOT(surface_effect()));

	//Algorithm switch
	connect(ui.comboBox, SIGNAL(currentIndexChanged(QString)), parent, SLOT(algorithm_config()));
	connect(ui.comboBox_2, SIGNAL(currentIndexChanged(QString)), parent, SLOT(algorithm_config()));


}

options::~options()
{}

double options::get_geoscale()
{
	return ui.geoscale->text().toDouble();
}

void options::set_geoscale(double geoscale)
{
	ui.geoscale->setValue(geoscale);
}

double options::get_edgewidth()
{
	return ui.lineWidth->text().toDouble();
}

void options::set_edgewidth(double edgewidth)
{
	ui.lineWidth->setValue(edgewidth);
}

double options::get_curvewidth()
{
	return ui.curveWidth->text().toDouble();
}

void options::set_curvewidth(double curvewidth)
{
	ui.curveWidth->setValue(curvewidth);
}

double options::get_selectcurvewidth()
{
	return ui.selectCurve->text().toDouble();
}

void options::set_selectcurvewidth(double selectcurvewidth)
{
	ui.selectCurve->setValue(selectcurvewidth);
}

double options::get_pointsize()
{
	return ui.pointSize->text().toDouble();
}

void options::set_pointsize(double pointsize)
{
	ui.pointSize->setValue(pointsize);
}

double options::get_selectpointsize()
{
	return ui.selectPointSize->text().toDouble();
}

void options::set_selectpointsize(double selectpointsize)
{
	ui.selectPointSize->setValue(selectpointsize);
}

double options::get_nodesize()
{
	return ui.pointSize_2->text().toDouble();
}

void options::set_nodesize(double nodesize)
{
	ui.pointSize_2->setValue(nodesize);
}

QString options::get2D()
{
	return ui.comboBox->currentText();
}

QString options::getOptim()
{
	return ui.comboBox_2->currentText();
}

void options::op_geo_clicked()
{
	this->setWindowTitle("选项-几何");
}

void options::op_mesh_clicked()
{
	this->setWindowTitle("选项-网格");
}

