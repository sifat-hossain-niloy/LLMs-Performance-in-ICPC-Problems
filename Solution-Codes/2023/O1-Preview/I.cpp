#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    long long sum = 0;  // We'll store the sum of percentages in a 64-bit integer.
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int val;
            cin >> val;
            sum += val;
        }
    }

    // Compute the average (as a double).
    // This is the overall water coverage percentage.
    double coverage = static_cast<double>(sum) / (n * m);

    // Print with an absolute error <= 10^-6.
    // Setting precision to e.g. 9 or more is sufficient.
    cout << fixed << setprecision(9) << coverage << "\n";

    return 0;
}
