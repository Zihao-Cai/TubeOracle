#include "TubeOracle.h"
#include "newproPanel.h"
#include <fstream>
#include <string>
#include <QFrame>
#include <qdatetime.h>

using namespace std;

#define TEXT_COLOR_RED(str)         "<font color = red>" + str + "</font>" + "<font color = black> </font>"
#define TEXT_COLOR_BLUE(str)        "<font color = blue>" + str + "</font>" + "<font color = black> </font>"
#define TEXT_COLOR_GREEN(str)        "<font color = green>" + str + "</font>" + "<font color = black> </font>"



TubeOracle::TubeOracle(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//UI配置初始化
	optionsWindow = new options(this);
	elementColorWindow = new elementsColor(this);
	trans_window = new distribution(this);
	mykey = new key(this);
	myabout = new about(this);
	graView = new MygraphicsView();
	graView->setmainwin(this);
	graView->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(182, 196, 255, 255), stop:1 rgba(255, 255, 255, 255));");
	QVBoxLayout* layout = new QVBoxLayout();
	layout->addWidget(graView);
	ui.verticalLayout->addLayout(layout, 5);
	ui.verticalLayout->addWidget(ui.consolemsg, 1);


	solverWidget = new SolverWidget(this);
	qualityview = new QualityView(this);

	setWindowTitle("TubeOracle");
	ui.centralWidget->setLayout(ui.horizontalLayout);
	consoleinit();

	//程序主控逻辑初始化
	reset();

	//槽函数配置初始化
	//file菜单链接
	connect(ui.newProject, SIGNAL(triggered()), this, SLOT(newProject(void)));
	connect(ui.openProject, SIGNAL(triggered()), this, SLOT(openProject(void)));

	//添加options窗口链接
	connect(ui.options, SIGNAL(triggered()), this, SLOT(showOptionsWindow()));

	//添加elementsColor窗口链接
	connect(ui.elecolor, SIGNAL(triggered()), this, SLOT(showElementColorWindow()));
	//添加keyboard窗口
	connect(ui.keyboard, SIGNAL(triggered()), this, SLOT(showKeyWindow()));
	//添加about窗口
	connect(ui.about, SIGNAL(triggered()), this, SLOT(showAboutWindow()));
	//打开文档
	connect(ui.documentation, SIGNAL(triggered()), this, SLOT(opendoc()));
	//view过滤器链接
	connect(ui.treeWidget, &QTreeWidget::itemChanged, this, &TubeOracle::view_filtrate);

	//添加addPoint窗口链接
	connect(ui.treeWidget, &QTreeWidget::itemClicked, this, &TubeOracle::showaddPointWindow);
	//添加线链接
	connect(ui.treeWidget, &QTreeWidget::itemClicked, this, &TubeOracle::addLine);
	//添加addRectangle窗口链接
	connect(ui.treeWidget, &QTreeWidget::itemClicked, this, &TubeOracle::showaddRectangleWindow);
	//添加addCircle窗口链接
	connect(ui.treeWidget, &QTreeWidget::itemClicked, this, &TubeOracle::showaddCircleWindow);
	//添加圆弧线链接
	connect(ui.treeWidget, &QTreeWidget::itemClicked, this, &TubeOracle::addCirclearc);
	//添加曲面链接
	connect(ui.treeWidget, &QTreeWidget::itemClicked, this, &TubeOracle::addPlaneSurface);
	//添加curve loop链接
	connect(ui.treeWidget, &QTreeWidget::itemClicked, this, &TubeOracle::addCurveLoop);
	//重置视图
	connect(ui.treeWidget, &QTreeWidget::itemClicked, this, &TubeOracle::on_resetView);
	//删除链接
	connect(ui.treeWidget, &QTreeWidget::itemClicked, this, &TubeOracle::Delete);

	//生成网格链接
	connect(ui.treeWidget, &QTreeWidget::itemClicked, this, &TubeOracle::Generate2D);
	//添加distribution窗口链接
	connect(ui.treeWidget, &QTreeWidget::itemClicked, this, &TubeOracle::showdistributionWindow);
	//网格重组链接
	connect(ui.treeWidget, &QTreeWidget::itemClicked, this, &TubeOracle::Recombine2D);
	//网格平滑处理链接
	connect(ui.treeWidget, &QTreeWidget::itemClicked, this, &TubeOracle::Optimize2D);
	//网格检查
	connect(ui.treeWidget, &QTreeWidget::itemClicked, this, &TubeOracle::Inspect);

	//求解链接
	connect(ui.treeWidget, &QTreeWidget::itemClicked, this, &TubeOracle::Solver);

	//模型配置文件初始化
	loadconfig();
	gmsh::initialize();
	cur_model = nullptr;
	project_dir = "";
	project_name = "";

	//gmsh测试
	//testgmsh();

	//line测试
	//testline();
}

TubeOracle::~TubeOracle()
{
	delete optionsWindow;
	delete elementColorWindow;
	delete trans_window;
	delete mykey;
	delete myabout;
	delete graView;
	delete solverWidget;
	delete qualityview;
	delete cur_model;

}
void TubeOracle::showKeyWindow() {
	if (mykey != nullptr) {
		mykey->show();
	}

}

void TubeOracle::showAboutWindow()
{
	if (myabout != nullptr) {
		myabout->show();
	}
}

void TubeOracle::opendoc()
{
	// 调用默认的PDF查看器打开PDF文件
	try {
		QDesktopServices::openUrl(QUrl::fromLocalFile("manual/TubeOracle.pdf"));
	}
	catch (exception e) {

	}
}

void TubeOracle::consoleinit()
{
	QString initmsg = "- - - - - - - - - - - - - - - - - - - - - - - - - -";
	ui.consolemsg->append(TEXT_COLOR_BLUE(initmsg));
	initmsg = "Product Version  : 1.0";
	ui.consolemsg->append(TEXT_COLOR_BLUE(initmsg));
	initmsg = "License	          : GNU General Public License";
	ui.consolemsg->append(TEXT_COLOR_BLUE(initmsg));
	initmsg = "Build OS	          : Windows X64";
	ui.consolemsg->append(TEXT_COLOR_BLUE(initmsg));
	initmsg = "Gmsh Version         : 4.12.1";
	ui.consolemsg->append(TEXT_COLOR_BLUE(initmsg));
	initmsg = "QT Version              : 5.14.2";
	ui.consolemsg->append(TEXT_COLOR_BLUE(initmsg));
	initmsg = "OCC Version             : 7.7.2";
	ui.consolemsg->append(TEXT_COLOR_BLUE(initmsg));
	initmsg = "QcustomPlot Version  : 2.1.1";
	ui.consolemsg->append(TEXT_COLOR_BLUE(initmsg));
	initmsg = "- - - - - - - - - - - - - - - - - - - - - - - - - -";
	ui.consolemsg->append(TEXT_COLOR_BLUE(initmsg));
	initmsg = "Info  : TubeOracle启动于 " + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
	ui.consolemsg->append(initmsg);
}

void TubeOracle::view_filtrate(QTreeWidgetItem* item, int column)
{

		if (item->checkState(column) == Qt::Checked) {
			cur_model->filtrate(item->text(column), true);
			return;
		}
		else {
			cur_model->filtrate(item->text(column), false);
			return;
		}
}

void TubeOracle::label_filtrate()
{
	
}

void TubeOracle::surface_effect()
{
	
}

void TubeOracle::algorithm_config()
{
	//2D算法设置
	QString algm = optionsWindow->get2D();
	if (algm == "MeshAdapt")	algorithm_2 = 1;
	if (algm == "Automatic")	algorithm_2 = 2;
	if (algm == "Delaunay")		algorithm_2 = 5;
	if (algm == "Frontal - Delaunay")	algorithm_2 = 6;
	if (algm == "BAMG")	algorithm_2 = 7;
	if (algm == "Frontal - Delaunay for Quads")	algorithm_2 = 8;
	if (algm == "Packing of Parallelograms")	algorithm_2 = 9;
	if (algm == "Quasi - structured Quad")	algorithm_2 = 11;
	gmsh::option::setNumber("Mesh.Algorithm", algorithm_2);

	//2D优化算法设置
	if (optionsWindow->getOptim().toStdString() == "Default tetrahedral") {
		optim_2 = "";
		return;
	}
	optim_2 = optionsWindow->getOptim().toStdString();
	
}

void TubeOracle::on_selNodes(Node* node)
{
}

void TubeOracle::open_pnt_sel()
{
	if (cur_model == nullptr)	return;
	//打开点选择器
	for (QGraphicsItem* pnt : cur_model->getscene()->items()) {
		if (dynamic_cast<ItemPoint*>(pnt)) {
			pnt->setFlag(QGraphicsItem::GraphicsItemFlag::ItemIsSelectable, true);
		}
	}
	cur_model->viewupdate();
}

void TubeOracle::close_pnt_sel()
{
	if (cur_model == nullptr)	return;
	//关闭点选择器
	for (QGraphicsItem* pnt : cur_model->getscene()->items()) {
		if (dynamic_cast<ItemPoint*>(pnt)) {
			ItemPoint* temp = (ItemPoint*)(pnt);
			temp->setSelected(false);
			temp->isselected = false;
			temp->setFlag(QGraphicsItem::GraphicsItemFlag::ItemIsSelectable, false);
		}
	}
	cur_model->viewupdate();
}

bool TubeOracle::circle_legal(ItemPoint* start, ItemPoint* center, ItemPoint* end)
{
	if (sqrt(pow(start->px - center->px, 2) + pow(start->py - center->py, 2)) == sqrt(pow(end->px - center->px, 2) + pow(end->py - center->py, 2))) {
		return true;
	}
	return false;
}

QList<int> TubeOracle::loop_legal()
{
	//参数为成员loop_box

	QList<int> loops;
	if (loop_box.size()<=1) {
		return loops; // 返回空列表
	}
	// 从第一条边开始
	int starttag = 0;
	int nexttag = 0;
	if (dynamic_cast<ItemLine*>(loop_box.front())) {
		ItemLine* currentCurve = (ItemLine*)loop_box.front();
		loops.push_back(currentCurve->tag);
		// 从第一条边的起点开始寻找下一条边
		nexttag = currentCurve->path.back();
		starttag = currentCurve->path.front();
		loop_box.pop_front();
	}
	else if (dynamic_cast<ItemCurve*>(loop_box.front())) {
		ItemCurve* currentCurve = (ItemCurve*)loop_box.front();
		loops.push_back(currentCurve->tag);
		// 从第一条边的起点开始寻找下一条边
		nexttag = currentCurve->path.back();
		starttag = currentCurve->path.front();
		loop_box.pop_front();

	}
	// 遍历边集合，直到闭环完成
	while (!loop_box.empty()) {
		for (auto it = loop_box.begin(); it != loop_box.end(); ++it) {
			QGraphicsObject* obj = *it;
			if (ItemLine* line = dynamic_cast<ItemLine*>(obj)) {
				if (line->path.front() == nexttag) {
					loops.push_back(line->tag);
					nexttag = line->path.back();
					loop_box.erase(it);
					break;
				}
				else if (line->path.back() == nexttag) {
					// 找到下一条边，但需要颠倒方向
					loops.push_back(-1*(line->tag));
					nexttag = line->path.front();
					loop_box.erase(it);
					break;
				}
			}
			else if (ItemCurve* cvs = dynamic_cast<ItemCurve*>(obj)) {
				if (cvs->path.front() == nexttag) {
					loops.push_back(cvs->tag);
					nexttag = cvs->path.back();
					loop_box.erase(it);
					break;
				}
				else if (cvs->path.back() == nexttag) {
					// 找到下一条边，但需要颠倒方向
					loops.push_back(-1 * (cvs->tag));
					nexttag = cvs->path.front();
					loop_box.erase(it);
					break;
				}

			}
	
		}
	}
	// 判断闭环是否完成
	if (loops.size() >=2 && starttag == nexttag) {
		return loops;
	}
	else {
		loops.clear();
		return loops;
	}
}

CurveLoop* TubeOracle::find_loop(ItemLine* curve)
{
	for (CurveLoop* lp : cur_model->loops) {
		for (int t : lp->loop_path) {
			if (t == curve->tag || t == -1 * curve->tag)	return lp;
		}
	}
	return nullptr;
}
CurveLoop* TubeOracle::find_loop(ItemCurve* curve)
{
	for (CurveLoop* lp : cur_model->loops) {
		for (int t : lp->loop_path) {
			if (t == curve->tag || t == -1 * curve->tag)	return lp;
		}
	}
	return nullptr;
}

QGraphicsObject* TubeOracle::find_curve(int tag)
{
	for (QGraphicsItem* obj : cur_model->getscene()->items()) {
		if (dynamic_cast<ItemLine*>(obj)) {
			ItemLine* line = (ItemLine*)obj;
			if (line->tag == tag || line->tag == -tag)	return line;
		}
		else if (dynamic_cast<ItemCurve*>(obj)) {
			ItemCurve* cvs = (ItemCurve*)obj;
			if (cvs->tag == tag || cvs->tag== -tag)	return cvs;
		}
	}
	return nullptr;
}

void TubeOracle::open_curves_sel()
{
	if (cur_model == nullptr)	return;
	//打开线选择器
	for (QGraphicsItem* cvs : cur_model->getscene()->items()) {
		if (dynamic_cast<ItemLine*>(cvs) || dynamic_cast<ItemCurve*>(cvs)) {
			cvs->setFlag(QGraphicsItem::GraphicsItemFlag::ItemIsSelectable, true);
		}
	}
	cur_model->viewupdate();
}

void TubeOracle::close_curves_sel()
{
	if (cur_model == nullptr)	return;
	//关闭线选择器
	for (QGraphicsItem* cvs : cur_model->getscene()->items()) {
		if (dynamic_cast<ItemLine*>(cvs)) {
			ItemLine* temp = (ItemLine*)(cvs);
			temp->setSelected(false);
			temp->isselected = false;
			temp->setFlag(QGraphicsItem::GraphicsItemFlag::ItemIsSelectable, false);
		}
		else if (dynamic_cast<ItemCurve*>(cvs)) {
			ItemCurve* temp = (ItemCurve*)(cvs);
			temp->setSelected(false);
			temp->isselected = false;
			temp->setFlag(QGraphicsItem::GraphicsItemFlag::ItemIsSelectable, false);
		}
	}
	cur_model->viewupdate();
}

void TubeOracle::reset()
{
	flag_working = false;
	line_flag = false;
	sirc_flag = false;
	loop_flag = false;
	plane_flag = false;
	trans_flag = false;
	line_box.clear();
	sircle_box.clear();
	loop_box.clear();
	plane_box.clear();
	trans_box.clear();
	close_pnt_sel();
	close_curves_sel();
	graView->setEnabled(flag_working);
	ui.treeWidget->setEnabled(flag_working);
}

void TubeOracle::loadconfig()
{
	try {
		//读取CONFIGURE
		QFile file("configs/CONFIGURE.txt");
		if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QTextStream stream(&file);
			QString text_line;
			while (!stream.atEnd()) {
				text_line = stream.readLine();
				if (text_line.contains("geoscale")) {
					sscanf_s(text_line.toStdString().c_str(), "geoscale=%lf", &geoscale);
					optionsWindow->set_geoscale(geoscale);
					continue;
				}
				if (text_line.contains("edgewidth")) {
					sscanf_s(text_line.toStdString().c_str(), "edgewidth=%lf", &edgewidth);
					optionsWindow->set_edgewidth(edgewidth);
					continue;
				}
				if (text_line.contains("lcc")) {	//无控件
					sscanf_s(text_line.toStdString().c_str(), "lcc=%lf", &lcc);
					continue;
				}
				if (text_line.contains("curvewidth")) {
					sscanf_s(text_line.toStdString().c_str(), "curvewidth=%lf", &curvewidth);
					optionsWindow->set_curvewidth(curvewidth);
					continue;
				}
				if (text_line.contains("selectcurvewidth")) {
					sscanf_s(text_line.toStdString().c_str(), "selectcurvewidth=%lf", &selectcurvewidth);
					optionsWindow->set_selectcurvewidth(selectcurvewidth);
					continue;
				}
				if (text_line.contains("pointsize")) {
					sscanf_s(text_line.toStdString().c_str(), "pointsize=%lf", &pointsize);
					optionsWindow->set_pointsize(pointsize);
					continue;
				}
				if (text_line.contains("selectpointsize")) {
					sscanf_s(text_line.toStdString().c_str(), "selectpointsize=%lf", &selectpointsize);
					optionsWindow->set_selectpointsize(selectpointsize);
					continue;
				}
				if (text_line.contains("nodesize")) {
					sscanf_s(text_line.toStdString().c_str(), "nodesize=%lf", &nodesize);
					optionsWindow->set_nodesize(nodesize);
					continue;
				}
				if (text_line.contains("selectnodesize")) {	//无控件
					sscanf_s(text_line.toStdString().c_str(), "selectnodesize=%lf", &selectnodesize);
					continue;
				}
				if (text_line.contains("dashlinewidth")) {	//无控件
					sscanf_s(text_line.toStdString().c_str(), "dashlinewidth=%lf", &dashlinewidth);
					continue;
				}

			}
			file.close();
		}
	}
	catch (exception e) {
		return;
	}

}

void TubeOracle::keyPressEvent(QKeyEvent* event)
{
	if (event->modifiers() == Qt::ControlModifier) {
		if (event->key() == Qt::Key_N) {
			newProject();
			return;
		}
		if (event->key() == Qt::Key_O) {
			openProject();
			return;
		}
		if (event->key() == Qt::Key_M) {
			//最大化
			if (!isMaximized()) {
				showMaximized();
			} 
			return;
		}
	}

	if (event->modifiers() == Qt::AltModifier) {
		if (event->key() == Qt::Key_O) {
			showOptionsWindow();
			return;
		}
		if (event->key() == Qt::Key_D) {
			//文档
			opendoc();
			return;
		}
		if (event->key() == Qt::Key_K) {
			showKeyWindow();
			return;
		}
		if (event->key() == Qt::Key_C) {
			showElementColorWindow();
			return;
		}
		if (event->key() == Qt::Key_A) {
			showAboutWindow();
			return;
		}

	}
	
}

void TubeOracle::globalconfig()
{
	if (optionsWindow == nullptr)	return;
	
	try {
		//写入CONFIGURE
		QFile file("configs/CONFIGURE.txt");
		if (file.open(QIODevice::WriteOnly)) {
			QTextStream stream(&file);
			QString s1;

			geoscale = optionsWindow->get_geoscale();
			s1 = "geoscale="+ QString::number(geoscale);
			stream << s1 << endl;

			edgewidth = optionsWindow->get_edgewidth();
			s1 = "edgewidth=" + QString::number(edgewidth);
			stream << s1 << endl;

			lcc = 1.0;
			s1 = "lcc=1.0";//*********默认值**********
			stream << s1 << endl;

			curvewidth = optionsWindow->get_curvewidth();
			s1 = "curvewidth=" + QString::number(curvewidth);
			stream << s1 << endl;

			selectcurvewidth = optionsWindow->get_selectcurvewidth();
			s1 = "selectcurvewidth=" + QString::number(selectcurvewidth);
			stream << s1 << endl;

			s1 = "selectnodesize=3.5";//**********默认值*************
			stream << s1 << endl;

			pointsize = optionsWindow->get_pointsize();
			s1 = "pointsize=" + QString::number(pointsize);
			stream << s1 << endl;

			selectpointsize = optionsWindow->get_selectpointsize();
			s1 = "selectpointsize=" + QString::number(selectpointsize);
			stream << s1 << endl;

			nodesize = optionsWindow->get_nodesize();
			s1 = "nodesize=" + QString::number(nodesize);
			stream << s1 << endl;

			dashlinewidth = 0.8;
			s1 = "dashlinewidth=0.8";//**********默认值************
			stream << s1 << endl;

			file.close();
		}
	}
	catch (exception e) {
		return;
	}
	if (cur_model) {
		cur_model->update_surface_geo();
		cur_model->viewupdate();
	}	
}

void TubeOracle::newProject()
{
	newproPanel* newpro = new newproPanel(this);
	newpro->show();
}

void TubeOracle::on_creted_pro(bool iscr, QString prodir, QString proname)
{
	project_dir = prodir.toStdString();
	project_name = proname.toStdString();
	QString msg = "Info  : 已创建工程: " + prodir;
	ui.consolemsg->append(msg);
	
	this->setWindowTitle("TubeOracle - " + prodir + "/" + proname + ".geo");
	if (cur_model != nullptr)	cur_model->~GModel();
	cur_model = nullptr;
	cur_model = new GModel(project_dir, project_name, graView, this);
	cur_model->loadmodel(prodir + "/" + proname + ".geo");
	reset();
	flag_working = true;
	ui.treeWidget->setEnabled(flag_working);
	graView->setEnabled(flag_working);
	msg = "Info  : 已打开工程: " + prodir;
	ui.consolemsg->append(msg);
}

void TubeOracle::openProject()
{
	bool last_working = flag_working;
	//打开目录选择，指定geo文件打开
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Geometry (*.geo)"));
	if (fileName=="") {
		reset();
		QString msg = "Error : 无法打开工程文件: " + fileName;
		ui.consolemsg->append(TEXT_COLOR_RED(msg));
		this->setWindowTitle("TubeOracle");
		ui.treeWidget->setEnabled(last_working);
		graView->setEnabled(last_working);
		return;
	}

	//获取文件目录及裸文件名,创建GModel
	QFileInfo fileinfo(fileName);
	project_dir = fileinfo.absolutePath().toStdString();
	project_name = fileinfo.baseName().toStdString();
	QString suffix = fileinfo.suffix();

	if (cur_model != nullptr)	cur_model->~GModel();
	cur_model = nullptr;
	cur_model = new GModel(project_dir, project_name, graView, this);
	bool fin = cur_model->loadmodel(fileName);
	if (fin) 
	{
		reset();
		flag_working = true;
		ui.treeWidget->setEnabled(flag_working);
		graView->setEnabled(flag_working);
		ui.consolemsg->append("Info  : 已打开工程: " + fileName); 
		this->setWindowTitle("TubeOracle - "+fileName);
		
	}
	else {
		reset();
		QString msg = "Error : 无法打开工程文件: " + fileName;
		ui.consolemsg->append(TEXT_COLOR_RED(msg));
		this->setWindowTitle("TubeOracle");
		ui.treeWidget->setEnabled(last_working);
		graView->setEnabled(last_working);
	}
}

void TubeOracle::showOptionsWindow() {
	if (optionsWindow != nullptr)
		optionsWindow->show();
}

void TubeOracle::showElementColorWindow() {
	if (elementColorWindow != nullptr)
		elementColorWindow->show();
}



void TubeOracle::showaddPointWindow(QTreeWidgetItem* item, int row) {
	QString itemName = item->text(row);
	//判断是否为对应组件
	if (itemName == "点(实体)") {
		// 生成窗口
		addPoint* addPointWindow = new addPoint(this);
		addPointWindow->show();
	}
}


void TubeOracle::addLine(QTreeWidgetItem* item, int row)
{
	QString itemName = item->text(row);
	//判断是否为对应组件
	if (itemName == "直线") {
		QString msg1 = "Info  : 启动选择模式 [按'q'退出]";
		ui.consolemsg->append(TEXT_COLOR_GREEN(msg1));
		msg1 = "Wait  : 请选择直线起点 [添加直线]";
		ui.consolemsg->append(TEXT_COLOR_GREEN(msg1));
		close_pnt_sel();
		line_box.clear();
		sircle_box.clear();
		plane_box.clear();
		loop_box.clear();
		trans_box.clear();
		line_flag = true;
		sirc_flag = false;
		loop_flag = false;
		plane_flag = false;
		trans_flag = false;
		open_pnt_sel();
	}
}
void TubeOracle::showaddRectangleWindow(QTreeWidgetItem* item, int row) {
	QString itemName = item->text(row);
	//判断是否为对应组件
	if (itemName == "矩形") {
		// 生成窗口
		addRectangle* addRectangleWindow = new addRectangle(this);
		addRectangleWindow->show();
	}
}


void TubeOracle::showaddCircleWindow(QTreeWidgetItem* item, int row) {
	QString itemName = item->text(row);
	//判断是否为对应组件
	if (itemName == "圆") {
		// 生成窗口
		addCircle* addCircleWindow = new addCircle(this);
		addCircleWindow->show();
	}
}


void TubeOracle::addCirclearc(QTreeWidgetItem* item, int row)
{
	QString itemName = item->text(row);
	//判断是否为对应组件
	if (itemName == "圆弧") {
		QString msg1 = "Info  : 启动选择模式 [按'q'退出]";
		ui.consolemsg->append(TEXT_COLOR_GREEN(msg1));
		msg1 = "Wait  : 请选择圆弧起点 [添加圆弧]";
		ui.consolemsg->append(TEXT_COLOR_GREEN(msg1));
		close_pnt_sel();
		line_box.clear();
		sircle_box.clear();
		plane_box.clear();
		loop_box.clear();
		trans_box.clear();
		line_flag = false;
		sirc_flag = true;
		loop_flag = false;
		plane_flag = false;
		trans_flag = false;
		open_pnt_sel();
	}
}


void TubeOracle::addCurveLoop(QTreeWidgetItem* item, int row)
{
	QString itemName = item->text(row);
	//判断是否为对应组件
	if (itemName == "曲线闭环") {
		QString msg1 = "Info  : 启动选择模式 [按'q'退出]";
		ui.consolemsg->append(TEXT_COLOR_GREEN(msg1));
		msg1 = "Wait  : 请选择曲线构造闭环 [按'e'确认]";
		ui.consolemsg->append(TEXT_COLOR_GREEN(msg1));
		close_curves_sel();
		line_box.clear();
		sircle_box.clear();
		plane_box.clear();
		loop_box.clear();
		trans_box.clear();
		loop_flag = true;
		line_flag = false;
		sirc_flag = false;
		plane_flag = false;
		trans_flag = false;
		open_curves_sel();
	}
}


void TubeOracle::addPlaneSurface(QTreeWidgetItem* item, int row)
{
	QString itemName = item->text(row);
	//判断是否为对应组件
	if (itemName == "二维平面") {
		QString msg1 = "Info  : 启动选择模式 [按'q'退出]";
		ui.consolemsg->append(TEXT_COLOR_GREEN(msg1));
		msg1 = "Wait  : 请选择曲线闭环构造二维平面 [按'e'确认]";
		ui.consolemsg->append(TEXT_COLOR_GREEN(msg1));
		close_curves_sel();
		line_box.clear();
		sircle_box.clear();
		plane_box.clear();
		loop_box.clear();
		trans_box.clear();
		loop_flag = false;
		line_flag = false;
		sirc_flag = false;
		plane_flag = true;
		trans_flag = false;
		open_curves_sel();
	}
}

void TubeOracle::on_resetView(QTreeWidgetItem* item, int row)
{
	QString itemName = item->text(row);
	//判断是否为对应组件
	if (itemName == "重置视图缩放") {
		graView->resetScale();
	}
}



void TubeOracle::Delete(QTreeWidgetItem* item, int row)
{
	QString itemName = item->text(row);
	//判断是否为对应组件
	if (itemName == "删除") {

	}
}



void TubeOracle::Generate2D(QTreeWidgetItem* item, int row)
{
	QString itemName = item->text(row);
	//判断是否为对应组件
	if (itemName == "2D") {
		if (cur_model->generate2D()) {
			QString msg = "Info  : 已输出网格 " + QString::fromStdString(project_dir + "/" + project_name + ".msh");
			ui.consolemsg->append(msg);
			ui.treeWidget->topLevelItem(0)->child(3)->setCheckState(0, Qt::Checked);
		}
		else {
			QString msg = "Error : 生成网格失败";
			ui.consolemsg->append(TEXT_COLOR_RED(msg));
		}
	}

}


void TubeOracle::showdistributionWindow(QTreeWidgetItem* item, int row) {
	QString itemName = item->text(row);
	//判断是否为对应组件
	if (itemName == "网格约束") {
		if (trans_window != nullptr)	trans_window->show();

		QString msg1 = "Info  : 启动选择模式 [按'q'退出]";
		ui.consolemsg->append(TEXT_COLOR_GREEN(msg1));
		msg1 = "Wait  : 请选择曲线设置网格约束 [按'e'确认]";
		ui.consolemsg->append(TEXT_COLOR_GREEN(msg1));
		close_curves_sel();
		line_box.clear();
		sircle_box.clear();
		plane_box.clear();
		loop_box.clear();
		trans_box.clear();
		loop_flag = false;
		line_flag = false;
		sirc_flag = false;
		plane_flag = false;
		trans_flag = true;
		open_curves_sel();
	}
}


void TubeOracle::Recombine2D(QTreeWidgetItem* item, int row)
{
	QString itemName = item->text(row);
	//判断是否为对应组件
	if (itemName == "2D网格重组") {
		/*if (cur_model->Recombine2D()) {
			QString msg = "Info  : 已重组网格 " + QString::fromStdString(project_dir + "/" + project_name + ".msh");
			ui.consolemsg->append(msg);
			msg = "Info  : 已输出网格 " + QString::fromStdString(project_dir + "/" + project_name + ".msh");
			ui.consolemsg->append(msg);
			ui.treeWidget->topLevelItem(0)->child(3)->setCheckState(0, Qt::Checked);
		}
		else {
			QString msg = "Error : 网格重组失败";
			ui.consolemsg->append(TEXT_COLOR_RED(msg));
		}*/
	} 
}


void TubeOracle::Optimize2D(QTreeWidgetItem* item, int row)
{
	QString itemName = item->text(row);
	//判断是否为对应组件
	if (itemName == "2D网格优化") {
		if (cur_model->Optimize()) {
			QString msg = "Info  : 已优化网格 " + QString::fromStdString(project_dir + "/" + project_name + ".msh");
			ui.consolemsg->append(msg);
			msg = "Info  : 已输出网格 " + QString::fromStdString(project_dir + "/" + project_name + ".msh");
			ui.consolemsg->append(msg);
			ui.treeWidget->topLevelItem(0)->child(3)->setCheckState(0, Qt::Checked);
		}
		else {
			QString msg = "Error : 优化网格失败";
			ui.consolemsg->append(TEXT_COLOR_RED(msg));
		}
	}
}


void TubeOracle::Inspect(QTreeWidgetItem* item, int row)
{
	QString itemName = item->text(row);
	//判断是否为对应组件
	if (itemName == "质量检查") {
		if (qualityview != nullptr) {
			QVector<qreal> quality;
			cur_model->getQuality(quality);
			qualityview->setData(quality);
			qualityview->show();
		}
	}
}



void TubeOracle::Solver(QTreeWidgetItem* item, int row)
{
	QString itemName = item->text(row);
	//判断是否为对应组件
	if (itemName == "求解器") {
		solverWidget->show();
	}
	
}


void TubeOracle::transfinite_get(QGraphicsObject* cvs)
{
	QString msg1 = "";
	if (trans_flag) {
		if (dynamic_cast<ItemLine*>(cvs)) {
			ItemLine* temp = (ItemLine*)(cvs);
			temp->isselected = true;
			cur_model->viewupdate();
			trans_box.push_back(temp);
			msg1 = "Info  : 已选择直线(" + QString::number(temp->tag) + "):{" + QString::number(temp->p1->tag) + "," + QString::number(temp->p2->tag) + "}";
			ui.consolemsg->append(msg1);
			msg1 = "Wait  : 请选择曲线设置网格约束 [按'e'确认]";
			ui.consolemsg->append(TEXT_COLOR_GREEN(msg1));
			return;
		}
		else if (dynamic_cast<ItemCurve*>(cvs)) {
			ItemCurve* temp = (ItemCurve*)(cvs);
			temp->isselected = true;
			cur_model->viewupdate();
			trans_box.push_back(temp);
			msg1 = "Info  : 已选择圆弧(" + QString::number(temp->tag) + "):{" + QString::number(temp->ps->tag) + "," + QString::number(temp->pc->tag) + "," + QString::number(temp->pe->tag) + "}";
			ui.consolemsg->append(msg1);
			msg1 = "Wait  : 请选择曲线设置网格约束 [按'e'确认]";
			ui.consolemsg->append(TEXT_COLOR_GREEN(msg1));
			return;
		}
	}

}

void TubeOracle::on_settransfinite()
{
	if (trans_flag) {
		QString msg1 = "";
		if (trans_box.isEmpty()) {
			msg1 = "Error : 设置网格约束失败";
			ui.consolemsg->append(TEXT_COLOR_RED(msg1));
			trans_box.clear();
			close_curves_sel();
			open_curves_sel();
			return;
		}
		//操作GModel添加curves网格约束
		QList<int> curveTags;
		for (auto obj : trans_box) {
			if (dynamic_cast<ItemLine*>(obj)) {
				ItemLine* line = (ItemLine*)obj;
				curveTags.push_back(line->tag);
				continue;
			}
			else if (dynamic_cast<ItemCurve*>(obj)) {
				ItemCurve* cvs = (ItemCurve*)obj;
				curveTags.push_back(cvs->tag);
				continue;
			}
		}
		if (cur_model->setTransfinite(curveTags, trans_window->getText().toInt())) {
			QStringList tt;
			for (int tag : curveTags) {
				tt << QString::number(tag);
			}
			msg1 = "{" + tt.join(", ") + "}";
			msg1 = "Info  : 已设置网格约束" + msg1 + "=" + trans_window->getText();
			ui.consolemsg->append(msg1);
			trans_box.clear();
			msg1 = "Wait  : 请选择曲线设置网格约束 [按'e'确认]";
			ui.consolemsg->append(TEXT_COLOR_GREEN(msg1));
			close_curves_sel();
			open_curves_sel();
			return;
		}
		else {
			msg1 = "Error : 设置网格约束失败";
			ui.consolemsg->append(TEXT_COLOR_RED(msg1));
			trans_box.clear();
			close_curves_sel();
			open_curves_sel();
			return;
		}
	}

}


void TubeOracle::on_recv_curves(QGraphicsObject* cvs)
{
	
	if (loop_flag) {
		QString msg1 = "";
		if (dynamic_cast<ItemLine*>(cvs)) {
			ItemLine* temp = (ItemLine*)(cvs);
			temp->isselected = true;
			cur_model->viewupdate();
			loop_box.push_back(temp);
			msg1 = "Info  : 已选择直线(" + QString::number(temp->tag) + "):{" + QString::number(temp->p1->tag) + "," + QString::number(temp->p2->tag) + "}";
			ui.consolemsg->append(msg1);
			msg1 = "Wait  : 请选择曲线构造闭环 [按'e'确认]";
			ui.consolemsg->append(TEXT_COLOR_GREEN(msg1));
			return;
		}
		else if (dynamic_cast<ItemCurve*>(cvs)) {
			ItemCurve* temp = (ItemCurve*)(cvs);
			temp->isselected = true;
			cur_model->viewupdate();
			loop_box.push_back(temp);
			msg1 = "Info  : 已选择圆弧(" + QString::number(temp->tag) + "):{" + QString::number(temp->ps->tag) + "," + QString::number(temp->pc->tag) + "," + QString::number(temp->pe->tag) + "}";
			ui.consolemsg->append(msg1);
			msg1 = "Wait  : 请选择曲线构造闭环 [按'e'确认]";
			ui.consolemsg->append(TEXT_COLOR_GREEN(msg1));
			return;
		}

	}
	else if (plane_flag) {
		QString msg1 = "";
		if (dynamic_cast<ItemLine*>(cvs)) {
			ItemLine* temp = (ItemLine*)(cvs);
			CurveLoop* sel_loop = find_loop(temp);
			if (sel_loop != nullptr) {
				plane_box.push_back(sel_loop);
				for (int curve_tag : sel_loop->loop_path) {
					QGraphicsObject* obj = find_curve(curve_tag);
					if (!(obj == nullptr)) {
						if (dynamic_cast<ItemLine*>(obj)) {
							ItemLine* tl = (ItemLine*)obj;
							tl->isselected = true;
						}
						else if (dynamic_cast<ItemCurve*>(obj)) {
							ItemCurve* tc = (ItemCurve*)obj;
							tc->isselected = true;
						}
					}
					else
					{
						break;
					}

				}
				cur_model->viewupdate();
				QStringList tt;
				for (int t : sel_loop->loop_path) {
					tt << QString::number(t);
				}
				msg1 = "{" + tt.join(", ") + "}";
				msg1 = "Info  : 已选择曲线闭环(" + QString::number(sel_loop->tag) + "):" + msg1;
				ui.consolemsg->append(msg1);
				msg1 = "Wait  : 请选择曲线闭环构造二维平面 [按'e'确认]";
				ui.consolemsg->append(TEXT_COLOR_GREEN(msg1));
				return;
			}
			else
			{
				return;
			}

		}
		else if (dynamic_cast<ItemCurve*>(cvs)) {
			ItemCurve* temp = (ItemCurve*)(cvs);
			CurveLoop* sel_loop = find_loop(temp);
			if (sel_loop != nullptr) {
				plane_box.push_back(sel_loop);
				for (int curve_tag : sel_loop->loop_path) {
					QGraphicsObject* obj = find_curve(curve_tag);
					if (!(obj == nullptr)) {
						if (dynamic_cast<ItemLine*>(obj)) {
							ItemLine* tl = (ItemLine*)obj;
							tl->isselected = true;
						}
						else if (dynamic_cast<ItemCurve*>(obj)) {
							ItemCurve* tc = (ItemCurve*)obj;
							tc->isselected = true;
						}
					}
					else
					{
						break;
					}

				}
				cur_model->viewupdate();
				QStringList tt;
				for (int t : sel_loop->loop_path) {
					tt << QString::number(t);
				}
				msg1 = "{" + tt.join(", ") + "}";
				msg1 = "Info  : 已选择曲线闭环(" + QString::number(sel_loop->tag) + "):" + msg1;
				ui.consolemsg->append(msg1);
				msg1 = "Wait  : 请选择曲线闭环构造二维平面 [按'e'确认]";
				ui.consolemsg->append(TEXT_COLOR_GREEN(msg1));
				return;
			}
			else
			{
				return;
			}
		}

	}


	return;
}


bool TubeOracle::on_addLoop()
{
	
	if (loop_flag) {
		QString msg1 = "";
		int fin = 0;
		//检验Loop合法性
		QList<int> loops = loop_legal();
		if (loops.empty()) {
			msg1 = "Error : 添加曲线闭环失败";
			ui.consolemsg->append(TEXT_COLOR_RED(msg1));
			loop_box.clear();
			close_curves_sel();
			open_curves_sel();
			return false;
		}
		//操作GModel添加Loop
		fin = cur_model->addLoop(loops);
		QStringList tt;
		for (int tag : loops) {
			tt << QString::number(tag);
		}
		msg1 = "{" + tt.join(", ") + "}";
		msg1 = "Info  : 已添加曲线闭环(" + QString::number(fin) + "):" + msg1;
		ui.consolemsg->append(msg1);
		loop_box.clear();
		msg1 = "Wait  : 请选择曲线构造闭环 [按'e'确认]";
		ui.consolemsg->append(TEXT_COLOR_GREEN(msg1));
		close_curves_sel();
		open_curves_sel();
		return fin;
	}

	return false;
}


bool TubeOracle::on_addSurface()
{
	
	if (plane_flag) {
		QString msg1 = "";
		int fin = 0;
		if (plane_box.size() > 0) {
			fin = cur_model->addsurface(plane_box);
			//状态栏提示
			QStringList tt;
			for (CurveLoop* cl : plane_box) {
				tt << QString::number(cl->tag);
			}
			msg1 = "{" + tt.join(", ") + "}";
			msg1 = "Info  : 已添加二维平面(" + QString::number(fin) + "):" + msg1;
			ui.consolemsg->append(msg1);
			plane_box.clear();
			msg1 = "Wait  : 请选择曲线闭环构造二维平面 [按'e'确认]";
			ui.consolemsg->append(TEXT_COLOR_GREEN(msg1));
			close_curves_sel();
			open_curves_sel();
			return fin;
		}
		else {
			//状态栏提示
			msg1 = "Error : 添加二维平面失败";
			ui.consolemsg->append(TEXT_COLOR_RED(msg1));
			plane_box.clear();
			close_curves_sel();
			open_curves_sel();
			return false;
		}
	}
	return false;
}


bool TubeOracle::on_addpnt(double pointx, double pointy)
{
	//操作GModel添加point
	int fin = cur_model->addpoint(pointx, pointy);
	//状态栏msg
	QString msg = "Info  : 已添加点(" + QString::number(fin) + "):{" + QString::number(pointx) + "," + QString::number(pointy) + "}";
	ui.consolemsg->append(msg);
	return fin;
}


bool TubeOracle::on_addline(ItemPoint* point)
{
	
	if (line_flag) {
		QString msg1 = "";
		int fin = 0;
		point->isselected = true;
		cur_model->viewupdate();
		switch (line_box.size())
		{
			case 0:
				line_box.push_back(point);
				msg1 = "Info  : 已选择直线起点(" + QString::number(point->tag) + "):{" + QString::number(point->px) + "," + QString::number(point->py) + "}";
				ui.consolemsg->append(msg1);
				msg1 = "Wait  : 请选择直线终点 [添加直线]";
				ui.consolemsg->append(TEXT_COLOR_GREEN(msg1));
				return true;
			case 1:
				line_box.push_back(point);
				msg1 = "Info  : 已选择直线终点(" + QString::number(point->tag) + "):{" + QString::number(point->px) + "," + QString::number(point->py) + "}";
				ui.consolemsg->append(msg1);
				//end点选择完成,生成对应直线
				fin = cur_model->addline(line_box.at(0), line_box.at(1));
				msg1 = "Info  : 已添加直线(" + QString::number(fin) + "):{" + QString::number(line_box.at(0)->tag) + "," + QString::number(line_box.at(1)->tag) + "}";
				ui.consolemsg->append(msg1);
				line_box.clear();
				msg1 = "Wait  : 请选择直线起点 [添加直线]";
				ui.consolemsg->append(TEXT_COLOR_GREEN(msg1));
				close_pnt_sel();
				open_pnt_sel();
				return true;
			default:
				return true;
		}
	}
	return false;
}


bool TubeOracle::on_addcirclearc(ItemPoint* point)
{
	
	if (sirc_flag) {
		QString msg1 = "";
		int fin = 0;
		point->isselected = true;
		cur_model->viewupdate();
		switch (sircle_box.size())
		{
		case 0:
			sircle_box.push_back(point);
			msg1 = "Info  : 已选择圆弧起点(" + QString::number(point->tag) + "):{" + QString::number(point->px) + "," + QString::number(point->py) + "}";
			ui.consolemsg->append(msg1);
			msg1 = "Wait  : 请选择圆弧中心 [添加圆弧]";
			ui.consolemsg->append(TEXT_COLOR_GREEN(msg1));
			return true;
		case 1:
			sircle_box.push_back(point);
			msg1 = "Info  : 已选择圆弧中心(" + QString::number(point->tag) + "):{" + QString::number(point->px) + "," + QString::number(point->py) + "}";
			ui.consolemsg->append(msg1);
			msg1 = "Wait  : 请选择圆弧终点 [添加圆弧]";
			ui.consolemsg->append(TEXT_COLOR_GREEN(msg1));
			return true;
		case 2:
			sircle_box.push_back(point);
			msg1 = "Info  : 已选择圆弧终点(" + QString::number(point->tag) + "):{" + QString::number(point->px) + "," + QString::number(point->py) + "}";
			ui.consolemsg->append(msg1);
			//end点选择完成,生成对应圆弧
			if (circle_legal(sircle_box.at(0), sircle_box.at(1), sircle_box.at(2))) {
				fin = cur_model->addcirclearc(sircle_box.at(0), sircle_box.at(1), sircle_box.at(2));
				if (fin == 0) {
					msg1 = "Error : 添加圆弧失败";
					ui.consolemsg->append(TEXT_COLOR_RED(msg1));
					sircle_box.clear();
					close_pnt_sel();
					open_pnt_sel();
					return false;
				}
				msg1 = "Info  : 已添加圆弧(" + QString::number(fin) + "):{" + QString::number(sircle_box.at(0)->tag) + "," + QString::number(sircle_box.at(1)->tag) + "," + QString::number(sircle_box.at(2)->tag) + "}";
				ui.consolemsg->append(msg1);
				sircle_box.clear();
				msg1 = "Wait  : 请选择圆弧起点 [添加圆弧]";
				ui.consolemsg->append(TEXT_COLOR_GREEN(msg1));
				close_pnt_sel();
				open_pnt_sel();
				return true;
			}
			else {
				msg1 = "Error : 添加圆弧失败";
				ui.consolemsg->append(TEXT_COLOR_RED(msg1));
				sircle_box.clear();
				close_pnt_sel();
				open_pnt_sel();
				return false;
			}
		default:
			return true;
		}
	}
	return false;
}


bool TubeOracle::on_addrect(double x, double y, double width, double height)
{
	int tag1, tag2, tag3, tag4;
	int fin1, fin2, fin3, fin4;
	QString msg = "Wait  : 添加矩形 [不含闭环和平面]";
	ui.consolemsg->append(TEXT_COLOR_GREEN(msg));

	tag1 = cur_model->addpoint(x, y);
	msg = "Info  : 已添加点(" + QString::number(tag1) + "):{" + QString::number(x) + "," + QString::number(y) + "}";
	ui.consolemsg->append(msg);

	tag2 = cur_model->addpoint(x, y+height);
	msg = "Info  : 已添加点(" + QString::number(tag2) + "):{" + QString::number(x) + "," + QString::number(y+height) + "}";
	ui.consolemsg->append(msg);

	ItemPoint* start = cur_model->getPoint(tag1);
	ItemPoint* end = cur_model->getPoint(tag2);
	fin1 = cur_model->addline(start, end);
	msg = "Info  : 已添加直线(" + QString::number(fin1) + "):{" + QString::number(tag1) + "," + QString::number(tag2) + "}";
	ui.consolemsg->append(msg);

	tag3 = cur_model->addpoint(x + width, y + height);
	msg = "Info  : 已添加点(" + QString::number(tag3) + "):{" + QString::number(x + width) + "," + QString::number(y + height) + "}";
	ui.consolemsg->append(msg);

	start = end;
	end = cur_model->getPoint(tag3);
	fin2 = cur_model->addline(start, end);
	msg = "Info  : 已添加直线(" + QString::number(fin2) + "):{" + QString::number(tag2) + "," + QString::number(tag3) + "}";
	ui.consolemsg->append(msg);

	tag4 = cur_model->addpoint(x + width, y);
	msg = "Info  : 已添加点(" + QString::number(tag4) + "):{" + QString::number(x + width) + "," + QString::number(y) + "}";
	ui.consolemsg->append(msg);

	start = end;
	end = cur_model->getPoint(tag4);
	fin3 = cur_model->addline(start, end);
	msg = "Info  : 已添加直线(" + QString::number(fin3) + "):{" + QString::number(tag3) + "," + QString::number(tag4) + "}";
	ui.consolemsg->append(msg);

	start = end;
	end = cur_model->getPoint(tag1);
	fin4 = cur_model->addline(start, end);
	msg = "Info  : 已添加直线(" + QString::number(fin4) + "):{" + QString::number(tag4) + "," + QString::number(tag1) + "}";
	ui.consolemsg->append(msg);

	msg = "Info  : 已添加矩形";
	ui.consolemsg->append(TEXT_COLOR_GREEN(msg));

	return true;
}


bool TubeOracle::on_addcircle(double cx, double cy, double radius)
{
	int fin;
	int tag1, tag2, tag3, tag4;
	QString msg = "Wait  : 添加圆 [不含闭环和平面]";
	ui.consolemsg->append(TEXT_COLOR_GREEN(msg));

	fin = cur_model->addpoint(cx, cy);
	msg = "Info  : 已添加点(" + QString::number(fin) + "):{" + QString::number(cx) + "," + QString::number(cy) + "}";
	ui.consolemsg->append(msg);

	tag1 = cur_model->addpoint(cx, cy+radius);
	msg = "Info  : 已添加点(" + QString::number(tag1) + "):{" + QString::number(cx) + "," + QString::number(cy+radius) + "}";
	ui.consolemsg->append(msg);

	tag2 = cur_model->addpoint(cx+radius, cy);
	msg = "Info  : 已添加点(" + QString::number(tag2) + "):{" + QString::number(cx+radius) + "," + QString::number(cy) + "}";
	ui.consolemsg->append(msg);

	tag3 = cur_model->addpoint(cx, cy-radius);
	msg = "Info  : 已添加点(" + QString::number(tag3) + "):{" + QString::number(cx) + "," + QString::number(cy-radius) + "}";
	ui.consolemsg->append(msg);

	tag4 = cur_model->addpoint(cx - radius, cy);
	msg = "Info  : 已添加点(" + QString::number(tag4) + "):{" + QString::number(cx - radius) + "," + QString::number(cy) + "}";
	ui.consolemsg->append(msg);

	ItemPoint* ps;
	ItemPoint* pc = cur_model->getPoint(fin);
	ItemPoint* pe;

	ps = cur_model->getPoint(tag1);
	pe = cur_model->getPoint(tag2);
	fin = cur_model->addcirclearc(ps, pc, pe);
	msg = "Info  : 已添加圆弧(" + QString::number(fin) + "):{" + QString::number(ps->tag) + "," + QString::number(pc->tag) + "," + QString::number(pe->tag) + "}";
	ui.consolemsg->append(msg);

	ps = pe;
	pe = cur_model->getPoint(tag3);
	fin = cur_model->addcirclearc(ps, pc, pe);
	msg = "Info  : 已添加圆弧(" + QString::number(fin) + "):{" + QString::number(ps->tag) + "," + QString::number(pc->tag) + "," + QString::number(pe->tag) + "}";
	ui.consolemsg->append(msg);

	ps = pe;
	pe = cur_model->getPoint(tag4);
	fin = cur_model->addcirclearc(ps, pc, pe);
	msg = "Info  : 已添加圆弧(" + QString::number(fin) + "):{" + QString::number(ps->tag) + "," + QString::number(pc->tag) + "," + QString::number(pe->tag) + "}";
	ui.consolemsg->append(msg);

	ps = pe;
	pe = cur_model->getPoint(tag1);
	fin = cur_model->addcirclearc(ps, pc, pe);
	msg = "Info  : 已添加圆弧(" + QString::number(fin) + "):{" + QString::number(ps->tag) + "," + QString::number(pc->tag) + "," + QString::number(pe->tag) + "}";
	ui.consolemsg->append(msg);

	msg = "Info  : 已添加圆";
	ui.consolemsg->append(TEXT_COLOR_GREEN(msg));
	return false;
}


void TubeOracle::quit_sel()
{
	QString msg = "Info  : 已退出选择模式";
	ui.consolemsg->append(TEXT_COLOR_GREEN(msg));
	line_flag = false;
	sirc_flag = false;
	loop_flag = false;
	plane_flag = false;
	trans_flag = false;

	line_box.clear();
	sircle_box.clear();
	loop_box.clear();
	plane_box.clear();
	trans_box.clear();

	close_pnt_sel();
	close_curves_sel();
	
	trans_window->hide();
}

