#include <bits/stdc++.h>
using namespace std;

/*
  DRY Compression on a binary string:
  
  We maintain an online prefix-function (KMP "failure function").
  Each time we add one new character, we update the prefix-function.
  Then we check if the new string ends with a square XX of some length.
  If it does, remove one copy of X, shorten the string, and continue checking.
  
  In the end, 'res' will be a valid shortest result of repeatedly
  removing consecutive repeated substrings.
*/

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    // We'll build the compressed result in a dynamic array 'res'.
    // Also keep a prefix-function array 'pf' of the same size as 'res'.
    // 'res' and 'pf' will have length up to n (where n = s.size()).
    vector<char> res; 
    res.reserve(s.size());
    vector<int> pf(s.size(), 0);

    int L = 0; // current length of 'res'
    for(char c : s) {
        // 1) Append c
        res.push_back(c);
        L++;

        // 2) Update prefix-function for index (L-1)
        if(L == 1) {
            // base case
            pf[0] = 0;
        } else {
            int k = pf[L - 2];  // pf of the previous index
            while(k > 0 && res[k] != c) {
                k = pf[k - 1];
            }
            if(res[k] == c) {
                k++;
            }
            pf[L - 1] = k;
        }

        // 3) Check for a repeated block XX at the end
        while(true) {
            // current prefix-function at the end
            int p = pf[L - 1];
            int sublen = L - p; // length of the 'unmatched' part

            // if p >= sublen, that means res has a suffix XX of length 2*sublen
            // i.e., the last sublen is repeated
            if(p >= sublen) {
                // remove one copy of that sublen
                // effectively, we want to shorten res by sublen
                for(int i = 0; i < sublen; i++){
                    res.pop_back();
                }
                L -= sublen;  // new length

                if(L == 0) {
                    break; // nothing left
                }
                // update prefix function to the new last index (L-1)
                // we can take pf[L-1] if L>0
            } else {
                break; // no repeated block at the end
            }
        }
    }

    // Convert res to a string for output
    string ans(res.begin(), res.end());
    cout << ans << "\n";

    return 0;
}
