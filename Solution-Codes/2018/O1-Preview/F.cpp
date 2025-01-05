#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

using namespace std;

const int MAXN = 2500;

int main() {
    int n;
    cin >> n;
    vector<string> words(n);
    int max_word_len = 0;
    int total_words_len = 0;

    // Read words and compute lengths
    for (int i = 0; i < n; ++i) {
        cin >> words[i];
        int len = words[i].length();
        max_word_len = max(max_word_len, len);
    }

    // Compute cumulative lengths
    vector<int> cumlen(n);
    vector<int> word_lens(n);
    for (int i = 0; i < n; ++i) {
        word_lens[i] = words[i].length();
    }

    cumlen[0] = word_lens[0];
    for (int i = 1; i < n; ++i) {
        cumlen[i] = cumlen[i - 1] + 1 + word_lens[i];
    }

    // Collect possible line widths
    set<int> line_widths;
    line_widths.insert(max_word_len);
    for (int i = 0; i < n; ++i) {
        line_widths.insert(cumlen[i]);
    }

    int max_river_length = 0;
    int min_line_width_with_max_river = 0;

    for (int W : line_widths) {
        if (W < max_word_len) continue;

        // Simulate wrapping
        vector<vector<int>> spaces_in_lines;
        vector<int> positions_set;

        int s = 0;
        while (s < n) {
            int line_length = word_lens[s];
            int e = s + 1;
            while (e < n && line_length + 1 + word_lens[e] <= W) {
                line_length += 1 + word_lens[e];
                ++e;
            }
            // Record positions of spaces in this line
            vector<int> spaces;
            int pos = 0;
            for (int i = s; i < e - 1; ++i) {
                pos += word_lens[i];
                positions_set.push_back(pos);
                spaces.push_back(pos);
                pos += 1; // one space
            }
            spaces_in_lines.push_back(spaces);
            s = e;
        }

        // Map positions to indices
        sort(positions_set.begin(), positions_set.end());
        positions_set.erase(unique(positions_set.begin(), positions_set.end()), positions_set.end());
        map<int, int> pos_to_index;
        for (int i = 0; i < positions_set.size(); ++i) {
            pos_to_index[positions_set[i]] = i;
        }
        int M = positions_set.size();

        // Initialize dp
        vector<map<int, int>> dp(spaces_in_lines.size());
        int current_max_river = 0;

        for (int i = 0; i < spaces_in_lines.size(); ++i) {
            for (int p : spaces_in_lines[i]) {
                int pos_index = pos_to_index[p];
                int max_length = 1;

                if (i > 0) {
                    for (int delta = -1; delta <= 1; ++delta) {
                        int neighbor_index = pos_index + delta;
                        if (neighbor_index >= 0 && neighbor_index < M) {
                            int neighbor_pos = positions_set[neighbor_index];
                            if (dp[i - 1].count(neighbor_pos)) {
                                max_length = max(max_length, dp[i - 1][neighbor_pos] + 1);
                            }
                        }
                    }
                }
                dp[i][p] = max_length;
                current_max_river = max(current_max_river, max_length);
            }
        }

        if (current_max_river > max_river_length) {
            max_river_length = current_max_river;
            min_line_width_with_max_river = W;
        } else if (current_max_river == max_river_length && W < min_line_width_with_max_river) {
            min_line_width_with_max_river = W;
        }
    }

    cout << min_line_width_with_max_river << " " << max_river_length << endl;

    return 0;
}
