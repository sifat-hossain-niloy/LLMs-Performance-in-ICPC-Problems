#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct Edge {
    int to;
    int cost;
};

const int MAXN = 250000;
vector<Edge> tree[MAXN + 1];
long long total_cost = 0;
vector<int> xi(MAXN + 1);
vector<int> yi(MAXN + 1);

long long dfs(int u, int parent) {
    long long surplus = xi[u] - yi[u];
    for (auto& edge : tree[u]) {
        int v = edge.to;
        if (v == parent) continue;
        long long child_surplus = dfs(v, u);
        // Decide the amount to move from child to parent
        long long amount = child_surplus;
        total_cost += abs(amount) * edge.cost;
        surplus += amount;
    }
    return surplus;
}

int main() {
    int n;
    cin >> n;
    // Read edges
    for (int i = 0; i < n - 1; ++i) {
        int u, v, c;
        cin >> u >> v >> c;
        tree[u].push_back({v, c});
        tree[v].push_back({u, c});
    }
    // Read xi and yi
    for (int i = 1; i <= n; ++i) {
        cin >> xi[i] >> yi[i];
    }
    dfs(1, 0);
    cout << total_cost << endl;
    return 0;
}
