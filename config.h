#pragma once
#include <string>

//+�������ű���
extern double geoscale;

//+�����߿�
extern double edgewidth;

//+���ε������С
extern double lcc;

//+һά���߿�
extern double curvewidth;

//+ѡ��һά���߿�
extern double selectcurvewidth;

//+0ά���С
extern double pointsize;

//+ѡ�е��С
extern double selectpointsize;

//+����ڵ��С
extern double nodesize;

//+ѡ�нڵ��С
extern double selectnodesize;

//+��άƽ�����߿�
extern double dashlinewidth;

//+2D�����㷨
extern int algorithm_2;
//MeshAdapt = 1
//Automatic = 2
//Delaunay = 5
//Frontal - Delaunay = 6
//BAMG = 7
//Frontal - Delaunay for Quads = 8
//Packing of Parallelograms = 9
//Quasi - structured Quad = 11

//+2D�����Ż��㷨
extern std::string optim_2;
//empty for default tetrahedral mesh optimizer
//Netgen
//HighOrder
//HighOrderElastic
//HighOrder - FastCurving
//Laplace2D
//QuadQuasiStructured