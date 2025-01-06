#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long k;
    cin >> n >> k;
    vector<int> a(n);

    for (int &x : a) cin >> x;

    if (k >= n / 2) {
        sort(a.begin(), a.end());
    } else {
        int first_k = k;
        int last_k = k;
        vector<int> indices;

        for (int i = 0; i < k; ++i) indices.push_back(i);
        for (int i = n - k; i < n; ++i) indices.push_back(i);

        vector<int> elements;
        for (int idx : indices) elements.push_back(a[idx]);

        sort(elements.begin(), elements.end());

        for (int i = 0; i < indices.size(); ++i) {
            a[indices[i]] = elements[i];
        }
    }

    for (int x : a) cout << x << ' ';
    cout << '\n';

    return 0;
}
