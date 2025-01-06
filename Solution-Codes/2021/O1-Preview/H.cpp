#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

struct Tablet {
    int index;
    int min_prefix;
    int total_balance;
};

int main() {
    int n;
    cin >> n;
    vector<Tablet> tablets(n);

    for (int i = 0; i < n; ++i) {
        string s;
        cin >> s;
        int balance = 0;
        int min_balance = 0;
        for (char c : s) {
            if (c == '(') balance++;
            else balance--;
            if (balance < min_balance) min_balance = balance;
        }
        tablets[i] = {i + 1, min_balance, balance};
    }

    // Sort tablets in decreasing order of min_prefix
    sort(tablets.begin(), tablets.end(), [](const Tablet &a, const Tablet &b) {
        return a.min_prefix > b.min_prefix;
    });

    int cumulative_balance = 0;
    int cumulative_min_balance = 0;
    bool possible = true;
    for (const auto &t : tablets) {
        cumulative_min_balance = min(cumulative_min_balance, cumulative_balance + t.min_prefix);
        cumulative_balance += t.total_balance;
        if (cumulative_min_balance < 0) {
            possible = false;
            break;
        }
    }

    if (possible) {
        for (const auto &t : tablets) {
            cout << t.index << endl;
        }
    } else {
        cout << "impossible" << endl;
    }

    return 0;
}
