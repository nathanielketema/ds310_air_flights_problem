#include <iostream>
#include <fstream>
#include <iomanip>
#include <queue>
#include <stack>
using namespace std;

class Graph {
    public:
        Graph(ifstream &file);
        void mprint();
        void bfs(int x);
        void bfssp(int x);
        void dfs(int x);
        void dfssp(int x);
        void dijkstra(int x);
        void prim();
        void kruskal();

    private:
        int adjacencyMatrix[100][100];
        int numberOfNodes;
        int numberOfEdges;
};
