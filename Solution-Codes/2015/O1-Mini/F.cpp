#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int r, c;
    cin >> r >> c;
    vector<string> grid(r);
    for(int i=0;i<r;i++) cin >> grid[i];
    // Assign unique key indices
    int key_count = 0;
    int char_to_key[128];
    memset(char_to_key, -1, sizeof(char_to_key));
    // To keep the order consistent, iterate row-wise
    for(int i=0;i<r;i++){
        for(int j=0;j<c;j++){
            char ch = grid[i][j];
            if(char_to_key[ch] == -1){
                char_to_key[ch] = key_count++;
            }
        }
    }
    // Collect cells for each key
    vector<vector<pair<int,int>>> key_cells(key_count, vector<pair<int,int>>());
    for(int i=0;i<r;i++){
        for(int j=0;j<c;j++){
            char ch = grid[i][j];
            int k = char_to_key[ch];
            key_cells[k].emplace_back(i, j);
        }
    }
    // Find Enter key index
    if(char_to_key['*'] == -1){
        // Should not happen as per problem statement
        return 0;
    }
    int enter_key = char_to_key['*'];
    // Read the text
    string text;
    cin >> text;
    // Directions: up, down, left, right
    int dx[4] = {-1, 1, 0, 0};
    int dy[4] = {0, 0, -1, 1};
    // Build adjacency list
    vector<vector<int>> adj(key_count, vector<int>());
    for(int A=0; A<key_count; A++){
        // For each direction
        for(int d=0; d<4; d++){
            set<int> reachable;
            for(auto &[x, y] : key_cells[A]){
                int nx = x + dx[d];
                int ny = y + dy[d];
                while(nx >=0 && nx < r && ny >=0 && ny < c){
                    char ch = grid[nx][ny];
                    int B = char_to_key[ch];
                    if(B != A){
                        reachable.insert(B);
                        break;
                    }
                    nx += dx[d];
                    ny += dy[d];
                }
            }
            for(auto B : reachable){
                adj[A].push_back(B);
            }
        }
    }
    // Precompute movement costs
    const int INF = 1e9;
    vector<vector<int>> movement_cost(key_count, vector<int>(key_count, INF));
    for(int A=0; A<key_count; A++){
        // BFS from A
        vector<int> dist(key_count, INF);
        dist[A] = 0;
        queue<int> q;
        q.push(A);
        while(!q.empty()){
            int u = q.front(); q.pop();
            for(auto &v : adj[u]){
                if(dist[v] > dist[u] + 1){
                    dist[v] = dist[u] + 1;
                    q.push(v);
                }
            }
        }
        for(int B=0; B<key_count; B++) movement_cost[A][B] = dist[B];
    }
    // Find starting key
    char start_ch = grid[0][0];
    int start_key = char_to_key[start_ch];
    // Initialize total_cost
    long long total_cost = 0;
    int prev_key = start_key;
    // Iterate through the text
    for(auto &ch : text){
        int current_key = char_to_key[ch];
        total_cost += movement_cost[prev_key][current_key];
        total_cost += 1; // for select
        prev_key = current_key;
    }
    // Move to Enter key
    total_cost += movement_cost[prev_key][enter_key];
    total_cost += 1; // for selecting Enter
    cout << total_cost;
}
