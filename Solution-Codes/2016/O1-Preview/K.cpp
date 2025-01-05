#include <iostream>
#include <vector>
#include <string>

using namespace std;

bool parse(const string &s, int k);

bool parse_sequence(const string &s, int k) {
    size_t pos = 0;
    bool found_quotation = false;

    while (pos < s.length()) {
        // Skip non-quote characters
        while (pos < s.length() && s[pos] != '\'') pos++;

        if (pos >= s.length()) break;

        // Try to parse a (k)-quotation starting at pos
        size_t start = pos;
        int quote_count = 0;
        while (pos < s.length() && s[pos] == '\'') {
            quote_count++;
            pos++;
        }

        if (quote_count < k) return false;

        // Find the matching ending quotes
        size_t inner_start = pos;
        int inner_end = -1;
        int nested_quote_count = 0;

        for (size_t i = pos; i < s.length(); i++) {
            if (s[i] == '\'') {
                nested_quote_count++;
                if (nested_quote_count == k) {
                    inner_end = i;
                    break;
                }
            } else {
                nested_quote_count = 0;
            }
        }

        if (inner_end == -1) return false;

        string inner = s.substr(inner_start, inner_end - inner_start - (k - 1));
        if (!parse(s.substr(start, inner_end - start + 1), k)) return false;

        found_quotation = true;
        pos = inner_end + 1;
    }

    return found_quotation;
}

bool parse(const string &s, int k) {
    if (k == 1) {
        if (s.length() < 2) return false;
        if (s[0] != '\'' || s[s.length() - 1] != '\'') return false;
        for (size_t i = 1; i < s.length() - 1; i++) {
            if (s[i] == '\'') return false;
        }
        return true;
    } else {
        if (s.length() < 2 * k) return false;
        for (int i = 0; i < k; i++) {
            if (s[i] != '\'' || s[s.length() - k + i] != '\'') return false;
        }
        string inner = s.substr(k, s.length() - 2 * k);
        return parse_sequence(inner, k - 1);
    }
}

int main() {
    int n;
    cin >> n;
    vector<int> ai(n);
    int min_ai = 100;
    for (int i = 0; i < n; i++) {
        cin >> ai[i];
        min_ai = min(min_ai, ai[i]);
    }

    // Construct the string
    string s;
    for (int i = 0; i < n; i++) {
        s += string(ai[i], '\'');
        if (i != n - 1) s += 'x'; // Add a non-quote character
    }

    // Try k from min_ai down to 1
    for (int k = min_ai; k >= 1; k--) {
        if (parse(s, k)) {
            cout << k << endl;
            return 0;
        }
    }

    cout << "no quotation" << endl;
    return 0;
}
