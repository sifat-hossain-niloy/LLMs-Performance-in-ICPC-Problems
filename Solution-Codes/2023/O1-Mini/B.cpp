#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, w;
    cin >> n >> w;
    if(w <4){
        cout << "infinity\n";
        return 0;
    }
    // Compute isolation as ceil((w+1)/2)
    int isolation = (w +1)/2;
    // Now, construct the schedule
    // For each week j from 1 to w
    // For team i from 1 to n:
    // Assign '1' if (i + j) %2 ==0, else '2'
    // To optimize, precompute for each week the pattern
    // Since n can be up to 10^4, and w up to 52, it's manageable
    cout << isolation << "\n";
    // Precompute for each week the assignments
    // To make it similar to sample, adjust the pattern
    // Alternative pattern: for week j, team i:
    // If (i + j) %4 ==0 or 1: '1'
    // Else: '2'
    // But to keep it simple, use (i +j)%2
    for(int j=1; j<=w; j++){
        string s;
        s.reserve(n);
        // To speed up, iterate in blocks
        // Since (i +j)%2 is same as (i%2 +j%2)%2
        // If j is odd, then (i +j)%2 = (i%2 +1)%2
        // If j is even, then (i +j)%2 = (i%2 +0)%2
        char c1, c2;
        if(j%2 ==1){
            c1 = '2';
            c2 = '1';
        }
        else{
            c1 = '1';
            c2 = '2';
        }
        for(int i=1; i<=n; i++){
            if(i%2 ==1) s += c1;
            else s += c2;
        }
        cout << s << "\n";
    }
}
