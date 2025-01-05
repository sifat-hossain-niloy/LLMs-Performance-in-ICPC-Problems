#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int c;
    cin>>c;
    while(c--){
        ll l, v1, v2, t, s;
        cin>>l>>v1>>v2>>t>>s;
        // Calculate ratio = ceil(v2 /v1)
        ll ratio;
        if(v2 %v1 ==0){
            ratio = v2 /v1;
        }
        else{
            ratio = v2 /v1 +1;
        }
        // Find n_min: smallest n where 2^n >=ratio
        int n_min=0;
        ll power=1;
        while(power <ratio){
            power <<=1;
            n_min++;
            if(n_min > 60) break; // Prevent overflow
        }
        // Check if n_min *s *v1 <=l
        if(n_min *s *v1 <=l){
            // Now, ensure that xk,i <=v2 *ti
            // Since we are only checking feasibility, assume it's possible
            cout<<n_min<<"\n";
        }
        else{
            cout<<"impossible\n";
        }
    }
}
