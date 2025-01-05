#include <bits/stdc++.h>
using namespace std;

struct Node {
    bool is_leaf;
    int k; // number of children
    vector<int> children; // 1-based indexing
    long long x, y;
    long long quality;
} nodes[10005];

int n;

// Recursive function to compute x, y and quality for node_id
void compute_node(int node_id){
    if(nodes[node_id].is_leaf){
        nodes[node_id].quality = nodes[node_id].x * nodes[node_id].x + nodes[node_id].y * nodes[node_id].y;
        return;
    }
    // Compute all children first
    for(auto child_id: nodes[node_id].children){
        compute_node(child_id);
    }
    // Compute sum of x and y
    long long x_sum=0, y_sum=0;
    for(auto child_id: nodes[node_id].children){
        x_sum += nodes[child_id].x;
        y_sum += nodes[child_id].y;
    }
    // Iterate over children to find maximum quality
    long long max_quality = -1;
    long long final_x = 0, final_y =0;
    for(auto child_id: nodes[node_id].children){
        long long x_new = x_sum - 2 * nodes[child_id].x;
        long long y_new = y_sum - 2 * nodes[child_id].y;
        long long q = x_new * x_new + y_new * y_new;
        if(q > max_quality){
            max_quality = q;
            final_x = x_new;
            final_y = y_new;
        }
    }
    nodes[node_id].x = final_x;
    nodes[node_id].y = final_y;
    nodes[node_id].quality = max_quality;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    cin >> n;
    // Read nodes from 1 to n
    for(int i=1;i<=n;i++){
        int first;
        cin >> first;
        if(first ==0){
            // Leaf node
            nodes[i].is_leaf = true;
            cin >> nodes[i].x >> nodes[i].y;
        }
        else{
            // Internal node
            nodes[i].is_leaf = false;
            nodes[i].k = first;
            nodes[i].children.resize(first);
            for(int j=0;j<first;j++) cin >> nodes[i].children[j];
        }
    }
    compute_node(1);
    cout << nodes[1].quality;
}
