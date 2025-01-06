#include <iostream>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

using namespace std;

const double EPS = 1e-8;
const int MAXV = 1000; // Maximum number of variables
const int MAXC = 2000; // Maximum number of constraints

struct LP {
    int n, m; // Number of variables and constraints
    double A[MAXC][MAXV]; // Coefficient matrix
    double b[MAXC]; // Right-hand side
    double c[MAXV]; // Objective function coefficients
    int var[MAXV + MAXC]; // Variables
    int pos[MAXV + MAXC]; // Position of variables
    int basis[MAXC]; // Basis variables

    // Constructor
    LP() {
        memset(A, 0, sizeof(A));
        memset(b, 0, sizeof(b));
        memset(c, 0, sizeof(c));
        memset(var, 0, sizeof(var));
        memset(pos, 0, sizeof(pos));
        memset(basis, 0, sizeof(basis));
        n = m = 0;
    }

    // Simplex algorithm
    double simplex() {
        int i, j, k, l;
        while (true) {
            // Find entering variable
            for (k = 0, j = 1; j <= n; j++)
                if (c[j] > EPS) { k = j; break; }
            if (!k) break; // Optimal

            // Find leaving variable
            double min = 1e20;
            for (i = 1; i <= m; i++)
                if (A[i][k] > EPS && b[i] / A[i][k] < min) {
                    min = b[i] / A[i][k];
                    l = i;
                }
            if (min == 1e20) return 1e20; // Unbounded

            pivot(l, k);
        }
        return -c[0];
    }

    // Pivot operation
    void pivot(int r, int c) {
        int i, j;
        double t = A[r][c];
        A[r][c] = 1;
        for (j = 0; j <= n; j++)
            A[r][j] /= t;
        b[r] /= t;
        for (i = 0; i <= m; i++) {
            if (i != r && fabs(A[i][c]) > EPS) {
                t = A[i][c];
                A[i][c] = 0;
                for (j = 0; j <= n; j++)
                    A[i][j] -= A[r][j] * t;
                b[i] -= b[r] * t;
            }
        }
        // Update basis
        swap(var[c], basis[r]);
    }
};

int main() {
    int n, p;
    double v, a;
    cin >> n >> p >> v >> a;

    int numEdges = p;
    vector<int> u(p), w(p), c(p);
    for (int i = 0; i < p; i++) {
        cin >> u[i] >> w[i] >> c[i];
        u[i]--; w[i]--;
    }

    // Build the LP
    LP lp;
    int numNodes = n;
    int numVars = 0;

    // Map for variable indices
    vector<vector<int>> f_idx(n, vector<int>(n, -1));
    vector<vector<int>> w_idx(n, vector<int>(n, -1));

    // Variables: f_{i,j}, w_{i,j}
    for (int e = 0; e < p; e++) {
        int i = u[e], j = w[e];
        f_idx[i][j] = numVars++;
        f_idx[j][i] = numVars++;
        w_idx[i][j] = numVars++;
        w_idx[j][i] = numVars++;
    }

    // Variable k
    int k_idx = numVars++;
    lp.n = numVars;

    // Objective: Maximize k
    lp.c[k_idx + 1] = -1; // Since we minimize in simplex

    // Constraints
    int numConstraints = 0;

    // Flow conservation for Flubber
    for (int i = 0; i < n; i++) {
        // Flubber
        if (i != 0 && i != 2) {
            lp.m++;
            numConstraints++;
            for (int e = 0; e < p; e++) {
                int u = ::u[e], v = ::w[e];
                if (u == i) {
                    lp.A[lp.m][f_idx[i][v] + 1] = 1;
                    lp.A[lp.m][f_idx[v][i] + 1] = -1;
                }
                if (v == i) {
                    lp.A[lp.m][f_idx[v][i] + 1] = 1;
                    lp.A[lp.m][f_idx[i][v] + 1] = -1;
                }
            }
            lp.b[lp.m] = 0;
        }
    }

    // Flow conservation for Water
    for (int i = 0; i < n; i++) {
        // Water
        if (i != 1 && i != 2) {
            lp.m++;
            numConstraints++;
            for (int e = 0; e < p; e++) {
                int u = ::u[e], v = ::w[e];
                if (u == i) {
                    lp.A[lp.m][w_idx[i][v] + 1] = 1;
                    lp.A[lp.m][w_idx[v][i] + 1] = -1;
                }
                if (v == i) {
                    lp.A[lp.m][w_idx[v][i] + 1] = 1;
                    lp.A[lp.m][w_idx[i][v] + 1] = -1;
                }
            }
            lp.b[lp.m] = 0;
        }
    }

    // Source and Sink constraints for Flubber and Water
    // Node 1 (Flubber source)
    lp.m++;
    for (int e = 0; e < p; e++) {
        int u = ::u[e], v = ::w[e];
        if (u == 0) {
            lp.A[lp.m][f_idx[0][v] + 1] = 1;
            lp.A[lp.m][f_idx[v][0] + 1] = -1;
        }
        if (v == 0) {
            lp.A[lp.m][f_idx[v][0] + 1] = 1;
            lp.A[lp.m][f_idx[0][v] + 1] = -1;
        }
    }
    lp.A[lp.m][k_idx + 1] = -a;
    lp.b[lp.m] = 0;

    // Node 2 (Water source)
    lp.m++;
    for (int e = 0; e < p; e++) {
        int u = ::u[e], v = ::w[e];
        if (u == 1) {
            lp.A[lp.m][w_idx[1][v] + 1] = 1;
            lp.A[lp.m][w_idx[v][1] + 1] = -1;
        }
        if (v == 1) {
            lp.A[lp.m][w_idx[v][1] + 1] = 1;
            lp.A[lp.m][w_idx[1][v] + 1] = -1;
        }
    }
    lp.A[lp.m][k_idx + 1] = -(1 - a);
    lp.b[lp.m] = 0;

    // Node 3 (FD)
    // Flubber
    lp.m++;
    for (int e = 0; e < p; e++) {
        int u = ::u[e], v = ::w[e];
        if (u == 2) {
            lp.A[lp.m][f_idx[u][v] + 1] = -1;
            lp.A[lp.m][f_idx[v][u] + 1] = 1;
        }
        if (v == 2) {
            lp.A[lp.m][f_idx[v][u] + 1] = -1;
            lp.A[lp.m][f_idx[u][v] + 1] = 1;
        }
    }
    lp.A[lp.m][k_idx + 1] = a;
    lp.b[lp.m] = 0;

    // Water
    lp.m++;
    for (int e = 0; e < p; e++) {
        int u = ::u[e], v = ::w[e];
        if (u == 2) {
            lp.A[lp.m][w_idx[u][v] + 1] = -1;
            lp.A[lp.m][w_idx[v][u] + 1] = 1;
        }
        if (v == 2) {
            lp.A[lp.m][w_idx[v][u] + 1] = -1;
            lp.A[lp.m][w_idx[u][v] + 1] = 1;
        }
    }
    lp.A[lp.m][k_idx + 1] = 1 - a;
    lp.b[lp.m] = 0;

    // Capacity constraints
    for (int e = 0; e < p; e++) {
        int u = ::u[e], v = ::w[e];

        // From u to v
        lp.m++;
        lp.A[lp.m][f_idx[u][v] + 1] = v;
        lp.A[lp.m][w_idx[u][v] + 1] = 1;
        lp.b[lp.m] = c[e];

        // From v to u
        lp.m++;
        lp.A[lp.m][f_idx[v][u] + 1] = v;
        lp.A[lp.m][w_idx[v][u] + 1] = 1;
        lp.b[lp.m] = c[e];
    }

    // Solve LP
    double res = lp.simplex();
    if (res == 1e20) {
        cout << "No solution" << endl;
        return 0;
    }

    // Retrieve flows
    vector<double> f_flow(p * 2), w_flow(p * 2);
    for (int e = 0; e < p; e++) {
        int u = ::u[e], v = ::w[e];
        int idx_fu_v = f_idx[u][v], idx_fv_u = f_idx[v][u];
        int idx_wu_v = w_idx[u][v], idx_wv_u = w_idx[v][u];
        double fu_v = 0, fv_u = 0, wu_v = 0, wv_u = 0;
        if (idx_fu_v >= 0) fu_v = lp.A[0][idx_fu_v + 1];
        if (idx_fv_u >= 0) fv_u = lp.A[0][idx_fv_u + 1];
        if (idx_wu_v >= 0) wu_v = lp.A[0][idx_wu_v + 1];
        if (idx_wv_u >= 0) wv_u = lp.A[0][idx_wv_u + 1];
        double f_total = fu_v - fv_u;
        double w_total = wu_v - wv_u;
        printf("%.10f %.10f\n", f_total, w_total);
    }

    // Compute F and W
    double k = -res;
    double F = k * a;
    double W = k * (1 - a);
    double value = pow(F, a) * pow(W, 1 - a);
    printf("%.10f\n", value);

    return 0;
}
