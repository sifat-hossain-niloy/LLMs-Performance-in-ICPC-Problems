#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cmath>

struct Interval {
    double start;
    double end;
};

bool compareIntervals(const Interval &a, const Interval &b) {
    return a.start < b.start;
}

int main() {
    int n;          // number of lanes
    double w;       // width of each lane
    double u, v;    // speed of ships and ferry
    double t1, t2;  // earliest and latest start times

    std::cin >> n >> w >> u >> v >> t1 >> t2;

    double T = w / v;  // Time taken to cross one lane

    std::vector<Interval> forbiddenIntervals;

    for (int i = 1; i <= n; ++i) {
        char dir;
        int mi;
        std::cin >> dir >> mi;

        std::vector<double> l(mi);
        std::vector<double> p(mi);

        for (int j = 0; j < mi; ++j) {
            std::cin >> l[j] >> p[j];
        }

        for (int j = 0; j < mi; ++j) {
            double t_ship_start, t_ship_end;

            if (dir == 'E') {
                t_ship_start = (-p[j]) / u;
                t_ship_end = (-p[j] + l[j]) / u;
            } else {
                t_ship_start = (p[j]) / u;
                t_ship_end = (p[j] + l[j]) / u;
            }

            // Ensure t_ship_start <= t_ship_end
            if (t_ship_start > t_ship_end)
                std::swap(t_ship_start, t_ship_end);

            double A = t_ship_start - i * T;
            double B = t_ship_end - (i - 1) * T;

            // Only add if A <= B
            if (A > B)
                continue;

            forbiddenIntervals.push_back({A, B});
        }
    }

    // Sort the forbidden intervals
    std::sort(forbiddenIntervals.begin(), forbiddenIntervals.end(), compareIntervals);

    // Merge overlapping intervals
    std::vector<Interval> mergedIntervals;
    for (const auto &interval : forbiddenIntervals) {
        if (mergedIntervals.empty() || mergedIntervals.back().end < interval.start) {
            mergedIntervals.push_back(interval);
        } else {
            mergedIntervals.back().end = std::max(mergedIntervals.back().end, interval.end);
        }
    }

    // Find the maximum allowed interval
    double maxD = 0.0;
    double lastEnd = t1;

    for (const auto &interval : mergedIntervals) {
        double start = std::max(lastEnd, t1);
        double end = std::min(interval.start, t2);

        if (end - start > maxD) {
            maxD = end - start;
        }

        lastEnd = std::max(lastEnd, interval.end);
    }

    // Check the interval after the last forbidden interval
    if (t2 - lastEnd > maxD && lastEnd < t2) {
        maxD = t2 - lastEnd;
    }

    // Output with precision up to 8 decimal places
    printf("%.8lf\n", maxD);

    return 0;
}
