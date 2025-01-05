#include <iostream>
#include <vector>
#include <string>
#include <map>
using namespace std;

typedef unsigned long long ull;
const int MAX_STATES = 105;

struct Automaton {
    int len_p;
    int num_states;
    vector<vector<int>> next;
    vector<bool> is_accept;

    Automaton(const string &p) {
        len_p = p.size();
        num_states = len_p + 1;
        next.assign(num_states, vector<int>(2, 0));
        is_accept.assign(num_states, false);
        buildKMP(p);
    }

    void buildKMP(const string &p) {
        vector<int> lps(len_p, 0);
        for (int i = 1, len = 0; i < len_p; ) {
            if (p[i] == p[len]) {
                lps[i++] = ++len;
            } else if (len) {
                len = lps[len - 1];
            } else {
                lps[i++] = 0;
            }
        }
        // Build automaton transitions
        for (int state = 0; state < num_states; ++state) {
            for (int c = 0; c <= 1; ++c) {
                if (state < len_p && p[state] - '0' == c) {
                    next[state][c] = state + 1;
                } else if (state) {
                    next[state][c] = next[lps[state - 1]][c];
                } else {
                    next[state][c] = 0;
                }
            }
        }
        is_accept[len_p] = true; // Accept state
    }
};

struct Matrix {
    vector<vector<ull>> mat;
    int size;

    Matrix(int n) : size(n) {
        mat.assign(n, vector<ull>(n, 0));
    }

    Matrix operator*(const Matrix &other) const {
        Matrix result(size);
        for (int i = 0; i < size; ++i)
            for (int k = 0; k < size; ++k)
                if (mat[i][k])
                    for (int j = 0; j < size; ++j)
                        result.mat[i][j] += mat[i][k] * other.mat[k][j];
        return result;
    }
};

Automaton automaton("");
map<int, pair<Matrix, vector<int>>> memo;

pair<Matrix, vector<int>> getMatrix(int n) {
    if (memo.count(n))
        return memo[n];
    int sz = automaton.num_states;
    if (n == 0) {
        // Transition matrix for F(0) = "0"
        Matrix M(sz);
        for (int s = 0; s < sz; ++s) {
            int t = automaton.next[s][0];
            M.mat[s][t]++;
        }
        // Suffixes after F(0)
        vector<int> suffix(sz);
        for (int s = 0; s < sz; ++s)
            suffix[s] = automaton.next[s][0];
        return memo[n] = make_pair(M, suffix);
    }
    if (n == 1) {
        // Transition matrix for F(1) = "1"
        Matrix M(sz);
        for (int s = 0; s < sz; ++s) {
            int t = automaton.next[s][1];
            M.mat[s][t]++;
        }
        // Suffixes after F(1)
        vector<int> suffix(sz);
        for (int s = 0; s < sz; ++s)
            suffix[s] = automaton.next[s][1];
        return memo[n] = make_pair(M, suffix);
    }
    // F(n) = F(n-1) + F(n-2)
    auto [M1, suf1] = getMatrix(n - 1);
    auto [M2, suf2] = getMatrix(n - 2);
    // Compute the combined matrix
    Matrix M(sz);
    // M = M1 + (suf1 * M2)
    // First, copy M1 into M
    for (int i = 0; i < sz; ++i)
        for (int j = 0; j < sz; ++j)
            M.mat[i][j] = M1.mat[i][j];
    // Now, add suf1 * M2
    for (int s = 0; s < sz; ++s)
        for (int t = 0; t < sz; ++t)
            M.mat[s][t] += (ull)suf1[s] * M2.mat[0][t];
    // Update suffixes
    vector<int> suffix(sz);
    for (int s = 0; s < sz; ++s)
        suffix[s] = suf2[s];
    return memo[n] = make_pair(M, suffix);
}

int main() {
    int testCase = 0;
    int n;
    string p;
    while (cin >> n >> p) {
        ++testCase;
        automaton = Automaton(p);
        memo.clear();
        auto [M, _] = getMatrix(n);
        ull total = 0;
        int sz = automaton.num_states;
        for (int s = 0; s < sz; ++s)
            if (automaton.is_accept[s])
                total += M.mat[0][s];
        cout << "Case " << testCase << ": " << total << endl;
    }
    return 0;
}
