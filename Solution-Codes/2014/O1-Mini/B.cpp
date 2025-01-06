#include <bits/stdc++.h>
using namespace std;

struct DiscreteDish {
    int wi;
    int ti;
    int deltai;
};

struct ContinuousDish {
    int tj;
    int deltaj;
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int d, w;
    cin >> d >> w;
    vector<DiscreteDish> D;
    vector<ContinuousDish> C_all;
    bool has_continuous_zero = false;
    double t_max = 0.0;
    for(int i=0;i<d;i++){
        char type;
        cin >> type;
        if(type == 'D'){
            DiscreteDish dish;
            cin >> dish.wi >> dish.ti >> dish.deltai;
            D.push_back(dish);
        }
        else if(type == 'C'){
            ContinuousDish dish;
            cin >> dish.tj >> dish.deltaj;
            if(dish.deltaj ==0){
                has_continuous_zero = true;
                t_max = max(t_max, (double)dish.tj);
            }
            else{
                C_all.push_back(dish);
            }
        }
    }
    // Initialize DP
    // dp_discrete[S] = max discrete tastiness for weight S
    // Initialize with -inf
    vector<double> dp(w+1, -1e18);
    dp[0] = 0.0;
    // Process discrete dishes
    for(auto &dish: D){
        if(dish.deltai ==0){
            // Unbounded knapsack
            for(int S = dish.wi; S <=w; S++){
                if(dp[S - dish.wi] + dish.ti > dp[S]){
                    dp[S] = dp[S - dish.wi] + (double)dish.ti;
                }
            }
        }
        else{
            // Bounded knapsack using binary splitting
            int max_count = min((int)(dish.ti / dish.deltai +1), (w / dish.wi));
            if(max_count <=0) continue;
            vector<pair<int, double>> batches;
            int count = max_count;
            int power =1;
            while(count >0){
                int current = min(power, count);
                batches.emplace_back(current, 0.0); // Placeholder for tastiness
                count -=current;
                power <<=1;
            }
            // Now, for each batch, compute weight and tastiness
            vector<pair<int, double>> actual_batches;
            for(auto &[c, _]: batches){
                int weight = c * dish.wi;
                double tastiness = (double)(c * dish.ti) - 0.5 * (double)(dish.deltai) * (c) * (c -1);
                actual_batches.emplace_back(weight, tastiness);
            }
            // Now, perform 0-1 knapsack for these batches
            for(auto &[weight, tastiness]: actual_batches){
                for(int S =w; S >=weight; S--){
                    if(dp[S - weight] + tastiness > dp[S]){
                        dp[S] = dp[S - weight] + tastiness;
                    }
                }
            }
        }
    }
    // Now, process continuous dishes
    double max_continuous_tastiness[w+1];
    memset(max_continuous_tastiness, 0, sizeof(max_continuous_tastiness));
    if(has_continuous_zero){
        // Find the maximum t_j among dishes with deltaj=0
        double t_max_val =0.0;
        for(auto &dish: C_all){
            // Only deltaj >0 are in C_all
            // So, no need to process here
        }
        // Now, find all continuous dishes with deltaj=0
        // Re-collect them
        // Since C_all only has deltaj >0, we need to collect them separately
        // Thus, iterate through all dishes again
        // But to save time, let's iterate through C_all and D
        // But D has discrete dishes
        // So we need to have stored the deltaj=0 dishes separately
        // To simplify, assume C_all only has deltaj >0
        // Thus, we need to collect deltaj=0 dishes separately
        // Since we already have t_max from deltaj=0
        // So, implement:
        for(int X=0; X<=w; X++){
            max_continuous_tastiness[X] = t_max * (double)X;
        }
    }
    else{
        if(!C_all.empty()){
            // Sort continuous dishes in decreasing order of t_j / deltaj
            sort(C_all.begin(), C_all.end(), [&](const ContinuousDish &a, const ContinuousDish &b) -> bool{
                double ratio_a = (double)a.tj / (double)a.deltaj;
                double ratio_b = (double)b.tj / (double)b.deltaj;
                return ratio_a > ratio_b;
            });
            int C = C_all.size();
            // Compute cumulative sums
            vector<double> sum_t_over_deltaj(C+1, 0.0);
            vector<double> sum_t2_over_deltaj(C+1, 0.0);
            vector<double> sum_1_over_deltaj(C+1, 0.0);
            for(int k=1; k<=C; k++){
                sum_t_over_deltaj[k] = sum_t_over_deltaj[k-1] + (double)C_all[k-1].tj / (double)C_all[k-1].deltaj;
                sum_t2_over_deltaj[k] = sum_t2_over_deltaj[k-1] + ((double)C_all[k-1].tj * (double)C_all[k-1].tj) / (double)C_all[k-1].deltaj;
                sum_1_over_deltaj[k] = sum_1_over_deltaj[k-1] + 1.0 / (double)C_all[k-1].deltaj;
            }
            // Compute X[k} = sum_t_over_deltaj[k} - t_j[k} * sum_1_over_deltaj[k}
            vector<pair<double, int>> breakpoints;
            for(int k=1; k<=C; k++){
                double Xk = sum_t_over_deltaj[k] - ((double)C_all[k-1].tj) * sum_1_over_deltaj[k];
                breakpoints.emplace_back(Xk, k);
            }
            // Sort breakpoints in increasing order
            sort(breakpoints.begin(), breakpoints.end(), [&](const pair<double,int> &a, const pair<double,int> &b) -> bool{
                return a.first < b.first;
            });
            // Precompute sum_j t_j / deltaj up to k
            double total_t_over_deltaj = sum_t_over_deltaj[C];
            // Precompute list of k's sorted by Xk
            // Now, for each X from0 tow, find the largest k where X >=Xk
            int current =0;
            for(int X=0; X<=w; X++){
                while(current <breakpoints.size() && X >= breakpoints[current].first){
                    current++;
                }
                int k_eff = current -1;
                if(k_eff >=0){
                    int k = breakpoints[k_eff].second;
                    if(X <= sum_t_over_deltaj[k]){
                        double sum_t2 = sum_t2_over_deltaj[k];
                        double sum_t = sum_t_over_deltaj[k];
                        double sum_1 = sum_1_over_deltaj[k];
                        double val = 0.5 * sum_t2 - 0.5 * ((sum_t - (double)X) * (sum_t - (double)X)) / sum_1;
                        max_continuous_tastiness[X] = val;
                    }
                    else{
                        max_continuous_tastiness[X] =0.0;
                    }
                }
                else{
                    max_continuous_tastiness[X] =0.0;
                }
            }
        }
    }
    // Now, iterate over all possible S
    double answer = -1e18;
    for(int S=0; S<=w; S++){
        if(dp[S] < -1e12) continue;
        int X = w - S;
        if(X <0) continue;
        double cont_tastiness =0.0;
        if(has_continuous_zero){
            cont_tastiness = t_max * (double)X;
        }
        else{
            if(!C_all.empty()){
                // Find the max_continuous_tastiness[X}
                // Already precomputed
                if(X <=w){
                    cont_tastiness = max_continuous_tastiness[X];
                }
                else{
                    cont_tastiness =0.0;
                }
            }
            else{
                if(X ==0){
                    cont_tastiness =0.0;
                }
                else{
                    // Impossible to assign X
                    continue;
                }
            }
        }
        double total = dp[S] + cont_tastiness;
        answer = max(answer, total);
    }
    // Check if answer is valid
    if(has_continuous_zero || !C_all.empty()){
        // If there are continuous dishes, it's always possible to assign X=w-S >=0
        // But max_continuous_tastiness[X} might be 0
        // Thus, answer could be >=0
        // To ensure that, check if answer was updated
        if(answer > -1e12){
            printf("%.6lf\n", answer);
            return 0;
        }
    }
    else{
        // No continuous dishes, need to have S=w
        if(dp[w] > -1e12){
            printf("%.6lf\n", dp[w]);
            return 0;
        }
    }
    // If not possible
    printf("impossible\n");
}
