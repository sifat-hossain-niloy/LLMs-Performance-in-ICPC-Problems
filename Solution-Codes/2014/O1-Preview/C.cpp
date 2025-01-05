#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <limits>

using namespace std;

const double EPS = 1e-9;

struct Point {
    double x, y;
};

double polygonArea(const vector<Point>& pts) {
    double area = 0.0;
    int n = pts.size();
    for (int i = 0; i < n; ++i) {
        area += (pts[i].x * pts[(i + 1) % n].y - pts[(i + 1) % n].x * pts[i].y);
    }
    return fabs(area) / 2.0;
}

Point polygonCentroid(const vector<Point>& pts) {
    double cx = 0.0, cy = 0.0;
    double area = 0.0;
    int n = pts.size();
    for (int i = 0; i < n; ++i) {
        double cross = pts[i].x * pts[(i + 1) % n].y - pts[(i + 1) % n].x * pts[i].y;
        cx += (pts[i].x + pts[(i + 1) % n].x) * cross;
        cy += (pts[i].y + pts[(i + 1) % n].y) * cross;
        area += cross;
    }
    area *= 0.5;
    cx /= (6.0 * area);
    cy /= (6.0 * area);
    return {cx, cy};
}

int main() {
    int n;
    cin >> n;
    vector<Point> pts(n);
    for (int i = 0; i < n; ++i) {
        cin >> pts[i].x >> pts[i].y;
    }

    double W_crane = polygonArea(pts);
    Point centroid = polygonCentroid(pts);

    // Find base points A and B (points on x-axis)
    double x_A = numeric_limits<double>::infinity();
    double x_B = -numeric_limits<double>::infinity();
    for (const auto& p : pts) {
        if (fabs(p.y) < EPS) { // y == 0
            x_A = min(x_A, p.x);
            x_B = max(x_B, p.x);
        }
    }

    double x1 = pts[0].x;

    double delta_x1A = x1 - x_A;
    double delta_x1B = x1 - x_B;
    double delta_xcA = centroid.x - x_A;
    double delta_xcB = centroid.x - x_B;

    double W_min = 0.0;
    double W_max = numeric_limits<double>::infinity();

    // Inequality 1 (prevent toppling over about A)
    if (fabs(delta_x1A) > EPS) {
        double W_candidate = -W_crane * delta_xcA / delta_x1A;
        if (delta_x1A > 0) {
            W_max = min(W_max, W_candidate);
        } else {
            W_min = max(W_min, W_candidate);
        }
    } else { // delta_x1A == 0
        if (W_crane * delta_xcA > EPS) {
            cout << "unstable" << endl;
            return 0;
        }
    }

    // Inequality 2 (prevent toppling over about B)
    if (fabs(delta_x1B) > EPS) {
        double W_candidate = -W_crane * delta_xcB / delta_x1B;
        if (delta_x1B < 0) {
            W_max = min(W_max, W_candidate);
        } else {
            W_min = max(W_min, W_candidate);
        }
    } else { // delta_x1B == 0
        if (W_crane * delta_xcB < -EPS) {
            cout << "unstable" << endl;
            return 0;
        }
    }

    if (W_min - W_max > EPS) {
        cout << "unstable" << endl;
    } else {
        W_min = max(W_min, 0.0); // Weight cannot be negative
        cout << static_cast<long long>(floor(W_min)) << " .. ";
        if (W_max == numeric_limits<double>::infinity()) {
            cout << "inf" << endl;
        } else {
            cout << static_cast<long long>(ceil(W_max)) << endl;
        }
    }

    return 0;
}
