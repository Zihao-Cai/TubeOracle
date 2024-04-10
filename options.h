#pragma once

#include <QMainWindow>
#include <ui_options.h>

class options  : public QMainWindow
{
	Q_OBJECT

public:
	options(QMainWindow *parent);
	~options();

	double get_geoscale();
	void set_geoscale(double geoscale);
	double get_edgewidth();
	void set_edgewidth(double edgewidth);
	double get_curvewidth();
	void set_curvewidth(double curvewidth);
	double get_selectcurvewidth();
	void set_selectcurvewidth(double selectcurvewidth);
	double get_pointsize();
	void set_pointsize(double pointsize);
	double get_selectpointsize();
	void set_selectpointsize(double selectpointsize);
	double get_nodesize();
	void set_nodesize(double nodesize);
	QString get2D();
	QString getOptim();

private:
	Ui::options ui;

private slots:
	void op_geo_clicked();
	void op_mesh_clicked();


};
