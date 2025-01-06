#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <limits.h>

using namespace std;

const int MAXN = 20;
const int INF = INT_MAX / 2;

int n, m;
int dist[MAXN][MAXN];
int dp[1 << MAXN][MAXN];
int pre[1 << MAXN][MAXN];

int main() {
    int case_num = 1;
    while (cin >> n >> m) {
        // Initialize distance matrix
        for (int i = 0; i < n; ++i)
            fill(dist[i], dist[i] + n, INF);
        for (int i = 0; i < n; ++i)
            dist[i][i] = 0;
        // Read edges
        for (int i = 0; i < m; ++i) {
            int u, v, t;
            cin >> u >> v >> t;
            dist[u][v] = min(dist[u][v], t);
            dist[v][u] = min(dist[v][u], t);
        }
        // Floyd-Warshall to compute all pairs shortest paths
        for (int k = 0; k < n; ++k)
            for (int i = 0; i < n; ++i)
                if (dist[i][k] < INF)
                    for (int j = 0; j < n; ++j)
                        if (dist[k][j] < INF)
                            dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
        // DP initialization
        int full_mask = (1 << n) - 1;
        memset(dp, 0x3f, sizeof(dp));
        dp[1 << 0][0] = 0;
        memset(pre, -1, sizeof(pre));
        // DP
        for (int mask = 0; mask < (1 << n); ++mask) {
            for (int u = 0; u < n; ++u) {
                if (dp[mask][u] < INF) {
                    for (int v = 1; v < n; ++v) { // Skip headquarters
                        if (!(mask & (1 << v))) {
                            int next_mask = mask | (1 << v);
                            if (dp[next_mask][v] > dp[mask][u] + dist[u][v]) {
                                dp[next_mask][v] = dp[mask][u] + dist[u][v];
                                pre[next_mask][v] = u;
                            }
                        }
                    }
                }
            }
        }
        // The attraction is node n-1
        int attraction = n - 1;
        int min_cost = INF;
        int end_mask = 0;
        // We need to consider masks where all hotels and the attraction are visited
        int hotel_mask = ((1 << n) - 1) & (~(1 << 0)); // Exclude headquarters
        for (int mask = 0; mask < (1 << n); ++mask) {
            if ((mask & (1 << attraction)) && ((mask & hotel_mask) == hotel_mask)) {
                if (min_cost > dp[mask][attraction]) {
                    min_cost = dp[mask][attraction];
                    end_mask = mask;
                }
            }
        }
        // Reconstruct the path
        vector<int> path;
        int curr = attraction;
        int mask = end_mask;
        while (curr != -1) {
            path.push_back(curr);
            int temp = pre[mask][curr];
            mask ^= (1 << curr);
            curr = temp;
        }
        reverse(path.begin(), path.end());
        // Compute reverse path cost
        int reverse_cost = 0;
        for (size_t i = path.size() - 1; i > 0; --i) {
            reverse_cost += dist[path[i]][path[i - 1]];
        }
        // Add cost from headquarters at the end
        reverse_cost += dist[path[0]][0];
        // Total tour time
        int total_cost = min_cost + reverse_cost;
        cout << "Case " << case_num++ << ": " << total_cost << endl;
    }
    return 0;
}
