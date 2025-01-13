#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int, int> pii;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    // Read first die
    int n_a;
    cin >> n_a;
    vector<int> A(n_a);
    for(auto &x:A) cin>>x;
    // Read second die
    int n_b;
    cin >> n_b;
    vector<int> B(n_b);
    for(auto &x:B) cin>>x;
    // Sort both
    vector<int> A_sorted = A;
    vector<int> B_sorted = B;
    sort(A_sorted.begin(), A_sorted.end());
    sort(B_sorted.begin(), B_sorted.end());
    // Compute score(A,B)
    // Group A_sorted
    vector<pair<int, int>> A_groups;
    int prev = A_sorted[0];
    int cnt =1;
    for(int i=1;i<n_a;i++){
        if(A_sorted[i]==prev){
            cnt++;
        }
        else{
            A_groups.emplace_back(make_pair(prev, cnt));
            prev = A_sorted[i];
            cnt=1;
        }
    }
    A_groups.emplace_back(make_pair(prev, cnt));
    // Compute counts for B_sorted
    double sum_A_B =0.0;
    for(auto &[x, freq]:A_groups){
        int cnt_less = lower_bound(B_sorted.begin(), B_sorted.end(), x) - B_sorted.begin();
        int cnt_eq = upper_bound(B_sorted.begin(), B_sorted.end(), x) - lower_bound(B_sorted.begin(), B_sorted.end(), x);
        sum_A_B += freq * (cnt_less + 0.5 * cnt_eq);
    }
    double score_A_B = sum_A_B / n_a;
    // Assign D1 and D2
    vector<int> D1_sorted, D2_sorted;
    int n1, n2;
    if(score_A_B > 0.5){
        D1_sorted = A_sorted;
        D2_sorted = B_sorted;
        n1 = n_a;
        n2 = n_b;
    }
    else{
        D1_sorted = B_sorted;
        D2_sorted = A_sorted;
        n1 = n_b;
        n2 = n_a;
    }
    // Collect all unique x's
    vector<int> unique_x;
    for(auto x:D1_sorted) unique_x.push_back(x);
    for(auto x:D2_sorted) unique_x.push_back(x);
    sort(unique_x.begin(), unique_x.end());
    unique_x.erase(unique(unique_x.begin(), unique_x.end()), unique_x.end());
    // Compute b(x) and c(x) for each x
    int m = unique_x.size();
    vector<pair<double, double>> bc(m, {0.0, 0.0});
    for(int i=0;i<m;i++){
        int x = unique_x[i];
        // b(x)=score(x,D1)
        int cnt_less = lower_bound(D1_sorted.begin(), D1_sorted.end(), x) - D1_sorted.begin();
        int cnt_eq = upper_bound(D1_sorted.begin(), D1_sorted.end(), x) - lower_bound(D1_sorted.begin(), D1_sorted.end(), x);
        double b = ((double)cnt_less + 0.5 * (double)cnt_eq) / n1;
        // c(x)=score(x,D2)
        int cnt_less_d2 = lower_bound(D2_sorted.begin(), D2_sorted.end(), x) - D2_sorted.begin();
        int cnt_eq_d2 = upper_bound(D2_sorted.begin(), D2_sorted.end(), x) - lower_bound(D2_sorted.begin(), D2_sorted.end(), x);
        double c = ((double)cnt_less_d2 + 0.5 * (double)cnt_eq_d2) / n2;
        bc[i] = {b, c};
    }
    // Sort unique_x in increasing order
    // Already sorted
    // Part1: min c, sum b>=0.5
    // Find x_low: largest x with b(x)<=0.5
    // Find x_high: smallest x with b(x)>=0.5
    int idx_low = -1, idx_high = -1;
    for(int i=0;i<m;i++){
        if(bc[i].first <=0.5){
            idx_low = i;
        }
        else{
            if(idx_high ==-1){
                idx_high =i;
            }
        }
    }
    double c_min;
    if(idx_high ==-1){
        // All b(x)<=0.5, take the last one
        c_min = bc[m-1].second;
    }
    else{
        if(idx_low ==-1){
            // All b(x)>=0.5, take the first one
            c_min = bc[idx_high].second;
        }
        else{
            if(abs(bc[idx_low].first -0.5) <1e-9){
                c_min = bc[idx_low].second;
            }
            else{
                // Mix idx_low and idx_high
                double b_low = bc[idx_low].first;
                double c_low = bc[idx_low].second;
                double b_high = bc[idx_high].first;
                double c_high = bc[idx_high].second;
                double p = (0.5 - b_low)/(b_high - b_low);
                // Clamp p between 0 and1
                p = max(0.0, min(1.0, p));
                c_min = p * c_high + (1.0 - p) * c_low;
            }
        }
    }
    // Part2: max c, score(D2,D3)>=0.5
    // Which means score(D2,D3)= sum p(x) d(x) >=0.5, where d(x)=P(D2>x)+0.5 P(D2=x)
    // Let us compute d(x)=score(D2,D3)
    // To compute d(x), which is score(D2,D3), but we have already defined d(x)=score(D2,D3)
    // However, in our bc list, b(x)=score(D3,D1), c(x)=score(D3,D2)
    // We need to compute d(x)=P(D2>x) +0.5 P(D2=x)
    // To compute d(x), precompute:
    // For D2_sorted
    // Precompute cumulative counts for D2_sorted
    // But to avoid recomputing, let's compute d(x) here
    // Compute d(x)= (n2 - upper_bound(D2_sorted, x)) +0.5 * (upper_bound(D2_sorted, x) - lower_bound(D2_sorted, x))
    // = (n2 - upper_bound) +0.5*(upper_bound - lower_bound)
    // =n2 - upper_bound +0.5 * (upper_bound - lower_bound)
    // =n2 - 0.5 * upper_bound -0.5 * lower_bound
    // Alternatively, compute directly
    // Compute d(x)
    vector<double> d(m, 0.0);
    for(int i=0;i<m;i++){
        int x = unique_x[i];
        int cnt_gt = n2 - upper_bound(D2_sorted.begin(), D2_sorted.end(), x) - (D2_sorted.back()==x ? 0 : 0);
        // Actually, better compute:
        int cnt_gt = n2 - upper_bound(D2_sorted.begin(), D2_sorted.end(), x) + (binary_search(D2_sorted.begin(), D2_sorted.end(), x) ? 0 : 0);
        // To compute correctly:
        int upper = upper_bound(D2_sorted.begin(), D2_sorted.end(), x) - D2_sorted.begin();
        int lower = lower_bound(D2_sorted.begin(), D2_sorted.end(), x) - D2_sorted.begin();
        int cnt_eq = upper - lower;
        int cnt_gt_correct = n2 - upper;
        double d_val = (double)cnt_gt_correct / n2 + 0.5 * ((double)cnt_eq / n2);
        d[i] = d_val;
    }
    // Now, to maximize sum p(x)*b(x)=score(D3,D1) under sum p(x)*d(x)>=0.5 and sum p(x)=1
    // To maximize sum p(x)*b(x), it's equivalent to maximizing score(D3,D1)
    // Assign as much p to x's with high b(x) and high c(x)
    // To maximize, select x's with highest b(x)
    // Implement two-point mixing
    // Find x_low: smallest d(x) >=0.5
    // and x_high: largest d(x) <0.5
    // But to maximize, better find x_high as the x with the highest b(x) where d(x)>=0.5
    // To simplify, sort x's by d(x)
    // Find x_low: the x with the smallest d(x)>=0.5
    // Find x_high: the x with the largest d(x)<0.5
    // Then mix them to achieve sum p*d(x_high) + (1-p)*d(x_low)>=0.5
    // and maximize sum p*b(x_high) + (1-p)*b(x_low)
    // Implement similar to part1
    // Find x_low_part2: the x with the largest d(x)<=0.5
    // Find x_high_part2: the x with the smallest d(x)>=0.5
    int idx_low_p2 = -1, idx_high_p2 = -1;
    for(int i=0;i<m;i++){
        if(d[i] <=0.5){
            idx_low_p2 =i;
        }
        else{
            if(idx_high_p2 ==-1){
                idx_high_p2 =i;
            }
        }
    }
    double c_max;
    if(idx_high_p2 ==-1){
        // All d(x)<=0.5, no solution
        // But problem states one die has advantage, so not possible
        c_max = 0.0;
    }
    else{
        if(idx_low_p2 ==-1){
            // All d(x)>=0.5, take the first one with highest b(x)
            // To maximize c(x), take the x with highest b(x)
            // But to maximize, take the x with highest b(x)
            // Which is the last x with d(x)>=0.5
            // To maximize, take the x with highest b(x)
            // Find the x with the highest b(x) among d(x)>=0.5
            double max_b = -1.0;
            double best_c = -1.0;
            for(int i=0;i<m;i++) if(d[i]>=0.5){
                if(bc[i].first > max_b){
                    max_b = bc[i].first;
                    best_c = bc[i].second;
                }
                else if(bc[i].first == max_b && bc[i].second > best_c){
                    best_c = bc[i].second;
                }
            }
            c_max = best_c;
        }
        else{
            if(abs(d[idx_low_p2] -0.5) <1e-9){
                // c_max = b(x_low_p2)
                c_max = bc[idx_low_p2].first;
            }
            else{
                // Mix idx_low_p2 and idx_high_p2
                double d_low = d[idx_low_p2];
                double b_low = bc[idx_low_p2].first;
                double c_low = bc[idx_low_p2].second;
                double d_high = d[idx_high_p2].first;
                double b_high = bc[idx_high_p2].first;
                double c_high = bc[idx_high_p2].second;
                double p = (0.5 - d_low)/(d_high - d_low);
                p = max(0.0, min(1.0, p));
                double b_mix = p * b_high + (1.0 - p) * b_low;
                double c_mix = p * c_high + (1.0 - p) * c_low;
                c_max = c_mix;
            }
        }
    }
    // Now, output c_min and c_max
    // According to problem statement, output the lowest score(D3,D2) and the highest score(D3,D1)
    // which corresponds to c_min and b_max respectively
    // But according to our implementation, c_max is the maximum score(D3,D1)
    // and c_min is the minimum score(D3,D2)
    // Thus, print c_min and c_max
    cout << fixed << setprecision(9) << c_min << " " << fixed << setprecision(9) << c_max;
}
