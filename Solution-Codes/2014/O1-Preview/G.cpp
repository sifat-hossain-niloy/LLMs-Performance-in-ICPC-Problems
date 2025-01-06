#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstring>
using namespace std;

const int MAXN = 205;

struct Edge {
    int u, v;
    int w;
    bool operator<(const Edge& e) const {
        return w < e.w;
    }
};

int n;
vector<Edge> edges;
vector<Edge> mstEdges;
int parent[MAXN];

int findSet(int u) {
    if (parent[u] != u)
        parent[u] = findSet(parent[u]);
    return parent[u];
}

void unionSet(int u, int v) {
    parent[findSet(u)] = findSet(v);
}

vector<pair<int, int>> adj[MAXN]; // adj[node] = vector of (neighbor, weight)

void dfs(int u, int excludeU, int excludeV, bool visited[], int& maxEdge) {
    visited[u] = true;
    for (auto& p : adj[u]) {
        int v = p.first;
        int w = p.second;
        if ((u == excludeU && v == excludeV) || (u == excludeV && v == excludeU))
            continue;
        if (!visited[v]) {
            if (w > maxEdge)
                maxEdge = w;
            dfs(v, excludeU, excludeV, visited, maxEdge);
        }
    }
}

int main() {
    // Read input
    cin >> n;
    vector<vector<int>> d(n+1, vector<int>(n+1, 0));
    for (int i = 1; i <= n; i++) {
        parent[i] = i;
    }
    int cnt = 0;
    for (int i = 1; i < n; i++) {
        for (int j = i+1; j <= n; j++) {
            int dist;
            cin >> dist;
            d[i][j] = d[j][i] = dist;
            edges.push_back({i, j, dist});
        }
    }

    // Build MST
    sort(edges.begin(), edges.end());
    for (auto& e : edges) {
        int u = e.u, v = e.v;
        int pu = findSet(u);
        int pv = findSet(v);
        if (pu != pv) {
            unionSet(u, v);
            mstEdges.push_back(e);
            adj[u].push_back({v, e.w});
            adj[v].push_back({u, e.w});
        }
        if (mstEdges.size() == n - 1)
            break;
    }

    int minSum = INT32_MAX;

    // For each edge in MST, remove it and compute disparities
    for (auto& e : mstEdges) {
        int u = e.u, v = e.v;
        bool visited[MAXN];
        memset(visited, 0, sizeof(visited));
        int maxEdge1 = 0, maxEdge2 = 0;
        dfs(u, u, v, visited, maxEdge1);
        dfs(v, u, v, visited, maxEdge2);
        int sum = maxEdge1 + maxEdge2;
        if (sum < minSum)
            minSum = sum;
    }

    cout << minSum << endl;

    return 0;
}
