#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <algorithm>

using namespace std;

// Define M_PI if it's not defined
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Multiply two polynomials
vector<double> poly_multiply(const vector<double>& a, const vector<double>& b) {
    vector<double> result(a.size() + b.size() - 1, 0.0);
    for (size_t i = 0; i < a.size(); ++i)
        for (size_t j = 0; j < b.size(); ++j)
            result[i + j] += a[i] * b[j];
    return result;
}

// Compute antiderivative of a polynomial
vector<double> poly_antiderivative(const vector<double>& a) {
    vector<double> result(a.size() + 1, 0.0);
    for (size_t i = 0; i < a.size(); ++i)
        result[i + 1] = a[i] / (i + 1);
    return result;
}

// Evaluate polynomial at x
double poly_eval(const vector<double>& a, double x) {
    double result = 0.0;
    for (int i = static_cast<int>(a.size()) - 1; i >= 0; --i)
        result = result * x + a[i];
    return result;
}

// Root finding using bisection method
double find_x(const vector<double>& R_poly, double c, double xlow, double xhigh) {
    const double EPS = 1e-7;
    double lo = xlow, hi = xhigh, mid;
    while (hi - lo > EPS) {
        mid = (lo + hi) / 2.0;
        double R_mid = poly_eval(R_poly, mid);
        if (R_mid - c >= 0.0)
            hi = mid;
        else
            lo = mid;
    }
    return (lo + hi) / 2.0;
}

int main() {
    int n;
    int case_num = 1;
    while (cin >> n) {
        vector<double> P_poly(n + 1);
        for (int i = 0; i <= n; ++i)
            cin >> P_poly[i];
        double xlow, xhigh, inc;
        cin >> xlow >> xhigh >> inc;

        // Compute Q(x) = [P(x)]^2
        vector<double> Q_poly = poly_multiply(P_poly, P_poly);

        // Compute R(x), the antiderivative of Q(x)
        vector<double> R_poly = poly_antiderivative(Q_poly);

        // Compute total volume
        double R_xlow = poly_eval(R_poly, xlow);
        double R_xhigh = poly_eval(R_poly, xhigh);
        double V_total = M_PI * (R_xhigh - R_xlow);

        cout << "Case " << case_num << ": " << fixed << setprecision(2) << V_total + 1e-8 << endl;

        vector<double> distances;
        for (int k = 1; k <= 8; ++k) {
            double c = R_xlow + (k * inc) / M_PI;
            if (c - R_xhigh > 1e-7) {
                break;
            }
            double x = find_x(R_poly, c, xlow, xhigh);
            distances.push_back(x - xlow);
        }

        if (distances.empty())
            cout << "insufficient volume" << endl;
        else {
            for (size_t i = 0; i < distances.size(); ++i) {
                if (i > 0) cout << " ";
                cout << fixed << setprecision(2) << distances[i] + 1e-8;
            }
            cout << endl;
        }
        case_num++;
    }
    return 0;
}
