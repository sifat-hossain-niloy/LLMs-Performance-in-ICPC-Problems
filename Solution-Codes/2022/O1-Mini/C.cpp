#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, m;
    cin >> n >> m;
    // Types are up to 1e6
    const int MAX_TYPE = 1000000;
    vector<vector<int>> initial_pos(MAX_TYPE+1, vector<int>());
    vector<vector<int>> target_pos(MAX_TYPE+1, vector<int>());
    // Read b_i and e_i
    vector<pair<int, int>> be(n+1);
    for(int i=1;i<=n;i++) cin >> be[i].first;
    for(int i=1;i<=n;i++) cin >> be[i].second;
    // Assign initial and target positions
    for(int i=1;i<=n;i++){
        initial_pos[be[i].first].push_back(i);
    }
    for(int i=1;i<=n;i++){
        target_pos[be[i].second].push_back(i);
    }
    // Assign permutation p[i] = j
    vector<int> p(n+1, 0);
    bool possible_assign = true;
    for(int t=1;t<=MAX_TYPE;t++){
        if(initial_pos[t].size() != target_pos[t].size()){
            possible_assign = false;
            break;
        }
        for(int k=0; k < initial_pos[t].size(); k++){
            p[initial_pos[t][k]] = target_pos[t][k];
        }
    }
    if(!possible_assign){
        cout << "impossible";
        return 0;
    }
    // Read tunnels
    vector<vector<int>> adj(n+1, vector<int>());
    for(int i=0;i<m;i++){
        int x, y;
        cin >> x >> y;
        adj[x].push_back(y);
        adj[y].push_back(x);
    }
    // Find connected components and check tree components
    vector<bool> visited(n+1, false);
    bool has_non_tree_component = false;
    bool impossible_flag = false;
    for(int i=1;i<=n;i++){
        if(!visited[i]){
            // BFS
            queue<int> q;
            q.push(i);
            visited[i] = true;
            int count_nodes = 0;
            int count_edges = 0;
            vector<int> nodes;
            while(!q.empty()){
                int u = q.front(); q.pop();
                nodes.push_back(u);
                count_nodes++;
                for(auto &v: adj[u]){
                    if(u < v) count_edges++;
                    if(!visited[v]){
                        visited[v] = true;
                        q.push(v);
                    }
                }
            }
            if(count_edges >= count_nodes){
                has_non_tree_component = true;
            }
            else{
                // It's a tree, check p[u] == u for all u in this component
                for(auto &u: nodes){
                    if(p[u] !=u){
                        impossible_flag = true;
                        break;
                    }
                }
                if(impossible_flag) break;
            }
        }
    }
    if(impossible_flag){
        cout << "impossible";
        return 0;
    }
    // Now compute permutation parity
    // Build permutation mapping p[1..n]
    // Compute number of cycles
    vector<bool> visited_perm(n+1, false);
    int cycles =0;
    for(int i=1;i<=n;i++){
        if(!visited_perm[i]){
            int j =i;
            while(!visited_perm[j]){
                visited_perm[j]=true;
                j = p[j];
            }
            cycles++;
        }
    }
    // Parity is (n - cycles) %2
    int parity = (n - cycles) %2;
    if(parity ==0 || has_non_tree_component){
        cout << "possible";
    }
    else{
        cout << "impossible";
    }
}
