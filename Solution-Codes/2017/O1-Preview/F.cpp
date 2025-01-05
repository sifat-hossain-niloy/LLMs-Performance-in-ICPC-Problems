#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cmath>

using namespace std;

typedef long long ll;
const ll INF = LLONG_MAX;

int main() {
    int d, K;
    cin >> d >> K;

    vector<int> r(d + 1);     // Red intensity values (1-indexed)
    vector<ll> p(d + 1);      // Counts of red intensity values
    for (int i = 1; i <= d; ++i) {
        cin >> r[i] >> p[i];
    }

    // Compute cumulative sums
    vector<ll> sum_p(d + 1, 0);
    vector<ll> sum_r(d + 1, 0);
    vector<ll> sum_r2(d + 1, 0);
    for (int i = 1; i <= d; ++i) {
        sum_p[i] = sum_p[i - 1] + p[i];
        sum_r[i] = sum_r[i - 1] + p[i] * r[i];
        sum_r2[i] = sum_r2[i - 1] + p[i] * (ll)r[i] * r[i];
    }

    // Precompute costs
    vector<vector<ll>> cost(d + 1, vector<ll>(d + 1, 0));
    for (int t = 1; t <= d; ++t) {
        for (int i = t; i <= d; ++i) {
            ll sp = sum_p[i] - sum_p[t - 1];
            ll sr = sum_r[i] - sum_r[t - 1];
            ll sr2 = sum_r2[i] - sum_r2[t - 1];

            // Optimal v_j is sr / sp (weighted mean)
            double v_j = (double)sr / sp;
            ll v_low = (ll)floor(v_j);
            ll v_high = (ll)ceil(v_j);

            ll min_E = INF;
            for (ll v = v_low - 1; v <= v_high + 1; ++v) {
                if (v < 0 || v > 255) continue;
                ll E = sp * v * v - 2 * sr * v + sr2;
                if (E < min_E) min_E = E;
            }
            cost[t][i] = min_E;
        }
    }

    // Initialize DP table
    vector<vector<ll>> dp(d + 1, vector<ll>(K + 1, INF));
    dp[0][0] = 0;

    // Dynamic Programming
    for (int k = 1; k <= K; ++k) {
        for (int i = k; i <= d; ++i) {
            dp[i][k] = INF;
            for (int t = k - 1; t <= i - 1; ++t) {
                if (dp[t][k - 1] == INF) continue;
                ll val = dp[t][k - 1] + cost[t + 1][i];
                if (val < dp[i][k]) dp[i][k] = val;
            }
        }
    }

    cout << dp[d][K] << endl;

    return 0;
}
