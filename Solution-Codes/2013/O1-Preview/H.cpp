#include <iostream>
#include <vector>
#include <set>
#include <cstring>

using namespace std;

const int MAXN = 505;

int n;
int size[MAXN];
bool dp[MAXN][MAXN];
bool can_assemble[MAXN];

int main() {
    cin >> n;
    for (int i = 1; i <= n; ++i) {
        cin >> size[i];
    }

    // Precompute dp[l][r]
    for (int len = 1; len <= n; ++len) {
        for (int l = 1; l + len -1 <= n; ++l) {
            int r = l + len -1;
            int max_size = 0;
            set<int> sizes;
            for (int k = l; k <= r; ++k) {
                max_size = max(max_size, size[k]);
                sizes.insert(size[k]);
            }
            bool valid = true;
            if ((int)sizes.size() != max_size) {
                valid = false;
            } else {
                for (int s = 1; s <= max_size; ++s) {
                    if (sizes.count(s) == 0) {
                        valid = false;
                        break;
                    }
                }
            }
            if (valid) {
                dp[l][r] = true;
                continue;
            }

            dp[l][r] = false;
            for (int m = l; m < r; ++m) {
                if (dp[l][m] && dp[m+1][r]) {
                    dp[l][r] = true;
                    break;
                }
            }
        }
    }

    memset(can_assemble, false, sizeof(can_assemble));
    can_assemble[0] = true;
    for (int i = 1; i <= n; ++i) {
        for (int j = i; j >= 1; --j) {
            if (can_assemble[j-1] && dp[j][i]) {
                can_assemble[i] = true;
                break;
            }
        }
    }

    if (can_assemble[n]) {
        cout << n << endl;
    } else {
        cout << "impossible" << endl;
    }

    return 0;
}
