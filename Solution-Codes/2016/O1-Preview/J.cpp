#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct DataPoint {
    int a, b, c, index;
};

int main() {
    int n;
    cin >> n;
    vector<DataPoint> data(n);
    for (int i = 0; i < n; ++i) {
        cin >> data[i].a >> data[i].b >> data[i].c;
        data[i].index = i;  // Original position
    }

    // Sort data points based on (a, b)
    sort(data.begin(), data.end(), [](const DataPoint &p1, const DataPoint &p2) {
        if (p1.a != p2.a) return p1.a < p2.a;
        return p1.b < p2.b;
    });

    // Positions of ci=1 points in the sorted list
    vector<int> positions;
    // Map to store first and last positions of ci=1 in each group
    vector<int> first_c1_pos(n, -1), last_c1_pos(n, -1);

    for (int i = 0; i < n; ++i) {
        if (data[i].c == 1) {
            positions.push_back(i);
            int group_start = i;
            // Find the group start (first occurrence of this (a, b))
            while (group_start > 0 && data[group_start - 1].a == data[i].a && data[group_start - 1].b == data[i].b) {
                --group_start;
            }
            int group_end = i;
            // Find the group end (last occurrence of this (a, b))
            while (group_end + 1 < n && data[group_end + 1].a == data[i].a && data[group_end + 1].b == data[i].b) {
                ++group_end;
            }
            // Update first and last positions of ci=1 in this group
            first_c1_pos[group_start] = group_start;
            last_c1_pos[group_start] = group_end;
        }
    }

    if (positions.empty()) {
        cout << 0 << endl;
        return 0;
    }

    int min_pos = positions[0], max_pos = positions[0];
    for (int pos : positions) {
        min_pos = min(min_pos, pos);
        max_pos = max(max_pos, pos);
    }

    // Adjust for worst-case ordering within tied groups
    for (int i = 0; i < n;) {
        int j = i;
        // Find the group with same (a, b)
        while (j + 1 < n && data[j + 1].a == data[i].a && data[j + 1].b == data[i].b) {
            ++j;
        }
        if (first_c1_pos[i] != -1) {
            // ci=1 points in this group, worst-case positions
            min_pos = min(min_pos, i);
            max_pos = max(max_pos, j);
        }
        i = j + 1;
    }

    int cluster_size = max_pos - min_pos + 1;
    cout << cluster_size << endl;
    return 0;
}
