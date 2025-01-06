#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// Structure to represent an edge in the graph
struct Edge {
    int to;
    int rev;
    ll cap;
    ll cost;
};

// Function to add an edge to the graph
void add_edge(vector<vector<Edge>> &graph, int from, int to, ll cap, ll cost){
    Edge a = {to, (int)graph[to].size(), cap, cost};
    Edge b = {from, (int)(graph[from].size()), 0, -cost};
    graph[from].push_back(a);
    graph[to].push_back(b);
}

// Min Cost Max Flow using the Successive Shortest Path algorithm with potentials
pair<ll, ll> min_cost_flow(vector<vector<Edge>> &graph, int S, int T, ll maxf){
    int n = graph.size();
    vector<ll> prevv(n, -1);
    vector<int> preve(n, -1);
    vector<ll> potential(n, 0);
    ll flow = 0;
    ll flow_cost = 0;
    while(flow < maxf){
        // Dijkstra's algorithm
        priority_queue<pair<ll, int>, vector<pair<ll, int>>, std::greater<pair<ll, int>>> pq;
        vector<ll> dist(n, 1e18);
        dist[S] = 0;
        pq.push({0, S});
        while(!pq.empty()){
            pair<ll, int> p = pq.top(); pq.pop();
            ll d = p.first;
            int u = p.second;
            if(d > dist[u]) continue;
            for(int i=0;i<graph[u].size();i++){
                Edge &e = graph[u][i];
                if(e.cap >0 && dist[e.to] > dist[u] + e.cost + potential[u] - potential[e.to]){
                    dist[e.to] = dist[u] + e.cost + potential[u] - potential[e.to];
                    prevv[e.to] = u;
                    preve[e.to] = i;
                    pq.push({dist[e.to], e.to});
                }
            }
        }
        if(dist[T] == 1e18){
            // Cannot flow more
            break;
        }
        for(int v=0; v<n; v++) potential[v] += (dist[v] < 1e18 ? dist[v] : 0);
        // Add as much as possible
        ll addf = maxf - flow;
        // Find the minimum possible flow to add
        // Here, since capacities are integers, we can add 1
        addf = 1;
        // Update flow and cost
        flow += addf;
        flow_cost += addf * potential[T];
        // Update residual capacities
        int v = T;
        while(v != S){
            int u = prevv[v];
            int idx = preve[v];
            graph[u][idx].cap -= addf;
            graph[v][graph[u][idx].rev].cap += addf;
            v = u;
        }
    }
    return {flow, flow_cost};
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, k;
    cin >> n >> k;
    // Locations are from1 ton+1
    // Requests are from1 ton, located at2 ton+1
    // Node numbering:
    // S:0
    // Company:1
    // Requests:2 ton+1
    // T:n+2
    int total_nodes = n + 3;
    int S = 0;
    int Company = 1;
    int T = n + 2;
    vector<vector<Edge>> graph(total_nodes, vector<Edge>());
    // Read the costs and build c[x][y}
    // c[x][y} where x from1 ton, y fromx+1 ton+1
    // Initialize c as a 2D vector
    vector<vector<ll>> c(n+2, vector<ll>(n+2, 0));
    for(int i=1; i<=n; i++){
        int num = n - i +1;
        for(int j=1; j<=num; j++){
            ll cost;
            cin >> cost;
            c[i][i+j] = cost;
        }
    }
    // Add edges
    // S to Company
    add_edge(graph, S, Company, k, 0);
    // Company to each request
    for(int i=1; i<=n; i++){
        int req_node = 2 + i -1;
        add_edge(graph, Company, req_node, 1, c[1][i+1]);
    }
    // Between requests
    for(int i=1; i<=n; i++){
        int from_node = 2 + i -1;
        for(int j=i+1; j<=n; j++){
            if(c[i][j+1] > 0){
                int to_node = 2 + j -1;
                add_edge(graph, from_node, to_node, 1, c[i][j+1]);
            }
        }
    }
    // Requests to T
    for(int i=1; i<=n; i++){
        int req_node = 2 + i -1;
        add_edge(graph, req_node, T, 1, 0);
    }
    // Now, find min cost flow of n
    pair<ll, ll> result = min_cost_flow(graph, S, T, n);
    // Assuming that it's always possible to assign all requests
    cout << result.second;
}
