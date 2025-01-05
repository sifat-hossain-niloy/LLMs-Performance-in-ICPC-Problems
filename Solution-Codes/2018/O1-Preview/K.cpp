#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstdio>
using namespace std;

const int MAXN = 10005;

int n, m;
int degree[MAXN];
int adjusted_degree[MAXN];
bool degree_changed[MAXN];
vector<pair<int, int>> edges;

int main() {
    scanf("%d %d", &n, &m);
    int u, v;
    int total_degree = 0;
    for (int i = 0; i < n; ++i) {
        degree[i] = 0;
        degree_changed[i] = false;
    }
    for (int i = 0; i < m; ++i) {
        scanf("%d %d", &u, &v);
        degree[u]++;
        degree[v]++;
    }
    total_degree = 0;
    for (int i = 0; i < n; ++i) {
        adjusted_degree[i] = degree[i];
        total_degree += degree[i];
    }

    int surplus = total_degree - 2 * (n - 1);

    // Adjust degrees to form a tree
    vector<pair<int, int>> nodes; // (degree, node)
    for (int i = 0; i < n; ++i) {
        nodes.push_back(make_pair(-adjusted_degree[i], i)); // Negative degree for decreasing order
    }
    sort(nodes.begin(), nodes.end());
    for (int i = 0; i < n && surplus > 0; ++i) {
        int idx = nodes[i].second;
        while (surplus > 0 && adjusted_degree[idx] > 1) {
            adjusted_degree[idx]--;
            surplus--;
            degree_changed[idx] = true;
        }
    }

    // Now construct the tree
    vector<int> deg(n);
    for (int i = 0; i < n; ++i) {
        deg[i] = adjusted_degree[i];
    }

    priority_queue<int, vector<int>, greater<int>> leaves;
    queue<int> others;
    for (int i = 0; i < n; ++i) {
        if (deg[i] == 1) {
            leaves.push(i);
        } else if (deg[i] > 1) {
            others.push(i);
        }
    }

    while (leaves.size() + others.size() > 1) {
        int u = leaves.top();
        leaves.pop();
        int v;
        if (!others.empty()) {
            v = others.front();
            others.pop();
        } else {
            v = leaves.top();
            leaves.pop();
        }
        edges.push_back(make_pair(u, v));
        deg[v]--;
        if (deg[v] == 1) {
            leaves.push(v);
        } else if (deg[v] > 1) {
            others.push(v);
        }
    }

    // Output the number of nodes whose degrees changed
    int changed_nodes = 0;
    for (int i = 0; i < n; ++i) {
        if (degree_changed[i]) {
            changed_nodes++;
        }
    }
    printf("%d\n", changed_nodes);

    // Output the tree
    printf("%d %d\n", n, (int)edges.size());
    for (size_t i = 0; i < edges.size(); ++i) {
        printf("%d %d\n", edges[i].first, edges[i].second);
    }

    return 0;
}
