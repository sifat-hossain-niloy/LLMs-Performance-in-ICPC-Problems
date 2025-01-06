#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    // Initialize adjacency mask
    vector<uint64_t> adj_mask(52, 0);

    // Function to map label to index
    auto label_to_index = [&](const string &label) -> int {
        if(label == "00") return -1;
        char letter = label[0];
        char sign = label[1];
        int index = 2*(letter - 'A') + (sign == '-' ? 1 : 0);
        return index;
    };

    // Read n molecule descriptions
    for(int i=0;i<n;i++){
        string s;
        cin >> s;
        // Split into four labels
        string labels[4];
        labels[0] = s.substr(0, 2);
        labels[1] = s.substr(2, 2);
        labels[2] = s.substr(4, 2);
        labels[3] = s.substr(6, 2);
        // Collect non-00 labels
        vector<int> non00;
        for(int j=0; j<4; j++){
            int idx = label_to_index(labels[j]);
            if(idx != -1){
                non00.push_back(idx);
            }
        }
        // For each label L in molecule:
        for(auto L : non00){
            int C = L ^ 1; // compatible label
            for(auto D : non00){
                if(D != L){
                    adj_mask[C] |= (1ULL << D);
                }
            }
        }
    }

    // Build adjacency list
    vector<vector<int>> adj(52, vector<int>());
    for(int C=0; C<52; C++){
        uint64_t mask = adj_mask[C];
        for(int D=0; D<52; D++){
            if(mask & (1ULL << D)){
                adj[C].push_back(D);
            }
        }
    }

    // DFS to detect cycle
    vector<int> color(52, 0); // 0=WHITE, 1=GRAY, 2=BLACK
    bool has_cycle = false;
    // To prevent stack overflow, use iterative DFS
    for(int u=0; u<52 && !has_cycle; u++){
        if(color[u] == 0){
            stack<pair<int, int>> stk;
            stk.emplace(u, 0);
            while(!stk.empty()){
                auto [node, idx] = stk.top(); stk.pop();
                if(idx == 0){
                    if(color[node] == 1){
                        has_cycle = true;
                        break;
                    }
                    if(color[node] == 2){
                        continue;
                    }
                    color[node] = 1; // GRAY
                }
                if(idx < adj[node].size()){
                    stk.emplace(node, idx + 1);
                    int v = adj[node][idx];
                    if(color[v] == 1){
                        has_cycle = true;
                        break;
                    }
                    if(color[v] == 0){
                        stk.emplace(v, 0);
                    }
                }
                else{
                    color[node] = 2; // BLACK
                }
            }
        }
    }

    if(has_cycle){
        cout << "unbounded";
    }
    else{
        cout << "bounded";
    }
}
