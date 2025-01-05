#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    ll n, k;
    cin >> n >> k;
    ll total = 2 * n * k;
    vector<ll> p(total);
    for(auto &x: p) cin >> x;
    sort(p.begin(), p.end());
    // Group mins are p[0] to p[2n-1]
    vector<ll> mins(p.begin(), p.begin() + 2*n);
    // Binary search for minimal d
    ll low = 0;
    ll high = mins[2*n-1] - mins[0];
    while(low < high){
        ll mid = low + (high - low) / 2;
        // Check if we can form at least n pairs with difference <=mid
        ll count = 0;
        ll i =0;
        while(i < 2*n -1 && count < n){
            if(mins[i+1] - mins[i] <= mid){
                count +=1;
                i +=2;
            }
            else{
                i +=1;
            }
        }
        if(count >= n){
            high = mid;
        }
        else{
            low = mid +1;
        }
    }
    cout << low;
}
