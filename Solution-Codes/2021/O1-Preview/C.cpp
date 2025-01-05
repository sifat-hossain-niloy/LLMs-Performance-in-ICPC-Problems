#include <iostream>
#include <algorithm>
#include <cstdint>

using namespace std;

typedef unsigned long long ull;

ull mod_mul(ull a, ull b, ull mod) {
    ull result = 0;
    a %= mod;
    b %= mod;
    while (b) {
        if (b & 1)
            result = (result + a) % mod;
        a = (2 * a)% mod;
        b >>= 1;
    }
    return result;
}

ull mod_pow(ull base, ull exponent, ull mod) {
    ull result = 1;
    base %= mod;
    while (exponent) {
        if (exponent & 1)
            result = mod_mul(result, base, mod);
        base = mod_mul(base, base, mod);
        exponent >>= 1;
    }
    return result;
}

int main() {
    ull n, m;
    cin >> n >> m;
    bool found = false;
    ull best_p = 0, best_q = 0;
    for (ull q = 2; q <= 1000; ++q) {
        for (ull p = 1; p < q; ++p) {
            ull mod = p * m;
            ull N = mod_pow(q, n, mod);
            ull M = mod_pow(q - p, n, mod);
            ull Numerator;
            if (N >= M)
                Numerator = N - M;
            else
                Numerator = N + mod - M;
            if (Numerator % p != 0)
                continue;
            ull S = Numerator / p;
            if (S == 0)
                continue;
            if (m % S != 0)
                continue;
            if (!found || q < best_q || (q == best_q && p < best_p)) {
                best_p = p;
                best_q = q;
                found = true;
            }
        }
    }
    if (found)
        cout << best_p << " " << best_q << endl;
    else
        cout << "impossible" << endl;
    return 0;
}
