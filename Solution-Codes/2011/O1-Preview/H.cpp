#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <cstring>

using namespace std;

const int MAXN = 50005;

int N, Case = 1;
vector<int> adj[MAXN];
int dfsnum[MAXN], low[MAXN], timer;
bool isArticulation[MAXN];
int parent[MAXN];
vector<vector<int>> biconnComponents;
stack<pair<int, int>> edgeStack;

void tarjan(int u, int p) {
    dfsnum[u] = low[u] = ++timer;
    int children = 0;

    for (int v : adj[u]) {
        if (!dfsnum[v]) {
            edgeStack.push({u, v});
            parent[v] = u;
            children++;
            tarjan(v, u);
            low[u] = min(low[u], low[v]);

            if ((dfsnum[u] == 1 && children > 1) || (dfsnum[u] > 1 && low[v] >= dfsnum[u])) {
                isArticulation[u] = true;
                vector<int> component;
                while (edgeStack.top() != make_pair(u, v) && edgeStack.top() != make_pair(v, u)) {
                    component.push_back(edgeStack.top().first);
                    component.push_back(edgeStack.top().second);
                    edgeStack.pop();
                }
                component.push_back(edgeStack.top().first);
                component.push_back(edgeStack.top().second);
                edgeStack.pop();
                biconnComponents.push_back(component);
            }
        } else if (v != p && dfsnum[v] < dfsnum[u]) {
            edgeStack.push({u, v});
            low[u] = min(low[u], dfsnum[v]);
        }
    }
}

int main() {
    while (cin >> N && N) {
        // Reset data structures
        for (int i = 0; i < MAXN; ++i) adj[i].clear();
        memset(dfsnum, 0, sizeof(dfsnum));
        memset(low, 0, sizeof(low));
        memset(isArticulation, 0, sizeof(isArticulation));
        memset(parent, -1, sizeof(parent));
        timer = 0;
        biconnComponents.clear();

        int maxNode = 0;
        for (int i = 0; i < N; ++i) {
            int u, v;
            cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
            maxNode = max(maxNode, max(u, v));
        }

        tarjan(1, -1);

        // Process remaining edges in the stack
        while (!edgeStack.empty()) {
            vector<int> component;
            while (!edgeStack.empty()) {
                component.push_back(edgeStack.top().first);
                component.push_back(edgeStack.top().second);
                edgeStack.pop();
            }
            biconnComponents.push_back(component);
        }

        // Build block tree and count leaf blocks
        int leafBlocks = 0;
        vector<int> degree(biconnComponents.size(), 0);
        vector<vector<int>> blockAdj(biconnComponents.size());

        // Map from node to blocks
        vector<vector<int>> nodeBlocks(maxNode + 1);

        for (int i = 0; i < biconnComponents.size(); ++i) {
            sort(biconnComponents[i].begin(), biconnComponents[i].end());
            biconnComponents[i].erase(unique(biconnComponents[i].begin(), biconnComponents[i].end()), biconnComponents[i].end());
            for (int node : biconnComponents[i]) {
                nodeBlocks[node].push_back(i);
            }
        }

        for (int node = 1; node <= maxNode; ++node) {
            if (isArticulation[node]) {
                for (int i = 0; i < nodeBlocks[node].size(); ++i) {
                    for (int j = i + 1; j < nodeBlocks[node].size(); ++j) {
                        // Connect blocks via articulation point
                        blockAdj[nodeBlocks[node][i]].push_back(nodeBlocks[node][j]);
                        blockAdj[nodeBlocks[node][j]].push_back(nodeBlocks[node][i]);
                    }
                }
            }
        }

        // Count leaf blocks in the block tree
        for (int i = 0; i < blockAdj.size(); ++i) {
            degree[i] = blockAdj[i].size();
        }
        for (int i = 0; i < blockAdj.size(); ++i) {
            if (degree[i] == 1) {
                leafBlocks++;
            }
        }

        // The minimal number of escape shafts is (number of leaf blocks + 1) / 2
        int minShafts = (leafBlocks + 1) / 2;

        // For the number of ways, if leafBlocks <= 1, ways = 1
        // Else, ways = choose(leafBlocks, minShafts)
        long long ways = 1;
        if (leafBlocks > 1) {
            int k = minShafts;
            int n = leafBlocks;
            // Compute C(n,k)
            long long numer = 1;
            long long denom = 1;
            for (int i = 1; i <= k; ++i) {
                numer *= (n - i + 1);
                denom *= i;
            }
            ways = numer / denom;
        }

        cout << "Case " << Case++ << ": " << minShafts << " " << ways << endl;
    }
    return 0;
}
