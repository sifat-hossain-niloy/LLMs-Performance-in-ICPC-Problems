#include <bits/stdc++.h>
using namespace std;

struct Sequence {
    string seq;
    int border_length;
    int index;
};

// Function to compute the prefix function (KMP) and return the last value as border length
int compute_border_length(const string& s) {
    int n = s.size();
    vector<int> pi(n, 0);
    for(int i=1;i<n;i++) {
        int j = pi[i-1];
        while(j > 0 && s[i] != s[j]) {
            j = pi[j-1];
        }
        if(s[i] == s[j]) {
            j++;
        }
        pi[i] = j;
    }
    return pi[n-1];
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, s;
    cin >> n >> s;
    vector<Sequence> sequences(s);
    for(int i=0;i<s;i++){
        cin >> sequences[i].seq;
        sequences[i].index = i;
        sequences[i].border_length = compute_border_length(sequences[i].seq);
    }
    // Sort by increasing border_length, then by index
    sort(sequences.begin(), sequences.end(), [&](const Sequence& a, const Sequence& b) -> bool{
        if(a.border_length != b.border_length){
            return a.border_length < b.border_length;
        }
        return a.index < b.index;
    });
    for(auto &seq : sequences){
        cout << seq.seq << "\n";
    }
}
