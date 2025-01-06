#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <queue>

using namespace std;

const int ALPHABET_SIZE = 3; // R, P, S

// Map characters to indices
int char_to_index(char c) {
    if (c == 'R') return 0;
    if (c == 'P') return 1;
    if (c == 'S') return 2;
    return -1; // Should not happen
}

struct Pattern {
    string s;
    double prob;
    int index; // Original order
};

vector<int> compute_failure_function(const string &pattern) {
    int m = pattern.size();
    vector<int> failure(m + 1);
    failure[0] = -1;
    int j = -1;
    for (int i = 0; i < m; ++i) {
        while (j != -1 && pattern[j] != pattern[i]) {
            j = failure[j];
        }
        ++j;
        failure[i + 1] = j;
    }
    return failure;
}

void compute_transition_function(const string &pattern, const vector<int> &failure,
                                 vector<vector<int>> &next_state) {
    int m = pattern.size();
    next_state.resize(m + 1, vector<int>(ALPHABET_SIZE, 0));
    for (int state = 0; state <= m; ++state) {
        for (int c = 0; c < ALPHABET_SIZE; ++c) {
            if (state == m) {
                next_state[state][c] = m;
                continue;
            }
            char ch;
            if (c == 0) ch = 'R';
            else if (c == 1) ch = 'P';
            else ch = 'S';

            int j = state;
            while (j != -1 && pattern[j] != ch) {
                j = failure[j];
            }
            next_state[state][c] = j + 1;
        }
    }
}

vector<double> multiply(const vector<double> &v, const vector<unordered_map<int, double>> &A) {
    int m = v.size();
    vector<double> res(m, 0.0);
    for (int s = 0; s < m; ++s) {
        if (v[s] > 0) {
            for (const auto &p : A[s]) {
                int s_next = p.first;
                double prob = p.second;
                res[s_next] += v[s] * prob;
            }
        }
    }
    return res;
}

vector<unordered_map<int, double>> multiply_matrix(const vector<unordered_map<int, double>> &A,
                                                   const vector<unordered_map<int, double>> &B) {
    int m = A.size();
    vector<unordered_map<int, double>> res(m);
    for (int s = 0; s < m; ++s) {
        for (const auto &p : A[s]) {
            int s_mid = p.first;
            double prob1 = p.second;
            for (const auto &q : B[s_mid]) {
                int s_next = q.first;
                double prob2 = q.second;
                res[s][s_next] += prob1 * prob2;
            }
        }
    }
    return res;
}

int main() {
    int n, s;
    cin >> n >> s;
    vector<Pattern> patterns(s);
    for (int i = 0; i < s; ++i) {
        cin >> patterns[i].s;
        patterns[i].index = i;
    }

    for (auto &pattern : patterns) {
        int m = pattern.s.size();
        auto failure = compute_failure_function(pattern.s);
        vector<vector<int>> next_state;
        compute_transition_function(pattern.s, failure, next_state);

        // Build the transition matrix A as a vector of maps for sparsity
        vector<unordered_map<int, double>> A(m + 1);
        for (int s = 0; s <= m; ++s) {
            if (s == m) continue; // Accepting state
            for (int c = 0; c < ALPHABET_SIZE; ++c) {
                int s_next = next_state[s][c];
                A[s][s_next] += 1.0 / 3.0;
            }
        }

        // Exponentiation by squaring
        vector<double> v(m + 1, 0.0);
        v[0] = 1.0; // Start from initial state
        int exp = n;
        vector<unordered_map<int, double>> A_power = A;
        while (exp > 0) {
            if (exp % 2 == 1) {
                v = multiply(v, A_power);
            }
            A_power = multiply_matrix(A_power, A_power);
            exp /= 2;
        }

        // Probability that pattern has not occurred
        double prob_not_occured = 0.0;
        for (int s = 0; s < m; ++s) {
            prob_not_occured += v[s];
        }
        pattern.prob = 1.0 - prob_not_occured;
    }

    // Sort the patterns based on probability, maintain original order for ties
    sort(patterns.begin(), patterns.end(), [](const Pattern &a, const Pattern &b) {
        if (abs(a.prob - b.prob) > 1e-12)
            return a.prob > b.prob;
        else
            return a.index < b.index;
    });

    for (const auto &pattern : patterns) {
        cout << pattern.s << endl;
    }

    return 0;
}
