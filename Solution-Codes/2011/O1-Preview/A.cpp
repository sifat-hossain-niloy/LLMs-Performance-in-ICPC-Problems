#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <algorithm>

using namespace std;

typedef long long ll;
typedef pair<ll, ll> pll;

struct State {
    ll S;
    ll T;
    string ops;

    State(ll S, ll T, string ops) : S(S), T(T), ops(ops) {}
};

int main() {
    ll a, m, p, q, r, s;
    int caseNum = 1;
    while (cin >> a >> m >> p >> q >> r >> s) {
        if (a == 0 && m == 0 && p == 0 && q == 0 && r == 0 && s == 0) break;

        cout << "Case " << caseNum++ << ": ";

        set<pll> visited;
        queue<State> qStates;

        qStates.push(State(1, 0, ""));

        bool found = false;
        string result;

        while (!qStates.empty()) {
            State cur = qStates.front();
            qStates.pop();

            if (visited.count({cur.S, cur.T})) continue;
            visited.insert({cur.S, cur.T});

            ll y_min = min(cur.S * p + cur.T, cur.S * q + cur.T);
            ll y_max = max(cur.S * p + cur.T, cur.S * q + cur.T);

            if (y_min >= r && y_max <= s) {
                // Found a valid sequence
                if (cur.ops.empty()) {
                    cout << "empty" << endl;
                } else {
                    // Convert the operations into required format
                    string output;
                    char prevOp = cur.ops[0];
                    int count = 1;
                    for (size_t i = 1; i < cur.ops.size(); ++i) {
                        if (cur.ops[i] == prevOp) {
                            count++;
                        } else {
                            output += to_string(count) + prevOp + " ";
                            prevOp = cur.ops[i];
                            count = 1;
                        }
                    }
                    output += to_string(count) + prevOp;
                    cout << output << endl;
                }
                found = true;
                break;
            }

            if (cur.ops.size() >= 15) continue; // Limit depth to prevent excessive computation

            // Apply 'A' operation
            ll newT_A = cur.T + a * cur.S;
            string newOps_A = cur.ops + "A";
            if (!visited.count({cur.S, newT_A})) {
                qStates.push(State(cur.S, newT_A, newOps_A));
            }

            // Apply 'M' operation
            ll newS_M = cur.S * m;
            ll newT_M = cur.T * m;
            string newOps_M = cur.ops + "M";
            if (!visited.count({newS_M, newT_M})) {
                qStates.push(State(newS_M, newT_M, newOps_M));
            }
        }

        if (!found) {
            cout << "impossible" << endl;
        }
    }

    return 0;
}
