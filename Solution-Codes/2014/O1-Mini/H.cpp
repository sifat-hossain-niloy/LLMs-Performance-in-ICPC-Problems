#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    int w, h;
    cin >> w >> h;
    
    int u, d, l, r;
    cin >> u >> d >> l >> r;
    double prob_u = u / 100.0;
    double prob_d_p = d / 100.0;
    double prob_l_p = l / 100.0;
    double prob_r_p = r / 100.0;
    
    vector<string> grid(h);
    vector<int> target_indices;
    vector<int> launch_cells;
    
    // To map cell index to target index
    // Initialize later after reading all targets
    // Temporarily store target cells
    vector<pair<int, int>> target_cells_order; // row, col
    
    for(int row=0; row<h; row++){
        cin >> grid[row];
        for(int col=0; col<w; col++){
            int cell = row * w + col;
            if(grid[row][col] == 'T'){
                target_cells_order.emplace_back(row, col);
                target_indices.push_back(cell);
            }
            if(row == 0 && grid[row][col] == '.'){
                launch_cells.push_back(cell);
            }
        }
    }
    
    int t = target_indices.size();
    int n = w * h;
    
    // Map cell index to target index
    // If not a target, set to -1
    vector<int> cell_to_target(n, -1);
    for(int ti=0; ti<t; ti++){
        cell_to_target[target_indices[ti]] = ti;
    }
    
    // Precompute S_i and transitions
    vector<double> S(n, 0.0);
    vector<vector<pair<int, double>>> transitions(n, vector<pair<int, double>>());
    // For each target, store list of (i, prob_d)
    vector<vector<pair<int, double>>> target_to_cells(t, vector<pair<int, double>>());
    
    // Directions: up, down, left, right
    vector<pair<int, double>> directions = {
        {-1, prob_u},
        {1, prob_d_p},
        {0, prob_l_p},
        {0, prob_r_p}
    };
    
    for(int row=0; row<h; row++){
        for(int col=0; col<w; col++){
            int i = row * w + col;
            for(int dir=0; dir<4; dir++){
                int dr = 0, dc = 0;
                double prob_d_val = 0.0;
                if(dir == 0){
                    dr = -1;
                    dc = 0;
                    prob_d_val = prob_u;
                }
                else if(dir == 1){
                    dr = 1;
                    dc = 0;
                    prob_d_val = prob_d_p;
                }
                else if(dir == 2){
                    dr = 0;
                    dc = -1;
                    prob_d_val = prob_l_p;
                }
                else{
                    dr = 0;
                    dc = 1;
                    prob_d_val = prob_r_p;
                }
                int new_row = row + dr;
                int new_col = col + dc;
                if(new_row < 0 || new_row >= h || new_col < 0 || new_col >= w){
                    S[i] += prob_d_val;
                }
                else{
                    int j = new_row * w + new_col;
                    if(grid[new_row][new_col] == 'X'){
                        S[i] += prob_d_val;
                    }
                    else if(grid[new_row][new_col] == 'T'){
                        int t_prime = cell_to_target[j];
                        if(t_prime != -1){
                            target_to_cells[t_prime].emplace_back(i, prob_d_val);
                        }
                        else{
                            // This should not happen
                            S[i] += prob_d_val;
                        }
                    }
                    else{
                        transitions[i].emplace_back(j, prob_d_val);
                    }
                }
            }
        }
    }
    
    // Precompute A[t] as list of (i, sum prob_d)
    vector<vector<pair<int, double>>> A(t, vector<pair<int, double>>());
    for(int ti=0; ti<t; ti++){
        // Sort target_to_cells[ti] by cell index
        vector<pair<int, double>> &lst = target_to_cells[ti];
        if(lst.empty()){
            continue;
        }
        sort(lst.begin(), lst.end(), [&](const pair<int, double> &a, const pair<int, double> &b) -> bool{
            return a.first < b.first;
        });
        // Sum prob_d for same i
        int current_i = lst[0].first;
        double sum_prob = lst[0].second;
        for(int k=1; k<lst.size(); k++){
            if(lst[k].first == current_i){
                sum_prob += lst[k].second;
            }
            else{
                A[ti].emplace_back(current_i, sum_prob);
                current_i = lst[k].first;
                sum_prob = lst[k].second;
            }
        }
        A[ti].emplace_back(current_i, sum_prob);
    }
    
    // Prepare list of non-target cells
    vector<int> non_target_cells;
    non_target_cells.reserve(n);
    for(int i=0; i<n; i++){
        if(cell_to_target[i] == -1 && grid[i / w][i % w] != 'T'){
            non_target_cells.push_back(i);
        }
    }
    
    // Initialize P array
    vector<double> P(n, 0.0);
    
    // For each target, perform iterative computation
    // To speed up, reuse P array
    // Initialize a temporary P_new array
    vector<double> P_new(n, 0.0);
    
    // Number of launch cells
    int num_launch = launch_cells.size();
    
    // To output in the correct order, sort targets by their position
    // They are already in top-to-bottom, left-to-right order based on input reading
    for(int ti=0; ti<t; ti++){
        // Initialize P
        fill(P.begin(), P.end(), 0.0);
        int target_cell = target_indices[ti];
        P[target_cell] = 1.0;
        // All other targets have P=0
        // No need to set them explicitly since P is initialized to 0.0
        // Iterative computation
        int iter = 0;
        double max_diff = 1.0;
        while(max_diff >= 1e-8 && iter < 1000){
            max_diff = 0.0;
            // Iterate over non-target cells
            for(auto &i : non_target_cells){
                double sum = 0.0;
                // Add A_i[t]
                // Find if A[ti] has i
                // Binary search since A[ti] is sorted
                // But since A[ti] is sorted, use two pointers
                // Implemented as linear scan
                // Use lower_bound
                // Implement binary search
                double A_i_t = 0.0;
                int left = 0, right = A[ti].size();
                while(left < right){
                    int mid = left + (right - left) / 2;
                    if(A[ti][mid].first == i){
                        A_i_t = A[ti][mid].second;
                        break;
                    }
                    else if(A[ti][mid].first < i){
                        left = mid + 1;
                    }
                    else{
                        right = mid;
                    }
                }
                sum += A_i_t;
                // Add transitions
                for(auto &[j, prob_d_val] : transitions[i]){
                    sum += prob_d_val * P[j];
                }
                // Compute new P[i]
                double denom = 1.0 - S[i];
                if(denom == 0.0){
                    // Prevent division by zero, set P[i] unchanged
                    continue;
                }
                double new_P = sum / denom;
                double diff = abs(new_P - P[i]);
                if(diff > max_diff){
                    max_diff = diff;
                }
                P_new[i] = new_P;
            }
            // Update P with P_new
            for(auto &i : non_target_cells){
                P[i] = P_new[i];
            }
            iter++;
        }
        // After convergence, sum P over launch cells
        double sum_P = 0.0;
        for(auto &i : launch_cells){
            sum_P += P[i];
        }
        double prob_t = sum_P / num_launch;
        // Output with 9 decimal places
        cout << fixed << setprecision(9) << prob_t << "\n";
    }
}
