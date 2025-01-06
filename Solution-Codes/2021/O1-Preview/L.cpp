#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <set>
#include <algorithm>
#include <iomanip>

using namespace std;

// Directions: right, up, left, down
const int dx_dirs[] = {1, 0, -1, 0};
const int dy_dirs[] = {0, 1, 0, -1};

int main() {
    int dx, dy;
    cin >> dx >> dy;
    vector<string> input_lines(dy);
    for (int i = 0; i < dy; ++i) {
        cin >> input_lines[i];
    }
    // Map the input grid to grid[x][y]
    vector<vector<char>> grid(dx + 2, vector<char>(dy + 2, '.')); // +2 for boundaries
    for (int y = 1; y <= dy; ++y) {
        string &line = input_lines[dy - y]; // dy - y
        for (int x = 1; x <= dx; ++x) {
            grid[x][y] = line[x - 1];
        }
    }
    // Precompute spiral steps
    vector<pair<int, int>> spiral_steps;
    int steps = 1;
    int dir = 0; // start with right
    int x = 0, y = 0;
    spiral_steps.emplace_back(0, 0); // starting position
    while (spiral_steps.size() <= 200) { // max steps we may need
        for (int i = 0; i < 2; ++i) {
            int dx_dir = dx_dirs[dir];
            int dy_dir = dy_dirs[dir];
            for (int s = 0; s < steps; ++s) {
                x += dx_dir;
                y += dy_dir;
                spiral_steps.emplace_back(x, y);
            }
            dir = (dir + 1) % 4;
        }
        steps++;
    }
    int total_positions = dx * dy;
    vector<int> steps_needed(total_positions, 0);
    vector<bool> identified(total_positions, false);
    vector<string> observed_sequences(total_positions);
    int positions_remaining = total_positions;
    double total_steps = 0.0;
    int max_steps = 0;
    // Map from observed sequences to list of indices of starting positions
    unordered_map<string, vector<int>> sequence_map;
    int position_index = 0;
    for (int x0 = 1; x0 <= dx; ++x0) {
        for (int y0 = 1; y0 <= dy; ++y0) {
            // Initialize observed sequences
            observed_sequences[position_index] = "";
            position_index++;
        }
    }
    int step = 0;
    while (positions_remaining > 0) {
        sequence_map.clear();
        position_index = 0;
        for (int x0 = 1; x0 <= dx; ++x0) {
            for (int y0 = 1; y0 <= dy; ++y0) {
                if (identified[position_index]) {
                    position_index++;
                    continue;
                }
                // Extend observed sequence
                int rel_x = spiral_steps[step].first;
                int rel_y = spiral_steps[step].second;
                int x = x0 + rel_x;
                int y = y0 + rel_y;
                char observation = '.';
                if (x >= 1 && x <= dx && y >= 1 && y <= dy) {
                    observation = grid[x][y];
                }
                observed_sequences[position_index] += observation;
                sequence_map[observed_sequences[position_index]].push_back(position_index);
                position_index++;
            }
        }
        // Check for unique sequences
        for (auto &entry : sequence_map) {
            if (entry.second.size() == 1) {
                int idx = entry.second[0];
                if (!identified[idx]) {
                    identified[idx] = true;
                    steps_needed[idx] = step + 1;
                    total_steps += steps_needed[idx];
                    max_steps = max(max_steps, steps_needed[idx]);
                    positions_remaining--;
                }
            }
        }
        step++;
        if (step >= (int)spiral_steps.size()) {
            // Should not happen, but just in case
            break;
        }
    }
    // Output the results
    double expected_steps = total_steps / total_positions;
    cout << fixed << setprecision(3) << expected_steps << endl;
    cout << max_steps << endl;
    // Collect starting positions that require max_steps
    vector<pair<int, int>> max_positions;
    position_index = 0;
    for (int x0 = 1; x0 <= dx; ++x0) {
        for (int y0 = 1; y0 <= dy; ++y0) {
            if (steps_needed[position_index] == max_steps) {
                max_positions.emplace_back(x0, y0);
            }
            position_index++;
        }
    }
    // Sort positions by increasing y, then x
    sort(max_positions.begin(), max_positions.end(), [](const pair<int, int> &a, const pair<int, int> &b) {
        if (a.second != b.second)
            return a.second < b.second;
        return a.first < b.first;
    });
    // Output positions
    for (size_t i = 0; i < max_positions.size(); ++i) {
        cout << "(" << max_positions[i].first << "," << max_positions[i].second << ")";
        if (i + 1 < max_positions.size())
            cout << " ";
    }
    cout << endl;
    return 0;
}
