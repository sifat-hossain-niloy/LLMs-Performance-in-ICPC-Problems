#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAX_A = 500000;

// Function to compute Euler's Totient function up to n
vector<int> compute_phi(int n){
    vector<int> phi(n+1);
    for(int i=0;i<=n;i++) phi[i]=i;
    for(int p=2;p<=n;p++){
        if(phi[p]==p){
            for(int multiple=p; multiple<=n; multiple+=p){
                phi[multiple] -= phi[multiple]/p;
            }
        }
    }
    return phi;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    // Compute phi
    vector<int> phi = compute_phi(MAX_A);
    
    // Compute w(a)
    // w(a) = sum_{d |a} phi(d) * (a/d)
    // Initialize w(a)
    vector<long long> w(MAX_A+1, 0LL);
    
    // For each d, add phi(d)*(a/d) to w(a) for all a multiple of d
    for(int d=1; d<=MAX_A; d++){
        for(int a=d; a<=MAX_A; a+=d){
            w[a] += (ll)phi[d] * (a / d);
        }
    }
    
    // Read test cases
    int n;
    cin >> n;
    while(n--){
        int alo, ahi;
        cin >> alo >> ahi;
        // Find a in [alo, ahi] with maximum w(a), smallest a if tie
        int best_a = alo;
        ll best_w = w[alo];
        for(int a=alo+1; a<=ahi; a++){
            if(w[a] > best_w){
                best_w = w[a];
                best_a = a;
            }
        }
        cout << best_a << " " << best_w << "\n";
    }
}
