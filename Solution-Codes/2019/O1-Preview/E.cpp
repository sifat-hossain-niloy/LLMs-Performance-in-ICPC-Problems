#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const int MAXN = 500005;

vector<int> adj[MAXN];
vector<pair<int, int>> signs;
bool visited[MAXN];

int dfs(int u, int parent) {
    visited[u] = true;
    bool need_sign = false;
    for (int v : adj[u]) {
        if (v == parent) continue;
        if (!visited[v]) {
            int res = dfs(v, u);
            if (res == -1) {
                // Child needs coverage, place sign at u-entrance of edge (u,v)
                signs.push_back({u, v});
                need_sign = true;
            }
        }
    }
    if (adj[u].size() == 1 && parent != -1) {
        // Leaf node
        return -1;
    }
    if (need_sign) {
        return 1; // Sign placed at u
    } else {
        if (parent == -1) {
            // Root node
            return 0;
        } else {
            return -1; // Needs coverage
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<pair<int, int>> edges(m);
    for (int i = 0; i < m; ++i) {
        int v, w;
        cin >> v >> w;
        adj[v].push_back(w);
        adj[w].push_back(v);
        edges[i] = {v, w};
    }

    for (int i = 1; i <= n; ++i) {
        if (!visited[i]) {
            dfs(i, -1);
        }
    }

    // Sort the signs as required
    sort(signs.begin(), signs.end());

    cout << signs.size() << '\n';
    for (auto& sign : signs) {
        cout << sign.first << ' ' << sign.second << '\n';
    }

    return 0;
}
