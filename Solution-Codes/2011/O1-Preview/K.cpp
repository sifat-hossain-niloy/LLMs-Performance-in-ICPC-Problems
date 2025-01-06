#include <iostream>
#include <vector>
#include <cmath>
#include <set>
#include <iomanip>
#include <algorithm>

using namespace std;

typedef pair<int, int> pii;

const double EPS = 1e-12;

struct Normal {
    double nx, ny;
    Normal(double x, double y) : nx(x), ny(y) {}

    bool operator<(const Normal& other) const {
        if (fabs(nx - other.nx) > EPS) return nx < other.nx;
        return ny < other.ny;
    }
};

int gcd(int a, int b) {
    return b == 0 ? abs(a) : gcd(b, a % b);
}

int main() {
    int case_num = 1;
    while (true) {
        int n;
        cin >> n;
        if (n == 0) break;
        vector<double> x(n), y(n);
        for (int i = 0; i < n; ++i) {
            int xi, yi;
            cin >> xi >> yi;
            x[i] = xi;
            y[i] = yi;
        }

        set<pii> directions;
        vector<Normal> normals;

        // For all pairs of vertices
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                int dx = x[j] - x[i];
                int dy = y[j] - y[i];
                if (dx == 0 && dy == 0) continue; // Skip same points
                int g = gcd(dx, dy);
                int ddx = dx / g;
                int ddy = dy / g;
                if (ddx < 0 || (ddx == 0 && ddy < 0)) {
                    ddx = -ddx;
                    ddy = -ddy;
                }
                pii dir = make_pair(ddx, ddy);
                if (directions.count(dir)) continue;
                directions.insert(dir);

                // Compute normal vector
                double l = sqrt(dx * dx + dy * dy);
                double nx = -dy / l;
                double ny = dx / l;
                normals.emplace_back(nx, ny);
            }
        }

        // For all edges
        for (int i = 0; i < n; ++i) {
            int j = (i + 1) % n;
            int dx = x[j] - x[i];
            int dy = y[j] - y[i];
            if (dx == 0 && dy == 0) continue; // Skip same points
            int g = gcd(dx, dy);
            int ddx = dx / g;
            int ddy = dy / g;
            if (ddx < 0 || (ddx == 0 && ddy < 0)) {
                ddx = -ddx;
                ddy = -ddy;
            }
            pii dir = make_pair(ddx, ddy);
            if (directions.count(dir)) continue;
            directions.insert(dir);

            // Compute normal vector
            double l = sqrt(dx * dx + dy * dy);
            double nx = -dy / l;
            double ny = dx / l;
            normals.emplace_back(nx, ny);
        }

        double min_width = 1e20;
        for (auto& normal : normals) {
            double nx = normal.nx;
            double ny = normal.ny;

            // Standardize normal vector direction
            if (nx < -EPS || (fabs(nx) < EPS && ny < -EPS)) {
                nx = -nx;
                ny = -ny;
            }

            double min_proj = 1e20, max_proj = -1e20;
            for (int i = 0; i < n; ++i) {
                double s = x[i] * nx + y[i] * ny;
                min_proj = min(min_proj, s);
                max_proj = max(max_proj, s);
            }
            double width = max_proj - min_proj;
            min_width = min(min_width, width);
        }

        // Round up to nearest multiple of 0.01
        min_width = ceil((min_width - EPS) * 100.0) / 100.0;

        cout << "Case " << case_num++ << ": ";
        cout << fixed << setprecision(2) << min_width << endl;
    }
    return 0;
}
