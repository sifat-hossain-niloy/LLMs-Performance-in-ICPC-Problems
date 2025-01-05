#include <bits/stdc++.h>
using namespace std;

// Function to trim leading and trailing spaces
string trim(const string& s) {
    size_t start = s.find_first_not_of(" ");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" ");
    return s.substr(start, end - start +1);
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    int case_num =1;
    while(cin >> n){
        // Read n stacks
        vector<vector<int>> stacks(n, vector<int>());
        vector<int> all_plates;
        for(int i=0;i<n;i++){
            int h;
            cin >> h;
            stacks[i].resize(h);
            for(int j=0;j<h;j++) cin >> stacks[i][j];
            for(int j=0;j<h;j++) all_plates.push_back(stacks[i][j]);
        }
        // Sort all plates in non-decreasing order
        vector<int> S = all_plates;
        sort(S.begin(), S.end());
        int m = S.size();
        // Generate all possible blocks and map them by first plate
        // Using map from first plate to list of blocks
        unordered_map<int, vector<vector<int>>> blocks_map;
        for(int i=0;i<n;i++){
            int h = stacks[i].size();
            for(int start=0; start <h; start++){
                vector<int> block;
                for(int end=start; end <h; end++){
                    block.push_back(stacks[i][end]);
                    blocks_map[block[0]].push_back(block);
                }
            }
        }
        // Initialize DP
        const int INF = 1e9;
        vector<int> dp(m+1, INF);
        dp[0] =0;
        for(int i=1;i<=m;i++){
            // Current plate to consider is S[i-1]
            // We need to find blocks that end at position i
            // Iterate j from 0 to i-1
            for(int j=0; j <i; j++){
                // The block we need is S[j] to S[i-1]
                // Check if there exists a block starting with S[j]
                if(blocks_map.find(S[j]) != blocks_map.end()){
                    // Iterate through all blocks starting with S[j]
                    for(auto &block: blocks_map[S[j]]){
                        int block_size = block.size();
                        if(block_size == (i -j)){
                            bool match = true;
                            for(int t=0; t <block_size; t++){
                                if(S[j + t] != block[t]){
                                    match = false;
                                    break;
                                }
                            }
                            if(match){
                                dp[i] = min(dp[i], dp[j] +1);
                            }
                        }
                    }
                }
            }
        }
        // Now, k = dp[m]
        int k = dp[m];
        // Calculate operations
        int operations;
        if(k > n){
            operations = 2*k - n -1;
        }
        else{
            operations = n -1;
        }
        cout << "Case " << case_num++ << ": " << operations << "\n";
    }
    return 0;
}
