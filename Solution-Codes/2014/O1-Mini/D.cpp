#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef unsigned int uint;

// Function to convert a string to a bitmask
inline uint string_to_bitmask(const string& s) {
    uint mask = 0;
    for(char c : s) {
        mask |= 1u << (c - 'a');
    }
    return mask;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    cin >> n;
    // Read options
    vector<vector<uint>> options(n, vector<uint>());
    string line;
    getline(cin, line); // Consume the remaining newline after n
    for(int p=0; p<n; p++){
        getline(cin, line);
        // Parse m and m strings
        int m;
        int pos = 0;
        // Read m
        while(pos < line.size() && line[pos] == ' ') pos++;
        int start = pos;
        while(pos < line.size() && line[pos] != ' ') pos++;
        m = stoi(line.substr(start, pos - start));
        // Read m strings
        for(int i=0; i<m; i++){
            while(pos < line.size() && line[pos] == ' ') pos++;
            start = pos;
            while(pos < line.size() && line[pos] != ' ') pos++;
            string s = line.substr(start, pos - start);
            uint mask = string_to_bitmask(s);
            options[p].push_back(mask);
        }
    }
    // Precompute dependencies
    vector<vector<pair<int, int>>> dependencies(n, vector<pair<int, int>>());
    for(int p=0; p<n; p++){
        for(int option_idx=0; option_idx < (int)options[p].size(); option_idx++){
            uint S = options[p][option_idx];
            for(int s=0; s<n; s++){
                if(S & (1u << s)){
                    dependencies[s].emplace_back(p, option_idx);
                }
            }
        }
    }
    // Initialize result matrix
    vector<vector<int>> result(n, vector<int>(n, -1));
    // For each target q
    for(int q=0; q<n; q++){
        // Initialize distance
        vector<int> distance(n, INT32_MAX);
        distance[q] = 0;
        // Initialize queue
        vector<int> queue;
        queue.reserve(n);
        queue.push_back(q);
        int head =0;
        int tail =1;
        while(head < tail){
            int s = queue[head++];
            // Iterate over dependencies[s]
            for(auto &[p, option_idx] : dependencies[s]){
                uint S = options[p][option_idx];
                // Compute max distance[s'] for s' in S
                int max_d = 0;
                uint temp_S = S;
                while(temp_S){
                    int s_prime = __builtin_ctz(temp_S);
                    if(distance[s_prime] > max_d){
                        max_d = distance[s_prime];
                    }
                    temp_S &= temp_S -1;
                }
                // Check for overflow
                if(max_d == INT32_MAX){
                    // Candidate distance remains INF, do not update
                    continue;
                }
                int candidate_distance = 1 + max_d;
                if(candidate_distance < distance[p]){
                    distance[p] = candidate_distance;
                    queue.push_back(p);
                    tail++;
                }
            }
        }
        // Fill the result matrix for target q
        for(int p=0; p<n; p++){
            if(distance[p] != INT32_MAX){
                result[p][q] = distance[p];
            }
            else{
                result[p][q] = -1;
            }
        }
    }
    // Output the result
    for(int p=0; p<n; p++){
        for(int q=0; q<n; q++){
            cout << result[p][q];
            if(q != n-1) cout << ' ';
        }
        cout << '\n';
    }
}
