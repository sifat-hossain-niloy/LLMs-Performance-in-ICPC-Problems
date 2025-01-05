#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, m;
    cin >> n >> m;
    vector<int> d(n, 0);
    for(int i=0;i<m;i++){
        int a, b;
        cin >> a >> b;
        d[a]++;
        d[b]++;
    }
    // Create list of nodes with their w_i = d_i -1
    struct Node {
        int id;
        int w;
    };
    vector<Node> nodes;
    for(int i=0;i<n;i++){
        nodes.push_back(Node{ i, d[i]-1 });
    }
    // Sort nodes by w ascending, then by id ascending
    sort(nodes.begin(), nodes.end(), [&](const Node &a, const Node &b) -> bool{
        if(a.w != b.w) return a.w < b.w;
        return a.id < b.id;
    });
    // Initial selection: include as many nodes as possible with sum(w_i) <=n-2
    vector<bool> inS(n, false);
    int sum =0;
    for(auto &node: nodes){
        if(sum + node.w <= n-2){
            inS[node.id] = true;
            sum += node.w;
        }
    }
    // Now, if sum <n-2, try to replace nodes in S with nodes not in S to increase sum
    if(sum < n-2){
        // Get list of S nodes sorted by w_i ascending
        vector<Node> S_nodes;
        vector<Node> notS_nodes;
        for(auto &node: nodes){
            if(inS[node.id]){
                S_nodes.push_back(node);
            }
            else{
                notS_nodes.push_back(node);
            }
        }
        // Sort S_nodes by w ascending
        sort(S_nodes.begin(), S_nodes.end(), [&](const Node &a, const Node &b) -> bool{
            if(a.w != b.w) return a.w < b.w;
            return a.id < b.id;
        });
        // Sort notS_nodes by w descending
        sort(notS_nodes.begin(), notS_nodes.end(), [&](const Node &a, const Node &b) -> bool{
            if(a.w != b.w) return a.w > b.w;
            return a.id > b.id;
        });
        bool updated = true;
        while(sum < n-2 && updated){
            updated = false;
            for(int i=0;i<S_nodes.size();i++){
                for(int j=0;j<notS_nodes.size();j++){
                    int new_sum = sum - S_nodes[i].w + notS_nodes[j].w;
                    if(new_sum <= n-2){
                        // Replace
                        inS[S_nodes[i].id] = false;
                        inS[notS_nodes[j].id] = true;
                        sum = new_sum;
                        // Update S_nodes and notS_nodes
                        // Remove S_nodes[i] and add notS_nodes[j]
                        S_nodes.erase(S_nodes.begin() + i);
                        notS_nodes.erase(notS_nodes.begin() + j);
                        S_nodes.push_back(notS_nodes[j]); // Add to S_nodes
                        // Re-sort S_nodes and notS_nodes
                        sort(S_nodes.begin(), S_nodes.end(), [&](const Node &a, const Node &b) -> bool{
                            if(a.w != b.w) return a.w < b.w;
                            return a.id < b.id;
                        });
                        sort(notS_nodes.begin(), notS_nodes.end(), [&](const Node &a, const Node &b) -> bool{
                            if(a.w != b.w) return a.w > b.w;
                            return a.id > b.id;
                        });
                        updated = true;
                        break;
                    }
                }
                if(updated) break;
            }
        }
    }
    // Now, ensure sum ==n-2
    // It should be, according to problem constraints
    // Now, prepare the degrees for the tree
    vector<int> t(n, 1);
    for(int i=0;i<n;i++) t[i] = 1;
    for(int i=0;i<n;i++) {
        if(inS[i]){
            t[i] = d[i];
        }
    }
    // Now, check sum(t)=2(n-1)
    long long total =0;
    for(int i=0;i<n;i++) total += t[i];
    // It should be exactly 2*(n-1)
    // Now, build the tree using a Prufer-like algorithm
    // Initialize degrees for tree
    vector<int> tree_deg(n, 0);
    for(int i=0;i<n;i++) tree_deg[i] = t[i];
    // Initialize min-heap for leaves
    priority_queue<int, vector<int>, std::greater<int>> leaves;
    // Initialize min-heap for available nodes
    priority_queue<int, vector<int>, std::greater<int>> available;
    for(int i=0;i<n;i++){
        if(tree_deg[i]==1){
            leaves.push(i);
        }
        else if(tree_deg[i] >1){
            available.push(i);
        }
    }
    vector<pair<int, int>> edges;
    while(leaves.size() >0 && available.size() >0){
        int leaf = leaves.top();
        leaves.pop();
        if(available.empty()){
            break;
        }
        int node = available.top();
        available.pop();
        edges.emplace_back(leaf, node);
        tree_deg[node]--;
        tree_deg[leaf]--;
        if(tree_deg[node]==1){
            leaves.push(node);
        }
        else{
            available.push(node);
        }
    }
    // Now, connect the last two nodes
    // Collect remaining nodes with degree==1
    vector<int> remaining;
    for(int i=0;i<n;i++) if(tree_deg[i]==1) remaining.push_back(i);
    while(remaining.size() >1){
        int a = remaining.back(); remaining.pop_back();
        int b = remaining.back(); remaining.pop_back();
        edges.emplace_back(a, b);
    }
    // Now, count the number of nodes changed
    int changed =0;
    for(int i=0;i<n;i++) if(t[i] != d[i]) changed++;
    // Output
    cout << changed << "\n";
    cout << n << " " << n-1 << "\n";
    for(auto &[a, b] : edges){
        cout << a << " " << b << "\n";
    }
}
