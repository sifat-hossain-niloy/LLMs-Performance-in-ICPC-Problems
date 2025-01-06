#include <bits/stdc++.h>
using namespace std;

// Function to perform convolution with K (3x3 all-ones kernel) modulo 2
vector<bitset<305>> convolve(const vector<bitset<305>> &P, int W, int H) {
    vector<bitset<305>> P_next(H, bitset<305>());
    for(int i = 0; i < H; ++i){
        bitset<305> above = (i > 0) ? P[i-1] : bitset<305>();
        bitset<305> current = P[i];
        bitset<305> below = (i < H-1) ? P[i+1] : bitset<305>();
        
        // Shift left, no shift, shift right for above
        bitset<305> sum = (above << 1) ^ above ^ (above >> 1);
        // Shift left, no shift, shift right for current
        sum ^= (current << 1) ^ current ^ (current >> 1);
        // Shift left, no shift, shift right for below
        sum ^= (below << 1) ^ below ^ (below >> 1);
        
        // Only keep W bits
        for(int j = 0; j < W; ++j){
            P_next[i][j] = sum[j];
        }
    }
    return P_next;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int W, H;
    cin >> W >> H;
    vector<string> S_str(H);
    for(auto &s: S_str) cin >> s;
    // Convert S to bitset grid
    vector<bitset<305>> S(H, bitset<305>());
    for(int i = 0; i < H; ++i){
        for(int j = 0; j < W; ++j){
            if(S_str[i][j] == '#') S[i].set(j);
        }
    }
    // Iterate over possible P0 sizes, starting from minimal area
    for(int area = 1; area <= 16; ++area){ // up to 4x4
        for(int h0 = 1; h0 <= min(area, 10); ++h0){
            if(area % h0 != 0) continue;
            int w0 = area / h0;
            if(w0 > W || h0 > H) continue;
            // Iterate over all possible P0 patterns
            int total_patterns = 1 << (w0 * h0);
            for(int mask = 1; mask < total_patterns; ++mask){ // non-empty
                // Build P0 pattern
                vector<string> P0_pattern(h0, string(w0, '.'));
                for(int bit = 0; bit < w0 * h0; ++bit){
                    if(mask & (1 << bit)){
                        int row = bit / w0;
                        int col = bit % w0;
                        P0_pattern[row][col] = '#';
                    }
                }
                // Compute t
                int t_w = (W - w0 + 1) / 2;
                int t_h = (H - h0 + 1) / 2;
                int t = max(t_w, t_h);
                // Check if t is non-negative
                if(t < 0) continue;
                // Check if P0 can fit after t steps
                if(w0 + 2*t < W || h0 + 2*t < H) continue;
                // Initialize P grid
                vector<bitset<305>> P(H, bitset<305>());
                // Place P0 at (t, t)
                for(int i = 0; i < h0; ++i){
                    if(t + i >= H) break;
                    for(int j = 0; j < w0; ++j){
                        if(t + j >= W) break;
                        if(P0_pattern[i][j] == '#') P[t + i].set(t + j);
                    }
                }
                // Simulate t steps
                bool failed = false;
                vector<bitset<305>> P_current = P;
                for(int step = 1; step <= t; ++step){
                    // Convolve
                    vector<bitset<305>> P_next = convolve(P_current, W, H);
                    // Compare with S
                    bool equal = true;
                    int diff_count = 0;
                    pair<int, int> flip_pos = {-1, -1};
                    for(int i = 0; i < H; ++i){
                        bitset<305> diff = P_next[i] ^ S[i];
                        if(diff.any()){
                            equal = false;
                            // Find first differing bit
                            int pos = diff._Find_first();
                            if(pos < W){
                                flip_pos = {i, pos};
                                break;
                            }
                        }
                    }
                    if(!equal){
                        if(flip_pos.first != -1){
                            // Flip the cell
                            P_next[flip_pos.first].flip(flip_pos.second);
                        }
                        else{
                            failed = true;
                            break;
                        }
                    }
                    P_current = P_next;
                }
                if(!failed){
                    // After t steps, check if P_current equals S
                    bool match = true;
                    for(int i = 0; i < H; ++i){
                        if(P_current[i] != S[i]){
                            match = false;
                            break;
                        }
                    }
                    if(match){
                        // Extract minimal bounding box of P0
                        int min_r = h0, max_r = -1, min_c = w0, max_c = -1;
                        for(int i = 0; i < h0; ++i){
                            for(int j = 0; j < w0; ++j){
                                if(P0_pattern[i][j] == '#'){
                                    min_r = min(min_r, i);
                                    max_r = max(max_r, i);
                                    min_c = min(min_c, j);
                                    max_c = max(max_c, j);
                                }
                            }
                        }
                        if(max_r == -1) continue; // no filled cells
                        // Extract the minimal pattern
                        int final_h = max_r - min_r + 1;
                        int final_w = max_c - min_c + 1;
                        vector<string> final_P0(final_h, string(final_w, '.'));
                        for(int i = min_r; i <= max_r; ++i){
                            for(int j = min_c; j <= max_c; ++j){
                                final_P0[i - min_r][j - min_c] = P0_pattern[i][j];
                            }
                        }
                        // Output the pattern
                        for(auto &row: final_P0) cout << row << "\n";
                        return 0;
                    }
                }
            }
        }
    }
    // If no pattern found, output S itself
    // (Though problem states there is at least one filled cell in borders)
    for(int i = 0; i < H; ++i){
        for(int j = 0; j < W; ++j){
            cout << (S[i][j] ? '#' : '.');
        }
        cout << "\n";
    }
}
