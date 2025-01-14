#include <bits/stdc++.h>
using namespace std;

/*
   We will:
   1) Read input.
   2) Use DSU (Union-Find) or DFS to identify connected components.
   3) For each component, count #vertices, #edges, and gather the multiset of
      initial animal types (b_i) and desired animal types (e_i).
   4) If component is a tree (#edges = #vertices - 1):
         - must have b_i == e_i for every vertex i in that component.
      Otherwise (has at least one cycle):
         - the frequency tables of b_i and e_i in that component must match.
   5) Output "possible" if all checks pass, otherwise "impossible".
*/

// Disjoint Set Union (Union-Find) for fast connected-component merging
struct DSU {
    int n;
    vector<int> parent, size;         // parent[i], and the size/rank for the root
    // Also track #edges in each connected component's root
    vector<long long> edgeCount;      // how many edges in the component whose root is "r"

    DSU(int n): n(n), parent(n), size(n, 1), edgeCount(n, 0) {
        for (int i = 0; i < n; i++)
            parent[i] = i;
    }

    int findp(int v) {
        if (parent[v] == v) return v;
        return parent[v] = findp(parent[v]);
    }

    void unite(int a, int b) {
        a = findp(a);
        b = findp(b);
        if (a == b) {
            // If same root, adding an edge within the same component
            edgeCount[a]++;
            return;
        }
        // union by size (or rank)
        if (size[a] < size[b]) std::swap(a, b);
        parent[b] = a;
        size[a] += size[b];
        // combine edge counts
        edgeCount[a] += edgeCount[b] + 1;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> b(n), e(n);
    for(int i = 0; i < n; i++){
        cin >> b[i] >> e[i];
    }

    DSU dsu(n);

    // read edges
    // (enclosures are 1-based in the input, so we convert to 0-based)
    for(int i = 0; i < m; i++){
        int x, y;
        cin >> x >> y;
        --x;
        --y;
        dsu.unite(x, y);
    }

    // Now gather frequency tables of b_i and e_i in each component
    vector< unordered_map<int,int> > freqB(n), freqE(n);
    // Also store the nodes in each component to do per-vertex checks if tree
    vector< vector<int> > compNodes(n);

    for(int i = 0; i < n; i++){
        int r = dsu.findp(i);
        freqB[r][ b[i] ]++;
        freqE[r][ e[i] ]++;
        compNodes[r].push_back(i);
    }

    // Check each connected component
    for(int i = 0; i < n; i++){
        // Only process if i is the root of its component
        if(dsu.findp(i) != i) continue;

        long long vCount = dsu.size[i];        // #vertices in this component
        long long eCount = dsu.edgeCount[i];   // #edges in this component
        if(vCount == 0) continue;

        // Multiset check: freqB[i] must match freqE[i]
        if(freqB[i].size() != freqE[i].size()){
            cout << "impossible\n";
            return 0;
        }
        for(auto & kv : freqB[i]){
            int animalType = kv.first;
            int countB = kv.second;
            if(freqE[i][animalType] != countB){
                cout << "impossible\n";
                return 0;
            }
        }

        // If it's a tree (edgeCount == vCount - 1), then no cycles => no moves => must match individually
        if(eCount == vCount - 1){
            for(int node : compNodes[i]){
                if(b[node] != e[node]){
                    cout << "impossible\n";
                    return 0;
                }
            }
        }
        // If not a tree, we have at least one cycle => can rearrange arbitrarily => no further checks
    }

    cout << "possible\n";
    return 0;
}
