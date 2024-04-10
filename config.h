#pragma once
#include <string>

//+几何缩放倍数
extern double geoscale;

//+网格线宽
extern double edgewidth;

//+几何点网格大小
extern double lcc;

//+一维曲线宽
extern double curvewidth;

//+选中一维曲线宽
extern double selectcurvewidth;

//+0维点大小
extern double pointsize;

//+选中点大小
extern double selectpointsize;

//+网格节点大小
extern double nodesize;

//+选中节点大小
extern double selectnodesize;

//+二维平面虚线宽
extern double dashlinewidth;

//+2D网格算法
extern int algorithm_2;
//MeshAdapt = 1
//Automatic = 2
//Delaunay = 5
//Frontal - Delaunay = 6
//BAMG = 7
//Frontal - Delaunay for Quads = 8
//Packing of Parallelograms = 9
//Quasi - structured Quad = 11

//+2D网格优化算法
extern std::string optim_2;
//empty for default tetrahedral mesh optimizer
//Netgen
//HighOrder
//HighOrderElastic
//HighOrder - FastCurving
//Laplace2D
//QuadQuasiStructured