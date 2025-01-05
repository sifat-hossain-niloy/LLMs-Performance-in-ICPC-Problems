#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll INF = 1e18;

// Edge structure
struct Edge {
    int to;
    ll cost;
};

// Function to perform Bellman-Ford
bool bellman_ford(int V, vector<vector<Edge>> &adj, int src, int dest, ll &dist_out) {
    vector<ll> dist(V, INF);
    dist[src] = 0;
    // Relax edges V-1 times
    for(int i=0;i<V-1;i++) {
        bool updated = false;
        for(int u=0;u<V;u++) {
            if(dist[u] == INF) continue;
            for(auto &edge: adj[u]) {
                if(dist[edge.to] > dist[u] + edge.cost) {
                    dist[edge.to] = dist[u] + edge.cost;
                    updated = true;
                }
            }
        }
        if(!updated) break;
    }
    // Check for negative cycles (not needed here)
    // Return distance to dest
    if(dist[dest] != INF){
        dist_out = dist[dest];
        return true;
    }
    return false;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int N, M;
    int case_num = 1;
    while(cin >> N >> M){
        // Read junctions
        struct Junction {
            int x, y, z, k;
        };
        vector<Junction> junctions(N);
        for(int i=0;i<N;i++) cin >> junctions[i].x >> junctions[i].y >> junctions[i].z >> junctions[i].k;
        // Read existing pipes
        vector<pair<int, int>> pipes(M);
        for(int i=0;i<M;i++){
            cin >> pipes[i].first >> pipes[i].second;
            // Convert to 0-based
            pipes[i].first--;
            pipes[i].second--;
        }
        // Node splitting:
        // For node i, in = 2*i, out = 2*i +1
        int V = 2*N;
        vector<vector<Edge>> adj(V, vector<Edge>());
        // Add edges for node splitting
        for(int i=0;i<N;i++){
            if(i != N-1){ // For all nodes except sink, add in to out with cost ki
                Edge e;
                e.to = 2*i +1;
                e.cost = junctions[i].k;
                adj[2*i].push_back(e);
            }
            // For sink node, do not add in to out edge
        }
        // Add edges for existing pipes
        for(auto &[u, v] : pipes){
            // u_out to v_in with cost -1
            Edge e1;
            e1.to = 2*v;
            e1.cost = -1;
            adj[2*u +1].push_back(e1);
            // v_out to u_in with cost -1
            Edge e2;
            e2.to = 2*u;
            e2.cost = -1;
            adj[2*v +1].push_back(e2);
        }
        // Define source and sink
        int source = 2*0; // node1_in
        int sink = 2*(N-1); // nodeN_in
        // Perform Bellman-Ford
        ll min_cost;
        bool possible = bellman_ford(V, adj, source, sink, min_cost);
        if(possible){
            // Output as floating with four decimals
            // Convert to double
            double res = (double)min_cost;
            cout << "Case " << case_num++ << ": " << fixed << setprecision(4) << res << "\n";
        }
        else{
            cout << "Case " << case_num++ << ": impossible\n";
        }
    }
}
