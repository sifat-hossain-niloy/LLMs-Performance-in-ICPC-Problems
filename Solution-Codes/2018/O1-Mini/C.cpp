#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

struct Edge {
    int to;
    int cost;
    int id;
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    cin >> n;
    vector<vector<Edge>> adj(n+1);
    for(int i=0;i<n-1;i++){
        int u,v,c;
        cin >> u >> v >> c;
        adj[u].push_back(Edge{v,c,i});
        adj[v].push_back(Edge{u,c,i});
    }
    // Read xi and yi
    vector<ll> net(n+1, 0);
    for(int i=1;i<=n;i++){
        ll xi, yi;
        cin >> xi >> yi;
        net[i] = xi - yi;
    }
    ll total_cost =0;
    // Implement iterative post-order DFS
    // Stack elements: {node, parent, next_child_index, accumulated_sum}
    struct StackFrame {
        int node;
        int parent;
        int next_child;
        ll accumulated;
    };
    stack<StackFrame> stk;
    stk.push(StackFrame{1, 0, 0, 0});
    // To store the sum_flow for each node
    vector<ll> sum_flow(n+1, 0);
    while(!stk.empty()){
        StackFrame &top = stk.top();
        int node = top.node;
        int parent = top.parent;
        if(top.next_child < adj[node].size()){
            // Process next child
            Edge child = adj[node][top.next_child];
            stk.top().next_child++;
            if(child.to != parent){
                stk.push(StackFrame{child.to, node, 0, 0});
            }
        }
        else{
            // All children processed
            stk.pop();
            ll current_flow = net[node];
            // Iterate through children
            for(auto &edge: adj[node]){
                if(edge.to != parent){
                    current_flow += sum_flow[edge.to];
                    total_cost += abs(sum_flow[edge.to]) * (ll)edge.cost;
                }
            }
            sum_flow[node] = current_flow;
            // If node is root, do not pass up
            // Otherwise, flows will be handled by the parent
        }
    }
    cout << total_cost;
}
