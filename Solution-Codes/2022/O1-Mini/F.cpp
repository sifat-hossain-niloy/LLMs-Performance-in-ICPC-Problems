#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    long long s, c;
    cin >> s >> c;
    // Compute R_max
    long long R_max = (c >=4) ? (c -4)/2 : 0;
    // L_used = R +4
    long long R_used = R_max;
    long long L_used = R_used +4;
    // c_used =2R +4
    long long c_used = 2*R_used +4;
    // S_max = floor(s /2)*2
    long long S_max = (s /2)*2;
    // n_max = c_used + S_max
    long long n_max = c_used + S_max;
    // number of gaps
    long long gaps = R_used +1;
    // L_per_gap and extra_L
    long long L_per_gap = L_used / gaps;
    long long extra_L = L_used % gaps;
    // S_per_gap and extra_S
    long long S_per_gap = (gaps >0) ? (S_max / gaps) : 0;
    long long extra_S = (gaps >0) ? (S_max % gaps) : 0;
    // Build the string
    string res;
    res.reserve(n_max);
    for(long long i=0;i<gaps;i++){
        long long num_L = L_per_gap + (i < extra_L ? 1 :0);
        long long num_S = S_per_gap + (i < extra_S ? 1 :0);
        if(num_L >0){
            res += string(num_L, 'L');
        }
        if(num_S >0){
            res += string(num_S, 'S');
        }
        if(i < R_used){
            res += 'R';
        }
    }
    cout << res;
}
