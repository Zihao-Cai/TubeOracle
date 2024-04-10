#pragma once
#include <QMainWindow>
#include "ui_SolverWidget.h"
#include "MygraphicsView.h"
#include "MyScene.h"
#include "Edge.h"
#include "PairHashEqual.h"
#include "Triangle.h"
#include "FE_Functions.h"
#include "SetF.h"
#include "SetQ.h"
#include "SetDis.h"
#include <set>
#include <qfile.h>
#include <qtextstream>
#include "PostView.h"


using namespace std;

class SolverWidget : public QMainWindow
{
	Q_OBJECT

public:
	SolverWidget(QWidget *parent = nullptr);
	~SolverWidget();
	bool isSolverdone();


private:
	Ui::SolverWidgetClass ui;
	MygraphicsView* view;
	MyScene* scene;
	string solvername;
	string solverdir;
	bool solver_done;
	vector<int> node_box;
	PostView* postview = nullptr;
	double minX, minY, maxX, maxY;

	//定义有限元法环境
	double E;
	double v;
	double t;
	double simpler;
	double stress_simpler;
	unordered_map<size_t, Triangle*> sumTriangles;
	int offset;
	int m;
	double** K;

	bool set_f;
	bool set_q;
	bool set_dis;
	SetF* fwin;
	SetQ* qwin;
	SetDis* diswin;
	//enddef


	void reset();
	void boundary_reset();
	void open_node_sel();
	void close_node_sel();
	void checkNullNodes(vector<int>& nullnodes);

	unordered_map<size_t, Node*> nodeMap;
	unordered_map<pair<Node*, Node*>, Edge*, PairHash, PairEqual> edgeMap;
	set<int> edgenodes;


private slots:
	void loadmesh();
	void on_selNodes(Node* node);
	void on_setf();
	void on_setq();
	void on_setdis();
	void showSetF(QTreeWidgetItem* item, int col);
	void showSetQ(QTreeWidgetItem* item, int col);
	void showSetdis(QTreeWidgetItem* item, int col);
	void initialize_FE();//初始化有限元法环境
	void quit_sel();
	void on_resetview();
	void on_compute();
	void on_plot();
	void on_outport();

private: signals:
	

};
