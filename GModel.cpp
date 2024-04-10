#include "GModel.h"
#include <qfiledialog.h>
#include <set>


void GModel::gridreset()
{
	//删除旧网格
	for (auto it = edgeMap.begin(); it != edgeMap.end(); ++it) {
		all_scene->removeItem(it->second);
		delete it->second;
	}
	for (auto it = nodeMap.begin(); it != nodeMap.end(); ++it) {
		all_scene->removeItem(it->second);
		delete it->second;
	}
	// 清空哈希表
	nodeMap.clear();
	edgeMap.clear();


	//添加新网格
	all_scene->setManualUpdate(true);
	vector<size_t>nodeTags;
	vector<double>coord;
	vector<double>null;
	gmsh::model::mesh::getNodes(nodeTags, coord, null);
	int c = 0;
	for (auto tag : nodeTags) {
		Node* node = new Node(coord.at(c), coord.at(c + 1), tag, par);
		nodeMap[tag] = node;
		all_scene->addItem(node);
		node->setZValue(1.1);
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
			Edge* edge = new Edge(nodeMap[it.first], nodeMap[it.second], par);
			edgeMap[make_pair(nodeMap[it.first], nodeMap[it.second])] = edge;
			all_scene->addItem(edge);
			edge->setZValue(1.0);
		}
	}
	all_scene->setManualUpdate(false);
	viewupdate();
}

GModel::GModel(string dir, string name, MygraphicsView* _view,QWidget* parent)
{
	if (!gmsh::isInitialized())	gmsh::initialize();
	//final init.

	lcc = 0.5;
	modeldir = dir;
	modelname = name;
	numcurve = 0;
	numpnt = 0;
	numsurface = 0;
	nummesh = 0;
	numpnt_tag = 0;
	numcurve_tag = 0;
	numsurface_tag = 0;
	numloop_tag = 0;
	this->modelview = _view;
	all_scene = new MyScene();
	abstract_scene = new QGraphicsScene();
	modelview->setScene(all_scene);
	all_scene->setItemIndexMethod(QGraphicsScene::NoIndex);

	par = parent;
}

GModel::~GModel()
{
	gmsh::model::setCurrent(modelname);
	gmsh::model::remove();

	delete all_scene;
	delete abstract_scene;
}

int GModel::getpointnum()
{
	return numpnt;
}

int GModel::getcurvenum()
{
	return numcurve;
}

int GModel::getsurfacenum()
{
	return numsurface;
}

int GModel::getmeshnum()
{
	return nummesh;
}

string GModel::getmodelname()
{
	return modelname;
}

int GModel::addpoint(double pointx, double pointy)
{
	//逻辑层添加点
	gmsh::model::setCurrent(modelname);
	numpnt_tag++;
	gmsh::model::geo::addPoint(pointx, pointy, 0, lcc, numpnt_tag);
	gmsh::model::geo::synchronize();
	numpnt++;
	//视图层添加点
	ItemPoint* pnt = new ItemPoint(pointx, pointy,numpnt_tag,par);
	all_scene->addItem(pnt);
	pnt->setZValue(0.9);
	viewupdate();
	//写入.geo
	QFile file(QString::fromStdString(modeldir + "/" + modelname + ".geo"));
	if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
		QTextStream stream(&file);
		stream << "//+" << endl;
		stream << "Point(" + QString::number(numpnt_tag) + ") = {" + QString::number(pointx) + ", " + QString::number(pointy) + ", " + QString::number(0) + ", " + QString::number(lcc) + "};" << endl;
		file.close();
	}
	return numpnt_tag;
}

int GModel::addline(ItemPoint* start, ItemPoint* end)
{
	//逻辑层添加line
	gmsh::model::setCurrent(modelname);
	numcurve_tag++;
	gmsh::model::geo::addLine(start->tag, end->tag, numcurve_tag);
	gmsh::model::geo::synchronize();
	numcurve++;
	//视图层添加line
	ItemLine* lne = new ItemLine(start, end, numcurve_tag, par);
	all_scene->addItem(lne);
	lne->setZValue(0.8);
	viewupdate();
	//写入.geo
	QFile file(QString::fromStdString(modeldir + "/" + modelname + ".geo"));
	if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
		QTextStream stream(&file);
		stream << "//+" << endl;
		stream << "Line(" + QString::number(numcurve_tag) + ") = {" + QString::number(start->tag) + ", " + QString::number(end->tag) + "};" << endl;
		file.close();
	}
	return numcurve_tag;
}

int GModel::addcirclearc(ItemPoint* start, ItemPoint* center, ItemPoint* end)
{
	//逻辑层添加circle
	gmsh::model::setCurrent(modelname);
	numcurve_tag++;
	try {
		gmsh::model::geo::addCircleArc(start->tag, center->tag, end->tag, numcurve_tag);
	}
	catch (exception e) {
		return 0;
	}
	
	gmsh::model::geo::synchronize();
	numcurve++;
	//视图层添加circle
	ItemCurve* arc = new ItemCurve(start, center, end, numcurve_tag, par);
	all_scene->addItem(arc);
	arc->setZValue(0.8);
	viewupdate();
	//写入.geo
	QFile file(QString::fromStdString(modeldir + "/" + modelname + ".geo"));
	if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
		QTextStream stream(&file);
		stream << "//+" << endl;
		stream << "Circle(" + QString::number(numcurve_tag) + ") = {" + QString::number(start->tag) + ", " + QString::number(center->tag) + ", " + QString::number(end->tag) + "};" << endl;
		file.close();
	}
	return numcurve_tag;
}

int GModel::addsurface(QList<CurveLoop*>curveLoops)
{
	//逻辑层添加surfacce
	gmsh::model::setCurrent(modelname);
	numsurface_tag++;
	vector<int> wires;
	for (CurveLoop* loop : curveLoops) {
		wires.push_back(loop->tag);
	}
	gmsh::model::geo::addPlaneSurface(wires, numsurface_tag);
	gmsh::model::geo::synchronize();
	numsurface++;
	//视图层添加surface
	Planesurface* sur = new Planesurface(curveLoops, numsurface_tag);
	all_scene->addItem(sur);
	sur->setZValue(0.8);
	viewupdate();
	//写入.geo
	QFile file(QString::fromStdString(modeldir + "/" + modelname + ".geo"));
	if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
		QTextStream stream(&file);
		stream << "//+" << endl;
		QStringList tt;
		for (CurveLoop* lp : curveLoops) {
			tt << QString::number(lp->tag);
		}
		QString msg = "{" + tt.join(", ") + "};";
		stream << "Plane Surface(" + QString::number(numsurface_tag) + ") = " + msg << endl;
		file.close();
	}
	return numsurface_tag;
}

int GModel::addLoop(QList<int> loops)
{
	//逻辑层添加Loop
	gmsh::model::setCurrent(modelname);
	numloop_tag++;
	vector<int> v1(loops.begin(), loops.end());
	gmsh::model::geo::addCurveLoop(v1, numloop_tag);
	gmsh::model::geo::synchronize();
	//伪视图层添加Loop
	CurveLoop* loop = new CurveLoop(loops, numloop_tag,outline_loop(loops));
	this->loops.append(loop);
	//写入.geo
	QFile file(QString::fromStdString(modeldir + "/" + modelname + ".geo"));
	if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
		QTextStream stream(&file);
		stream << "//+" << endl;
		QStringList tt;
		for (int t : loops) {
			tt << QString::number(t);
		}
		QString msg = "{" + tt.join(", ") + "};";
		stream << "Curve Loop(" + QString::number(numloop_tag) + ") = " + msg << endl;
		file.close();
	}
	return numloop_tag;
}

bool GModel::setTransfinite(QList<int>curveTags, int numNode)
{
	gmsh::model::setCurrent(modelname);
	try {
		for (int tag : curveTags) {
			gmsh::model::mesh::setTransfiniteCurve(tag, numNode);
		}
		gmsh::model::geo::synchronize();
		//写入geo
		QFile file(QString::fromStdString(modeldir + "/" + modelname + ".geo"));
		if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
			QTextStream stream(&file);
			stream << "//+" << endl;
			QStringList tt;
			for (int t : curveTags) {
				tt << QString::number(t);
			}
			QString msg = "{" + tt.join(", ") + "} = ";
			stream << "Transfinite Curve " + msg + QString::number(numNode) + " Using Progression 1;" << endl;
			file.close();
		}
		return true;
	}
	catch (exception e) {
		return false;
	}
}


bool GModel::viewupdate()
{
	all_scene->update();
	return true;
}

bool GModel::changeto_all()
{
	modelview->setScene(all_scene);
	return true;
}

bool GModel::generate2D()
{
	gmsh::model::setCurrent(modelname);
	try {
		gmsh::model::mesh::generate(2);
		gmsh::write(modeldir + "/" + modelname + ".msh");
	}
	catch (exception e) {
		return false;
	}
	gridreset();
	filtrate("Grid", true);
	return true;
}

bool GModel::Recombine2D()
{
	gmsh::model::setCurrent(modelname);
	try {
		gmsh::model::mesh::recombine();
		gmsh::write(modeldir + "/" + modelname + ".msh");
	}
	catch (exception e) {
		return false;
	}
	gridreset();
	filtrate("Grid", true);
	return true;
}

bool GModel::Optimize()
{
	gmsh::model::setCurrent(modelname);
	try {
		gmsh::model::mesh::optimize(optim_2);
		gmsh::write(modeldir + "/" + modelname + ".msh");
	}
	catch (exception e) {
		return false;
	}
	gridreset();
	filtrate("Grid", true);
	return true;
}

bool GModel::loadmodel(QString filename)
{
	//读取模型文件
	try {
		gmsh::open(filename.toStdString());
		gmsh::model::geo::synchronize();
		string logic_name;
		gmsh::model::getCurrent(logic_name);
		if (logic_name != modelname) {
			gmsh::model::remove();
			gmsh::model::add(modelname);
		}
	}
	catch (exception e) {
		return false;
	}
	QFileInfo fileinfo(filename);
	modeldir = fileinfo.absolutePath().toStdString();
	
	//分别获取几何实体同步视图层
	vector<pair<int, int>> entities;
	//获取point
	gmsh::model::getEntities(entities, 0);
	vector<double> coords;
	vector<double> null;
	for (auto it : entities) {
		gmsh::model::getValue(it.first, it.second, null, coords);
		ItemPoint* pnt = new ItemPoint(coords.at(0), coords.at(1), it.second, par);
		all_scene->addItem(pnt);
		pnt->setZValue(0.9);
		numpnt++;
	}
	//* 由于gmsh自身缺陷,curve不能返回其构造点,故Circl Arc *//
	//* 等几何的构造点并不能完全准确读取以生成视图图形项 *//
	//* 这里使用自定义方法读取除点以外的其他基础几何 *//
	try{
	QFile file(filename);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream stream(&file);
		QString text_line;
		int tag;
		while (!stream.atEnd()) {
			text_line = stream.readLine();
			if (text_line.contains("Line")) {
				int start, end;
				sscanf_s(text_line.toStdString().c_str(), "Line(%d) = {%d, %d};", &tag, &start, &end);
				ItemPoint* p1 = getPoint(start);
				ItemPoint* p2 = getPoint(end);
				if (p1 != nullptr && p2 != nullptr) {
					ItemLine* line = new ItemLine(p1, p2, tag, par);
					all_scene->addItem(line);
					line->setZValue(0.8);
					numcurve++;
				}
				continue;
			}
			else if (text_line.contains("Circle")) {
				int start, center, end;
				sscanf_s(text_line.toStdString().c_str(), "Circle(%d) = {%d, %d, %d};", &tag, &start, &center, &end);
				ItemPoint* ps = getPoint(start);
				ItemPoint* pc = getPoint(center);
				ItemPoint* pe = getPoint(end);
				if (ps != nullptr && pc != nullptr && pe != nullptr) {
					ItemCurve* cvs = new ItemCurve(ps, pc, pe, tag, par);
					all_scene->addItem(cvs);
					cvs->setZValue(0.8);
					numcurve++;
				}
				continue;
			}
			else if (text_line.contains("Curve Loop")) {
				QList<int>loops;
				sscanf_s(text_line.toStdString().c_str(), "Curve Loop(%d) = {", &tag);
				QStringList segmentsList = text_line.split(QRegExp("[{},;\\s]+"), QString::SkipEmptyParts);
				for (QString s : segmentsList) {
					if (s.toInt() != 0)	loops.push_back(s.toInt());
				}
				CurveLoop* loop = new CurveLoop(loops, tag,outline_loop(loops));
				this->loops.append(loop);
				if (tag > numloop_tag)	numloop_tag = tag;
				continue;
			}
			else if (text_line.contains("Plane Surface")) {
				QList<CurveLoop*> loops;
				sscanf_s(text_line.toStdString().c_str(), "Plane Surface(%d) = {", &tag);
				QStringList segmentsList = text_line.split(QRegExp("[{},;\\s]+"), QString::SkipEmptyParts);
				for (QString s : segmentsList) {
					CurveLoop* cl = getLoop(s.toInt());
					if (cl != nullptr)	loops.push_back(cl);
				}
				Planesurface* sur = new Planesurface(loops, tag);
				all_scene->addItem(sur);
				sur->setZValue(0.8);
				numsurface++;
				continue;
			}

		}
		file.close();
	}
	else {
		return false;
	}
	}
	catch (exception e) {
		return false;
	}
	//同步tag标签
	numpnt_tag =  gmsh::model::geo::getMaxTag(0);
	numcurve_tag = gmsh::model::geo::getMaxTag(1);
	numsurface_tag = gmsh::model::geo::getMaxTag(2);
	return true;
}

void GModel::filtrate(QString ItemType, bool State)
{
	if (ItemType == "点") {
		for (auto item : all_scene->items()) {
			if (dynamic_cast<ItemPoint*>(item)) {
				item->setVisible(State);
			}
		}
		return;
	}
	else if (ItemType == "线") {
		for (auto item : all_scene->items()) {
			if (dynamic_cast<ItemCurve*>(item) || dynamic_cast<ItemLine*>(item)) {
				item->setVisible(State);
			}
		}
		return;
	}else if(ItemType == "面"){
		for (auto item : all_scene->items()) {
			if (dynamic_cast<Planesurface*>(item)) {
				item->setVisible(State);
			}
		}
		return;
	}
	else if (ItemType == "节点与边") {
		for (auto it = edgeMap.begin(); it != edgeMap.end(); ++it) {
			it->second->setVisible(State);
		}
		for (auto it = nodeMap.begin(); it != nodeMap.end(); ++it) {
			it->second->setVisible(State);
		}
		return;
	}

}

QList<double> GModel::outline_loop(QList<int> curves)
{
	QList<double> outline;
	double x, y, top, left, bottom, right;
	top = left = 999999;
	bottom = right = -999999;
	
	//确定正交中心
	for (auto it : curves) {
		if (it < 0)	it = abs(it);
		for (auto obj : all_scene->items()) {
			if (dynamic_cast<ItemLine*>(obj)) {
				ItemLine* line = (ItemLine*)obj;
				if (it == line->tag) {
					if (top > line->top)	top = line->top;
					if (left > line->left)	left = line->left;
					if (bottom < line->bottom)	bottom = line->bottom;
					if (right < line->right)	right = line->right;
					break;
				}
			}
			else if (dynamic_cast<ItemCurve*>(obj)) {
				ItemCurve* curve = (ItemCurve*)obj;
				if (it == curve->tag) {
					if (top > curve->top) top = curve->top;
					if (left > curve->left) left = curve->left;
					if (bottom < curve->bottom)  bottom = curve->bottom;
					if (right < curve->right)  right = curve->right;
					break;
				}
			 }
		}
	}
	x = (left + right) / 2;
	y = (top + bottom) / 2; 

	//确定截断边界
	top = bottom = y;
	left = right = x;
	for (auto it : curves) {
		if (it < 0)	it = abs(it);
		for (auto obj : all_scene->items()) {
			if (dynamic_cast<ItemLine*>(obj)) {
				ItemLine* line = (ItemLine*)obj;
				if (it == line->tag) {
					double t = line->locateY(x);
					if (t != -999999 && t < top)	top = t;
					if (t != -999999 && t > bottom)	bottom = t;
					double v = line->locateX(y);
					if (v != -999999 && v < left)	left = v;
					if (v != -999999 && v > right)	right = v;
				}
			}
			else if (dynamic_cast<ItemCurve*>(obj)) {
				ItemCurve* curve = (ItemCurve*)obj;
				if (it == curve->tag) {
					QList<double> t = curve->locateY(x);
					for (auto it : t) {
						if (it < top)	top = it;
						if (it > bottom)	bottom = it;
					}
					QList<double> v = curve->locateX(y);
					for (auto it : v) {
						if (it < left)	left = it;
						if (it > right)	right = it;
					}
				}
			}
		}
	}

	//输入轮廓信息
	outline.push_back(x);
	outline.push_back(y);
	outline.push_back(top);
	outline.push_back(left);
	outline.push_back(bottom);
	outline.push_back(right);
	return outline;
}


ItemPoint* GModel::getPoint(int tag)
{
	for (QGraphicsItem* obj : all_scene->items()) {
		if (dynamic_cast<ItemPoint*>(obj)) {
			ItemPoint* p = (ItemPoint*)obj;
			if (tag == p->tag)	return p;
		}
	}

	return nullptr;
}

CurveLoop* GModel::getLoop(int tag)
{
	for (CurveLoop* lp : this->loops) {
		if (tag == lp->tag)	return lp;
	}

	return nullptr;
}

void GModel::getQuality(QVector<qreal>& quality)
{
	gmsh::model::setCurrent(modelname);
	quality.clear();

	//获取网格质量
	vector<size_t> eleTags;
	vector<size_t> nodeTags;
	vector<double> q;
	gmsh::model::mesh::getElementsByType(2, eleTags, nodeTags);
	gmsh::model::mesh::getElementQualities(eleTags, q,"gamma");
	//质量参考指标采用gamma值
	//质量q写入quality
	double asp[10] = { 0 };
	for (auto i : q) {
		if (0 < i <= 0.1) {
			asp[0]++;
			continue;
		}
		else if (0.1 < i <= 0.2) {
			asp[1]++;
			continue;
		}
		else if (0.2 < i <= 0.3) {
			asp[2]++;
			continue;
		}
		else if (0.3 < i <= 0.4) {
			asp[3]++;
			continue;
		}
		else if (0.4 < i <= 0.5) {
			asp[4]++;
			continue;
		}
		else if (0.5 < i <= 0.6) {
			asp[5]++;
			continue;
		}
		else if (0.6 < i <= 0.7) {
			asp[6]++;
			continue;
		}
		else if (0.7 < i <= 0.8) {
			asp[7]++;
			continue;
		}
		else if (0.8 < i <= 0.9) {
			asp[8]++;
			continue;
		}
		else if (0.9 < i <= 1) {
			asp[9]++;
			continue;
		}
	}
	for (int i = 0; i < 10; i++) {
		quality.append(asp[i]);
	}

}

void GModel::update_surface_geo()
{
	for (auto obj : all_scene->items()) {
		if (dynamic_cast<Planesurface*>(obj)) {
			Planesurface* ple = (Planesurface*)obj;
			ple->updateoutline();
		}
	}

}

QGraphicsScene* GModel::getscene()
{
	if (all_scene == nullptr)	return new QGraphicsScene();
	return all_scene;
}
