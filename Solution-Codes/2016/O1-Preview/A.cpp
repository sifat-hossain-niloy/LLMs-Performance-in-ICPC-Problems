#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;
typedef long long ll;

int m, k;
vector<int> a;
vector<int> b;
vector<ll> si;
ll S;

ll CeilDiv(ll a, ll b) {
    if (b == 0) throw "Division by zero";
    if (a >= 0) {
        return (a + b - 1) / b;
    } else {
        return a / b;
    }
}

ll FloorDiv(ll a, ll b) {
    if (b == 0) throw "Division by zero";
    if (a >= 0) {
        return a / b;
    } else {
        return (a - b + 1) / b;
    }
}

bool check(ll t, vector<ll>& di) {
    ll sum_Li = 0, sum_Ui = 0;
    for (int i = 0; i < m; ++i) {
        ll ai = a[i];
        ll N_i = (-1 - si[i]) * S + ai * (k + t);
        ll N_i_prime = (1 - si[i]) * S + ai * (k + t);

        ll Li = CeilDiv(N_i, S);
        ll Ui = FloorDiv(N_i_prime, S);

        Li = max(0LL, Li);
        Ui = min(t, Ui);

        if (Li > Ui) return false;

        sum_Li += Li;
        sum_Ui += Ui;
    }
    return sum_Li <= t && t <= sum_Ui;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    cin >> m >> k;
    a.resize(m);
    si.resize(m, 0);
    S = 0;
    for (int i = 0; i < m; ++i) {
        cin >> a[i];
        S += a[i];
    }

    b.resize(k);
    for (int i = 0; i < k; ++i) {
        cin >> b[i];
        --b[i]; // zero-based indexing
        si[b[i]]++;
    }

    vector<ll> di(m);
    for (int i = 0; i < m; ++i) {
        ll num = si[i] * S - k * a[i];
        di[i] = num; // Denominator is S
    }

    ll t_low = 0, t_high = 1e15;
    ll ans = -1;
    while (t_low <= t_high) {
        ll t_mid = (t_low + t_high) / 2;
        if (check(t_mid, di)) {
            ans = t_mid;
            t_low = t_mid + 1;
        } else {
            t_high = t_mid - 1;
        }
    }

    if (ans >= 1e15) {
        cout << "forever" << endl;
    } else {
        cout << ans << endl;
    }

    return 0;
}
