#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <bitset>

using namespace std;

const int INF = 1e9;

int main() {
    int n;
    cin >> n;
    vector<vector<bitset<25>>> options(n); // options for each position
    string line;
    getline(cin, line); // consume the newline
    for (int i = 0; i < n; ++i) {
        // Read options for position i
        getline(cin, line);
        int m = 0;
        size_t pos = 0;
        // Parse m
        while (pos < line.size() && line[pos] != ' ') {
            m = m * 10 + (line[pos] - '0');
            ++pos;
        }
        ++pos; // skip the space
        vector<bitset<25>> ops;
        for (int j = 0; j < m; ++j) {
            // Read each option
            while (pos < line.size() && line[pos] == ' ') ++pos;
            string s;
            while (pos < line.size() && line[pos] != ' ') {
                s += line[pos];
                ++pos;
            }
            bitset<25> bs;
            for (char c : s) {
                int idx = c - 'a';
                bs.set(idx);
            }
            ops.push_back(bs);
        }
        options[i] = ops;
    }

    int n_positions = n;
    vector<string> output(n_positions, "");

    for (int goal = 0; goal < n_positions; ++goal) {
        vector<int> f(n_positions, INF);
        f[goal] = 0;
        queue<int> q;
        q.push(goal);
        vector<bool> in_queue(n_positions, false);
        in_queue[goal] = true;

        while (!q.empty()) {
            int p = q.front();
            q.pop();
            in_queue[p] = false;
            for (int prev = 0; prev < n_positions; ++prev) {
                // Try to update f[prev] based on options at position prev
                if (prev == p) continue;
                int min_candidate = INF;
                for (const auto& S_i : options[prev]) {
                    if (S_i.none()) continue;
                    // Compute max f(r) for r in S_i
                    int max_f = -1;
                    for (int r = 0; r < n_positions; ++r) {
                        if (S_i.test(r)) {
                            if (f[r] == INF) {
                                max_f = INF;
                                break;
                            }
                            max_f = max(max_f, f[r]);
                        }
                    }
                    int candidate = 1 + max_f;
                    min_candidate = min(min_candidate, candidate);
                }
                if (min_candidate < f[prev]) {
                    f[prev] = min_candidate;
                    if (!in_queue[prev]) {
                        q.push(prev);
                        in_queue[prev] = true;
                    }
                }
            }
        }

        // Prepare output for this goal position
        for (int p = 0; p < n_positions; ++p) {
            if (!output[p].empty()) output[p] += " ";
            if (f[p] == INF) {
                output[p] += "-1";
            } else {
                output[p] += to_string(f[p]);
            }
        }
    }

    // Output the results
    for (int p = 0; p < n_positions; ++p) {
        cout << output[p] << endl;
    }

    return 0;
}
