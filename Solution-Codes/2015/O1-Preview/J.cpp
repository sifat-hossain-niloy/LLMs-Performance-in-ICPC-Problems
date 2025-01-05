#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const int MAX_AREA = 500000;

int main() {
    int n;
    cin >> n;
    vector<pair<int, int>> queries(n);
    int max_ahi = 0;
    for (int i = 0; i < n; ++i) {
        cin >> queries[i].first >> queries[i].second;
        max_ahi = max(max_ahi, queries[i].second);
    }

    vector<long long> w(MAX_AREA + 1, 0);

    for (int W = 1; W * W <= MAX_AREA; ++W) {
        for (int H = W; W * H <= MAX_AREA; ++H) {
            int area = W * H;
            if (area > max_ahi) break;
            long long num_parallelograms = 4LL * (W - 1) * (H - 1);
            if (W != H) num_parallelograms *= 2; // Account for swapping W and H
            w[area] += num_parallelograms;
        }
    }

    for (int i = 1; i <= max_ahi; ++i) {
        if (w[i] == 0) continue;
        if (w[i] % 2 == 0) {
            w[i] /= 2;
        }
    }

    for (const auto& q : queries) {
        int alo = q.first, ahi = q.second;
        long long max_w = -1;
        int best_a = -1;
        for (int a = alo; a <= ahi; ++a) {
            if (w[a] > max_w) {
                max_w = w[a];
                best_a = a;
            }
        }
        cout << best_a << " " << max_w << endl;
    }

    return 0;
}
