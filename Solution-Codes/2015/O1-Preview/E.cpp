#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

int main() {
    int n;
    std::cin >> n;
    std::string current_species;
    std::cin >> current_species;

    std::vector<std::string> sequences(n);
    std::unordered_map<std::string, int> seq_index;
    std::vector<int> length_list;
    int max_length = 0;

    for (int i = 0; i < n; ++i) {
        std::cin >> sequences[i];
        seq_index[sequences[i]] = i;
        length_list.push_back(sequences[i].size());
        max_length = std::max(max_length, (int)sequences[i].size());
    }

    // Initialize path assignments
    std::vector<int> path(n, 0); // 0: unassigned, 1: path 1, 2: path 2

    // Group sequences by length
    std::vector<std::vector<int>> sequences_by_length(max_length + 1);
    for (int i = 0; i < n; ++i) {
        int len = sequences[i].size();
        sequences_by_length[len].push_back(i);
    }

    int path1_count = 0, path2_count = 0;
    int next_path_to_assign = 1; // Start with path 1

    // Process sequences in order of increasing length
    for (int len = 1; len <= max_length; ++len) {
        for (int idx : sequences_by_length[len]) {
            const std::string& seq = sequences[idx];
            std::unordered_set<int> possible_paths;
            for (size_t i = 0; i < seq.size(); ++i) {
                std::string pred = seq.substr(0, i) + seq.substr(i + 1);
                auto it = seq_index.find(pred);
                if (it != seq_index.end()) {
                    int pred_idx = it->second;
                    if (path[pred_idx] != 0) {
                        possible_paths.insert(path[pred_idx]);
                    }
                }
            }
            if (possible_paths.size() > 1) {
                std::cout << "impossible" << std::endl;
                return 0;
            } else if (possible_paths.size() == 1) {
                path[idx] = *possible_paths.begin();
                if (path[idx] == 1) path1_count++;
                else path2_count++;
            } else {
                // Assign to the path with fewer sequences
                if (path1_count <= path2_count) {
                    path[idx] = 1;
                    path1_count++;
                } else {
                    path[idx] = 2;
                    path2_count++;
                }
            }
        }
    }

    if (path1_count == 0 || path2_count == 0) {
        std::cout << "impossible" << std::endl;
        return 0;
    }

    std::cout << path1_count << " " << path2_count << std::endl;

    // Output sequences for path 1
    for (int len = 1; len <= max_length; ++len) {
        for (int idx : sequences_by_length[len]) {
            if (path[idx] == 1) {
                std::cout << sequences[idx] << std::endl;
            }
        }
    }

    // Output sequences for path 2
    for (int len = 1; len <= max_length; ++len) {
        for (int idx : sequences_by_length[len]) {
            if (path[idx] == 2) {
                std::cout << sequences[idx] << std::endl;
            }
        }
    }

    return 0;
}
