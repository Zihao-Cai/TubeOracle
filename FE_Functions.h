#pragma once
#include "Node.h"
#include <vector>
#include <unordered_map>

using namespace std;

void Concentration(Node* node, double fx, double fy, double** k, int rows, int cols);
void Uniform(Node* node1, Node* node2, double q, double** k, int m, unordered_map<size_t, Node*>& sumNodes, vector<int>& sumSideNodes);
void Fixed(vector<Node*> nodes, long n, double** k, int type = 0);
void Nullnode(vector<int> nullTags, double** k);
void Gaussian_elimination(double** a, int rows, int cols);
