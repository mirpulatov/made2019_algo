/*
* Дан неориентированный связный граф.
* Требуется найти вес минимального остовного дерева в этом графе.
* С помощью алгоритма Прима.
*/

#include <iostream>
#include <vector>
#include <list>
#include <set>

using namespace std;

const int INF = 1000000000;

typedef pair<int, int> node;

class Graph {
public:
    int vertices_count;

    vector<pair<int, int>>* adjList;

    Graph(int vertices_count);

    ~Graph();

    void addNode(int src, int dest, int weight);

    void Prima();
};

Graph::Graph(int vertices_count) {
    this->vertices_count = vertices_count;
    adjList = new vector<node>[vertices_count];
}

Graph::~Graph() {
    delete[] adjList;
}

void Graph::addNode(int src, int dest, int weight) {
    adjList[src].emplace_back(make_pair(dest, weight));
    adjList[dest].emplace_back(make_pair(src, weight));
}

void Graph::Prima() {
    int mst_weight = 0;
    vector<int> key(vertices_count, INF);
    vector<int> p(vertices_count, -1);
    vector<bool> visited(vertices_count, false);
    set <pair<int, int>> prior_queue;

    key[0] = 0;
    prior_queue.insert(make_pair(0, 0));

    for (int i = 0; i < vertices_count; ++i) {
        if (prior_queue.empty()) {
            cout << 0;
            exit(0);
        }

        int weight = prior_queue.begin()->first;
        int v = prior_queue.begin()->second;
        prior_queue.erase(prior_queue.begin());

        visited[v] = true;

        if (p[v] != -1) {
            mst_weight += weight;
        }

        for (int j = 0; j < adjList[v].size(); ++j) {
            int u = adjList[v][j].first,
                w = adjList[v][j].second;
            if (!visited[u] && w < key[u]) {
                prior_queue.erase(make_pair(key[u], u));
                key[u] = w;
                p[u] = v;
                prior_queue.insert(make_pair(key[u], u));
            }
        }
    }
    cout << mst_weight << endl;
}

int main()
{
    int n, m;
    int b, e, w;

    cin >> n >> m;

    Graph g(n);
    for (int i = 0; i < m; i++) {
        cin >> b >> e >> w;
        g.addNode(b - 1 , e - 1, w);
    }
    g.Prima();

    return 0;
}