#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    double w, u, v, t1, t2;
    cin >> n >> w >> u >> v >> t1 >> t2;
    vector<pair<double, double>> forbidden;
    for(int i=1;i<=n;i++){
        char dir;
        int m;
        cin >> dir >> m;
        for(int j=0;j<m;j++){
            double l_j, p_j;
            cin >> l_j >> p_j;
            double s_start, s_end;
            if(dir == 'E'){
                s_start = max(-p_j / u, 0.0);
                s_end = (l_j - p_j) / u;
            }
            else { // 'W'
                s_start = max(p_j / u, 0.0);
                s_end = (p_j + l_j) / u;
            }
            if(s_start > s_end) continue;
            // Compute forbidden t interval
            double forbidden_l = s_start - i * w / v;
            double forbidden_r = s_end - (i-1) * w / v;
            // Intersect with [t1, t2]
            double l = max(forbidden_l, (double)t1);
            double r = min(forbidden_r, (double)t2);
            if(l <= r){
                forbidden.emplace_back(l, r);
            }
        }
    }
    // Sort forbidden intervals
    sort(forbidden.begin(), forbidden.end());
    // Merge forbidden intervals
    vector<pair<double, double>> merged;
    for(auto &[l, r] : forbidden){
        if(merged.empty()){
            merged.emplace_back(l, r);
        }
        else{
            if(l > merged.back().second){
                merged.emplace_back(l, r);
            }
            else{
                merged.back().second = max(merged.back().second, r);
            }
        }
    }
    // Find maximum allowed gap
    double last_end = t1;
    double max_d = 0.0;
    for(auto &[l, r] : merged){
        if(last_end < l){
            max_d = max(max_d, l - last_end);
        }
        last_end = max(last_end, r);
    }
    if(last_end < t2){
        max_d = max(max_d, t2 - last_end);
    }
    // Output with at least 6 decimal places
    cout << fixed << setprecision(10) << max_d;
}
