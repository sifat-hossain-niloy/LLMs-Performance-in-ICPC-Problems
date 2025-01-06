#include <bits/stdc++.h>
using namespace std;

// Hopcroft-Karp implementation
struct HopcroftKarp {
    int n, m;
    vector<vector<int>> adj;
    vector<int> pair_u, pair_v, dist;

    HopcroftKarp(int size_u, int size_v) : n(size_u), m(size_v), adj(size_u, vector<int>()), pair_u(size_u, -1), pair_v(size_v, -1), dist(size_u + 1, 0) {}

    void add_edge(int u, int v){
        adj[u].push_back(v);
    }

    bool bfs(){
        queue<int> q;
        for(int u=0; u<n; u++) {
            if(pair_u[u]==-1){
                dist[u] = 0;
                q.push(u);
            }
            else{
                dist[u] = INT32_MAX;
            }
        }
        dist[n] = INT32_MAX;
        while(!q.empty()){
            int u = q.front(); q.pop();
            if(u < n){
                for(auto &v : adj[u]){
                    if(pair_v[v]==-1){
                        if(dist[n] == INT32_MAX){
                            dist[n] = dist[u] + 1;
                        }
                    }
                    else{
                        if(dist[pair_v[v]] == INT32_MAX){
                            dist[pair_v[v]] = dist[u] + 1;
                            q.push(pair_v[v]);
                        }
                    }
                }
            }
        }
        return dist[n] != INT32_MAX;
    }

    bool dfs(int u){
        if(u != n){
            for(auto &v : adj[u]){
                if(pair_v[v]==-1 || (dist[pair_v[v]] == dist[u] + 1 && dfs(pair_v[v]))){
                    pair_u[u] = v;
                    pair_v[v] = u;
                    return true;
                }
            }
            dist[u] = INT32_MAX;
            return false;
        }
        return true;
    }

    int max_matching(){
        int matching = 0;
        while(bfs()){
            for(int u=0; u<n; u++) {
                if(pair_u[u]==-1){
                    if(dfs(u)){
                        matching++;
                    }
                }
            }
        }
        return matching;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    cin >> n;
    string S;
    cin >> S;
    vector<string> fossils(n);
    for(int i=0; i<n; i++) cin >> fossils[i];
    // Assign IDs: fossils 0 to n-1, S as n
    // Build a sorted list for binary search
    vector<string> all_seqs = fossils;
    all_seqs.push_back(S);
    sort(all_seqs.begin(), all_seqs.end());
    all_seqs.erase(unique(all_seqs.begin(), all_seqs.end()), all_seqs.end());
    // Check if S is unique
    // Map from sequence to ID
    unordered_map<string, int> seq_map;
    for(int i=0; i<(int)all_seqs.size(); i++) seq_map[all_seqs[i]] = i;
    // Check all sequences are unique
    if(all_seqs.size() != n +1){
        // There are duplicates, but the problem states all are distinct
        // So it's safe to proceed
    }
    // Now, assign IDs: fossils are 0 to n-1, S is n
    // Find the ID of S
    int id_S = seq_map[S];
    // Build a list of fossil IDs
    vector<int> fossil_ids;
    for(int i=0; i<n; i++){
        fossil_ids.push_back(seq_map[fossils[i]]);
    }
    // Now, total right nodes: all_seqs.size() + 2 (for S1 and S2)
    // But according to our earlier plan, right nodes are fossils plus S1 and S2
    // Assign right node IDs: 0 to all_seqs.size()-1, S1 = all_seqs.size(), S2 = all_seqs.size()+1
    int right_size = (int)all_seqs.size() + 2;
    // Create a mapping from fossil index to sequence ID
    // But Hopcroft-Karp needs left nodes as fossils (0 to n-1)
    // and right nodes as fossils (0 to all_seqs.size()-1) plus S1 and S2
    // So right nodes go from 0 to m-1, where m = all_seqs.size()+2
    // We need to map fossils to their sequence IDs
    // Initialize Hopcroft-Karp
    HopcroftKarp hk(n, (int)all_seqs.size()+2);
    // Build reverse map from sequence ID to index in Hopcroft-Karp
    // Actually, left nodes are 0 to n-1, right nodes are 0 to m-1
    // For each fossil (left node u = 0 to n-1), find all possible successors
    // by inserting one nucleotide
    for(int u = 0; u < n; u++){
        string A = fossils[u];
        int id_a = fossil_ids[u];
        int L = A.length();
        // Generate all possible insertions
        for(int pos=0; pos<=L; pos++){
            for(char c : {'A', 'C', 'M'}){
                string B = A.substr(0, pos) + string(1, c) + A.substr(pos, L - pos);
                if(seq_map.find(B) != seq_map.end()){
                    int id_b = seq_map[B];
                    if(id_b != id_S){
                        // Check if B is a fossil
                        // Find if B is among fossils
                        // Binary search in fossils
                        // Use the fossil_ids to check
                        // We have n fossils with IDs fossil_ids[0 to n-1]
                        // Create a map from sequence ID to fossil index
                        // To speed up, create a reverse map
                        // But n=4000, acceptable to iterate
                        bool is_fossil = false;
                        int fossil_index = -1;
                        // Binary search
                        // Alternatively, use a separate array
                        // Create a vector to mark fossil IDs
                        // Initialize once
                        // Implement as follows:
                        // To speed up, create a boolean vector
                        // indicating which sequence IDs are fossils
                        // Do it outside the loop
                        // Implement it here
                        // Temporarily implement as follows:
                        // Iterate through fossils to see if id_b is among them
                        // To speed up, create a set
                        // Precompute
                        // Let's precompute a vector
                    }
                }
            }
        }
    }
    // To optimize, create a vector to mark which sequence IDs are fossils
    vector<bool> is_fossil_seq(all_seqs.size(), false);
    for(int u=0; u<n; u++) is_fossil_seq[fossil_ids[u]] = true;
    // Now, iterate again
    for(int u = 0; u < n; u++){
        string A = fossils[u];
        int id_a = fossil_ids[u];
        int L = A.length();
        // Generate all possible insertions
        for(int pos=0; pos<=L; pos++){
            for(char c : {'A', 'C', 'M'}){
                string B = A.substr(0, pos) + string(1, c) + A.substr(pos, L - pos);
                auto it = seq_map.find(B);
                if(it != seq_map.end()){
                    int id_b = it->second;
                    if(id_b != id_S){
                        if(is_fossil_seq[id_b]){
                            // Find the fossil index for id_b
                            // Since fossils have unique IDs, iterate to find
                            // To speed up, create a mapping from sequence ID to fossil index
                            // Precompute
                            // Create a vector where fossil_seq_to_index[id] = fossil index or -1
                            // Initialize
                            // Initialize once
                            static vector<int> fossil_seq_to_index_max(all_seqs.size(), -1);
                            if(fossil_seq_to_index_max[0] == -1){
                                for(int fi=0; fi<n; fi++) fossil_seq_to_index_max[fossil_ids[fi]] = fi;
                            }
                            int fi = fossil_seq_to_index_max[id_b];
                            if(fi != -1){
                                hk.add_edge(u, fi);
                            }
                        }
                    }
                    else{
                        // B is S, add edges to S1 and S2
                        hk.add_edge(u, (int)all_seqs.size()); // S1
                        hk.add_edge(u, (int)all_seqs.size()+1); // S2
                    }
                }
            }
        }
    }
    // Now, compute maximum matching
    int matching = hk.max_matching();
    // The minimum path cover is p = n - matching
    int p = n - matching;
    if(p > 2){
        cout << "impossible";
        return 0;
    }
    // Now, reconstruct the path cover
    // Build pair_u and pair_v
    vector<int> pair_u_final = hk.pair_u;
    vector<int> pair_v_final = hk.pair_v;
    // Build predecessor array
    // For each right node, if it's a fossil, set predecessor
    // Initialize
    vector<int> predecessor(n, -1);
    for(int v=0; v<(int)pair_v_final.size(); v++){
        if(pair_v_final[v] != -1 && v < (int)all_seqs.size()){
            int u = pair_v_final[v];
            // u is a left node, v is a right node
            // If v is a fossil
            if(v < n && is_fossil_seq[v]){
                predecessor[v] = u;
            }
        }
    }
    // Find starting nodes: left nodes not in predecessor
    vector<bool> has_pred(n, false);
    for(int v=0; v<n; v++) {
        if(predecessor[v] != -1){
            has_pred[v] = true;
        }
    }
    vector<int> starts;
    for(int u=0; u<n; u++) {
        // If u is not a predecessor of any node
        bool is_start = true;
        for(int v=0; v<n; v++) {
            if(predecessor[v] == u){
                // u is a predecessor
                // It may still be a start if it's not matched as a successor
                // But to find starts, we need nodes not having any predecessor
                // So check if u has a predecessor
                // Which is predecessor[u] == -1
            }
        }
        // To find if u has a predecessor
        // u has a predecessor if there exists some v where predecessor[v] == u
        // But we need to find if u itself has a predecessor
        // Find if u has a predecessor
        // To do this, check if u is matched to some node
        // Actually, simpler to find nodes where predecessor[u] == -1
        // But predecessor array is for v, not for u
        // Recompute correctly
        // Correct approach:
        // A node u is a start if it is not matched by any other node
        // That is, no v has pair_u[v] matched to u
        // So check if any v has pair_u[v] == u
        // Implement:
        bool is_start_node = true;
        for(int v=0; v<(int)pair_v_final.size(); v++){
            if(pair_v_final[v] == u && v < n){
                is_start_node = false;
                break;
            }
        }
        if(is_start_node){
            starts.push_back(u);
        }
    }
    // Now, for each start, follow the path
    vector<vector<int>> paths;
    for(auto u : starts){
        vector<int> path;
        int current = u;
        while(current != -1){
            path.push_back(current);
            if(pair_u_final[current] == -1){
                break;
            }
            int matched_right = pair_u_final[current];
            if(matched_right >= (int)all_seqs.size()){
                // Matched to S1 or S2
                break;
            }
            current = matched_right;
        }
        paths.push_back(path);
    }
    // Now, p should be equal to paths.size()
    // Assign paths to path1 and path2
    if(p == 0 && n ==0){
        // No fossils, assign s1=0 and s2=0
        cout << "0 0";
        return 0;
    }
    if(p ==0 && n >0){
        // Impossible, since at least one path is needed
        cout << "impossible";
        return 0;
    }
    if(p >=1 && p <=2){
        // Assign paths to path1 and path2
        // According to sample input 4, assign to path2 if p==1
        if(p ==1){
            // Assign the single path to path2, s1=0
            cout << "0 " << paths[0].size() << "\n";
            for(auto seq_id : paths[0]){
                cout << all_seqs[seq_id] << "\n";
            }
        }
        else{
            // p ==2
            cout << paths[0].size() << " " << paths[1].size() << "\n";
            for(auto seq_id : paths[0]){
                cout << all_seqs[seq_id] << "\n";
            }
            for(auto seq_id : paths[1]){
                cout << all_seqs[seq_id] << "\n";
            }
        }
    }
    else{
        cout << "impossible";
    }
}
