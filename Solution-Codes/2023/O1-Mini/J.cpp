#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, c;
    cin >> n >> c;
    vector<ll> t(n);
    for(int i=0;i<n;i++) cin >> t[i];
    sort(t.begin(), t.end());
    ll total_time =0;
    if(c ==2){
        // Implement standard c=2 strategy
        int remaining = n;
        while(remaining >3){
            // Option1: t2 + t1 + tn + t2
            ll option1 = t[1] + t[0] + t[remaining-1] + t[1];
            // Option2: t1 + t2 + tn-2 + t1
            ll option2 = t[0] + t[1] + t[remaining-2] + t[0];
            total_time += min(option1, option2);
            remaining -=2;
        }
        if(remaining ==3){
            total_time += t[0] + t[1] + t[2];
        }
        else if(remaining ==2){
            total_time += t[1];
        }
        else if(remaining ==1){
            total_time += t[0];
        }
    }
    else{
        // Implement grouping strategy
        ll sum_max =0;
        ll m =0;
        for(int i=n-1;i>=0;i-=c){
            sum_max += t[i];
            m +=1;
        }
        if(m >=1){
            total_time = sum_max + t[0]*(m-1);
        }
    }
    cout << total_time;
}
