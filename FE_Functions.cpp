#include "FE_Functions.h"


//处理集中力
void Concentration(Node* node, double fx, double fy, double** k, int rows, int cols)
{
    k[(node->tag - 1) * 2][cols - 1] = fx;
    k[(node->tag - 1) * 2 + 1][cols - 1] = fy;
}


//处理分布力
void Uniform(Node* node1, Node* node2, double q, double** k, int m, unordered_map<size_t, Node*>& sumNodes, vector<int>& sumSideNodes)
{
    if (node1->ny == node2->ny)
    {
        if (node1->nx < node2->nx)
        {
            for (auto it = sumNodes.begin(); it != sumNodes.end(); ++it)
            {
                if (it->second->nx >= node1->nx && it->second->nx <= node2->nx && it->second->ny == node1->ny)
                {
                    sumSideNodes.push_back(it->first);
                }
            }
        }
        else if (node1->nx > node2->nx)
        {
            for (auto it = sumNodes.begin(); it != sumNodes.end(); ++it)
            {
                if (it->second->nx >= node2->nx && it->second->nx <= node1->nx && it->second->ny == node1->ny)
                {
                    sumSideNodes.push_back(it->first);
                }
            }
        }
        for (int i = 0; i < sumSideNodes.size() - 1; i++) {
            for (int j = 0; j < sumSideNodes.size() - 1 - i; j++) {
                if (sumNodes[sumSideNodes[j]]->nx > sumNodes[sumSideNodes[j + 1]]->nx) {
                    int temp = sumSideNodes[j];
                    sumSideNodes[j] = sumSideNodes[j + 1];
                    sumSideNodes[j + 1] = temp;
                }
            }
        }
        for (int i = 0; i < sumSideNodes.size() - 1; i++)
        {
            double len = sumNodes[sumSideNodes[i + 1]]->nx - sumNodes[sumSideNodes[i]]->nx;
            k[(sumSideNodes[i] - 1) * 2 + 1][m] += q * len / 2;
            k[(sumSideNodes[i + 1] - 1) * 2 + 1][m] += q * len / 2;
        }
    }
    else if (node1->nx == node2->nx)
    {
        if (node1->ny < node2->ny)
        {
            for (auto it = sumNodes.begin(); it != sumNodes.end(); ++it)
            {
                if (it->second->ny >= node1->ny && it->second->ny <= node2->ny && it->second->nx == node1->nx)
                {
                    sumSideNodes.push_back(it->first);
                }
            }
        }
        else if (node1->ny > node2->ny)
        {
            for (auto it = sumNodes.begin(); it != sumNodes.end(); ++it)
            {
                if (it->second->ny >= node2->ny && it->second->ny <= node1->ny && it->second->nx == node1->nx)
                {
                    sumSideNodes.push_back(it->first);
                }
            }
        }
        for (int i = 0; i < sumSideNodes.size() - 1; i++) {
            for (int j = 0; j < sumSideNodes.size() - 1 - i; j++) {
                if (sumNodes[sumSideNodes[j]]->nx > sumNodes[sumSideNodes[j + 1]]->nx) {
                    int temp = sumSideNodes[j];
                    sumSideNodes[j] = sumSideNodes[j + 1];
                    sumSideNodes[j + 1] = temp;
                }
            }
        }
        for (int i = 0; i < sumSideNodes.size() - 1; i++)
        {
            double len = sumNodes[sumSideNodes[i + 1]]->ny - sumNodes[sumSideNodes[i]]->ny;
            k[(sumSideNodes[i] - 1) * 2][m] += q * len / 2;
            k[(sumSideNodes[i + 1] - 1) * 2][m] += q * len / 2;
        }
    }
}


//处理位移约束    参数4指向约束类型：0=固定，1=水平，2=竖直
void Fixed(vector<Node*> nodes, long n, double** k, int type)
{
    int bor;
    for (Node* node : nodes) {
        bor = node->tag - 1;
        if (type == 1) {
            for (int i = 0; i < n; i++)
            {
                k[bor * 2][i] = 0;
                k[i][bor * 2] = 0;
                k[bor * 2][bor * 2] = 1;
            }
            continue;
        }
        if (type == 2) {
            for (int i = 0; i < n; i++)
            {
                k[bor * 2 + 1][i] = 0;
                k[i][bor * 2 + 1] = 0;
                k[bor * 2 + 1][bor * 2 + 1] = 1;
            }
            continue;
        }
        if (type == 0) {
            for (int i = 0; i < n; i++)
            {
                k[bor * 2][i] = 0;
                k[bor * 2 + 1][i] = 0;
                k[i][bor * 2] = 0;
                k[i][bor * 2 + 1] = 0;
                k[bor * 2][bor * 2] = 1;
                k[bor * 2 + 1][bor * 2 + 1] = 1;
            }
            continue;
        }
    }

}


//处理无效节点
void Nullnode(vector<int> nullTags, double** k)
{
    for (auto tag : nullTags) {
        k[(tag - 1) * 2][(tag - 1) * 2] = 1;
        k[(tag - 1) * 2 + 1][(tag - 1) * 2 + 1] = 1;
    }
}


//高斯消元
void Gaussian_elimination(double** a, int rows, int cols)
{
    double factor = 0;
    for (int i = 0; i < rows - 1; i++) {
        int r = i;
        //采用列主元消去法，每一步选取增广矩阵中绝对值最大的元素作为主元素，以使高斯消元法具有较好的数值稳定性.
        for (int j = i + 1; j < rows; j++) {
            if (fabs(a[j][i]) > fabs(a[r][i]))
                r = j;
        }
        if (r != i)
            for (int j = i; j < cols; j++) {
                swap(a[i][j], a[r][j]);
            }
        for (int j = i + 1; j < rows; j++) {
            if (a[i][i] == 0) {
            }

            factor = a[j][i] / a[i][i];
            for (int k = i; k < cols; k++)
                a[j][k] -= a[i][k] * factor;
        }
    }
    for (int i = rows - 1; i >= 0; i--) {
        for (int j = i + 1; j < rows; j++) {
            a[i][cols - 1] -= a[i][j] * a[j][cols - 1];
        }
        a[i][cols - 1] /= a[i][i];
    }
}