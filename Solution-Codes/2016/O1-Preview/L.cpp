#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Drive {
    long long a; // old capacity
    long long b; // new capacity
    long long surplus; // a_i - b_i
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<Drive> drives(n);
    for (int i = 0; i < n; ++i) {
        cin >> drives[i].a >> drives[i].b;
        drives[i].surplus = drives[i].a - drives[i].b;
    }

    // Sort drives in increasing order of surplus
    sort(drives.begin(), drives.end(), [](const Drive &d1, const Drive &d2) {
        return d1.surplus < d2.surplus;
    });

    long long cumulative_a = 0, cumulative_b = 0, E = 0;
    for (const auto &drive : drives) {
        cumulative_a += drive.a;
        long long E_k = cumulative_a - cumulative_b;
        if (E_k > E)
            E = E_k;
        cumulative_b += drive.b;
    }

    cout << E << "\n";
    return 0;
}
