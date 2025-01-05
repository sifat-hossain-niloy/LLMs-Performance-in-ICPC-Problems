#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

const int N_MAX = 1000;
const double EPS = 1e-15;

int main() {
    double x_input, p_input;
    cin >> x_input >> p_input;

    double x = x_input / 100.0;
    double q = p_input / 100.0;
    double ln_q = log(q);
    double ln_1_minus_q = log(1 - q);

    // Precompute ln_factorial
    vector<double> ln_fact(N_MAX + 1);
    ln_fact[0] = 0.0;
    for (int i = 1; i <= N_MAX; ++i) {
        ln_fact[i] = ln_fact[i - 1] + log(i);
    }

    double max_EG = 0.0;
    for (int N = 1; N <= N_MAX; ++N) {
        double E_L_N = 0.0;
        double max_ln_P_W = -1e100; // To normalize probabilities
        vector<double> ln_P_W(N + 1);

        for (int W = 0; W <= N; ++W) {
            int S_N = 2 * W - N;
            if (S_N >= 0) continue; // Only consider negative net gains

            // Compute ln_P_W[W]
            double ln_P = ln_fact[N] - ln_fact[W] - ln_fact[N - W] + W * ln_q + (N - W) * ln_1_minus_q;
            ln_P_W[W] = ln_P;

            if (ln_P > max_ln_P_W) {
                max_ln_P_W = ln_P;
            }
        }

        // Compute E_L_N
        double sum = 0.0;
        for (int W = 0; W <= N; ++W) {
            int S_N = 2 * W - N;
            if (S_N >= 0) continue;

            double ln_P = ln_P_W[W];
            double P = exp(ln_P - max_ln_P_W); // Normalize to avoid underflow
            double L = -S_N;
            sum += P * L;
        }
        double total_P = 0.0;
        for (int W = 0; W <= N; ++W) {
            int S_N = 2 * W - N;
            if (S_N >= 0) continue;
            double ln_P = ln_P_W[W];
            double P = exp(ln_P - max_ln_P_W); // Normalize
            total_P += P;
        }
        if (total_P < EPS) continue; // Skip if probability is negligible

        E_L_N = sum / total_P; // Expected loss when net gain is negative
        double E_S_N = N * (2 * q - 1);
        double E_G_N = E_S_N + x * E_L_N;

        if (E_G_N > max_EG) {
            max_EG = E_G_N;
        }
    }

    if (max_EG < EPS) max_EG = 0.0;
    cout << fixed << setprecision(7) << max_EG << endl;

    return 0;
}
