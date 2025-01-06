#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <climits>

using namespace std;

typedef vector<vector<int>> Grid;

struct Pattern {
    Grid grid;
    int size;
};

int w, h;

Grid read_final_pattern() {
    cin >> w >> h;
    Grid final_pattern(h, vector<int>(w));
    for (int i = 0; i < h; ++i) {
        string line;
        cin >> line;
        for (int j = 0; j < w; ++j) {
            final_pattern[i][j] = (line[j] == '#') ? 1 : 0;
        }
    }
    return final_pattern;
}

int hamming_distance(const Grid& a, const Grid& b) {
    int h = a.size();
    int w = a[0].size();
    int dist = 0;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (a[i][j] != b[i][j]) dist++;
        }
    }
    return dist;
}

Grid simulate(const Grid& initial_pattern, int T, int target_h, int target_w, int offset_row, int offset_col) {
    int init_h = initial_pattern.size();
    int init_w = initial_pattern[0].size();

    int grid_h = init_h + 2 * T;
    int grid_w = init_w + 2 * T;

    Grid grid(grid_h, vector<int>(grid_w, 0));
    // Place initial pattern in the center
    for (int i = 0; i < init_h; ++i) {
        for (int j = 0; j < init_w; ++j) {
            grid[T + i][T + j] = initial_pattern[i][j];
        }
    }

    Grid next_grid = grid;
    for (int t = 0; t < T; ++t) {
        for (int i = 1; i < grid_h - 1; ++i) {
            for (int j = 1; j < grid_w - 1; ++j) {
                int sum = 0;
                for (int di = -1; di <= 1; ++di) {
                    for (int dj = -1; dj <= 1; ++dj) {
                        sum += grid[i + di][j + dj];
                    }
                }
                next_grid[i][j] = sum % 2;
            }
        }
        grid.swap(next_grid);
    }

    // Extract the part of the grid that corresponds to the final pattern
    Grid result(target_h, vector<int>(target_w, 0));
    for (int i = 0; i < target_h; ++i) {
        for (int j = 0; j < target_w; ++j) {
            int ii = T + offset_row + i;
            int jj = T + offset_col + j;
            if (ii >= 0 && ii < grid_h && jj >= 0 && jj < grid_w) {
                result[i][j] = grid[ii][jj];
            }
        }
    }

    return result;
}

void print_pattern(const Grid& pattern) {
    int h = pattern.size();
    int w = pattern[0].size();
    for (int i = 0; i < h; ++i) {
        string line;
        for (int j = 0; j < w; ++j) {
            line += (pattern[i][j] == 1) ? '#' : '.';
        }
        cout << line << endl;
    }
}

int main() {
    Grid final_pattern = read_final_pattern();

    int min_area = INT_MAX;
    Pattern best_pattern;

    int target_h = final_pattern.size();
    int target_w = final_pattern[0].size();

    int max_N = 3;

    for (int N = 1; N <= max_N; ++N) {
        int total_patterns = 1 << (N * N);
        for (int p = 1; p < total_patterns; ++p) { // Start from 1 to skip all-empty pattern
            Grid initial_pattern(N, vector<int>(N, 0));
            for (int k = 0; k < N * N; ++k) {
                if (p & (1 << k)) {
                    int i = k / N;
                    int j = k % N;
                    initial_pattern[i][j] = 1;
                }
            }

            // Estimate minimal T needed
            int T = ceil((max(target_w, target_h) - N) / 2.0) + 2;
            // Try different positions for the initial pattern
            for (int offset_row = -T; offset_row <= T; ++offset_row) {
                for (int offset_col = -T; offset_col <= T; ++offset_col) {
                    Grid simulated_pattern = simulate(initial_pattern, T, target_h, target_w, offset_row, offset_col);
                    int D = hamming_distance(simulated_pattern, final_pattern);
                    if (D <= T) {
                        int area = N * N;
                        if (area < min_area) {
                            min_area = area;
                            best_pattern.grid = initial_pattern;
                            best_pattern.size = N;
                        }
                    }
                }
            }
        }
        if (min_area != INT_MAX) {
            break; // Found minimal pattern, no need to check larger sizes
        }
    }

    if (min_area != INT_MAX) {
        print_pattern(best_pattern.grid);
    } else {
        // As per problem statement, this should not happen
        // Since we need to output a non-empty minimal initial pattern
    }

    return 0;
}
