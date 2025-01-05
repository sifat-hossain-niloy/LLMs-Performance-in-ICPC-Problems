#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstring>

using namespace std;

const double EPS = 1e-8;
const double INF = 1e100;

struct LP {
    int n; // number of variables
    int m; // number of constraints
    vector<vector<double>> A; // Coefficient matrix
    vector<double> b; // Right-hand side
    vector<double> c; // Objective function
    vector<double> x; // Solution
    vector<int> B, N; // Basis and non-basis indices

    LP(int n_, int m_) : n(n_), m(m_), A(m_, vector<double>(n_)), b(m_), c(n_) {}

    void pivot(int r, int s) {
        double inv = 1.0 / A[r][s];
        for (int i = 0; i < m; i++) {
            if (i != r) {
                double coeff = A[i][s];
                for (int j = 0; j < n; j++) {
                    if (j != s) {
                        A[i][j] -= coeff * A[r][j] * inv;
                    }
                }
                b[i] -= coeff * b[r] * inv;
                A[i][s] = -coeff * inv;
            }
        }
        for (int j = 0; j < n; j++) {
            if (j != s) {
                A[r][j] *= inv;
            }
        }
        b[r] *= inv;
        A[r][s] = inv;
        swap(B[r], N[s]);
    }

    bool simplex() {
        while (true) {
            int s = -1;
            for (int j = 0; j < n; j++) {
                if (c[j] > EPS) {
                    s = j;
                    break;
                }
            }
            if (s == -1) break;

            int r = -1;
            double min_ratio = INF;
            for (int i = 0; i < m; i++) {
                if (A[i][s] > EPS) {
                    double ratio = b[i] / A[i][s];
                    if (ratio < min_ratio - EPS) {
                        min_ratio = ratio;
                        r = i;
                    }
                }
            }
            if (r == -1) return false; // Unbounded
            pivot(r, s);
        }
        x.assign(n, 0);
        for (int i = 0; i < m; i++) {
            if (B[i] < n) x[B[i]] = b[i];
        }
        return true;
    }

    bool solve(vector<double>& result) {
        B.resize(m);
        N.resize(n);
        for (int i = 0; i < m; i++) B[i] = n + i;
        for (int i = 0; i < n; i++) N[i] = i;
        if (!simplex()) return false;
        result = x;
        return true;
    }
};

int n;
double dist[30][30];
vector<pair<int, int>> edges; // (from, to)
vector<int> edge_indices[30][30];
double d_e[100];
double t_e_min[100], t_e_max[100];
vector<int> route_edges[100]; // edges in each route
double T_r[100];
vector<int> query_edges[100]; // edges in each query
int main() {
    cin >> n;
    int edge_cnt = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double d;
            cin >> d;
            if (d >= 0 && i != j) {
                dist[i][j] = d;
                edges.push_back({i, j});
                edge_indices[i][j].push_back(edge_cnt);
                d_e[edge_cnt] = d;
                t_e_min[edge_cnt] = d / 60.0;
                t_e_max[edge_cnt] = d / 30.0;
                edge_cnt++;
            } else {
                dist[i][j] = INF;
            }
        }
    }
    // Compute shortest paths (distances)
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j] - EPS) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
    // Build path edges for shortest paths
    int r;
    cin >> r;
    for (int i = 0; i < r; i++) {
        int s, d;
        double t;
        cin >> s >> d >> t;
        T_r[i] = t;
        // Reconstruct the shortest path from s to d
        vector<int> path_edges;
        int u = s;
        while (u != d) {
            for (int v = 0; v < n; v++) {
                if (u != v && dist[s][u] + dist[u][v] + dist[v][d] == dist[s][d] - EPS) {
                    // Edge from u to v is on the shortest path
                    // Get edge index
                    int idx = edge_indices[u][v][0]; // Since unique
                    path_edges.push_back(idx);
                    u = v;
                    break;
                }
            }
        }
        route_edges[i] = path_edges;
    }
    int q;
    cin >> q;
    for (int i = 0; i < q; i++) {
        int s, d;
        cin >> s >> d;
        // Reconstruct the shortest path from s to d
        vector<int> path_edges;
        int u = s;
        while (u != d) {
            for (int v = 0; v < n; v++) {
                if (u != v && dist[s][u] + dist[u][v] + dist[v][d] == dist[s][d] - EPS) {
                    // Edge from u to v is on the shortest path
                    // Get edge index
                    int idx = edge_indices[u][v][0]; // Since unique
                    path_edges.push_back(idx);
                    u = v;
                    break;
                }
            }
        }
        query_edges[i] = path_edges;
    }
    // Now, for each query, solve two LPs
    for (int qi = 0; qi < q; qi++) {
        // Minimize total time
        int num_vars = edge_cnt;
        int num_constraints = r;
        LP lp_min(num_vars, num_constraints);
        LP lp_max(num_vars, num_constraints);
        // Set up constraints
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < num_vars; j++) {
                lp_min.A[i][j] = 0.0;
                lp_max.A[i][j] = 0.0;
            }
            for (int idx : route_edges[i]) {
                lp_min.A[i][idx] = 1.0;
                lp_max.A[i][idx] = 1.0;
            }
            lp_min.b[i] = T_r[i];
            lp_max.b[i] = T_r[i];
        }
        // Objective function
        for (int j = 0; j < num_vars; j++) {
            lp_min.c[j] = 0.0;
            lp_max.c[j] = 0.0;
        }
        for (int idx : query_edges[qi]) {
            lp_min.c[idx] = 1.0;
            lp_max.c[idx] = -1.0;
        }
        // Solve LP for minimum
        vector<double> result_min;
        if (!lp_min.solve(result_min)) {
            cout << "No solution" << endl;
            continue;
        }
        double min_time = 0.0;
        for (int idx : query_edges[qi]) {
            min_time += result_min[idx];
        }
        // Solve LP for maximum
        vector<double> result_max;
        if (!lp_max.solve(result_max)) {
            cout << "No solution" << endl;
            continue;
        }
        double max_time = 0.0;
        for (int idx : query_edges[qi]) {
            max_time += result_max[idx];
        }
        // Output the results
        cout << query_edges[qi][0] << " " << query_edges[qi].back() << " ";
        printf("%.6lf %.6lf\n", min_time, -max_time);
    }
    return 0;
}
