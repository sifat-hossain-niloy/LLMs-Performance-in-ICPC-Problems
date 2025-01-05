#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

// Constants
const int MAX = 2001;

// Variables
int n, q;
ll S = 0;
vector<vector<pair<int, ll>>> adj(MAX, vector<pair<int, ll>>());
ll dist_matrix[MAX][MAX];
int parent_matrix[MAX][12]; // For LCA, up to 2^11=2048 >2000
int depth_matrix[MAX][MAX];

// Function to compute all-pairs distances using BFS
void compute_all_pairs_dist() {
    for(int si = 1; si <= n; si++) {
        // BFS from si
        queue<int> q_nodes;
        q_nodes.push(si);
        dist_matrix[si][si] = 0;
        vector<bool> visited(n+1, false);
        visited[si] = true;
        while(!q_nodes.empty()) {
            int u = q_nodes.front(); q_nodes.pop();
            for(auto &[v, w] : adj[u]) {
                if(!visited[v]) {
                    dist_matrix[si][v] = dist_matrix[si][u] + w;
                    visited[v] = true;
                    q_nodes.push(v);
                }
            }
        }
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    // Input
    cin >> n >> q;
    for(int i=1;i<n;i++){
        int u, v;
        ll w;
        cin >> u >> v >> w;
        adj[u].emplace_back(v, w);
        adj[v].emplace_back(u, w);
        S += w;
    }

    // Compute all-pairs distances
    compute_all_pairs_dist();

    // Precompute max distance for each si
    ll max_dist[MAX];
    for(int si=1; si<=n; si++){
        max_dist[si] = 0;
        for(int l=1; l<=n; l++) {
            if(dist_matrix[si][l] > max_dist[si])
                max_dist[si] = dist_matrix[si][l];
        }
    }

    // Function to compute LCA using Binary Lifting
    // Precompute depth and parents
    // Since n is small, use simple DFS for each root
    // and store parent information
    // Not needed for this problem as we use distance checks

    // Process Queries
    while(q--){
        int si, ki, ti;
        cin >> si >> ki >> ti;
        // Check if ti is on the path from si to ki
        // Using distance: if distance(si, ki) == distance(si, ti) + distance(ti, ki)
        if(dist_matrix[si][ki] == dist_matrix[si][ti] + dist_matrix[ti][ki]){
            cout << "impossible\n";
        }
        else{
            // Minimal traversal time is 2 * S - max distance from si
            ll traversal_time = 2 * S - max_dist[si];
            cout << traversal_time << "\n";
        }
    }
}
