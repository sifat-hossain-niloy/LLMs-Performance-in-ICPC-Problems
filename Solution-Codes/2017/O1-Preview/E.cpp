#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

using namespace std;

int n;
double t;
vector<double> d, s;

double computeTime(double c) {
    double totalTime = 0.0;
    for (int i = 0; i < n; ++i) {
        double speed = s[i] + c;
        if (speed <= 0) return 1e18; // Return a large time if speed is non-positive
        totalTime += d[i] / speed;
    }
    return totalTime;
}

int main() {
    cin >> n >> t;
    d.resize(n);
    s.resize(n);
    double c_min = -1e9;
    for (int i = 0; i < n; ++i) {
        cin >> d[i] >> s[i];
        c_min = max(c_min, -s[i]);
    }
    double left = c_min + 1e-8;
    double right = c_min + 1e10; // A sufficiently large number
    for (int iter = 0; iter < 1000; ++iter) {
        double mid = (left + right) / 2.0;
        double currentTime = computeTime(mid);
        if (currentTime > t) {
            left = mid;
        } else {
            right = mid;
        }
    }
    cout << fixed << setprecision(9) << (left + right) / 2.0 << endl;
    return 0;
}
