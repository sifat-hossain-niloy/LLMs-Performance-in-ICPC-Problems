#include <iostream>
#include <vector>
#include <climits>

using namespace std;

int main() {
    int n, k;
    cin >> n >> k;

    const int MAX_N = 105;
    vector<vector<int>> cost(MAX_N, vector<int>(MAX_N, INT_MAX));

    // Initialize diagonal costs to 0
    for (int i = 1; i <= n + 1; ++i) {
        cost[i][i] = 0;
    }

    // Read the costs
    for (int i = 1; i <= n; ++i) {
        int cnt = n - i + 1;
        for (int j = 1; j <= cnt; ++j) {
            int c;
            cin >> c;
            cost[i][i + j] = c;
        }
    }

    // DP array to store the minimum cost to service requests up to i
    vector<int> dp(n + 1, INT_MAX);
    dp[0] = 0; // Base case: no cost to service 0 requests

    for (int i = 1; i <= n; ++i) {
        // Try all possible previous positions to partition the requests
        for (int t = 0; t < i; ++t) {
            int segmentCost;
            if (t == 0) {
                // Starting from the company to request i
                segmentCost = cost[1][i + 1];
            } else {
                // From request t to request i
                segmentCost = cost[t + 1][i + 1];
            }

            if (segmentCost != INT_MAX && dp[t] != INT_MAX) {
                dp[i] = min(dp[i], dp[t] + segmentCost);
            }
        }
    }

    cout << dp[n] << endl;

    return 0;
}
