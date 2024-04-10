#pragma once
#include <gmsh.h>
using namespace std;

#include "MyScene.h"
#include "MygraphicsView.h"
#include "ItemPoint.h"
#include "ItemLine.h"
#include "ItemCurve.h"
#include "CurveLoop.h"
#include "Planesurface.h"
#include <qfile.h>
#include <qtextstream>
#include "Node.h"
#include "Edge.h"
#include <unordered_map>
#include "PairHashEqual.h"


class GModel
{
private:
	string modelname;
	string modeldir;
	int numpnt;
	int numcurve;
	int numsurface;
	int numpnt_tag;
	int numcurve_tag;
	int numsurface_tag;
	int numloop_tag;
	int nummesh;
	double lcc;
	MygraphicsView* modelview = nullptr;
	MyScene* all_scene = nullptr;
	QGraphicsItemGroup* group = nullptr;

	unordered_map<size_t, Node*> nodeMap;
	unordered_map<pair<Node*, Node*>, Edge*, PairHash, PairEqual> edgeMap;

	void gridreset();


public:
	GModel(string dir, string name, MygraphicsView* _view, QWidget* parent);
	~GModel();
	int getpointnum();
	int getcurvenum();
	int getsurfacenum();
	int getmeshnum();
	string getmodelname();
	QGraphicsScene* abstract_scene = nullptr;
	QList<CurveLoop*>loops;

	int addpoint(double pointx, double pointy);
	int addline(ItemPoint* start,ItemPoint* end);
	int addcirclearc(ItemPoint* start, ItemPoint* center, ItemPoint* end);
	int addsurface(QList<CurveLoop*>curveLoops);
	int addLoop(QList<int>loops);
	bool setTransfinite(QList<int>curveTags,int numNode);

	bool viewupdate();
	bool changeto_all();
	bool generate2D();
	bool Recombine2D();
	bool Optimize();
	bool loadmodel(QString filename);
	void filtrate(QString ItemType, bool State);
	QList<double> outline_loop(QList<int>curves);
	ItemPoint* getPoint(int tag);
	CurveLoop* getLoop(int tag);
	void getQuality(QVector<qreal>& quality);
	void update_surface_geo();

	QGraphicsScene* getscene();
	QWidget* par = nullptr;
};

