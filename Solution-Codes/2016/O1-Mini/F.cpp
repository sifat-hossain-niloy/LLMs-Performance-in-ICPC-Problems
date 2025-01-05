#include <bits/stdc++.h>
using namespace std;

struct River {
    string name;
    int c;
    long d;
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    cin >> n >> m;
    vector<River> rivers(n);
    for(int i=0; i<n; ++i){
        cin >> rivers[i].name >> rivers[i].c >> rivers[i].d;
    }
    // Confluences are numbered from 1 to m
    // confluences[k] = pair<c_k, d_k>
    vector<pair<int, long>> confluences(m+1, {0, 0});
    for(int k=1; k<=m; ++k){
        cin >> confluences[k].first >> confluences[k].second;
    }
    // Build adjacency list: adj[k] contains list of confluences that have c_k = k
    vector<vector<int>> adj(m+1, vector<int>());
    for(int k=1; k<=m; ++k){
        int downstream = confluences[k].first;
        if(downstream != 0){
            adj[downstream].push_back(k);
        }
    }
    // Compute total_length for confluences
    // total_length[k] = d_k + total_length[c_k]
    // BFS starting from confluences with c_k = 0
    vector<long> total_length(m+1, 0);
    queue<int> q;
    // Initialize queue with confluences that have c_k = 0
    for(int k=1; k<=m; ++k){
        if(confluences[k].first == 0){
            q.push(k);
        }
    }
    while(!q.empty()){
        int k = q.front(); q.pop();
        total_length[k] = confluences[k].second + (confluences[k].first == 0 ? 0 : total_length[confluences[k].first]);
        for(auto &j : adj[k]){
            q.push(j);
        }
    }
    // Now, compute total length for each river
    vector<long> river_lengths(n, 0);
    for(int i=0; i<n; ++i){
        if(rivers[i].c == 0){
            river_lengths[i] = rivers[i].d;
        }
        else{
            river_lengths[i] = rivers[i].d + total_length[rivers[i].c];
        }
    }
    // Create sorted unique lengths in descending order
    vector<long> sorted_unique = river_lengths;
    sort(sorted_unique.begin(), sorted_unique.end(), greater<long>());
    sorted_unique.erase(unique(sorted_unique.begin(), sorted_unique.end()), sorted_unique.end());
    // For each river, find its rank
    // Since sorted_unique is sorted descendingly, use binary search
    // Implement a lambda for binary search
    auto get_rank = [&](long length) -> int {
        int left = 0, right = sorted_unique.size();
        while(left < right){
            int mid = left + (right - left)/2;
            if(sorted_unique[mid] > length){
                left = mid + 1;
            }
            else if(sorted_unique[mid] < length){
                right = mid;
            }
            else{
                return mid + 1;
            }
        }
        return left + 1;
    };
    // Compute ranks
    vector<int> ranks(n, 0);
    for(int i=0; i<n; ++i){
        ranks[i] = get_rank(river_lengths[i]);
    }
    // Output in input order
    for(int i=0; i<n; ++i){
        cout << rivers[i].name << " " << ranks[i] << "\n";
    }
}
