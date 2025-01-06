#include <bits/stdc++.h>
using namespace std;

typedef long double ld;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, d, r;
    cin >> n >> d >> r;
    ld F = 0.0;
    for(int t=1; t<=d; t++){
        ld prob = (F + r) / (n + t -1);
        F += prob;
    }
    ld E_sum = F + r;
    cout << fixed << setprecision(10) << E_sum;
}
