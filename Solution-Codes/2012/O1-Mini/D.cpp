#include <bits/stdc++.h>
using namespace std;

// Structure to hold count, prefix, and suffix for each Fibonacci word
struct FibWord {
    __int128 count;
    string prefix;
    string suffix;
};

// Function to convert __int128 to string
string int128_to_string(__int128 n) {
    bool is_negative = false;
    if (n < 0) {
        is_negative = true;
        n = -n;
    }
    string s = "";
    if (n == 0) return "0";
    while (n > 0) {
        char c = '0' + (n % 10);
        s += c;
        n /= 10;
    }
    if (is_negative) s += '-';
    reverse(s.begin(), s.end());
    return s;
}

// Function to count occurrences of p in s, allowing overlaps
__int128 count_p_in(const string &s, const string &p) {
    if (p.empty()) return 0;
    __int128 count = 0;
    size_t m = p.size();
    if (s.size() < m) return 0;
    for (size_t i = 0; i + m <= s.size(); ++i) {
        if (s.compare(i, m, p) == 0) {
            count++;
        }
    }
    return count;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    string p;
    int case_num = 1;
    while(cin >> n >> p){
        size_t m = p.size();
        // Initialize Fibonacci words up to n
        vector<FibWord> fib(max(n+1, 2));
        // Initialize F(0)
        fib[0].count = (p == "0") ? 1 : 0;
        if(m-1 > 0){
            fib[0].prefix = "0";
            fib[0].suffix = "0";
        }
        else{
            fib[0].prefix = "";
            fib[0].suffix = "";
        }
        // Initialize F(1)
        fib[1].count = (p == "1") ? 1 : 0;
        if(m-1 > 0){
            fib[1].prefix = "1";
            fib[1].suffix = "1";
        }
        else{
            fib[1].prefix = "";
            fib[1].suffix = "";
        }
        // Compute F(2) to F(n)
        for(int i=2; i<=n; ++i){
            FibWord fw;
            fw.count = fib[i-1].count + fib[i-2].count;
            // Concatenate suffix of F(n-1) and prefix of F(n-2)
            string concatenated = fib[i-1].suffix + fib[i-2].prefix;
            fw.count += count_p_in(concatenated, p);
            // Compute prefix
            if(fib[i-1].prefix.size() == m-1){
                fw.prefix = fib[i-1].prefix;
            }
            else{
                fw.prefix = fib[i-1].prefix + fib[i-2].prefix;
                if(fw.prefix.size() > m-1){
                    fw.prefix = fw.prefix.substr(0, m-1);
                }
            }
            // Compute suffix
            if(fib[i-1].suffix.size() == m-1){
                fw.suffix = fib[i-1].suffix;
            }
            else{
                fw.suffix = fib[i-1].suffix + fib[i-2].suffix;
                if(fw.suffix.size() > m-1){
                    fw.suffix = fw.suffix.substr(fw.suffix.size() - (m-1));
                }
            }
            fib[i] = fw;
        }
        // Get the count for F(n)
        __int128 final_count = fib[n].count;
        // Convert to string
        string count_str = int128_to_string(final_count);
        // Output
        cout << "Case " << case_num++ << ": " << count_str << "\n";
    }
}
