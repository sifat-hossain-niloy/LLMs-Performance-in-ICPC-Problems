#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

// Fast IO
struct FastIO {
    FastIO() {
        ios::sync_with_stdio(false);
        cin.tie(nullptr);
    }
};

// BIT implementation for range queries
struct BIT {
    int n;
    vector<int> tree;

    BIT(int size) : n(size), tree(size + 2, 0) {}

    void update(int idx, int val=1) {
        while (idx <= n) {
            tree[idx] += val;
            idx += idx & -idx;
        }
    }

    int query(int l, int r) const {
        // Inclusive
        auto sum = [&](int x) -> int {
            int res =0;
            int y = x;
            while (y >0) {
                res += tree[y];
                y -= y & -y;
            }
            return res;
        };
        return sum(r) - sum(l -1);
    }
};

// Structure to hold information about nodes
struct Node {
    int id;
    int d; // destination
    bool in_cycle;
    int cycle_id;
    int pos_in_cycle;
    int depth; // depth in tree
};

// Euler Tour structure
struct EulerTour {
    int n;
    vector<vector<int>> adj;
    vector<int> in_time;
    vector<int> out_time;
    int timer;

    EulerTour(int size) : n(size), adj(size +1), in_time(size +1, 0), out_time(size +1, 0), timer(1) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
    }

    void dfs(int u) {
        in_time[u] = timer;
        timer++;
        for(auto &v: adj[u]){
            dfs(v);
        }
        out_time[u] = timer -1;
    }
};

int main(){
    FastIO fio;
    int n, k;
    cin >> n >> k;
    vector<int> d(n +1);
    for(int i=1;i<=n;i++) cin >> d[i];
    
    // Step1: Decompose the graph into cycles and trees
    vector<Node> nodes(n +1, Node{0, 0, false, 0, 0, 0});
    for(int i=1;i<=n;i++) {
        nodes[i].id =i;
        nodes[i].d =d[i];
    }
    
    // To find cycles and assign cycle_id and pos_in_cycle
    int cycle_cnt =0;
    vector<int> visited(n +1, 0); // 0: unvisited, 1: visiting, 2: visited
    vector<int> which_cycle(n +1, -1);
    vector<vector<int>> cycles;
    for(int i=1;i<=n;i++) {
        if(!visited[i]){
            vector<int> path;
            int u =i;
            while(!visited[u]){
                visited[u] =1;
                path.push_back(u);
                u = nodes[u].d;
            }
            if(visited[u] ==1){
                // Found a cycle
                int idx =0;
                while(idx <path.size() && path[idx] !=u) idx++;
                if(idx <path.size()){
                    vector<int> cycle;
                    for(int j=idx; j<path.size(); j++) cycle.push_back(path[j]);
                    cycles.push_back(cycle);
                    for(auto &c: cycle){
                        nodes[c].in_cycle = true;
                        nodes[c].cycle_id = cycle_cnt;
                        nodes[c].pos_in_cycle = &cycles[cycle_cnt] - &cycles[0] +1; // Temporary
                    }
                    cycle_cnt++;
                }
            }
            for(auto &c: path){
                if(!nodes[c].in_cycle){
                    nodes[c].cycle_id = -1;
                }
                visited[c] =2;
            }
        }
    }
    
    // Assign proper pos_in_cycle
    for(int cid=0; cid <cycle_cnt; cid++){
        for(int i=0; i<cycles[cid].size(); i++){
            int u = cycles[cid][i];
            nodes[u].pos_in_cycle =i;
        }
    }
    
    // Build trees: reversed graph
    // For trees, build adjacency list for reversed graph
    // Also, assign trees to cycles
    // To perform Euler Tour, assign each tree to its cycle root
    // For each cycle, collect trees attached to it
    // We need to know the trees attached to each cycle node
    // Implemented as separate trees
    
    // Assign trees
    // For each node not in cycle, find its cycle and depth
    // Implement BFS from cycle nodes
    vector<int> depth(n +1, -1);
    vector<int> cycle_root(n +1, -1);
    queue<int> q;
    for(int cid=0; cid <cycle_cnt; cid++){
        for(auto &u: cycles[cid]){
            depth[u] =0;
            cycle_root[u] =u;
            q.push(u);
        }
    }
    // Build reversed graph
    vector<vector<int>> rev_graph(n +1, vector<int>());
    for(int i=1;i<=n;i++) rev_graph[d[i]].push_back(i);
    while(!q.empty()){
        int u =q.front(); q.pop();
        for(auto &v: rev_graph[u]){
            if(depth[v]==-1){
                depth[v] = depth[u] +1;
                cycle_root[v] = cycle_root[u];
                q.push(v);
            }
        }
    }
    
    // Now, for trees attached to cycle nodes, collect them
    // For each cycle, collect the trees attached to its nodes
    vector<vector<int>> trees_attached(cycle_cnt, vector<int>());
    for(int i=1;i<=n;i++) {
        if(!nodes[i].in_cycle){
            int root = cycle_root[i];
            // Find which cycle this root belongs to
            // Find the cycle id
            int cid =-1;
            for(int c=0; c <cycle_cnt; c++) {
                if(find(cycles[c].begin(), cycles[c].end(), root) != cycles[c].end()){
                    cid =c;
                    break;
                }
            }
            if(cid !=-1){
                trees_attached[cid].push_back(i);
            }
        }
    }
    
    // Now, perform Euler Tour for trees
    // Assign in_time and out_time
    // Since trees are forests, perform separate Euler Tours
    // We need to build the adjacency list for trees
    // Reversed graph already built
    // So build adjacency for reversed trees
    // Each tree node points to its children in the reversed tree
    // Which are the nodes that point to it
    // So reversed trees are from cycle nodes outwards
    // Implement Euler Tour per tree
    // Assign in_time and out_time
    // Assign depths are already assigned
    // But we don't need to answer queries for trees, just count number of nodes in subtree with depth <=k
    
    // To count for trees, we can precompute for each node the number of descendants within k steps
    // Implemented via sorting nodes by depth and using a BIT
    // But it's easier to implement separately
    
    // Implement for trees:
    // For each tree, collect nodes and their depths
    // Sort nodes by depth
    // Use BIT to count number of nodes in subtree with depth <=k
    // Implemented as follows:
    
    // Prepare trees attached to cycles
    // For each cycle, build trees as separate trees
    // Assign in_time and out_time
    // To do this, perform DFS for each tree attached to cycle nodes
    // But multiple trees can be attached to a cycle node
    
    // Implement Euler Tour
    // Assign unique in_time
    int timer =1;
    vector<int> in_time(n +1, 0);
    vector<int> out_time_time(n +1, 0);
    // Build adjacency list for reversed trees
    vector<vector<int>> tree_adj(n +1, vector<int>());
    for(int i=1;i<=n;i++) {
        if(!nodes[i].in_cycle){
            tree_adj[d[i]].push_back(i);
        }
    }
    // Perform DFS for each tree
    for(int cid=0; cid <cycle_cnt; cid++){
        for(auto &u: cycles[cid]){
            // u is the root of trees attached to it
            for(auto &v: tree_adj[u]){
                // DFS from v
                stack<pair<int, int>> s;
                s.emplace(v, 0);
                while(!s.empty()){
                    auto [node, child_idx] = s.top(); s.pop();
                    if(in_time[node] ==0){
                        in_time[node] =timer;
                        timer++;
                        // Push out_time
                        s.emplace(node, -1);
                        // Push children
                        for(auto &child: tree_adj[node]){
                            s.emplace(child, 0);
                        }
                    }
                    else{
                        out_time_time[node] =timer -1;
                    }
                }
            }
        }
    }
    
    // Now, prepare for tree queries
    // Collect all tree nodes with their depth
    struct TreeNode {
        int in_t;
        int depth;
    };
    vector<TreeNode> tree_nodes;
    for(int i=1;i<=n;i++) {
        if(!nodes[i].in_cycle){
            tree_nodes.push_back(TreeNode{in_time[i], depth[i]});
        }
    }
    // Sort tree nodes by depth
    sort(tree_nodes.begin(), tree_nodes.end(), [&](const TreeNode &a, const TreeNode &b) -> bool {
        return a.depth < b.depth;
    });
    
    // Prepare queries: for each tree node, query number of nodes in subtree with depth <=k
    // Implemented via sorted tree_nodes and BIT
    // But we don't need to answer, instead we can compute the counts directly
    // Instead, precompute for each tree node the number of descendants within k steps
    // Implemented via a BFS-like approach
    // However, it's complicated, proceed with the BIT approach
    
    // Initialize BIT
    BIT bit(timer);
    // Sort tree nodes by depth
    sort(tree_nodes.begin(), tree_nodes.end(), [&](const TreeNode &a, const TreeNode &b) -> bool {
        return a.depth < b.depth;
    });
    
    // Prepare queries
    // For each tree node, the upper depth is depth +k
    // Sort queries by upper depth
    // Prepare a list of queries sorted by upper depth
    struct Query {
        int node;
        int in_t;
        int out_t;
        int upper_depth;
    };
    vector<Query> queries;
    for(int i=1;i<=n;i++) {
        if(!nodes[i].in_cycle){
            queries.push_back(Query{ i, in_time[i], out_time_time[i], depth[i] +k});
        }
    }
    // Sort queries by upper_depth
    sort(queries.begin(), queries.end(), [&](const Query &a, const Query &b) -> bool {
        return a.upper_depth < b.upper_depth;
    });
    // Now, iterate through tree_nodes and answer queries
    int q_ptr =0;
    vector<int> tree_answer(n +1,0);
    for(auto &tn: tree_nodes){
        // Insert into BIT
        bit.update(tn.in_t, 1);
        // Answer queries with upper_depth == tn.depth
        while(q_ptr <queries.size() && queries[q_ptr].upper_depth == tn.depth){
            int cnt = bit.query(queries[q_ptr].in_t, queries[q_ptr].out_t);
            tree_answer[queries[q_ptr].node] = cnt;
            q_ptr++;
        }
    }
    // Handle queries with upper_depth > max depth
    while(q_ptr <queries.size()){
        int cnt = bit.query(queries[q_ptr].in_t, queries[q_ptr].out_t);
        tree_answer[queries[q_ptr].node] = cnt;
        q_ptr++;
    }
    
    // Now, handle cycles
    // For each cycle, compute the sum as described
    // Assign the answer for cycle nodes
    // Also, collect trees attached to cycle nodes
    // To simplify, collect trees attached to each cycle node
    vector<vector<int>> trees_per_cycle(cycle_cnt, vector<int>());
    for(int i=1;i<=n;i++) {
        if(!nodes[i].in_cycle){
            int root = cycle_root[i];
            // Find cycle id
            int cid =-1;
            for(int c=0; c <cycle_cnt; c++) {
                if(find(cycles[c].begin(), cycles[c].end(), root) != cycles[c].end()){
                    cid =c;
                    break;
                }
            }
            if(cid !=-1){
                trees_per_cycle[cid].push_back(i);
            }
        }
    }
    
    // For each cycle, collect trees attached to each cycle node
    // and sort their depths
    // To map trees to their cycle node's position
    vector<vector<int>> trees_sorted(cycle_cnt, vector<int>());
    // Also, for each cycle node, store their trees' depths
    vector<vector<int>> trees_depth(cycle_cnt, vector<int>());
    for(int cid=0; cid <cycle_cnt; cid++){
        // For trees attached to this cycle
        // Collect their depths
        // The depth of tree node is distance from cycle node
        // But we need the distance from tree node to cycle node, which is depth[i}
        // So, for each tree node, d = depth[i}
        for(auto &u: trees_per_cycle[cid]){
            trees_depth[cid].push_back(depth[u]);
        }
        // Sort the depths
        sort(trees_depth[cid].begin(), trees_depth[cid].end());
    }
    
    // Now, for each cycle, compute F[C][l} and sum[A}
    // Store the answer for cycle nodes
    vector<int> cycle_answer(n +1,0);
    for(int cid=0; cid <cycle_cnt; cid++){
        int c_size = cycles[cid].size();
        // min(k, c_size -1}
        int l_max = min(k, c_size -1);
        // Precompute F[C][l} for all C and l
        // F[C][l} = number of trees attached to C with d <=k - l}
        // Implemented with pointers
        vector<int> F(c_size,0);
        // For each cycle node, precompute F[C][l}
        // We can store F as a vector of vectors
        // But to save space, compute on the fly
        // Initialize F for all C
        vector<vector<int>> F_list(c_size, vector<int>(l_max +1,0));
        for(int i=0;i<c_size;i++){
            int C =i;
            int p = trees_depth[cid][C].size();
            // Since trees_depth[cid][C] is sorted in increasing order
            // We can iterate l from0 to l_max and find number of trees with d <=k - l
            // Initialize pointer
            int ptr = trees_depth[cid].size();
            for(int l=0; l <=l_max; l++){
                if(k - l <0){
                    F_list[C][l} =0;
                    continue;
                }
                // Find number of trees with d <=k - l
                // Using upper_bound
                F_list[C][l} = upper_bound(trees_depth[cid].begin(), trees_depth[cid].end(), k - l) - trees_depth[cid].begin();
            }
        }
        // Now, compute sum[A} =c + sum over l=0 to l_max of F[C][l} where C=(A - l) modc}
        // Implemented with nested loops
        // Initialize sum[A} =c
        vector<int> sum(c_size, cycle_cnt >0 ? 0 :0);
        // Initialize sum[A} with c_size
        for(int i=0;i<c_size;i++) sum[i] += c_size;
        // Now, add F[C][l} for all C and l
        for(int l=0; l <=l_max; l++){
            for(int C=0; C <c_size; C++){
                int A = (C + l) %c_size;
                sum[A] += F_list[C][l];
            }
        }
        // Assign sum to cycle nodes
        for(int i=0;i<c_size;i++){
            int u = cycles[cid][i];
            cycle_answer[u] =sum[i];
        }
    }
    
    // Now, prepare the final answers
    // For nodes in trees, the answer is tree_answer[i}
    // For nodes in cycles, the answer is cycle_answer[i}
    vector<int> answer(n +1,0);
    for(int i=1;i<=n;i++) {
        if(!nodes[i].in_cycle){
            answer[i] =tree_answer[i];
        }
        else{
            answer[i] =cycle_answer[i];
        }
    }
    // Output the answers
    for(int i=1;i<=n;i++) cout << answer[i] << "\n";
}
