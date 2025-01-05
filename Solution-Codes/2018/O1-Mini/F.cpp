#include <bits/stdc++.h>
using namespace std;

// Sparse Table for Range Maximum Query
struct SparseTable {
    int n;
    int logn;
    vector<vector<int>> st;
    vector<int> log_table;

    SparseTable(const vector<int> &a){
        n = a.size();
        logn = 0;
        while((1 << logn) <= n) logn++;
        st.assign(logn, vector<int>(n, 0));
        for(int i=0; i<n; ++i) st[0][i] = a[i];
        for(int k=1; k<logn; ++k){
            for(int i=0; i + (1 <<k) <=n; ++i){
                st[k][i] = max(st[k-1][i], st[k-1][i + (1 <<(k-1))]);
            }
        }
        log_table.assign(n +1, 0);
        for(int i=2; i<=n; ++i){
            log_table[i] = log_table[i/2] +1;
        }
    }

    int query_max(int l, int r){
        if(l > r) return 0;
        int k = log_table[r - l +1];
        return max(st[k][l], st[k][r - (1 <<k) +1]);
    }
};

struct Arrangement {
    int W;
    vector<pair<int, int>> lines; // pairs of (start_index, end_index)
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    cin >> n;
    vector<string> words;
    words.reserve(n);
    string word;
    while(words.size() < n && cin >> word){
        words.push_back(word);
    }
    // Compute word lengths
    vector<int> word_lengths(n, 0);
    int W_min = 0;
    for(int i=0; i<n; ++i){
        word_lengths[i] = words[i].size();
        if(word_lengths[i] > W_min){
            W_min = word_lengths[i];
        }
    }
    // Compute prefix sums
    vector<long long> prefix_sum(n+1, 0);
    for(int i=0; i<n; ++i){
        prefix_sum[i+1] = prefix_sum[i] + word_lengths[i];
    }
    // Enumerate all arrangements
    vector<Arrangement> arrangements;
    int W = W_min;
    while(true){
        // Greedy line wrapping for current W
        Arrangement arr;
        arr.W = W;
        int line_start = 0;
        while(line_start < n){
            int line_end = line_start;
            long long current_length = word_lengths[line_start];
            while(line_end +1 < n && current_length + 1 + word_lengths[line_end +1] <= W){
                current_length += 1 + word_lengths[line_end +1];
                line_end +=1;
            }
            arr.lines.emplace_back(line_start, line_end);
            line_start = line_end +1;
        }
        arrangements.push_back(arr);
        // Find the next W'
        long long W_next = LLONG_MAX;
        for(auto &line : arr.lines){
            int s = line.first;
            int e = line.second;
            if(e +1 < n){
                // Compute W'_line
                long long sum_words = prefix_sum[e +1] - prefix_sum[s];
                int k = e - s +1;
                long long next_word_length = word_lengths[e +1];
                long long W_line = sum_words + next_word_length + k;
                if(W_line > W){
                    W_next = min(W_next, (long long)W_line);
                }
            }
        }
        if(W_next == LLONG_MAX){
            break;
        }
        W = (int)W_next;
    }
    // Now, for each arrangement, compute the river length
    int max_river_length = 0;
    int best_W = 0;
    for(auto &arr : arrangements){
        // Compute space positions per line
        vector<vector<int>> space_positions_per_line;
        space_positions_per_line.reserve(arr.lines.size());
        for(auto &line : arr.lines){
            int s = line.first;
            int e = line.second;
            vector<int> spaces;
            spaces.reserve(e - s);
            int pos =0;
            for(int i=s; i<=e; ++i){
                pos += word_lengths[i];
                if(i != e){
                    pos +=1; // space
                    spaces.push_back(pos);
                }
            }
            sort(spaces.begin(), spaces.end());
            space_positions_per_line.push_back(spaces);
        }
        // Compute river length via DP
        if(space_positions_per_line.empty()){
            continue;
        }
        // Initialize for the first line
        vector<int> prev_line_spaces = space_positions_per_line[0];
        if(prev_line_spaces.empty()){
            continue;
        }
        vector<int> dp_prev(prev_line_spaces.size(), 1);
        // Initialize max river length
        int current_max_river = 1;
        // Iterate through subsequent lines
        for(int i=1; i<space_positions_per_line.size(); ++i){
            vector<int> current_line_spaces = space_positions_per_line[i];
            if(current_line_spaces.empty()){
                // No spaces in current line, river cannot continue
                continue;
            }
            // Sort previous line's spaces
            vector<int> p_prev_sorted = prev_line_spaces;
            sort(p_prev_sorted.begin(), p_prev_sorted.end());
            // Sort dp_prev accordingly
            vector<int> dp_prev_sorted;
            dp_prev_sorted.reserve(p_prev_sorted.size());
            // Create a map from position to dp_prev
            // Since p_prev_sorted is sorted, and prev_line_spaces may not be
            // We need to align dp_prev_sorted with p_prev_sorted
            // Create a vector of pairs and sort them
            vector<pair<int, int>> pairs;
            pairs.reserve(prev_line_spaces.size());
            for(int j=0; j<prev_line_spaces.size(); ++j){
                pairs.emplace_back(prev_line_spaces[j], dp_prev[j]);
            }
            sort(pairs.begin(), pairs.end());
            for(auto &p : pairs){
                dp_prev_sorted.push_back(p.second);
            }
            // Build Sparse Table
            SparseTable st_prev(dp_prev_sorted);
            // Now, for each pos in current_line_spaces, find max_p
            vector<int> dp_current;
            dp_current.reserve(current_line_spaces.size());
            for(auto pos : current_line_spaces){
                int l_pos = pos -1;
                int r_pos = pos +1;
                // Find the range [l, r] in p_prev_sorted
                int l = lower_bound(p_prev_sorted.begin(), p_prev_sorted.end(), l_pos) - p_prev_sorted.begin();
                int r_bound = upper_bound(p_prev_sorted.begin(), p_prev_sorted.end(), r_pos) - p_prev_sorted.begin() -1;
                if(l <= r_bound){
                    int max_p = st_prev.query_max(l, r_bound);
                    dp_current.push_back(max_p +1);
                    if(max_p +1 > current_max_river){
                        current_max_river = max_p +1;
                    }
                }
                else{
                    dp_current.push_back(1);
                }
            }
            // Update for next iteration
            prev_line_spaces = current_line_spaces;
            dp_prev = dp_current;
        }
        // Update the maximum river length and corresponding W
        if(current_max_river > max_river_length){
            max_river_length = current_max_river;
            best_W = arr.W;
        }
        else if(current_max_river == max_river_length){
            if(arr.W < best_W){
                best_W = arr.W;
            }
        }
    }
    cout << best_W << " " << max_river_length;
}
