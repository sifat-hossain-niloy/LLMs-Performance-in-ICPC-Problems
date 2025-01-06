#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
typedef long long ll;

// Safe exponentiation to avoid overflow
ll power(ll a, ll n, ll limit) {
    ll res = 1;
    for (int i = 0; i < n; ++i) {
        if (res > limit / a)
            return limit + 1;
        res *= a;
    }
    return res;
}

// Convert y to base b and collect digits
bool getDigits(ll y, ll b, vector<int> &digits) {
    digits.clear();
    while (y > 0) {
        int d = y % b;
        if (d > 9) return false;
        digits.push_back(d);
        y /= b;
    }
    return true;
}

int main() {
    ll y, l;
    cin >> y >> l;

    ll max_b = 0;

    for (int k = 1; k <= 60; ++k) {
        ll b_low, b_high;

        if (k == 1) {
            b_low = y;
            b_high = y;
            if (b_low < 10) continue;
        } else {
            // Find b_low such that b_low^(k-1) <= y
            ll left = 10, right = y, b_low_candidate = -1;
            while (left <= right) {
                ll mid = left + (right - left) / 2;
                ll mid_pow = power(mid, k - 1, y);
                if (mid_pow <= y) {
                    b_low_candidate = mid;
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            }
            if (b_low_candidate == -1) continue;
            b_low = b_low_candidate;

            // Find b_high such that b_high^k > y
            left = 10, right = y;
            ll b_high_candidate = -1;
            while (left <= right) {
                ll mid = left + (right - left) / 2;
                ll mid_pow = power(mid, k, y);
                if (mid_pow <= y) {
                    b_high_candidate = mid;
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            }
            if (b_high_candidate == -1) continue;
            b_high = b_high_candidate;
        }

        for (ll b = b_low; b <= b_high; ++b) {
            vector<int> digits;
            if (!getDigits(y, b, digits)) continue;
            if ((int)digits.size() != k) continue;
            ll N = 0;
            for (int i = digits.size() - 1; i >= 0; --i) {
                N = N * 10 + digits[i];
            }
            if (N >= l) {
                if (b > max_b) {
                    max_b = b;
                }
            }
        }
    }

    cout << max_b << endl;

    return 0;
}
