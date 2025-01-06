#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>

using namespace std;

// Segment positions within a digit's 7x5 grid (row, col offsets)
const vector<pair<int, int>> digit_segments[7] = {
    {{0, 1}, {0, 2}, {0, 3}},         // Segment 0 (top horizontal)
    {{1, 4}, {2, 4}},                 // Segment 1 (top-right vertical)
    {{4, 4}, {5, 4}},                 // Segment 2 (bottom-right vertical)
    {{6, 1}, {6, 2}, {6, 3}},         // Segment 3 (bottom horizontal)
    {{4, 0}, {5, 0}},                 // Segment 4 (bottom-left vertical)
    {{1, 0}, {2, 0}},                 // Segment 5 (top-left vertical)
    {{3, 1}, {3, 2}, {3, 3}}          // Segment 6 (middle horizontal)
};

// Standard 7-segment representations for digits 0-9
const int digit_repr[10] = {
    0b1111110, // 0
    0b0110000, // 1
    0b1101101, // 2
    0b1111001, // 3
    0b0110011, // 4
    0b1011011, // 5
    0b1011111, // 6
    0b1110000, // 7
    0b1111111, // 8
    0b1111011  // 9
};

// Positions of the digits in the 7x21 grid (start columns)
const vector<int> digit_start_cols = {0, 6, 14, 20};

// Positions of the colon segments
const vector<pair<int, int>> colon_segments = {
    {2, 11}, {4, 11}
};

// Total number of segments: 7 segments per digit x 4 digits + 2 colon segments = 30
const int TOTAL_SEGMENTS = 30;

// Mapping from segment index to grid positions
vector<pair<int, int>> segment_positions[TOTAL_SEGMENTS];

void initialize_segment_positions() {
    // Initialize segment positions for each digit
    for (int d = 0; d < 4; ++d) { // 4 digits
        int start_col = digit_start_cols[d];
        for (int s = 0; s < 7; ++s) { // 7 segments per digit
            int segment_index = d * 7 + s;
            for (auto pos : digit_segments[s]) {
                int row = pos.first;
                int col = start_col + pos.second;
                segment_positions[segment_index].push_back({row, col});
            }
        }
    }
    // Colon segments
    segment_positions[28].push_back(colon_segments[0]); // Colon segment 1
    segment_positions[29].push_back(colon_segments[1]); // Colon segment 2
}

int get_digit(char ch) {
    if (ch >= '0' && ch <= '9') return ch - '0';
    return -1;
}

int main() {
    initialize_segment_positions();

    int n;
    cin >> n;
    cin.ignore();

    // Read the displays
    vector<vector<string>> displays(n, vector<string>(7));
    for (int i = 0; i < n; ++i) {
        for (int row = 0; row < 7; ++row) {
            getline(cin, displays[i][row]);
        }
        if (i != n - 1) getline(cin, string()); // Read the blank line
    }

    // Store the observed values for each display and segment
    vector<vector<int>> observed(n, vector<int>(TOTAL_SEGMENTS, -1)); // -1: uninitialized, 0: off, 1: on
    for (int idx = 0; idx < n; ++idx) {
        for (int s = 0; s < TOTAL_SEGMENTS; ++s) {
            int val = -1;
            for (auto pos : segment_positions[s]) {
                int row = pos.first;
                int col = pos.second;
                char ch = displays[idx][row][col];
                if (ch == 'X') {
                    val = 1;
                } else if (ch == '.') {
                    if (val != 1) val = 0;
                }
            }
            observed[idx][s] = val;
        }
    }

    // Possible statuses for each segment across all valid t's
    vector<set<char>> segment_statuses(TOTAL_SEGMENTS);

    // Flag to check if any valid t exists
    bool possible = false;

    // Iterate over all possible starting times
    for (int t = 0; t < 1440; ++t) {
        bool valid = true;
        vector<set<char>> possible_statuses(TOTAL_SEGMENTS, {'0', '1', 'W'});
        for (int idx = 0; idx < n; ++idx) {
            int minutes = (t + idx) % 1440;
            int hh = minutes / 60;
            int mm = minutes % 60;
            string time_str = (hh < 10 ? "0" : "") + to_string(hh) + (mm < 10 ? "0" : "") + to_string(mm);

            // Expected segment values for the time
            vector<int> expected_segments(TOTAL_SEGMENTS, 0); // 0: off, 1: on
            for (int d = 0; d < 4; ++d) {
                int digit = get_digit(time_str[d]);
                int repr = digit_repr[digit];
                for (int s = 0; s < 7; ++s) {
                    int segment_index = d * 7 + s;
                    expected_segments[segment_index] = (repr >> (6 - s)) & 1;
                }
            }
            // Colon segments are always on
            expected_segments[28] = 1;
            expected_segments[29] = 1;

            // Update possible statuses for each segment
            for (int s = 0; s < TOTAL_SEGMENTS; ++s) {
                int o = observed[idx][s];       // Observed value: 0 or 1
                int e = expected_segments[s];   // Expected value: 0 or 1
                set<char> new_statuses;
                if (o == 1) {
                    if (e == 1) {
                        new_statuses = {'1', 'W'};
                    } else {
                        new_statuses = {'1'};
                    }
                } else if (o == 0) {
                    if (e == 1) {
                        new_statuses = {'0'};
                    } else {
                        new_statuses = {'0', 'W'};
                    }
                } else {
                    // Should not happen
                    valid = false;
                    break;
                }
                // Intersection with previous possible statuses
                set<char> intersect;
                for (char c : possible_statuses[s]) {
                    if (new_statuses.count(c)) {
                        intersect.insert(c);
                    }
                }
                if (intersect.empty()) {
                    valid = false;
                    break;
                }
                possible_statuses[s] = intersect;
            }
            if (!valid) break;
        }
        if (valid) {
            possible = true;
            // Update overall segment statuses
            for (int s = 0; s < TOTAL_SEGMENTS; ++s) {
                if (segment_statuses[s].empty()) {
                    segment_statuses[s] = possible_statuses[s];
                } else {
                    set<char> union_set;
                    for (char c : possible_statuses[s]) {
                        segment_statuses[s].insert(c);
                    }
                }
            }
        }
    }

    if (!possible) {
        cout << "impossible" << endl;
    } else {
        // Output the result
        vector<string> output_display(7, string(21, '.'));
        for (int s = 0; s < TOTAL_SEGMENTS; ++s) {
            char c;
            if (segment_statuses[s].size() == 1) {
                c = *segment_statuses[s].begin();
            } else {
                c = '?';
            }
            for (auto pos : segment_positions[s]) {
                output_display[pos.first][pos.second] = c;
            }
        }
        for (int row = 0; row < 7; ++row) {
            cout << output_display[row] << endl;
        }
    }

    return 0;
}
