#pragma once

class Triangle
{
public:
    int index;
    double A;
    int nodesIndex[3];  //���������ʵ�ʽڵ�����-1��ֵ
    double a[3];
    double b[3];
    double c[3];
    double B[3][6];
    double S[3][6];
    double strain[3];
    double stress[3];
};

