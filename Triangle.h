#pragma once

class Triangle
{
public:
    int index;
    double A;
    int nodesIndex[3];  //这个索引是实际节点索引-1的值
    double a[3];
    double b[3];
    double c[3];
    double B[3][6];
    double S[3][6];
    double strain[3];
    double stress[3];
};

