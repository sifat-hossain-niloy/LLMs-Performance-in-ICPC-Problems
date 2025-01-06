#include <bits/stdc++.h>
using namespace std;

int main(){
    int n;
    cin >> n;
    vector<int> dolls(n);
    for(auto &x: dolls) cin >> x;
    vector<bool> dp(n+1, false);
    dp[0] = true;
    for(int i=1;i<=n;i++){
        // Try all possible j from i downto1
        int m = 0;
        vector<int> counts(501, 0);
        for(int j=i; j>=1; j--){
            m = max(m, dolls[j-1]);
            counts[dolls[j-1]]++;
            int size = i - j +1;
            if(m > size) continue;
            if(m == size){
                bool flag = true;
                for(int k=1; k<=m; k++) {
                    if(counts[k] !=1){
                        flag = false;
                        break;
                    }
                }
                if(flag && dp[j-1]){
                    dp[i] = true;
                    break;
                }
            }
        }
    }
    if(dp[n]){
        cout << n;
    }
    else{
        cout << "impossible";
    }
}
