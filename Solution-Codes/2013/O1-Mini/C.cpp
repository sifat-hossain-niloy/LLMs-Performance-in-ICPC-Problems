#include <bits/stdc++.h>
using namespace std;

// Define constants
const long long INF = 1e18;

// Structure to represent a road
struct Road {
    int u;
    int v;
    int t;
};

// Hopcroft-Karp implementation for bipartite matching
struct HopcroftKarp {
    int n, m;
    vector<vector<int>> adj;
    vector<int> pair_u, pair_v, dist;

    HopcroftKarp(int size_u, int size_v) : n(size_u), m(size_v), adj(n, vector<int>()), pair_u(n, -1), pair_v(m, -1), dist(n + 1, 0) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
    }

    bool bfs() {
        queue<int> q;
        for(int u=0; u<n; u++) {
            if(pair_u[u]==-1) {
                dist[u] = 0;
                q.push(u);
            }
            else {
                dist[u] = INF;
            }
        }
        dist[n] = INF;
        while(!q.empty()) {
            int u = q.front(); q.pop();
            if(u < n) {
                for(auto &v : adj[u]) {
                    if(pair_v[v]==-1) {
                        if(dist[n] == INF) {
                            dist[n] = dist[u] + 1;
                        }
                    }
                    else {
                        if(dist[pair_v[v]] == INF) {
                            dist[pair_v[v]] = dist[u] + 1;
                            q.push(pair_v[v]);
                        }
                    }
                }
            }
        }
        return dist[n] != INF;
    }

    bool dfs(int u) {
        if(u != n) {
            for(auto &v : adj[u]) {
                if(pair_v[v]==-1 || (dist[pair_v[v]] == dist[u] + 1 && dfs(pair_v[v]))) {
                    pair_u[u] = v;
                    pair_v[v] = u;
                    return true;
                }
            }
            dist[u] = INF;
            return false;
        }
        return true;
    }

    int max_matching() {
        int matching = 0;
        while(bfs()) {
            for(int u=0; u<n; u++) {
                if(pair_u[u]==-1) {
                    if(dfs(u)) {
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
    int n, m, c;
    cin >> n >> m >> c;
    vector<Road> roads(m);
    // Adjacency list for Dijkstra
    vector<vector<pair<int, int>>> adj(n+1, vector<pair<int, int>>());
    for(int i=0; i<m; i++){
        cin >> roads[i].u >> roads[i].v >> roads[i].t;
        adj[roads[i].u].emplace_back(roads[i].v, roads[i].t);
        adj[roads[i].v].emplace_back(roads[i].u, roads[i].t);
    }
    // Read commuter starting points
    vector<int> commuters(c);
    for(int i=0; i<c; i++) cin >> commuters[i];
    // Dijkstra from node1
    vector<long long> dist(n+1, INF);
    dist[1] = 0;
    // Min-heap priority queue
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, std::greater<pair<long long, int>>> pq;
    pq.emplace(0, 1);
    while(!pq.empty()){
        auto [d, u] = pq.top(); pq.pop();
        if(d > dist[u]) continue;
        for(auto &[v, t] : adj[u]){
            if(dist[v] > dist[u] + t){
                dist[v] = dist[u] + t;
                pq.emplace(dist[v], v);
            }
        }
    }
    // Assign road-direction resource IDs
    // Only directions that are on some shortest path are considered
    // Each usable road-direction gets a unique resource ID
    // Map (road index, direction) to resource ID
    // direction: 0 = u->v, 1 = v->u
    // We assign resource IDs sequentially
    int resource_id = 0;
    // Use a map or vector to store mapping
    // Since m <= 50,000 and two directions, use a pair to index
    // Create a vector to map road-direction to resource ID, -1 if not usable
    vector<int> road_dir_to_res(m*2, -1);
    for(int i=0; i<m; i++){
        int u = roads[i].u;
        int v = roads[i].v;
        int t = roads[i].t;
        // Check u->v
        if(dist[v] == dist[u] - t){
            road_dir_to_res[2*i] = resource_id++;
        }
        // Check v->u
        if(dist[u] == dist[v] - t){
            road_dir_to_res[2*i+1] = resource_id++;
        }
    }
    // Now, total resources are from 0 to resource_id -1
    // Prepare commuters: count those at node1 and others
    int c1 = 0;
    vector<vector<int>> commuter_edges; // Only for commuters not at node1
    for(int i=0; i<c; i++){
        if(commuters[i] == 1){
            c1++;
        }
        else{
            int start = commuters[i];
            vector<int> options;
            // Iterate over all roads connected to 'start'
            for(int road_idx=0; road_idx<m; road_idx++){
                if(roads[road_idx].u == start){
                    // direction u->v
                    int res = road_dir_to_res[2*road_idx];
                    if(res != -1){
                        options.push_back(res);
                    }
                }
                if(roads[road_idx].v == start){
                    // direction v->u
                    int res = road_dir_to_res[2*road_idx+1];
                    if(res != -1){
                        options.push_back(res);
                    }
                }
            }
            // It is guaranteed that all commuters can reach downtown via some path
            // So options should not be empty
            commuter_edges.emplace_back(options);
        }
    }
    int c2 = c - c1;
    // Now, set up Hopcroft-Karp
    // Left nodes: 0 to c2-1
    // Right nodes: 0 to resource_id -1
    HopcroftKarp hk(c2, resource_id);
    for(int i=0; i<c2; i++){
        for(auto &res : commuter_edges[i]){
            hk.add_edge(i, res);
        }
    }
    int matching = hk.max_matching();
    // Total maximum commuters without congestion
    cout << (c1 + matching) << "\n";
}
