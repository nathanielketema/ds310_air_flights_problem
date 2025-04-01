#include <iostream>
#include <fstream>
#include <iomanip>
#include <queue>
#include <stack>
#include <unordered_map>
#include <vector>
using namespace std;

struct FlightDetails {
    string depTime;
    string arrTime;
    int cost;
    int miles;
    string airline;
    string flightID;
};

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
        unordered_map< string, vector<FlightDetails> > adjacencyList;
};
