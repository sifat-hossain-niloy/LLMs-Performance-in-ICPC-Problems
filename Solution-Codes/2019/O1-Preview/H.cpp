#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int MAXN = 500005;

vector<int> adj[MAXN];
int status[MAXN]; // 0: unvisited, 1: visiting, 2: visited
int depth[MAXN];
int cycleSize[MAXN];
int successor[MAXN];
int n, k;
vector<int> cycleNodes;

void dfs(int u) {
    status[u] = 1;
    int v = successor[u];
    if (status[v] == 0) {
        dfs(v);
        depth[u] = depth[v] + 1;
        cycleSize[u] = cycleSize[v];
    } else if (status[v] == 1) {
        // Found a cycle
        cycleNodes.clear();
        cycleNodes.push_back(u);
        int cur = v;
        while (cur != u) {
            cycleNodes.push_back(cur);
            cur = successor[cur];
        }
        int c = cycleNodes.size();
        for (int node : cycleNodes) {
            depth[node] = 0;
            cycleSize[node] = c;
        }
    } else if (status[v] == 2) {
        depth[u] = depth[v] + 1;
        cycleSize[u] = cycleSize[v];
    }
    status[u] = 2;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> k;
    for (int i = 1; i <= n; ++i) {
        cin >> successor[i];
    }

    fill(status, status + n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        if (status[i] == 0) {
            dfs(i);
        }
    }

    for (int i = 1; i <= n; ++i) {
        int ans;
        if (depth[i] == 0) {
            // Node is in a cycle
            ans = min(k + 1, cycleSize[i]);
        } else {
            if (k < depth[i]) {
                ans = k + 1;
            } else {
                int s = k - depth[i] + 1;
                ans = depth[i] + min(s, cycleSize[i]);
            }
        }
        cout << ans << '\n';
    }

    return 0;
}
