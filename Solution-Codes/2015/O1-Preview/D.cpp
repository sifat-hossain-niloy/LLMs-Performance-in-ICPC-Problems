#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdio>

using namespace std;

const double EPS = 1e-7;
const double PI = acos(-1.0);

struct Hole {
    double r;      // radius in micrometers
    double z0;     // center z-coordinate in micrometers
};

int n, s;
vector<Hole> holes;

// Function to compute the cumulative cheese volume up to depth z
double f(double z) {
    // Volume of the block up to depth z
    double V_block = 100000.0 * 100000.0 * z;
    double V_holes = 0.0;

    for (int i = 0; i < n; ++i) {
        double r = holes[i].r;
        double z_center = holes[i].z0;

        if (z <= z_center - r) {
            // Hole hasn't started yet
            continue;
        } else if (z >= z_center + r) {
            // Hole has ended
            V_holes += (4.0 / 3.0) * PI * r * r * r;
        } else {
            // Hole is partially within [0, z]
            double h_cap = z - (z_center - r);
            // h_cap should be in [0, 2r]
            // Volume of spherical cap
            double V_hole = (PI / 3.0) * h_cap * h_cap * (3.0 * r - h_cap);
            V_holes += V_hole;
        }
    }

    double V_cheese = V_block - V_holes;
    return V_cheese;
}

int main() {
    cin >> n >> s;
    holes.resize(n);
    for (int i = 0; i < n; ++i) {
        int r_i, x_i, y_i, z_i;
        cin >> r_i >> x_i >> y_i >> z_i;
        holes[i].r = r_i;         // in micrometers
        holes[i].z0 = z_i;        // in micrometers
        // x_i and y_i are not used in this problem
    }

    // Total volume of cheese
    double total_cheese_volume = f(100000.0);

    // Target volume per slice
    double target_volume = total_cheese_volume / s;

    vector<double> z_positions(s + 1);
    z_positions[0] = 0.0;
    z_positions[s] = 100000.0;

    // For each slice boundary, find z_i such that f(z_i) = i * target_volume
    for (int i = 1; i < s; ++i) {
        double target = i * target_volume;
        double z_low = z_positions[i - 1];
        double z_high = 100000.0;
        for (int iter = 0; iter < 50; ++iter) {
            double z_mid = (z_low + z_high) / 2.0;
            double f_mid = f(z_mid);
            if (f_mid < target) {
                z_low = z_mid;
            } else {
                z_high = z_mid;
            }
        }
        z_positions[i] = (z_low + z_high) / 2.0;
    }

    // Output the slice thicknesses in millimeters
    for (int i = 1; i <= s; ++i) {
        double thickness = (z_positions[i] - z_positions[i - 1]) / 1000.0;  // Convert to millimeters
        printf("%.9f\n", thickness);
    }

    return 0;
}
