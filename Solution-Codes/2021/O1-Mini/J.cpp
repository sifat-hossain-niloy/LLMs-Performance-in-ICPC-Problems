#include <bits/stdc++.h>
using namespace std;

// Define node types
enum NodeType { I_TYPE, S_Y_TYPE, S_Z_TYPE, M_TYPE };

// Structure to represent each node
struct Node {
    NodeType type;
    int a;
    int b;
} nodes_struct;

// Maximum possible output identifier
const int MAX_X = 200005;

// Arrays to store node information, lengths, in-degrees, and dependents
Node nodes[MAX_X];
long long len_[MAX_X];
int in_degree_arr[MAX_X];
vector<int> dependents[MAX_X];

// Function to traverse back to the input node and find the k-th number
long long traverse(int x, long long k, long long m) {
    while(x != 1){
        if(nodes[x].type == S_Y_TYPE){
            long long a_index = (k - 1) * 2 + 1;
            if(a_index < 1 || a_index > len_[nodes[x].a]) return -1;
            x = nodes[x].a;
            k = a_index;
        }
        else if(nodes[x].type == S_Z_TYPE){
            long long a_index = k * 2;
            if(a_index < 1 || a_index > len_[nodes[x].a]) return -1;
            x = nodes[x].a;
            k = a_index;
        }
        else if(nodes[x].type == M_TYPE){
            if(k % 2 == 1){
                long long a_index = (k + 1) / 2;
                if(a_index < 1 || a_index > len_[nodes[x].a]) return -1;
                x = nodes[x].a;
                k = a_index;
            }
            else{
                long long b_index = k / 2;
                if(b_index < 1 || b_index > len_[nodes[x].b]) return -1;
                x = nodes[x].b;
                k = b_index;
            }
        }
        else{
            return -1;
        }
    }
    if(1 <= k && k <= m){
        return k;
    }
    else{
        return -1;
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    
    long long m;
    int n, q;
    cin >> m >> n >> q;
    
    // Initialize all nodes to I_TYPE
    for(int x = 1; x < MAX_X; x++){
        nodes[x].type = I_TYPE;
        nodes[x].a = 0;
        nodes[x].b = 0;
    }
    
    // Read n nodes and build the dependency graph
    for(int i = 0; i < n; i++){
        char type;
        cin >> type;
        if(type == 'S'){
            int a, y, z;
            cin >> a >> y >> z;
            nodes[y].type = S_Y_TYPE;
            nodes[y].a = a;
            nodes[y].b = 0;
            nodes[z].type = S_Z_TYPE;
            nodes[z].a = a;
            nodes[z].b = 0;
            dependents[a].push_back(y);
            dependents[a].push_back(z);
        }
        else if(type == 'M'){
            int a, b, z;
            cin >> a >> b >> z;
            nodes[z].type = M_TYPE;
            nodes[z].a = a;
            nodes[z].b = b;
            dependents[a].push_back(z);
            dependents[b].push_back(z);
        }
    }
    
    // Initialize in-degrees
    for(int x = 1; x < MAX_X; x++){
        if(x == 1){
            in_degree_arr[x] = 0;
        }
        else{
            if(nodes[x].type == S_Y_TYPE || nodes[x].type == S_Z_TYPE){
                in_degree_arr[x] = 1;
            }
            else if(nodes[x].type == M_TYPE){
                in_degree_arr[x] = 2;
            }
            else{
                in_degree_arr[x] = -1; // Undefined
            }
        }
    }
    
    // Initialize lengths
    for(int x = 1; x < MAX_X; x++){
        len_[x] = 0;
    }
    len_[1] = m;
    
    // Perform Kahn's algorithm for topological sorting
    queue<int> q_queue;
    if(in_degree_arr[1] == 0){
        q_queue.push(1);
    }
    
    while(!q_queue.empty()){
        int x = q_queue.front();
        q_queue.pop();
        
        for(auto &y : dependents[x]){
            if(in_degree_arr[y] > 0){
                in_degree_arr[y]--;
                if(in_degree_arr[y] == 0){
                    // Compute len[y]
                    if(nodes[y].type == S_Y_TYPE){
                        len_[y] = (len_[nodes[y].a] + 1) / 2;
                    }
                    else if(nodes[y].type == S_Z_TYPE){
                        len_[y] = len_[nodes[y].a] / 2;
                    }
                    else if(nodes[y].type == M_TYPE){
                        len_[y] = len_[nodes[y].a] + len_[nodes[y].b];
                    }
                    q_queue.push(y);
                }
            }
        }
    }
    
    // Process each query
    while(q--){
        int x;
        long long k;
        cin >> x >> k;
        if(x >= MAX_X || len_[x] < k || len_[x] == 0){
            cout << "none\n";
            continue;
        }
        long long res = traverse(x, k, m);
        if(res != -1){
            cout << res << "\n";
        }
        else{
            cout << "none\n";
        }
    }
}
