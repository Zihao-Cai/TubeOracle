#include "SolverWidget.h"
#include <QtWidgets/qfiledialog.h>
#include <gmsh.h>


SolverWidget::SolverWidget(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	view = new MygraphicsView();
	view->setmainwin(this);
	view->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(182, 196, 255, 255), stop:1 rgba(255, 255, 255, 255));");
	ui.cenlayout->addWidget(ui.PreParaBox, 1);
	ui.cenlayout->addWidget(view, 2);
	ui.centralWidget->setLayout(ui.cenlayout);

	scene = new MyScene();
	view->setScene(scene);
	scene->setItemIndexMethod(QGraphicsScene::NoIndex);
	fwin = new SetF(this);
	fwin->setWindowTitle("集中力");
	qwin = new SetQ(this);
	qwin->setWindowTitle("分布力");
	diswin = new SetDis(this);
	postview = new PostView(this);

	ui.pathedit->setEnabled(false);
	ui.entityNum->setEnabled(false);
	ui.nodeNum->setEnabled(false);
	ui.eleNum->setEnabled(false);
	ui.initflag->setEnabled(false);
	ui.solveflag->setEnabled(false);
	ui.eedit->setValidator(new QDoubleValidator(0.000000, 999999999.999999, 6, this));
	ui.vedit->setValidator(new QDoubleValidator(0.000000, 999999999.999999, 6, this));

	set_f = false;
	set_q = false;
	set_dis = false;
	solver_done = false;

	//未初始化有限元环境下,边界条件不可用
	ui.boundarypage->setEnabled(false);

	//信号绑定
	connect(ui.loadbtn, SIGNAL(clicked()), this, SLOT(loadmesh()));
	connect(ui.init, SIGNAL(clicked()), this, SLOT(initialize_FE()));
	connect(ui.treeWidget, &QTreeWidget::itemClicked, this, &SolverWidget::showSetF);
	connect(ui.treeWidget, &QTreeWidget::itemClicked, this, &SolverWidget::showSetQ);
	connect(ui.treeWidget, &QTreeWidget::itemClicked, this, &SolverWidget::showSetdis);
	connect(ui.resetview, SIGNAL(triggered()), this, SLOT(on_resetview()));
	connect(ui.compute, SIGNAL(clicked()), this, SLOT(on_compute()));
	connect(ui.plot, SIGNAL(clicked()), this, SLOT(on_plot()));
	connect(ui.outport, SIGNAL(clicked()), this, SLOT(on_outport()));
}

SolverWidget::~SolverWidget()
{
	delete view;
	delete scene;
	delete fwin;
	delete qwin;
	delete diswin;
	delete postview;
}

bool SolverWidget::isSolverdone()
{
	return solver_done;
}


void SolverWidget::initialize_FE()
{
	reset();
	t = 1;
	E = ui.eedit->text().toDouble() * 1e9;
	v = ui.vedit->text().toDouble();
	simpler = E * t / (4.0 * (1.0 - pow(v, 2)));
	stress_simpler = E * 1.0 / (2 * (1.0 - pow(v, 2)));
	//
	ui.nodeNum->setText(QString::number(nodeMap.size()));
	vector<pair<int, int>>dimtag;
	gmsh::model::getEntities(dimtag);
	ui.entityNum->setText(QString::number(dimtag.size()));
	vector<size_t>eleTags;
	vector<size_t>nodeTags;
	gmsh::model::mesh::getElementsByType(2, eleTags, nodeTags);
	ui.eleNum->setText(QString::number(eleTags.size()));
	offset = eleTags.at(0);
	int n = 0;
	int i, j, k;
	for (auto it : eleTags)
	{
		Triangle* triangle = new Triangle;
		triangle->index = it;
		for (i = n; i < n + 3; i++)
		{
			int nodeIndex = nodeTags.at(i);
			triangle->nodesIndex[i % 3] = nodeIndex - 1;

			if (i == n)
			{
				j = nodeTags.at(i + 1) - 1;
				k = nodeTags.at(i + 2) - 1;
			}
			else if (i == n + 1)
			{
				j = nodeTags.at(i + 1) - 1;
				k = nodeTags.at(i - 1) - 1;
			}
			else
			{
				j = nodeTags.at(i - 2) - 1;
				k = nodeTags.at(i - 1) - 1;
			}
			triangle->a[i % 3] = nodeMap[j + 1]->nx * nodeMap[k + 1]->ny - nodeMap[k + 1]->nx * nodeMap[j + 1]->ny;
			triangle->b[i % 3] = nodeMap[j + 1]->ny - nodeMap[k + 1]->ny;
			triangle->c[i % 3] = -nodeMap[j + 1]->nx + nodeMap[k + 1]->nx;
		}//
		double area = 0;
		for (int i = 0; i < 3; i++)
		{
			area += triangle->a[i];
			triangle->strain[i] = 0;
			triangle->stress[i] = 0;
		}
		triangle->A = fabs(area) / 2.0;
		
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 6; j += 2) {
				if (i == 0) {
					triangle->B[i][j] = triangle->b[j / 2];
					triangle->B[i][j + 1] = 0;
				}
				else if (i == 1) {
					triangle->B[i][j] = 0;
					triangle->B[i][j + 1] = triangle->c[j / 2];
				}
				else {
					triangle->B[i][j] = triangle->c[j / 2];
					triangle->B[i][j + 1] = triangle->b[j / 2];
				}
			}
		}
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 6; j++) {
				triangle->B[i][j] *= 1.0 / (2 * triangle->A);
			}
		}

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 6; j += 2) {
				if (i == 0) {
					triangle->S[i][j] = triangle->b[j / 2];
					triangle->S[i][j + 1] = v * triangle->c[j / 2];
				}
				else if (i == 1) {
					triangle->S[i][j] = v * triangle->b[j / 2];
					triangle->S[i][j + 1] = triangle->c[j / 2];
				}
				else {
					triangle->S[i][j] = (1 - v) / 2 * triangle->c[j / 2];
					triangle->S[i][j + 1] = (1 - v) / 2 * triangle->b[j / 2];
				}
			}
		}
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 6; j++) {
				triangle->S[i][j] *= stress_simpler / triangle->A;
			}
		}
		sumTriangles[it] = triangle;
		n += 3;
	}
	//分配K矩阵
	m = nodeMap.size() * 2;
	K = new double* [m];
	for (int i = 0; i < m; i++)
	{
		K[i] = new double[m + 1];
	}
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < m + 1; j++)
		{
			K[i][j] = 0;
		}
	}
	//构建k矩阵，即总刚度矩阵
	for (auto it=sumTriangles.begin();it!=sumTriangles.end();++it)
	{
		double tsimpler = simpler / it->second->A;
		for (int r = 0; r < 3; r++)
		{
			for (int s = 0; s < 3; s++)
			{
				K[it->second->nodesIndex[r] * 2][it->second->nodesIndex[s] * 2] += tsimpler * (it->second->b[r] * it->second->b[s] + ((1 - v) / 2) * it->second->c[r] * it->second->c[s]);
				K[it->second->nodesIndex[r] * 2][it->second->nodesIndex[s] * 2 + 1] += tsimpler * (v * it->second->b[r] * it->second->c[s] + ((1 - v) / 2) * it->second->c[r] * it->second->b[s]);
				K[it->second->nodesIndex[r] * 2 + 1][it->second->nodesIndex[s] * 2] += tsimpler * (v * it->second->c[r] * it->second->b[s] + ((1 - v) / 2) * it->second->b[r] * it->second->c[s]);
				K[it->second->nodesIndex[r] * 2 + 1][it->second->nodesIndex[s] * 2 + 1] += tsimpler * (it->second->c[r] * it->second->c[s] + ((1 - v) / 2) * it->second->b[r] * it->second->b[s]);
			}
		}
	}
	//处理无效节点
	vector<int> nullnodes;
	checkNullNodes(nullnodes);
	Nullnode(nullnodes, K);

	//初始化环境完成
	ui.statusBar->showMessage("初始化成功", 5000);
	ui.initflag->setChecked(true);
	ui.boundarypage->setEnabled(true);
	ui.compute->setEnabled(true);
}

void SolverWidget::quit_sel()
{
	set_f = false;
	set_q = false;
	set_dis = false;
	node_box.clear();
	close_node_sel();
	fwin->hide();
	qwin->hide();
	diswin->hide();

	ui.statusBar->showMessage("已退出选择模式", 5000);
}

void SolverWidget::on_resetview()
{
	view->resetScale();
}

void SolverWidget::on_compute()
{
	if (ui.algorithm->currentText() == "高斯消去法") {
		try {
			Gaussian_elimination(K, m, m + 1);
		}
		catch (exception e) {
			ui.statusBar->showMessage("求解失败", 5000);
			return;
		}
		//计算应力应变
		for (auto it = sumTriangles.begin(); it != sumTriangles.end(); ++it)
		{
			for (int q = 0; q < 3; q++)
			{
				for (int n = 0; n < 6; n += 2)
				{
					it->second->strain[q] += K[(it->second->nodesIndex[n / 2]) * 2][m] * it->second->B[q][n];
					it->second->strain[q] += K[(it->second->nodesIndex[n / 2]) * 2 + 1][m] * it->second->B[q][n + 1];
					it->second->stress[q] += K[(it->second->nodesIndex[n / 2]) * 2][m] * it->second->S[q][n];
					it->second->stress[q] += K[(it->second->nodesIndex[n / 2]) * 2 + 1][m] * it->second->S[q][n + 1];
				}
			}
		}
		ui.statusBar->showMessage("求解完成", 5000);
		solver_done = true;
		ui.postpage->setEnabled(true);
		ui.solveflag->setChecked(true);
	}
}

void SolverWidget::on_plot()
{
	//绘制云图
	if (postview != nullptr) {
		vector<double> choords;
		choords.push_back(0.5);
		choords.push_back(minX);
		choords.push_back(maxX);
		choords.push_back(minY);
		choords.push_back(maxY);
		
		postview->setData(nodeMap, sumTriangles, choords);
		postview->show();
		ui.statusBar->showMessage("绘图成功", 5000);
	}
}

void SolverWidget::on_outport()
{
	//写入txt
	QFile file1(QString::fromStdString(solverdir + "/" + "位移.txt"));
	QFile file2(QString::fromStdString(solverdir + "/" + "应力应变.txt"));
	if (file1.open(QIODevice::WriteOnly)) {
		QTextStream stream(&file1);
		stream << "****************" << endl << "Data format : " << endl << "Tag of Node" << endl << "u" << endl << "v" << endl << "****************" << endl;
		for (int i = 0; i < m - 1; i += 2) {
			stream << i / 2 + 1 << endl;
			stream << K[i][m] << endl;
			stream << K[i + 1][m] << endl;
			stream << endl;
		}
		file1.close();
		ui.statusBar->showMessage("数据导出成功:" + QString::fromStdString(solverdir) + "/" + "位移.txt", 5000);
	}
	if (file2.open(QIODevice::WriteOnly)) {
		QTextStream stream2(&file2);
		stream2 << "****************" << endl << "Data format : " << endl << "Tag of Element" << endl << "Tag of Nodes" << endl << "strain vector" << endl << "stress vector" << endl << "****************" << endl;
		for (auto it = sumTriangles.begin(); it != sumTriangles.end(); ++it) {
			stream2 << it->first << endl;
			stream2 << it->second->nodesIndex[0] + 1 << " " << it->second->nodesIndex[1] + 1 << " " << it->second->nodesIndex[2] + 1 << endl;
			stream2 << it->second->strain[0] << " " << it->second->strain[1] << " " << it->second->strain[2] << endl;
			stream2 << it->second->stress[0] << " " << it->second->stress[1] << " " << it->second->stress[2] << endl;
			stream2 << endl;
		}
		file2.close();
		ui.statusBar->showMessage("数据导出成功:" + QString::fromStdString(solverdir) + "/" + "应力应变.txt", 5000);
	}
}

void SolverWidget::open_node_sel()
{
	//打开节点选择器
	for (auto it = nodeMap.begin(); it != nodeMap.end(); ++it) {
		it->second->setFlag(QGraphicsItem::GraphicsItemFlag::ItemIsSelectable, true);
	}
}

void SolverWidget::close_node_sel()
{
	//关闭节点选择器
	for (auto it:node_box) {
		nodeMap[it]->setSelected(false);
		nodeMap[it]->isselected = false;
		nodeMap[it]->setFlag(QGraphicsItem::GraphicsItemFlag::ItemIsSelectable, false);
	}
}

void SolverWidget::reset()
{
	if (ui.initflag->isChecked()) {
		for (int i = 0; i < m; i++)
		{
			delete[] K[i];
		}
		delete[] K;
	}
	//
	solver_done = false;
	ui.solveflag->setChecked(false);
	//
	node_box.clear();
	ui.initflag->setChecked(false);
	ui.boundarypage->setEnabled(false);
	ui.compute->setEnabled(false);
	ui.postpage->setEnabled(solver_done);
	//
	set_f = false;
	set_q = false;
	set_dis = false;
}

void SolverWidget::loadmesh()
{
	reset();
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Mesh (*.msh)"));

	QFileInfo fileinfo(fileName);
	solverdir = fileinfo.absolutePath().toStdString();
	solvername = fileinfo.baseName().toStdString();

	try {
		gmsh::model::setCurrent(solvername + "m");
		gmsh::model::remove();
	}
	catch (exception e) {
	//若之前已经存在该模型则删除之前的模型
	}

	if (!gmsh::isInitialized())	gmsh::initialize();
	//读取模型文件
	try {
		gmsh::model::add(solvername + "m");
		gmsh::merge(fileName.toStdString());
		gmsh::model::geo::synchronize();
	}
	catch (exception e) {
		gmsh::model::remove();
		ui.statusBar->showMessage("加载网格失败 " + fileName, 5000);
		return;
	}
	ui.pathedit->setText(fileName);
	/*vector<string> names; string logic_name; cout << "list :" << endl;
	gmsh::model::list(names); for (auto it : names)	cout << it << endl;
	gmsh::model::getCurrent(logic_name); cout << "logic_name :" << logic_name << endl;
	*/

	//删除旧网格
	for (auto it = edgeMap.begin(); it != edgeMap.end(); ++it) {
		scene->removeItem(it->second);
		delete it->second;
	}
	for (auto it = nodeMap.begin(); it != nodeMap.end(); ++it) {
		scene->removeItem(it->second);
		delete it->second;
	}
	// 清空哈希表
	nodeMap.clear();
	edgeMap.clear();

	//添加新网格
	scene->setManualUpdate(true);
	vector<size_t>eleTags;
	vector<size_t>nodeTags;
	vector<double>coord;
	vector<double>null;
	gmsh::model::mesh::getNodes(nodeTags, coord, null);
	int c = 0;
	minX = maxX = coord[0];
	minY = maxY = coord[1];
	for (auto tag : nodeTags) {
		Node* node = new Node(coord.at(c), coord.at(c + 1), tag, this);
		nodeMap[tag] = node;
		scene->addItem(node);
		node->setZValue(1.1);
		if (node->nx > maxX)	maxX = node->nx;
		if (node->nx < minX)	minX = node->nx;
		if (node->ny > maxY)	maxY = node->ny;
		if (node->ny < minY)	minY = node->ny;
		c += 3;
	}
	nodeTags.clear();
	c = 0;
	gmsh::model::mesh::getElementEdgeNodes(2, nodeTags);
	set<pair<int, int>> edges;
	if (nodeTags.size() >= 2) {
		for (c = 0; c < nodeTags.size() - 1; c += 2) {
			if (nodeTags[c] < nodeTags[c + 1]) {
				pair<int, int> edge = make_pair(nodeTags[c], nodeTags[c + 1]);
				edgenodes.insert(nodeTags[c]);
				edgenodes.insert(nodeTags[c + 1]);
				edges.insert(edge);
				continue;
			}
			else {
				pair<int, int> edge = make_pair(nodeTags[c + 1], nodeTags[c]);
				edges.insert(edge);
				continue;
			}
		}
		for (auto it : edges) {
			Edge* edge = new Edge(nodeMap[it.first], nodeMap[it.second], this);
			edgeMap[make_pair(nodeMap[it.first], nodeMap[it.second])] = edge;
			scene->addItem(edge);
			edge->setZValue(1.0);
		}
	}
	scene->setManualUpdate(false);
	scene->update();

	ui.statusBar->showMessage("已加载网格 " + fileName, 5000);

}

void SolverWidget::on_selNodes(Node* node)
{
	if (set_f) {
		node->isselected = true;
		node_box.push_back(node->tag);
		fwin->appendNode(QString::number(node->tag));
		ui.statusBar->showMessage("新增集中力约束节点：" + QString::number(node->tag), 5000);
		scene->update();
		return;
	}
	else if (set_q) {
		if (node_box.size() == 0) {
			node->isselected = true;
			node_box.push_back(node->tag);
			ui.statusBar->showMessage("设置分布力端点1：" + QString::number(node->tag), 5000);
			qwin->setn1(QString::number(node->tag));
			scene->update();
			return;
		}
		else if (node_box.size() == 1) {
			node->isselected = true;
			node_box.push_back(node->tag);
			ui.statusBar->showMessage("设置分布力端点2：" + QString::number(node->tag), 5000);
			qwin->setn2(QString::number(node->tag));
			scene->update();
			return;
		}
		else {
			node_box.clear();
			close_node_sel();
			qwin->setn2("");
			node->isselected = true;
			node_box.push_back(node->tag);
			ui.statusBar->showMessage("设置分布力端点1：" + QString::number(node->tag), 5000);
			qwin->setn1(QString::number(node->tag));
			scene->update();
			return;
		}
	}
	else if (set_dis) {
		node->isselected = true;
		node_box.push_back(node->tag);
		diswin->appendNodes(QString::number(node->tag));
		ui.statusBar->showMessage("新增位移约束节点：" + QString::number(node->tag), 5000);
		scene->update();
		return;
	}
	
}

void SolverWidget::on_setf()
{
	if (!set_f)	return;
	if (node_box.empty()) {
		ui.statusBar->showMessage("未选中任何节点",5000);
		return;
	}
	for (auto it : node_box) {
		Concentration(nodeMap[it], fwin->getHF()*1e3, fwin->getVF()*1e3, K, m, m + 1);
	}
	ui.statusBar->showMessage("设置集中力成功", 5000);
	close_node_sel();
	node_box.clear();
	fwin->clear();
}

void SolverWidget::boundary_reset()
{
	node_box.clear();
	close_node_sel();

	set_f = false;
	set_q = false;
	set_dis = false;

	fwin->hide();
	fwin->clear();
	qwin->hide();
	qwin->setn1("");
	qwin->setn2("");
	diswin->hide();
	diswin->clear();
}


void SolverWidget::on_setq()
{
	if (!set_q)	return;
	if (node_box.size() != 2) {
		ui.statusBar->showMessage("分布力设置错误", 5000);
		close_node_sel();
		return;
	}
	vector<int> sumSideNodes;
	Uniform(nodeMap[node_box[0]], nodeMap[node_box[1]], qwin->getq()*1e3, K, m, nodeMap, sumSideNodes);
	ui.statusBar->showMessage("设置分布力成功", 5000);
	close_node_sel();
	node_box.clear();
	qwin->setn1("");
	qwin->setn2("");
}

void SolverWidget::on_setdis()
{
	if (!set_dis)	return;
	if (node_box.empty()) {
		ui.statusBar->showMessage("未选中任何节点", 5000);
		return;
	}
	vector<Node*> nodes;
	for (auto it : node_box) {
		nodes.push_back(nodeMap[it]);
	}
	if (diswin->windowTitle() == "水平约束") {
		Fixed(nodes, m, K, 1);
		ui.statusBar->showMessage("设置水平约束成功", 5000);
		close_node_sel();
		node_box.clear();
		diswin->clear();
		return;
	}
	else if (diswin->windowTitle() == "竖直约束") {
		Fixed(nodes, m, K, 2);
		ui.statusBar->showMessage("设置竖直约束成功", 5000);
		close_node_sel();
		node_box.clear();
		diswin->clear();
		return;
	}
	else if (diswin->windowTitle() == "固定约束") {
		Fixed(nodes, m, K);
		ui.statusBar->showMessage("设置固定约束成功", 5000);
		close_node_sel();
		node_box.clear();
		diswin->clear();
		return;
	}

}

void SolverWidget::showSetF(QTreeWidgetItem* item, int col)
{
	//判断是否为对应组件
	if (item->text(col) == "集中力") {
		boundary_reset();
		set_f = true;
		fwin->show();
	}
}

void SolverWidget::showSetQ(QTreeWidgetItem* item, int col)
{
	//判断是否为对应组件
	if (item->text(col) == "分布力") {
		boundary_reset();
		set_q = true;
		qwin->show();
	}
}

void SolverWidget::checkNullNodes(vector<int>& nullnodes)
{
	
	for (auto it : nodeMap) {
		if (edgenodes.find(it.first) == edgenodes.end()) {
			nullnodes.push_back(it.first);
		}
	}
	return;
}

void SolverWidget::showSetdis(QTreeWidgetItem* item, int col)
{
	//判断是否为对应组件
	if (item->text(col) == "水平位移") {
		diswin->setWindowTitle("水平约束");
		boundary_reset();
		set_dis = true;
		diswin->show();
	}
	else if (item->text(col) == "竖直位移") {
		diswin->setWindowTitle("竖直约束");
		boundary_reset();
		set_dis = true;
		diswin->show();
	}
	else if (item->text(col) == "固定约束") {
		diswin->setWindowTitle("固定约束");
		boundary_reset();
		set_dis = true;
		diswin->show();
	}
}
