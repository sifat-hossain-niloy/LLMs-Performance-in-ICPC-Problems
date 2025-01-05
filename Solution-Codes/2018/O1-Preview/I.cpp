#include <iostream>
#include <cstdio>
#include <map>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

typedef pair<int, int> pii;

int main() {
    int r, c;
    scanf("%d %d\n", &r, &c);

    int max_L = 2 * r - 1;
    vector<string> grid(max_L + 2); // +2 to avoid out-of-bounds access

    for (int L = 1; L <= max_L; ++L) {
        getline(cin, grid[L]);
        // Ensure the line is the expected length
        // If the line is shorter, pad it with spaces
        if (grid[L].length() < 2 * c - 1) {
            grid[L].resize(2 * c - 1, ' ');
        }
    }

    map<pii, int> pos_to_id;
    vector<pii> id_to_pos;
    int vertex_id = 0;

    // First pass: assign IDs to vertices
    for (int L = 1; L <= max_L; ++L) {
        int mod = L % 4;
        int P0 = (mod == 1) ? 0 : (mod == 3) ? 2 : -1;
        if (P0 == -1) continue; // Lines without vertices
        for (int P = P0; P < grid[L].length(); P += 4) {
            if (grid[L][P] == 'x') {
                pos_to_id[make_pair(L, P)] = vertex_id++;
                id_to_pos.push_back(make_pair(L, P));
            }
        }
    }

    // Build adjacency list
    vector<vector<int>> adj(vertex_id);

    // Second pass: process edges
    for (int L = 1; L <= max_L; ++L) {
        for (int P = 0; P < grid[L].length(); ++P) {
            char ch = grid[L][P];
            if (ch == '-') {
                // Check for horizontal edge '---'
                if (P + 2 < grid[L].length() && grid[L][P + 1] == '-' && grid[L][P + 2] == '-') {
                    pii pos1 = make_pair(L, P - 1);
                    pii pos2 = make_pair(L, P + 3);
                    auto it1 = pos_to_id.find(pos1);
                    auto it2 = pos_to_id.find(pos2);
                    if (it1 != pos_to_id.end() && it2 != pos_to_id.end()) {
                        int u = it1->second;
                        int v = it2->second;
                        adj[u].push_back(v);
                        adj[v].push_back(u);
                    }
                }
            } else if (ch == '/' || ch == '\\') {
                // Diagonal edges
                int delta_L = (ch == '/') ? -1 : -1;
                int delta_P1 = (ch == '/') ? +1 : -1;
                int delta_P2 = (ch == '/') ? -1 : +1;
                pii pos1 = make_pair(L + delta_L, P + delta_P1);
                pii pos2 = make_pair(L - delta_L, P + delta_P2);
                auto it1 = pos_to_id.find(pos1);
                auto it2 = pos_to_id.find(pos2);
                if (it1 != pos_to_id.end() && it2 != pos_to_id.end()) {
                    int u = it1->second;
                    int v = it2->second;
                    adj[u].push_back(v);
                    adj[v].push_back(u);
                }
            }
        }
    }

    // Sort adjacency lists
    for (int i = 0; i < vertex_id; ++i) {
        sort(adj[i].begin(), adj[i].end());
    }

    // Count triangles
    long long triangle_count = 0;
    for (int u = 0; u < vertex_id; ++u) {
        for (int vi = 0; vi < adj[u].size(); ++vi) {
            int v = adj[u][vi];
            if (v > u) {
                // Find common neighbors of u and v
                vector<int>& adj_u = adj[u];
                vector<int>& adj_v = adj[v];
                int p1 = 0, p2 = 0;
                while (p1 < adj_u.size() && p2 < adj_v.size()) {
                    int w1 = adj_u[p1], w2 = adj_v[p2];
                    if (w1 <= u) { ++p1; continue; }
                    if (w2 <= v) { ++p2; continue; }
                    if (w1 == w2) {
                        triangle_count++;
                        ++p1; ++p2;
                    } else if (w1 < w2) {
                        ++p1;
                    } else {
                        ++p2;
                    }
                }
            }
        }
    }

    cout << triangle_count << endl;
    return 0;
}
