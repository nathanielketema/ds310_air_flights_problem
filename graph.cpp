#include "graph.h"
#include <utility>

Graph::Graph(ifstream &file) {
    int source, destination, weight;

    file >> numberOfNodes;
    file >> numberOfEdges;

    for (int i = 0; i < numberOfEdges; i++) {
        file >> source >> destination >> weight;
        adjacencyMatrix[source][destination] = weight;
        adjacencyMatrix[destination][source] = weight;
    }
}

void Graph::mprint() {
    for (int r = 1; r <= numberOfNodes; r++) {
        for (int c = 1; c <= numberOfNodes; c++)
            cout << setw(4) << adjacencyMatrix[r][c] << " ";
        cout << endl;
    }
}

void Graph::bfs(int x) {
    queue<int> queueBfs;
    bool visited[100] = {false};

    queueBfs.push(x);

    while (!queueBfs.empty()) {
        int value = queueBfs.front();
        queueBfs.pop();

        if (!visited[value]) {
            cout << value << " ";
            visited[value] = true;

            for (int i = 1; i <= numberOfNodes; i++) {
                if (adjacencyMatrix[value][i] != 0 && !visited[i])
                    queueBfs.push(i);
            }
        }
    }
}

void Graph::bfssp(int x) {
    queue< pair<int, int> > queueBfs;
    bool visited[100] = {false};

    queueBfs.push(make_pair(x, 0));

    while (!queueBfs.empty()) {
        int value = queueBfs.front().first;
        int parent = queueBfs.front().second;
        queueBfs.pop();

        if (!visited[value]) {
            visited[value] = true;
            if (parent != 0)
                cout << parent << "-" << value << " ";

            for (int i = 1; i <= numberOfNodes; i++) {
                if (adjacencyMatrix[value][i] != 0 && !visited[i])
                    queueBfs.push(make_pair(i, value));
            }
        }
    }
}

void Graph::dfs(int x) {
    stack<int> stackDfs;
    bool visited[100] = {false};

    stackDfs.push(x);

    while (!stackDfs.empty()) {
        int value = stackDfs.top();
        stackDfs.pop();

        if (!visited[value]) {
            cout << value << " ";
            visited[value] = true;

            for (int i = numberOfNodes; i >= 1; i--) {
                if (adjacencyMatrix[value][i] != 0 && !visited[i])
                    stackDfs.push(i);
            }
        }
    }
}

void Graph::dijkstra(int start) {
    int distance[100];
    bool explored[100];

    for (int i = 1; i <= numberOfNodes; i++) {
        distance[i] = 9999;
        explored[i] = false;
    }

    distance[start] = 0;

    for (int i = 1; i <= numberOfNodes; i++) {
        int minDistance = 9999;
        int currentNode = -1;
        for (int j = 1; j <= numberOfNodes; j++) {
            if (!explored[j] && distance[j] < minDistance) {
                minDistance = distance[j];
                currentNode = j;
            }
        }

        explored[currentNode] = true;

        cout << "After locking in node " << currentNode << ": ";
        for (int j = 1; j <= numberOfNodes; j++) {
            if (distance[j] == 9999)
                cout << "INF ";
            else
                cout << distance[j] << " ";
        }
        cout << endl;

        for (int neighbor = 1; neighbor <= numberOfNodes; neighbor++) {
            if (adjacencyMatrix[currentNode][neighbor] != 0 && !explored[neighbor]) {
                int newDistance;

                newDistance =
                    distance[currentNode] + adjacencyMatrix[currentNode][neighbor];
                if (newDistance < distance[neighbor])
                    distance[neighbor] = newDistance;
            }
        }
    }

    // Print the final distances
    cout << "Final distances from node " << start << ": ";
    for (int i = 1; i <= numberOfNodes; i++) {
        if (distance[i] == 9999)
            cout << "INF ";
        else
            cout << distance[i] << " ";
    }
    cout << endl;
}

void Graph::prim() {
    int key[100];
    int parent[100];
    bool lockedin[100];
    int totalCost = 0;

    int start;
    cout << "Enter starting node for prim: ";
    cin >> start;

    for (int i = 1; i <= numberOfNodes; i++) {
        key[i] = 9999;
        parent[i] = -1;
        lockedin[i] = false;
    }

    key[start] = 0;

    cout << "Selected Edges:\n";

    for (int count = 1; count <= numberOfNodes; count++) {
        int minKey = 9999;
        int u = -1;

        for (int v = 1; v <= numberOfNodes; v++) {
            if (!lockedin[v] && key[v] < minKey) {
                minKey = key[v];
                u = v;
            }
        }

        lockedin[u] = true;

        if (parent[u] != -1) {
            cout << parent[u] << " - " << u << " (Weight: " << key[u] << ")\n";
            totalCost += key[u];
        }

        for (int v = 1; v <= numberOfNodes; v++) {
            if (adjacencyMatrix[u][v] != 0 && !lockedin[v] &&
                adjacencyMatrix[u][v] < key[v]) {
                key[v] = adjacencyMatrix[u][v];
                parent[v] = u;
            }
        }
    }

    cout << "Total Cost of the Minimum Spanning Tree: " << totalCost << endl;
}

void Graph::kruskal() {
    int edges[100][3];
    int edgeCount = 0;
    int totalCost = 0;

    for (int u = 1; u <= numberOfNodes; u++) {
        for (int v = u + 1; v <= numberOfNodes; v++) {
            if (adjacencyMatrix[u][v] != 0) {
                edges[edgeCount][0] = adjacencyMatrix[u][v];
                edges[edgeCount][1] = u;
                edges[edgeCount][2] = v;
                edgeCount++;
            }
        }
    }

    for (int i = 0; i < edgeCount - 1; i++) {
        for (int j = 0; j < edgeCount - i - 1; j++) {
            if (edges[j][0] > edges[j + 1][0]) {
                for (int k = 0; k < 3; k++) {
                    int temp = edges[j][k];
                    edges[j][k] = edges[j + 1][k];
                    edges[j + 1][k] = temp;
                }
            }
        }
    }

    int parent[100], rank[100];
    for (int i = 1; i <= numberOfNodes; i++) {
        parent[i] = i;
        rank[i] = 0;
    }

    cout << "Selected Edges:\n";

    for (int i = 0; i < edgeCount; i++) {
        int weight = edges[i][0];
        int u = edges[i][1];
        int v = edges[i][2];

        if (u > v) {
            int temp = u;
            u = v;
            v = temp;
        }

        int findU = u;
        while (findU != parent[findU])
            findU = parent[findU];

        int findV = v;
        while (findV != parent[findV])
            findV = parent[findV];

        if (findU != findV) {
            if (rank[findU] > rank[findV]) {
                parent[findV] = findU;
            } else if (rank[findU] < rank[findV]) {
                parent[findU] = findV;
            } else {
                parent[findV] = findU;
                rank[findU]++;
            }

            cout << u << " - " << v << " (Weight: " << weight << ")\n";
            totalCost += weight;
        }
    }

    cout << "Total Cost of the Minimum Spanning Tree: " << totalCost << endl;
}
void Graph::dfssp(int x) {
    stack< pair<int, int> > stackDfs;
    bool visited[100] = {false};

    stackDfs.push(make_pair(x, 0));

    while (!stackDfs.empty()) {
        int value = stackDfs.top().first;
        int parent = stackDfs.top().second;
        stackDfs.pop();

        if (!visited[value]) {
            visited[value] = true;
            if (parent != 0)
                cout << parent << "-" << value << " ";

            for (int i = numberOfNodes; i >= 1; i--) {
                if (adjacencyMatrix[value][i] != 0 && !visited[i])
                    stackDfs.push(make_pair(i, value));
            }
        }
    }
}
