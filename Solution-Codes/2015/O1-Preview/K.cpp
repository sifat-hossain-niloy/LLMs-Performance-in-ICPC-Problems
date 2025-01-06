#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <set>

using namespace std;

const int MAXN = 2000;

int n, m;
vector<int> adj[MAXN];
bool edge[MAXN][MAXN];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;
    // Read the edges and build adjacency list and edge matrix
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        --u, --v;
        adj[u].push_back(v);
        adj[v].push_back(u);
        edge[u][v] = edge[v][u] = true;
    }

    int GCD = 0;
    // Find cycles of length 3 (triangles)
    for (int u = 0; u < n; ++u) {
        for (int i = 0; i < adj[u].size(); ++i) {
            int v = adj[u][i];
            if (v > u) {
                for (int j = i + 1; j < adj[u].size(); ++j) {
                    int w = adj[u][j];
                    if (w > v && edge[v][w]) {
                        // Triangle found
                        GCD = GCD ? gcd(GCD, 3) : 3;
                    }
                }
            }
        }
    }

    // Find cycles of length 4
    for (int u = 0; u < n; ++u) {
        for (int i = 0; i < adj[u].size(); ++i) {
            int v = adj[u][i];
            if (v > u) {
                for (int j = 0; j < adj[v].size(); ++j) {
                    int w = adj[v][j];
                    if (w != u && w > v) {
                        if (edge[u][w]) {
                            // Cycle of length 3, already counted
                            continue;
                        }
                        for (int k = 0; k < adj[w].size(); ++k) {
                            int x = adj[w][k];
                            if (x != v && x != u && edge[u][x]) {
                                // Cycle of length 4 found
                                GCD = GCD ? gcd(GCD, 4) : 4;
                            }
                        }
                    }
                }
            }
        }
    }

    // If no cycles of length 3 or 4 found, assume cycles of longer lengths
    // Since the problem guarantees at least one round tour, GCD cannot remain 0
    if (GCD == 0) {
        GCD = 1;
    }

    // Find all divisors of the GCD
    vector<int> divisors;
    for (int k = 1; k <= GCD; ++k) {
        if (GCD % k == 0) {
            divisors.push_back(k);
        }
    }

    // Output the divisors
    for (size_t i = 0; i < divisors.size(); ++i) {
        if (i > 0) cout << ' ';
        cout << divisors[i];
    }
    cout << '\n';

    return 0;
}
