#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Point {
    int idx;
    long long r, c;
};

bool compare_increasing(const Point &a, const Point &b) {
    if (a.r != b.r) return a.r < b.r;
    return a.c < b.c;
}

bool compare_decreasing(const Point &a, const Point &b) {
    if (a.r != b.r) return a.r > b.r;
    return a.c > b.c;
}

bool try_matching(int n, vector<Point> top_left, vector<Point> bottom_right, vector<int> &match, bool reverse_bottom) {
    if (reverse_bottom)
        sort(bottom_right.begin(), bottom_right.end(), compare_decreasing);
    else
        sort(bottom_right.begin(), bottom_right.end(), compare_increasing);
    
    match.resize(n);
    for (int i = 0; i < n; ++i) {
        int idx_top = top_left[i].idx;
        int idx_bottom = bottom_right[i].idx;
        long long r1 = top_left[i].r;
        long long c1 = top_left[i].c;
        long long r2 = bottom_right[i].r;
        long long c2 = bottom_right[i].c;

        // Check validity of the rectangle
        if (r1 >= r2 || c1 >= c2) return false;

        match[idx_top] = idx_bottom + 1;  // +1 because indices are from 1 to n
    }

    // Check for nesting or disjointness
    for (int i = 0; i < n - 1; ++i) {
        long long r1_curr = top_left[i].r;
        long long c1_curr = top_left[i].c;
        long long r2_curr = bottom_right[i].r;
        long long c2_curr = bottom_right[i].c;

        long long r1_next = top_left[i + 1].r;
        long long c1_next = top_left[i + 1].c;
        long long r2_next = bottom_right[i + 1].r;
        long long c2_next = bottom_right[i + 1].c;

        // Check for overlapping borders
        if (r1_curr >= r1_next || c1_curr >= c1_next) return false;

        if (r2_curr >= r2_next && c2_curr >= c2_next) {
            // Proper nesting
            continue;
        } else if (r2_curr <= r1_next || r1_curr >= r2_next ||
                   c2_curr <= c1_next || c1_curr >= c2_next) {
            // Disjoint rectangles
            continue;
        } else {
            // Rectangles overlap improperly
            return false;
        }
    }

    return true;
}

int main() {
    int n;
    cin >> n;
    vector<Point> top_left(n), bottom_right(n);
    for (int i = 0; i < n; ++i) {
        cin >> top_left[i].r >> top_left[i].c;
        top_left[i].idx = i;
    }
    for (int i = 0; i < n; ++i) {
        cin >> bottom_right[i].r >> bottom_right[i].c;
        bottom_right[i].idx = i;
    }

    vector<int> match;
    sort(top_left.begin(), top_left.end(), compare_increasing);
    bool success = try_matching(n, top_left, bottom_right, match, false);

    if (!success) {
        success = try_matching(n, top_left, bottom_right, match, true);
    }

    if (success) {
        for (int i = 0; i < n; ++i) {
            cout << match[i] << '\n';
        }
    } else {
        cout << "syntax error\n";
    }

    return 0;
}
