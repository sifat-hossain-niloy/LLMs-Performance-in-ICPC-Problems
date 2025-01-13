#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, c;
    cin >> n >> c;
    vector<ll> t(n);
    for(auto &x:t) cin >> x;
    sort(t.begin(), t.end());
    ll total_time =0;
    if(c ==2){
        // Implement standard two-person strategy
        int left =n;
        while(left >0){
            if(left ==1){
                total_time += t[0];
                left -=1;
            }
            else if(left ==2){
                total_time += t[1];
                left -=2;
            }
            else if(left ==3){
                total_time += t[0] + t[1] + t[2];
                left -=3;
            }
            else{
                // Option1: t1 + t0 + tn-1 + t1
                ll option1 = t[1] + t[0] + t[left-1] + t[1];
                // Option2: tn-2 + t0 + tn-1 + t0
                ll option2 = t[left-2] + t[0] + t[left-1] + t[0];
                total_time += min(option1, option2);
                left -=2;
            }
        }
    }
    else{
        // Implement batch approach
        int left =n;
        while(left >c){
            // Send c-1 slowest plus t0
            total_time += t[left-1];
            // Return with t0
            total_time += t[0];
            left -= (c-1);
        }
        if(left >0){
            total_time += t[left-1];
        }
    }
    cout << total_time;
}
