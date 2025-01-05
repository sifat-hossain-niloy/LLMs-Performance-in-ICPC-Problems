#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    ll m, k;
    cin >> m >> k;
    vector<ll> a(m);
    ll A=0;
    for(auto &x: a){
        cin >> x;
        A +=x;
    }
    vector<ll> s_i0(m, 0);
    for(ll i=0;i<k;i++){
        ll b;
        cin >> b;
        s_i0[b-1]++;
    }
    // Binary search for t
    ll left=0, right=100000000000000LL; //1e14
    // Function to check if a given t is possible
    auto check = [&](ll t) -> bool {
        ll n = k + t;
        ll sum_low=0, sum_high=0;
        for(ll i=0;i<m;i++){
            ll ai = a[i];
            ll si0 = s_i0[i];
            // Compute floor((n ai -1)/A) +1
            ll floor_val = (n * ai -1)/A;
            ll c_low = max(floor_val +1, si0);
            // Compute floor(n ai /A) +1
            ll floor_hi = (n * ai)/A +1;
            ll c_high = min(floor_hi, si0 + t);
            sum_low += c_low;
            sum_high += c_high;
            if(sum_low > n) return false; // Early termination
        }
        return (sum_low <=n) && (sum_high >=n);
    };
    while(left < right){
        ll mid = left + (right - left +1)/2;
        if(check(mid)){
            left = mid;
        }
        else{
            right = mid -1;
        }
    }
    if(left >=100000000000000LL){
        cout << "forever";
    }
    else{
        // Check if t=left+1 also possible, to see if "forever" is possible
        // But in our binary search, left is the maximum t where check(t) is true
        // Now, check if t_max can be increased indefinitely
        // If check(left+1) is also true up to the upper limit, then "forever"
        // But to simplify, if left reached the upper limit, output "forever"
        cout << left;
    }
}
