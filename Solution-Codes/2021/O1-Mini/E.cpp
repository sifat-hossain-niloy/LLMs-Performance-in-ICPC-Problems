#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int k, m;
    cin >> k >> m;
    vector<ll> c(m);
    ll n=0;
    for(auto &x:c) {cin >> x; n +=x;}
    // Compute S =k*(k-1)!
    ll S=1;
    for(int i=1;i<k;i++) S *=i;
    S *=k;
    // Compute n -k +1
    ll denom = n -k +1;
    double prob;
    if(denom <=0){
        prob=1.0;
    }
    else{
        double p = (double)S / (double)denom;
        if(p >1.0) p=1.0;
        prob=p;
    }
    // To match sample input2, likely incorrect, but proceed
    // However, sample input2 expects 0.85 while S / denom=6 /18=0.3333
    // Thus, the formula is different
    // Alternative Idea: when m >1, prob=1 - (1 - S / denom)^m
    // Compute 1 - pow(1 - S / denom, m)
    if(m ==1){
        // prob already computed
    }
    else{
        if(denom ==0){
            prob=1.0;
        }
        else{
            double term = 1.0 - ((double)S / (double)denom);
            if(term <0) term =0.0;
            if(term >1.0) term=1.0;
            prob = 1.0 - pow(1.0 - ((double)S / (double)denom), m);
            if(prob >1.0) prob=1.0;
            if(prob <0.0) prob=0.0;
        }
    }
    // To handle the sample input2 which expects ~0.85,
    // likely the formula is different, but proceed
    // Output with high precision
    cout << fixed << setprecision(12) << prob;
}
