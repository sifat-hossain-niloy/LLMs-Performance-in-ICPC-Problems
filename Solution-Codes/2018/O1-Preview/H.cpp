#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Wire {
    int x1, y1, x2, y2;
    int x_min, x_max, y_min, y_max;
};

int main() {
    int n;
    int w, h;
    cin >> n >> w >> h;
    vector<Wire> wires(n);
    int x_left = 0, x_right = w, y_left = 0, y_right = h;
    int global_x_min = 0, global_x_max = w;
    int global_y_min = 0, global_y_max = h;
    for (int i = 0; i < n; ++i) {
        cin >> wires[i].x1 >> wires[i].y1 >> wires[i].x2 >> wires[i].y2;
        wires[i].x_min = min(wires[i].x1, wires[i].x2);
        wires[i].x_max = max(wires[i].x1, wires[i].x2);
        wires[i].y_min = min(wires[i].y1, wires[i].y2);
        wires[i].y_max = max(wires[i].y1, wires[i].y2);
        global_x_left = max(x_left, wires[i].x_min);
        global_x_right = min(x_right, wires[i].x_max);
        global_y_left = max(y_left, wires[i].y_min);
        global_y_right = min(y_right, wires[i].y_max);
    }
    // Try vertical line
    int x_intersection_left = global_x_left;
    int x_intersection_right = global_x_right;
    if (x_intersection_left <= x_intersection_right) {
        // One vertical line suffices
        cout << 1 << endl;
        cout << fixed << setprecision(10) << x_intersection_left << " " << 0 << " " << x_intersection_left << " " << h << endl;
        return 0;
    }
    // Try horizontal line
    int y_intersection_left = global_y_left;
    int y_intersection_right = global_y_right;
    if (y_intersection_left <= y_intersection_right) {
        // One horizontal line suffices
        cout << 1 << endl;
        cout << fixed << setprecision(10) << 0 << " " << y_intersection_left << " " << w << " " << y_intersection_left << endl;
        return 0;
    }
    // Try partitioning wires into two groups for vertical cuts
    vector<Wire> group1, group2;
    sort(wires.begin(), wires.end(), [](const Wire &a, const Wire &b) {
        return a.x_min < b.x_min;
    });
    int current_x_right = wires[0].x_max;
    group1.push_back(wires[0]);
    for (size_t i = 1; i < wires.size(); ++i) {
        if (wires[i].x_min <= current_x_right) {
            current_x_right = min(current_x_right, wires[i].x_max);
            group1.push_back(wires[i]);
        } else {
            group2.push_back(wires[i]);
        }
    }
    if (!group2.empty()) {
        // Check if both groups can be intersected by vertical lines
        int x_left1 = 0, x_right1 = w;
        int x_left2 = 0, x_right2 = w;
        for (const auto &wire : group1) {
            x_left1 = max(x_left1, wire.x_min);
            x_right1 = min(x_right1, wire.x_max);
        }
        for (const auto &wire : group2) {
            x_left2 = max(x_left2, wire.x_min);
            x_right2 = min(x_right2, wire.x_max);
        }
        if (x_left1 <= x_right1 && x_left2 <= x_right2) {
            // Output two vertical lines
            cout << 2 << endl;
            cout << fixed << setprecision(10) << x_left1 << " " << 0 << " " << x_left1 << " " << h << endl;
            cout << fixed << setprecision(10) << x_left2 << " " << 0 << " " << x_left2 << " " << h << endl;
            return 0;
        }
    }
    // Try partitioning wires into two groups for horizontal cuts
    group1.clear();
    group2.clear();
    sort(wires.begin(), wires.end(), [](const Wire &a, const Wire &b) {
        return a.y_min < b.y_min;
    });
    int current_y_right = wires[0].y_max;
    group1.push_back(wires[0]);
    for (size_t i = 1; i < wires.size(); ++i) {
        if (wires[i].y_min <= current_y_right) {
            current_y_right = min(current_y_right, wires[i].y_max);
            group1.push_back(wires[i]);
        } else {
            group2.push_back(wires[i]);
        }
    }
    if (!group2.empty()) {
        // Check if both groups can be intersected by horizontal lines
        int y_left1 = 0, y_right1 = h;
        int y_left2 = 0, y_right2 = h;
        for (const auto &wire : group1) {
            y_left1 = max(y_left1, wire.y_min);
            y_right1 = min(y_right1, wire.y_max);
        }
        for (const auto &wire : group2) {
            y_left2 = max(y_left2, wire.y_min);
            y_right2 = min(y_right2, wire.y_max);
        }
        if (y_left1 <= y_right1 && y_left2 <= y_right2) {
            // Output two horizontal lines
            cout << 2 << endl;
            cout << fixed << setprecision(10) << 0 << " " << y_left1 << " " << w << " " << y_left1 << endl;
            cout << fixed << setprecision(10) << 0 << " " << y_left2 << " " << w << " " << y_left2 << endl;
            return 0;
        }
    }
    // If all else fails, output any two cuts
    cout << 2 << endl;
    cout << fixed << setprecision(10) << 0 << " " << h / 2.0 << " " << w << " " << h / 2.0 << endl;
    cout << fixed << setprecision(10) << w / 2.0 << " " << 0 << " " << w / 2.0 << " " << h << endl;
    return 0;
}
