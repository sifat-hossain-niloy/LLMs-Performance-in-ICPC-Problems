#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <map>

using namespace std;

const int MAX_DEPTH = 100;

int n;
vector<vector<int>> maze;
vector<vector<int>> exit_orders;
unordered_map<int, unordered_map<int, string>> memo;

string compute_label(int node, int parent, int depth) {
    if (depth == 0) return "";

    int key = node * 1000 + parent; // Unique key for memoization
    if (memo[depth].count(key)) return memo[depth][key];

    vector<string> neighbor_labels;
    for (size_t i = 0; i < maze[node].size(); ++i) {
        int neighbor = maze[node][i];
        if (neighbor != parent) {
            string label = compute_label(neighbor, node, depth - 1);
            neighbor_labels.push_back(label);
        } else {
            neighbor_labels.push_back("PARENT");
        }
    }

    // Create a string that uniquely represents this node's label
    string label = "(" + to_string(maze[node].size()) + ",";
    for (const auto& l : neighbor_labels) {
        label += l + ",";
    }
    label += ")";
    memo[depth][key] = label;
    return label;
}

int main() {
    cin >> n;
    maze.resize(n);
    exit_orders.resize(n);

    for (int i = 0; i < n; ++i) {
        int k;
        cin >> k;
        maze[i].resize(k);
        for (int j = 0; j < k; ++j) {
            int room;
            cin >> room;
            maze[i][j] = room - 1; // Convert to 0-based index
        }
    }

    unordered_map<string, vector<int>> label_groups;
    for (int i = 0; i < n; ++i) {
        memo.clear();
        string label = compute_label(i, -1, MAX_DEPTH);
        label_groups[label].push_back(i + 1); // Convert back to 1-based index
    }

    vector<vector<int>> result;
    for (const auto& group : label_groups) {
        if (group.second.size() > 1) {
            vector<int> rooms = group.second;
            sort(rooms.begin(), rooms.end());
            result.push_back(rooms);
        }
    }

    sort(result.begin(), result.end(), [](const vector<int>& a, const vector<int>& b) {
        return a[0] < b[0];
    });

    if (result.empty()) {
        cout << "none" << endl;
    } else {
        for (const auto& group : result) {
            for (size_t i = 0; i < group.size(); ++i) {
                cout << group[i];
                if (i + 1 < group.size()) cout << " ";
            }
            cout << endl;
        }
    }

    return 0;
}
