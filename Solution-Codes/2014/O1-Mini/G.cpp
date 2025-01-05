#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

// 2-SAT implementation using Tarjan's algorithm
struct TwoSAT {
    int n;
    vector<vector<int>> adj;
    vector<int> index_scc;
    int idx;
    vector<int> indices;
    vector<int> lowlink;
    vector<bool> onStack;
    stack<int> S;
    
    TwoSAT(int num_vars) : n(num_vars), adj(2*num_vars, vector<int>()), index_scc(2*num_vars, -1), idx(0), indices(2*num_vars, -1), lowlink(2*num_vars, -1), onStack(2*num_vars, false) {}
    
    void add_implication(int u, int v) {
        adj[u].push_back(v);
    }
    
    void add_clause(int u, bool u_val, int v, bool v_val) {
        // (u is u_val) OR (v is v_val)
        // Which translates to (~u → v) and (~v → u)
        int u_lit = u * 2 + (u_val ? 0 : 1);
        int v_lit = v * 2 + (v_val ? 0 : 1);
        int not_u = u * 2 + (!u_val ? 1 : 0);
        int not_v = v * 2 + (!v_val ? 1 : 0);
        add_implication(not_u, v_lit);
        add_implication(not_v, u_lit);
    }
    
    bool satisfiable() {
        int num = 2*n;
        indices.assign(num, -1);
        lowlink.assign(num, -1);
        onStack.assign(num, false);
        index_scc = vector<int>(num, -1);
        idx = 0;
        while (!S.empty()) S.pop();
        
        // To store the order of SCCs
        vector<int> scc_id;
        for(int v=0; v<num; v++) {
            if(indices[v]==-1){
                tarjan(v, scc_id);
            }
        }
        
        // Check for conflicts
        for(int i=0; i<n; i++) {
            if(index_scc[2*i] == index_scc[2*i+1]) return false;
        }
        return true;
    }
    
    void tarjan(int v, vector<int> &scc_id) {
        indices[v] = idx;
        lowlink[v] = idx;
        idx++;
        S.push(v);
        onStack[v] = true;
        for(auto &w : adj[v]) {
            if(indices[w]==-1){
                tarjan(w, scc_id);
                lowlink[v] = min(lowlink[v], lowlink[w]);
            }
            else if(onStack[w]){
                lowlink[v] = min(lowlink[v], indices[w]);
            }
        }
        if(lowlink[v] == indices[v]){
            while(1){
                int w = S.top(); S.pop();
                onStack[w] = false;
                index_scc[w] = scc_id.size();
                if(w == v) break;
            }
            scc_id.push_back(v);
        }
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    cin >> n;
    // Initialize distance matrix
    vector<vector<int>> d(n, vector<int>(n, 0));
    for(int i=0; i<n-1; i++) {
        for(int j=1; j<=n-i-1; j++) {
            cin >> d[i][i+j];
            d[i+j][i] = d[i][i+j];
        }
    }
    // Collect all unique distances
    vector<int> unique_dist;
    for(int i=0; i<n; i++) {
        for(int j=i+1; j<n; j++) {
            unique_dist.push_back(d[i][j]);
        }
    }
    sort(unique_dist.begin(), unique_dist.end());
    unique_dist.erase(unique(unique_dist.begin(), unique_dist.end()), unique_dist.end());
    // Now, sorted unique distances
    int k = unique_dist.size();
    // Binary search approach
    ll answer = LLONG_MAX;
    // Precompute all possible d1 and d2 sorted
    // Iterate d1 in increasing order
    for(int i=0; i<k; i++){
        int d1 = unique_dist[i];
        // Binary search for minimal d2
        int left = 0, right = k-1;
        int best_d2 = -1;
        while(left <= right){
            int mid = left + (right - left)/2;
            int d2 = unique_dist[mid];
            // Setup 2-SAT
            TwoSAT ts(n);
            // Add clauses based on d1 and d2
            for(int u=0; u<n; u++) {
                for(int v=u+1; v<n; v++) {
                    int dist = d[u][v];
                    if(dist > d1 && dist > d2){
                        // (A_u OR A_v) AND (~A_u OR ~A_v)
                        // (A_u OR A_v)
                        ts.add_clause(u, true, v, true);
                        // (~A_u OR ~A_v)
                        ts.add_clause(u, false, v, false);
                    }
                    else if(dist > d1 && dist <= d2){
                        // (A_u OR A_v)
                        ts.add_clause(u, true, v, true);
                    }
                    else if(dist <= d1 && dist > d2){
                        // (~A_u OR ~A_v)
                        ts.add_clause(u, false, v, false);
                    }
                    // else: no constraints
                }
            }
            if(ts.satisfiable()){
                best_d2 = d2;
                right = mid -1;
            }
            else{
                left = mid +1;
            }
        }
        if(best_d2 != -1){
            answer = min(answer, (ll)d1 + (ll)best_d2);
        }
    }
    cout << answer;
}
