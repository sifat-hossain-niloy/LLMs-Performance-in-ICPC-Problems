#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstring>
#include <cmath>

using namespace std;

const int MAXN = 1000;
const int INF = 1e9;

struct Edge {
    int to, rev;
    int capacity;
};

vector<Edge> graph[2 + 2 * 40 + 1600]; // S, T, rows, columns, edges
int level[2 + 2 * 40 + 1600];
int iter[2 + 2 * 40 + 1600];

void add_edge(int from, int to, int capacity) {
    graph[from].push_back((Edge){to, (int)graph[to].size(), capacity});
    graph[to].push_back((Edge){from, (int)graph[from].size() - 1, 0});
}

void bfs(int s, int t) {
    memset(level, -1, sizeof(level));
    queue<int> que;
    level[s] = 0;
    que.push(s);
    while (!que.empty()) {
        int v = que.front(); que.pop();
        for (auto &e : graph[v]) {
            if (e.capacity > 0 && level[e.to] < 0) {
                level[e.to] = level[v] + 1;
                que.push(e.to);
            }
        }
    }
}

int dfs(int v, int t, int upTo) {
    if (v == t) return upTo;
    for (int &i = iter[v]; i < graph[v].size(); ++i) {
        Edge &e = graph[v][i];
        if (e.capacity > 0 && level[v] < level[e.to]) {
            int d = dfs(e.to, t, min(upTo, e.capacity));
            if (d > 0) {
                e.capacity -= d;
                graph[e.to][e.rev].capacity += d;
                return d;
            }
        }
    }
    return 0;
}

int max_flow(int s, int t) {
    int flow = 0;
    for (;;) {
        bfs(s, t);
        if (level[t] < 0) break;
        memset(iter, 0, sizeof(iter));
        int f;
        while ((f = dfs(s, t, INF)) > 0) {
            flow += f;
        }
    }
    return flow;
}

int main() {
    int N, A, B, case_num = 1;
    while (cin >> N >> A >> B, N || A || B) {
        vector<string> grid(N);
        for (int i = 0; i < N; ++i) {
            cin >> grid[i];
        }

        int preplaced_components = 0;
        vector<int> preplaced_row(N, 0), preplaced_col(N, 0);
        vector<int> available_row(N, 0), available_col(N, 0);
        int total_empty_slots = 0;

        // Check if initial counts in rows and columns are equal
        bool possible = true;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                char c = grid[i][j];
                if (c == 'C') {
                    preplaced_components++;
                    preplaced_row[i]++;
                    preplaced_col[j]++;
                } else if (c == '.') {
                    available_row[i]++;
                    available_col[j]++;
                    total_empty_slots++;
                }
            }
            if (preplaced_row[i] != preplaced_col[i]) {
                possible = false;
            }
        }

        if (!possible) {
            cout << "Case " << case_num++ << ": impossible" << endl;
            continue;
        }

        int max_possible_components = preplaced_components + total_empty_slots;
        int max_added_components = total_empty_slots;
        int found = -1;

        for (int total_components = max_possible_components; total_components >= preplaced_components; --total_components) {
            int max_components_per_row = (A * total_components) / B;
            // Check capacities
            vector<int> cap_row(N), cap_col(N);
            bool feasible = true;
            for (int i = 0; i < N; ++i) {
                cap_row[i] = min(available_row[i], max_components_per_row - preplaced_row[i]);
                if (cap_row[i] < 0) {
                    feasible = false;
                    break;
                }
                cap_col[i] = min(available_col[i], max_components_per_row - preplaced_col[i]);
                if (cap_col[i] < 0) {
                    feasible = false;
                    break;
                }
            }
            if (!feasible) continue;

            // Build flow network
            int S = 0, T = 1;
            int node_id = 2;
            int row_offset = node_id;
            node_id += N;
            int col_offset = node_id;
            node_id += N;

            // Reset graph
            for (int i = 0; i < node_id; ++i) {
                graph[i].clear();
            }

            // Edges from S to rows
            for (int i = 0; i < N; ++i) {
                if (cap_row[i] > 0)
                    add_edge(S, row_offset + i, cap_row[i]);
            }

            // Edges from columns to T
            for (int i = 0; i < N; ++i) {
                if (cap_col[i] > 0)
                    add_edge(col_offset + i, T, cap_col[i]);
            }

            // Edges from rows to columns
            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < N; ++j) {
                    if (grid[i][j] == '.') {
                        add_edge(row_offset + i, col_offset + j, 1);
                    }
                }
            }

            int required_flow = total_components - preplaced_components;
            int flow = max_flow(S, T);
            if (flow == required_flow) {
                found = required_flow;
                break;
            }
        }

        if (found != -1) {
            cout << "Case " << case_num++ << ": " << found << endl;
        } else {
            cout << "Case " << case_num++ << ": impossible" << endl;
        }
    }
    return 0;
}
