#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

int main() {
    int c;
    cin >> c;
    while (c--) {
        double l, v1, v2, t, s;
        cin >> l >> v1 >> v2 >> t >> s;
        double vl = v1, vr = v2;
        double U = vr - vl;
        int i = 0;
        bool possible = false;
        int max_knocks = 1000; // Arbitrary large number to prevent infinite loops
        for (i = 1; i <= max_knocks; ++i) {
            double ti = i * s;
            if (ti > l / vl + 1e-8) {
                break;
            }
            double xi = (vl + vr) / 2 * ti;
            if (xi > l) xi = l;
            double v_cutoff = xi / ti;
            if (v_cutoff <= vl + 1e-8 || v_cutoff >= vr - 1e-8) {
                if (U <= t + 1e-8) {
                    possible = true;
                    --i; // We didn't get new information this knock
                    break;
                } else {
                    break;
                }
            }
            if ((vr - v_cutoff) >= (v_cutoff - vl)) {
                vl = v_cutoff;
            } else {
                vr = v_cutoff;
            }
            U = vr - vl;
            if (U <= t + 1e-8) {
                possible = true;
                break;
            }
        }
        if (possible) {
            cout << i << endl;
        } else {
            cout << "impossible" << endl;
        }
    }
    return 0;
}
