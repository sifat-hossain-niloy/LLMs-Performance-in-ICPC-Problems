#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int to;
    int rev;
    double cap;
};

class FlowNetwork {
public:
    int n;
    vector<vector<Edge>> graph;
    vector<int> level;
    vector<int> ptr;

    FlowNetwork(int size) : n(size), graph(size, vector<Edge>()), level(size, -1), ptr(size, 0) {}

    void add_edge(int from, int to, double cap) {
        Edge a = {to, (int)graph[to].size(), cap};
        Edge b = {from, (int)(graph[from].size()), 0.0};
        graph[from].push_back(a);
        graph[to].push_back(b);
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        q.push(s);
        level[s] = 0;
        while(!q.empty()) {
            int v = q.front(); q.pop();
            for(auto &e : graph[v]) {
                if(e.cap > 1e-12 && level[e.to] == -1) {
                    level[e.to] = level[v] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] != -1;
    }

    double dfs(int v, int t, double pushed) {
        if(v == t) return pushed;
        while(ptr[v] < graph[v].size()) {
            Edge &e = graph[v][ptr[v]];
            if(e.cap > 1e-12 && level[e.to] == level[v] + 1) {
                double tr = dfs(e.to, t, min(pushed, e.cap));
                if(tr > 1e-12) {
                    e.cap -= tr;
                    graph[e.to][e.rev].cap += tr;
                    return tr;
                }
            }
            ptr[v]++;
        }
        return 0.0;
    }

    double max_flow(int s, int t) {
        double flow = 0.0;
        while(bfs(s, t)) {
            fill(ptr.begin(), ptr.end(), 0);
            while(true) {
                double pushed = dfs(s, t, 1e18);
                if(pushed < 1e-12) break;
                flow += pushed;
            }
        }
        return flow;
    }
};

struct Pipe {
    int j, k;
    double c;
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, p;
    double v, a;
    cin >> n >> p >> v >> a;
    vector<Pipe> pipes(p);
    for(int i=0;i<p;i++) cin >> pipes[i].j >> pipes[i].k >> pipes[i].c;
    // Nodes are 1-based, convert to 0-based
    // FD is node3
    int factory = 0; // node1
    int water_source = 1; // node2
    int FD = 2; // node3
    double k = a / (1.0 - a);
    // Binary search on W
    double low = 0.0;
    double high = 0.0;
    for(int i=0;i<p;i++) high += pipes[i].c / (1.0 + v * k);
    double eps = 1e-7;
    double best_W = 0.0;
    // To store best flows
    vector<pair<double, double>> best_flows(p, {0.0, 0.0});
    while(high - low > eps){
        double mid = (low + high) / 2.0;
        double W = mid;
        double F = k * W;
        // Build FlowW network
        FlowNetwork flowW(n, 200000);
        // To store edge indices
        vector<pair<int, int>> flowW_edges(p);
        for(int i=0;i<p;i++){
            int u = pipes[i].j - 1;
            int v_ = pipes[i].k - 1;
            double cap = pipes[i].c / (1.0 + v * k);
            flowW.add_edge(u, v_, cap);
            flowW.add_edge(v_, u, cap);
            // Store the indices
            flowW_edges[i].first = flowW.graph[u].size()-2; // forward edge
            flowW_edges[i].second = flowW.graph[v_].size()-2; // backward edge
        }
        double flowW_val = flowW.max_flow(water_source, FD);
        if(flowW_val + 1e-9 < W){
            high = mid;
            continue;
        }
        // Now, retrieve W flows
        // Initialize remaining capacities for FlowF
        // First, get W flows
        // To get the flows, we need to reconstruct the flows
        // Flows in FlowW.graph are residual capacities
        // Original capacities were c / (1 + v k)
        // Flow = original capacity - residual capacity
        vector<double> W_forward(p, 0.0);
        vector<double> W_backward(p, 0.0);
        for(int i=0;i<p;i++){
            int u = pipes[i].j - 1;
            int v_ = pipes[i].k - 1;
            // Forward edge
            double original_cap = pipes[i].c / (1.0 + v * k);
            double residual_cap = flowW.graph[u][flowW_edges[i].first].cap;
            double flow = original_cap - residual_cap;
            W_forward[i] = flow;
            // Backward edge
            residual_cap = flowW.graph[v_][flowW_edges[i].second].cap;
            flow = (pipes[i].c / (1.0 + v * k)) - residual_cap;
            W_backward[i] = flow;
        }
        // Now, compute remaining capacities for FlowF
        // (c - (1 + v k) W) / v
        vector<double> remaining_cap(p, 0.0);
        bool feasible = true;
        for(int i=0;i<p;i++){
            double total_W = W_forward[i] + W_backward[i];
            double rem = (pipes[i].c - (1.0 + v * k) * total_W) / v;
            if(rem < -1e-9){
                feasible = false;
                break;
            }
            remaining_cap[i] = rem;
        }
        if(!feasible){
            high = mid;
            continue;
        }
        // Build FlowF network
        FlowNetwork flowF(n, 200000);
        vector<pair<int, int>> flowF_edges(p);
        for(int i=0;i<p;i++){
            int u = pipes[i].j - 1;
            int v_ = pipes[i].k - 1;
            double cap = remaining_cap[i];
            flowF.add_edge(u, v_, cap);
            flowF.add_edge(v_, u, cap);
            // Store the indices
            flowF_edges[i].first = flowF.graph[u].size()-2; // forward edge
            flowF_edges[i].second = flowF.graph[v_].size()-2; // backward edge
        }
        double flowF_val = flowF.max_flow(factory, FD);
        if(flowF_val + 1e-9 >= F){
            // Feasible, store the flows
            best_W = W;
            // Retrieve F flows
            vector<double> F_forward(p, 0.0);
            vector<double> F_backward(p, 0.0);
            for(int i=0;i<p;i++){
                int u = pipes[i].j - 1;
                int v_ = pipes[i].k - 1;
                // Forward edge
                double original_cap = remaining_cap[i];
                double residual_cap = flowF.graph[u][flowF_edges[i].first].cap;
                double flow = original_cap - residual_cap;
                F_forward[i] = flow;
                // Backward edge
                residual_cap = flowF.graph[v_][flowF_edges[i].second].cap;
                flow = remaining_cap[i] - residual_cap;
                F_backward[i] = flow;
            }
            // Store the flows
            for(int i=0;i<p;i++){
                // Flubber flow
                double net_F = F_forward[i] - F_backward[i];
                // Water flow
                double net_W = W_forward[i] - W_backward[i];
                best_flows[i].first = net_F;
                best_flows[i].second = net_W;
            }
            low = mid;
        }
        else{
            high = mid;
        }
    }
    // After binary search, best_W and best_flows are set
    double F_final = k * best_W;
    // Compute F^a * W^(1 - a)
    double value = pow(F_final, a) * pow(best_W, 1.0 - a);
    // Now, need to output flows per original pipe
    for(int i=0;i<p;i++){
        printf("%.9lf %.9lf\n", best_flows[i].first, best_flows[i].second);
    }
    printf("%.11lf\n", value);
}
