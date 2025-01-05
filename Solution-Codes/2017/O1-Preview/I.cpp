#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main() {
    int m, n;
    cin >> m >> n;
    vector<vector<bool>> can_translate(26, vector<bool>(26, false));

    // Initialize can_translate[i][i] = true
    for (int i = 0; i < 26; ++i) {
        can_translate[i][i] = true;
    }

    // Read translations
    for (int i = 0; i < m; ++i) {
        char from_char, to_char;
        cin >> from_char >> to_char;
        int from = from_char - 'a';
        int to = to_char - 'a';
        can_translate[from][to] = true;
    }

    // Floyd-Warshall algorithm to compute transitive closure
    for (int k = 0; k < 26; ++k) {
        for (int i = 0; i < 26; ++i) {
            if (can_translate[i][k]) {
                for (int j = 0; j < 26; ++j) {
                    if (can_translate[k][j]) {
                        can_translate[i][j] = true;
                    }
                }
            }
        }
    }

    // Process each word pair
    string word1, word2;
    getline(cin, word1); // Consume the remaining newline character
    for (int pair_idx = 0; pair_idx < n; ++pair_idx) {
        getline(cin, word1);
        size_t space_pos = word1.find(' ');
        word2 = word1.substr(space_pos + 1);
        word1 = word1.substr(0, space_pos);

        if (word1.length() != word2.length()) {
            cout << "no" << endl;
            continue;
        }

        bool match = true;
        for (size_t i = 0; i < word1.length(); ++i) {
            int c1 = word1[i] - 'a';
            int c2 = word2[i] - 'a';
            if (!can_translate[c1][c2]) {
                match = false;
                break;
            }
        }

        if (match) {
            cout << "yes" << endl;
        } else {
            cout << "no" << endl;
        }
    }

    return 0;
}
