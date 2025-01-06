#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>

using namespace std;

int main() {
    int w, h;
    cin >> w >> h;
    int u_perc, d_perc, l_perc, r_perc;
    cin >> u_perc >> d_perc >> l_perc >> r_perc;

    double u = u_perc / 100.0;
    double d = d_perc / 100.0;
    double l = l_perc / 100.0;
    double r = r_perc / 100.0;

    vector<string> grid(h);
    vector<pair<int, int>> targets; // Store target positions in order

    // Read the grid
    for (int i = 0; i < h; ++i) {
        cin >> grid[i];
        for (int j = 0; j < w; ++j) {
            if (grid[i][j] == 'T') {
                targets.emplace_back(i, j);
            }
        }
    }

    // Find starting positions (open spaces in the top row)
    vector<pair<int, int>> starting_positions;
    for (int j = 0; j < w; ++j) {
        if (grid[0][j] == '.') {
            starting_positions.emplace_back(0, j);
        }
    }

    int n0 = starting_positions.size();

    // Initialize probability matrices
    vector<vector<double>> P(h, vector<double>(w, 0.0)); // Current probabilities
    vector<vector<double>> P_new(h, vector<double>(w, 0.0)); // For updates
    vector<vector<double>> T_probs(h, vector<double>(w, 0.0)); // Accumulated target probabilities

    // Set initial probabilities
    for (auto& pos : starting_positions) {
        P[pos.first][pos.second] = 1.0 / n0;
    }

    // Movement directions
    const int dx[] = {-1, 1, 0, 0}; // Up, Down, Left, Right
    const int dy[] = {0, 0, -1, 1};
    const double move_probs[] = {u, d, l, r};

    const double EPS = 1e-8;
    double total_prob = 1.0; // Total probability mass in open cells

    while (total_prob > EPS) {
        // Reset P_new and total_prob
        for (int i = 0; i < h; ++i) {
            fill(P_new[i].begin(), P_new[i].end(), 0.0);
        }
        total_prob = 0.0;

        // Update probabilities
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                if (P[i][j] > 0.0) {
                    if (grid[i][j] == 'T') {
                        // Accumulate probability at target
                        T_probs[i][j] += P[i][j];
                        P[i][j] = 0.0;
                        continue;
                    }
                    // Distribute probability to neighbors
                    for (int dir = 0; dir < 4; ++dir) {
                        int ni = i + dx[dir];
                        int nj = j + dy[dir];
                        double prob = move_probs[dir] * P[i][j];

                        // Check if movement is blocked
                        if (ni < 0 || ni >= h || nj < 0 || nj >= w || grid[ni][nj] == 'X') {
                            // Stay in the same cell
                            P_new[i][j] += prob;
                        } else {
                            P_new[ni][nj] += prob;
                        }
                    }
                }
            }
        }

        // Update P and compute total_prob
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                P[i][j] = P_new[i][j];
                if (grid[i][j] != 'T' && P[i][j] > 0.0) {
                    total_prob += P[i][j];
                }
            }
        }
    }

    // Collect and output the probabilities for each target
    cout << fixed << setprecision(9);
    for (auto& t_pos : targets) {
        int i = t_pos.first;
        int j = t_pos.second;
        cout << T_probs[i][j] << endl;
    }

    return 0;
}
