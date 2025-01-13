#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, m;
    cin >> n >> m;
    long long sum = 0;
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            int x;
            cin >> x;
            sum += x;
        }
    }
    double average = (double)sum / (n * (long long)m);
    cout << fixed << setprecision(9) << average;
}
