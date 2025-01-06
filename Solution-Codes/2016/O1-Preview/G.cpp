#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

struct Event {
    long double theta;
    int type; // 1 for start, -1 for end
    long long width;

    Event(long double t, int ty, long long w) : theta(t), type(ty), width(w) {}
};

bool eventCompare(const Event &a, const Event &b) {
    if (fabsl(a.theta - b.theta) > 1e-12) {
        return a.theta < b.theta;
    } else {
        return a.type > b.type; // Starts (1) before ends (-1) when angles are equal
    }
}

int main() {
    int n;
    cin >> n;
    vector<Event> events;
    const long double PI = acosl(-1.0L);
    for (int i = 0; i < n; ++i) {
        long long x0, x1, y;
        cin >> x0 >> x1 >> y;
        long long width = abs(x1 - x0);

        // Compute angles
        long double theta0 = atan2l((long double)y, (long double)x0);
        long double theta1 = atan2l((long double)y, (long double)x1);

        // Adjust angles to be within (-PI/2, PI/2)
        if (theta0 > PI/2.0L) theta0 -= PI;
        if (theta0 <= -PI/2.0L) theta0 += PI;
        if (theta1 > PI/2.0L) theta1 -= PI;
        if (theta1 <= -PI/2.0L) theta1 += PI;

        // Swap if necessary to ensure theta0 <= theta1
        if (theta0 > theta1) {
            swap(theta0, theta1);
        }

        // Create events
        events.emplace_back(theta0, 1, width);
        events.emplace_back(theta1, -1, width);
    }

    // Sort events
    sort(events.begin(), events.end(), eventCompare);

    // Sweep through events
    long long current_sum = 0, max_sum = 0;
    for (const auto &e : events) {
        if (e.type == 1) {
            current_sum += e.width;
            if (current_sum > max_sum) {
                max_sum = current_sum;
            }
        } else {
            current_sum -= e.width;
        }
    }

    cout << max_sum << endl;
    return 0;
}
