#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

// Structure to store edges
struct Edge {
    int to;
    int id;
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, m;
    cin >> n >> m;
    // Original graph
    vector<vector<pair<int, int>>> adj(n+1);
    // List of streets
    vector<pair<int, int>> streets(m);
    for(int i=0;i<m;i++){
        int u, v;
        cin >> u >> v;
        streets[i] = {u, v};
        adj[u].emplace_back(v, i);
        adj[v].emplace_back(u, i);
    }
    // Tarjan's algorithm to find bridges
    vector<int> disc(n+1, -1);
    vector<int> low(n+1, -1);
    int time_d = 0;
    vector<bool> is_bridge(m, false);
    // DFS to find bridges
    function<void(int, int)> dfs = [&](int u, int parent_edge) {
        disc[u] = low[u] = ++time_d;
        for(auto &[v, eid] : adj[u]){
            if(eid == parent_edge) continue;
            if(disc[v] == -1){
                dfs(v, eid);
                low[u] = min(low[u], low[v]);
                if(low[v] > disc[u]){
                    is_bridge[eid] = true;
                }
            }
            else{
                low[u] = min(low[u], disc[v]);
            }
        };
    };
    for(int u=1; u<=n; u++) {
        if(disc[u]==-1){
            dfs(u, -1);
        }
    }
    // Assign components
    int comp_id = 0;
    vector<int> component(n+1, -1);
    function<void(int)> assign = [&](int u) {
        component[u] = comp_id;
        for(auto &[v, eid] : adj[u]){
            if(component[v]==-1 && !is_bridge[eid]){
                assign(v);
            }
        };
    };
    for(int u=1; u<=n; u++){
        if(component[u]==-1){
            assign(u);
            comp_id++;
        }
    }
    // Build bridge tree
    int num_comps = comp_id;
    vector<vector<pair<int, int>>> bridge_tree(num_comps);
    // For mapping bridge tree edges to streets
    // Each bridge tree edge is stored as (child, street_id)
    struct BridgeEdge {
        int parent;
        int child;
        int street_id;
    };
    vector<BridgeEdge> bridge_edges;
    for(int i=0;i<m;i++){
        if(is_bridge[i]){
            int u = streets[i].first;
            int v = streets[i].second;
            int cu = component[u];
            int cv = component[v];
            if(cu != cv){
                bridge_tree[cu].emplace_back(cv, i);
                bridge_tree[cv].emplace_back(cu, i);
                bridge_edges.push_back(BridgeEdge{cu, cv, i});
            }
        }
    }
    // Choose root
    int root = component[1];
    // To keep track of parent in bridge tree
    vector<int> parent(num_comps, -1);
    // To keep track of the bridge id between parent and child
    vector<int> bridge_to_parent(num_comps, -1);
    // BFS to assign parent
    queue<int> q;
    q.push(root);
    parent[root] = -2; // mark as root
    while(!q.empty()){
        int u = q.front(); q.pop();
        for(auto &[v, eid] : bridge_tree[u]){
            if(parent[v]==-1){
                parent[v] = u;
                bridge_to_parent[v] = eid;
                q.push(v);
            }
        }
    }
    // Now perform DFS to place signs
    vector<pair<int, int>> signs;
    // Function to perform DFS
    function<bool(int)> dfs_sign = [&](int u) -> bool {
        bool has_leaf = false;
        for(auto &[v, eid] : bridge_tree[u]){
            if(v == parent[u]) continue;
            bool child_has_leaf = dfs_sign(v);
            if(child_has_leaf){
                if(u == root){
                    // Place sign from child to u
                    int bridge_eid = bridge_to_parent[v];
                    int street_u = streets[bridge_eid].first;
                    int street_v = streets[bridge_eid].second;
                    // component[u] contains street_u
                    // component[v] contains street_v
                    // So sign is at street_v towards street_u
                    // street_u is in parent, street_v is in child
                    // Thus, sign is (street_v, street_u)
                    signs.emplace_back(street_v, street_u);
                }
                else{
                    // Place sign from u to parent
                    int bridge_eid = bridge_to_parent[u];
                    int street_u = streets[bridge_eid].first;
                    int street_v = streets[bridge_eid].second;
                    // component[u] contains street_u or street_v
                    // component[parent[u]] contains the other
                    // Need to place sign at u towards parent[u]
                    // Determine which node is in u and which in parent
                    if(component[street_u] == u){
                        // street_u is in u, street_v is in parent
                        signs.emplace_back(street_u, street_v);
                    }
                    else{
                        signs.emplace_back(street_v, street_u);
                    }
                }
                has_leaf = true;
            }
        }
        // Check if u is a leaf
        int degree = bridge_tree[u].size();
        if(u != root && degree ==1){
            has_leaf = true;
        }
        return has_leaf;
    };
    dfs_sign(root);
    // Remove duplicates if any
    sort(signs.begin(), signs.end());
    signs.erase(unique(signs.begin(), signs.end()), signs.end());
    // Sort as per problem statement
    sort(signs.begin(), signs.end(), [&](const pair<int,int> &a, const pair<int,int> &b) -> bool {
        if(a.first != b.first) return a.first < b.first;
        return a.second < b.second;
    });
    // Output
    cout << signs.size() << "\n";
    for(auto &[v, w] : signs){
        cout << v << " " << w << "\n";
    }
}
