#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int N;
    int case_num = 1;
    while(cin >> N){
        if(N == 0) break;
        // Build graph
        // Find maximum node number
        int max_node = 0;
        vector<pair<int, int>> edges(N);
        for(int i=0;i<N;i++){
            cin >> edges[i].first >> edges[i].second;
            max_node = max(max_node, max(edges[i].first, edges[i].second));
        }
        int V = max_node;
        vector<vector<int>> adj(V+1, vector<int>());
        for(auto &[u, v] : edges){
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        // Tarjan's algorithm
        int time_dfs = 0;
        vector<int> disc(V+1, -1);
        vector<int> low(V+1, -1);
        vector<bool> is_articulation(V+1, false);
        stack<pair<int, int>> st;
        vector<vector<int>> bcc; // list of biconnected components
        // To handle multiple components, though the graph is connected
        // But implement generally
        // Assume the graph is connected
        // DFS from node 1
        function<void(int, int)> dfs = [&](int u, int parent_u)-> void{
            disc[u] = low[u] = ++time_dfs;
            int children = 0;
            for(auto &v: adj[u]){
                if(v == parent_u) continue;
                if(disc[v] == -1){
                    st.emplace(u, v);
                    dfs(v, u);
                    low[u] = min(low[u], low[v]);
                    if(low[v] >= disc[u]){
                        // u is an articulation point
                        if(parent_u != -1){
                            is_articulation[u] = true;
                        }
                        // Pop all edges from stack until (u,v)
                        vector<int> component;
                        while(!st.empty()){
                            auto [x, y] = st.top(); st.pop();
                            component.push_back(x);
                            component.push_back(y);
                            if(x == u && y == v) break;
                        }
                        // Remove duplicates
                        sort(component.begin(), component.end());
                        component.erase(unique(component.begin(), component.end()), component.end());
                        bcc.emplace_back(component);
                    }
                    children++;
                }
                else if(disc[v] < disc[u] && v != parent_u){
                    low[u] = min(low[u], disc[v]);
                    st.emplace(u, v);
                }
            }
            if(parent_u == -1 && children >1){
                is_articulation[u] = true;
                // Pop remaining edges as a biconnected component
                vector<int> component;
                while(!st.empty()){
                    auto [x, y] = st.top(); st.pop();
                    component.push_back(x);
                    component.push_back(y);
                }
                // Remove duplicates
                sort(component.begin(), component.end());
                component.erase(unique(component.begin(), component.end()), component.end());
                if(!component.empty()) bcc.emplace_back(component);
            }
        };
        dfs(1, -1);
        // Find articulation points
        // Now, mark articulation points
        // Already marked in is_articulation
        // Now, process biconnected components
        // To count minimal escape shafts and number of ways
        ll min_shaft = 0;
        ll ways = 1;
        for(auto &component : bcc){
            int non_articulation = 0;
            for(auto &u : component){
                if(!is_articulation[u]){
                    non_articulation++;
                }
            }
            if(non_articulation >0){
                min_shaft +=1;
                ways *= non_articulation;
            }
            else{
                // All are articulation points
                int k = 0;
                for(auto &u : component){
                    if(is_articulation[u]) k++;
                }
                if(k >=2){
                    min_shaft += (k -1);
                    ways *= (ll)k;
                }
                // If k <2, nothing to do
            }
        }
        // Now, it's possible that some articulation points are not in any biconnected component with non-articulation points
        // For example, if the entire graph is a single biconnected component with only articulation points
        // But in standard Tarjan's algorithm, such cases are handled
        // Now, print the result
        cout << "Case " << case_num++ << ": " << min_shaft << " " << ways << "\n";
    }
}
