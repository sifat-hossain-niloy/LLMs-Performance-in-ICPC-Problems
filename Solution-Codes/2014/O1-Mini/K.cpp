#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    cin >> n;
    ll total = 0;
    for(int i=0;i<n;i++){
        ll k;
        cin >> k;
        if(k %2){
            total += (k +1);
        }
        // else f(k) =0, do nothing
    }
    cout << total;
}
