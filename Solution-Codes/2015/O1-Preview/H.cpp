#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

int main() {
    double w, h;
    int n;
    cin >> w >> h >> n;

    double s = h / w;
    double p = 1.0 / (1.0 + s);

    // Calculate shaft positions
    double positions[n];
    for (int i = 1; i <= n; ++i) {
        double fraction = (double)i / (n + 1);
        positions[i - 1] = w * (1.0 - pow(1.0 - fraction, p));
    }

    // Calculate total cost
    double total_cost = 0.0;
    double x_prev = 0.0;
    for (int i = 0; i <= n; ++i) {
        double x_curr = (i == n) ? w : positions[i];
        double h_i = s * x_prev;
        double a = x_prev;
        double b = x_curr;

        // Cost for this segment
        double L1 = (1 + s) * x_prev - a;
        double L2 = b + (s - 1) * x_prev;
        double cost = 0.0;
        cost += (1.0 / 6.0) * (pow(L2, 3) - pow(L1, 3));

        total_cost += cost;
        x_prev = x_curr;
    }

    // Cost for the mother well
    double L = w + (s - 1) * w;
    total_cost += (1.0 / 2.0) * pow(L, 2) - (1.0 / 2.0) * pow(w + (s - 1) * x_prev, 2);

    cout << fixed << setprecision(6) << total_cost << endl;
    int m = min(n, 10);
    for (int i = 0; i < m; ++i) {
        cout << fixed << setprecision(6) << positions[i] << endl;
    }

    return 0;
}
