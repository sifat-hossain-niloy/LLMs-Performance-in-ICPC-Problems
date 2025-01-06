#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>

using namespace std;

const int MAX_N = 1e6 + 5;

int main() {
    int n;
    cin >> n;
    vector<pair<char, int>> markers(n);
    for (int i = 0; i < n; ++i) {
        string s;
        cin >> s;
        char c = s[0]; // 's' or 'e'
        int idx = stoi(s.substr(1)); // gene type
        markers[i] = {c, idx};
    }

    unordered_map<int, vector<int>> gene_positions; // gene_type -> positions in markers
    unordered_map<int, vector<char>> gene_markers;  // gene_type -> sequence of 's' and 'e'
    unordered_map<int, vector<int>> gene_marker_positions; // gene_type -> positions in original sequence

    for (int i = 0; i < n; ++i) {
        int gene_type = markers[i].second;
        gene_positions[gene_type].push_back(i);
        gene_markers[gene_type].push_back(markers[i].first);
        gene_marker_positions[gene_type].push_back(i);
    }

    unordered_map<int, int> position_count; // position -> number of gene types

    for (auto& entry : gene_markers) {
        int gene_type = entry.first;
        vector<char>& seq = entry.second;
        vector<int>& positions = gene_marker_positions[gene_type];
        int k = seq.size();

        int total_balance = 0;
        int min_balance = INT_MAX;
        int min_pos = -1;
        int balance = 0;

        for (int i = 0; i < k; ++i) {
            if (seq[i] == 's') {
                balance++;
            } else {
                balance--;
            }
            if (balance < min_balance) {
                min_balance = balance;
                min_pos = i;
            }
        }
        total_balance = balance;
        if (total_balance != 0) continue; // Cannot form properly nested sequence

        // Rotation starting from min_pos + 1 (mod k)
        int rotation_idx = (min_pos + 1) % k;
        int cutting_position = (positions[rotation_idx] + 1); // Convert to 1-based index
        if (cutting_position > n) cutting_position -= n; // Wrap around if necessary
        position_count[cutting_position]++;
    }

    // Find the position with the maximum count
    int max_count = 0;
    int optimal_position = n + 1; // Initialize with a value larger than any possible position

    for (auto& entry : position_count) {
        int pos = entry.first;
        int count = entry.second;
        if (count > max_count || (count == max_count && pos < optimal_position)) {
            max_count = count;
            optimal_position = pos;
        }
    }

    if (max_count == 0) optimal_position = 1; // If no gene types can form properly nested sequences, default to position 1

    cout << optimal_position << " " << max_count << endl;

    return 0;
}
