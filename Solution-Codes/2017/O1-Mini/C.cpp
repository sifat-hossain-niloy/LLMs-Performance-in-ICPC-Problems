#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int r, c;
    cin >> r >> c;
    vector<vector<ll>> h(r, vector<ll>(c, 0));
    ll original_sum =0;
    for(int i=0;i<r;i++) {
        for(int j=0;j<c;j++) {
            cin >> h[i][j];
            original_sum += h[i][j];
        }
    }
    // Compute row_max and col_max
    vector<ll> row_max(r, 0);
    vector<ll> col_max(c, 0);
    for(int i=0;i<r;i++) {
        for(int j=0;j<c;j++) {
            row_max[i] = max(row_max[i], h[i][j]);
            col_max[j] = max(col_max[j], h[i][j]);
        }
    }
    // Compute sum_min and overlaps
    ll sum_min=0;
    int overlaps=0;
    for(int i=0;i<r;i++) {
        for(int j=0;j<c;j++) {
            if(h[i][j] >0){
                ll mn = min(row_max[i], col_max[j]);
                sum_min += mn;
                if(mn == row_max[i] && mn == col_max[j]){
                    overlaps++;
                }
            }
        }
    }
    // Compute stolen = original_sum - (sum_min - overlaps)
    ll stolen = original_sum - (sum_min - overlaps);
    cout << stolen;
}
