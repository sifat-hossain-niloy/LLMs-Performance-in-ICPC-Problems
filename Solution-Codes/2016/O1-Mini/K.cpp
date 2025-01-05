#include <bits/stdc++.h>
using namespace std;

int main(){
    int n;
    cin >> n;
    vector<int> a(n);
    for(auto &x: a) cin >> x;
    long long sum_quotes =0;
    for(int i=0;i<n;i+=2){
        sum_quotes += a[i];
    }
    // Find maximum k such that k*(k+1) <= sum_quotes
    int max_k = 0;
    for(int k=1; k<=100; k++){
        if(1LL *k * (k+1) <= sum_quotes){
            max_k = k;
        }
        else{
            break;
        }
    }
    if(max_k >=1){
        cout << max_k;
    }
    else{
        cout << "no quotation";
    }
}
