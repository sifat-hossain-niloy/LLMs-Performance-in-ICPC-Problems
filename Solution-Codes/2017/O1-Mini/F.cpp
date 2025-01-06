#include <bits/stdc++.h>
using namespace std;

typedef unsigned long long ull;
const ull INF = 1ULL << 60;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int d, k;
    cin >> d >> k;
    vector<int> r(d+1);
    vector<ull> p(d+1);
    for(int i=1;i<=d;i++) {
        cin >> r[i] >> p[i];
    }
    // Compute prefix sums
    vector<ull> S1(d+1, 0);
    vector<ull> S2(d+1, 0);
    vector<ull> S3(d+1, 0);
    for(int i=1;i<=d;i++){
        S1[i] = S1[i-1] + p[i];
        S2[i] = S2[i-1] + p[i] * (ull)r[i];
        S3[i] = S3[i-1] + p[i] * (ull)r[i] * (ull)r[i];
    }
    // Precompute cost[i][j]
    vector<vector<ull>> cost(d+1, vector<ull>(d+1, 0));
    for(int i=1;i<=d;i++){
        for(int j=i;j<=d;j++){
            ull sum_p = S1[j] - S1[i-1];
            ull sum_pr = S2[j] - S2[i-1];
            ull sum_pr2 = S3[j] - S3[i-1];
            // Compute mean
            ull mean_floor = sum_pr / sum_p;
            ull remainder = sum_pr % sum_p;
            vector<int> candidates;
            candidates.push_back((int)mean_floor);
            if(remainder != 0){
                candidates.push_back((int)(mean_floor + 1));
            }
            // Clamp candidates to [0,255]
            for(auto &v: candidates){
                if(v < 0) v = 0;
                if(v > 255) v = 255;
            }
            // Remove duplicates
            sort(candidates.begin(), candidates.end());
            candidates.erase(unique(candidates.begin(), candidates.end()), candidates.end());
            // Compute cost for each candidate
            ull min_cost = INF;
            for(auto v: candidates){
                ull cost_v = sum_pr2;
                cost_v += (ull)v * v * sum_p;
                cost_v -= 2ULL * v * sum_pr;
                if(cost_v < min_cost){
                    min_cost = cost_v;
                }
            }
            cost[i][j] = min_cost;
        }
    }
    // Initialize DP
    // dp_prev[j]: min cost for first j with c-1
    vector<ull> dp_prev(d+1, INF);
    dp_prev[0] = 0;
    // Temporary array
    vector<ull> dp_current(d+1, INF);
    for(int c=1;c<=k;c++){
        fill(dp_current.begin(), dp_current.end(), INF);
        for(int j=1;j<=d;j++){
            // Find min over i from 1 to j of dp_prev[i-1] + cost[i][j]
            // Iterate i from 1 to j
            // To optimize, we can keep track of the minimum
            // But with d=256, just iterate
            ull min_val = INF;
            for(int i=1;i<=j;i++){
                ull temp = dp_prev[i-1] + cost[i][j];
                if(temp < min_val){
                    min_val = temp;
                }
            }
            dp_current[j] = min_val;
        }
        dp_prev = dp_current;
    }
    cout << dp_prev[d] << "\n";
}
