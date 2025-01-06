#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

// Union-Find structure with additional information
struct UnionFind {
    int n;
    vector<int> parent;
    vector<int> rank;
    // Number of odd degree nodes in each component
    vector<int> odd_deg;

    UnionFind(int size) : n(size), parent(size+1), rank(size+1, 0), odd_deg(size+1, 0) {
        for(int i=0;i<=size;i++) parent[i]=i;
    }

    int find_set(int x){
        if(parent[x]!=x)
            parent[x]=find_set(parent[x]);
        return parent[x];
    }

    void union_set(int x, int y){
        int fx = find_set(x);
        int fy = find_set(y);
        if(fx == fy) return;
        // Union by rank
        if(rank[fx] < rank[fy]){
            parent[fx] = fy;
            odd_deg[fy] += odd_deg[fx];
        }
        else{
            parent[fy] = fx;
            odd_deg[fx] += odd_deg[fy];
            if(rank[fx] == rank[fy]) rank[fx]++;
        }
    }

    void set_odd(int x){
        int fx = find_set(x);
        odd_deg[fx]++;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, m, s;
    cin >> n >> m >> s;
    // Assuming strands are arranged in circular order: 1 to n, connected to (t mod n) +1
    // Initialize degrees
    vector<int> deg(n+1, 0);
    // Initialize Union-Find
    UnionFind uf(n);
    // Read bridges
    for(int i=0;i<m;i++){
        ll d;
        int t;
        cin >> d >> t;
        int u = t;
        int v = t % n +1;
        deg[u]++;
        deg[v]++;
        uf.union_set(u, v);
    }
    // After all unions, count odd degrees per component
    // Reset odd_deg in Union-Find
    fill(uf.odd_deg.begin(), uf.odd_deg.end(), 0);
    for(int i=1;i<=n;i++) if(deg[i]%2) uf.odd_deg[uf.find_set(i)]++;
    // Now, for each i, compute the answer
    // Precompute for each i: component and degree parity
    // Precompute for each i: find_set and odd_deg
    // Find the component of s
    int fs = uf.find_set(s);
    int k_o = uf.odd_deg[fs];
    // Precompute for all components
    // Now, iterate through each i
    for(int i=1;i<=n;i++){
        int fi = uf.find_set(i);
        if(fi == fs){
            if(i ==s){
                if(k_o ==2 && deg[s]%2){
                    cout << "0\n";
                }
                else{
                    // ceil(k_o /2 ) 
                    int res = (k_o +1 ) /2;
                    // If k_o ==0, res=0
                    cout << res << "\n";
                }
            }
            else{
                if(deg[i]%2 && deg[s]%2){
                    if(k_o >=2){
                        int res = (k_o -2 +1 ) /2;
                        cout << res << "\n";
                    }
                    else{
                        // Need to add bridges
                        // To make at least two odd degrees
                        int res = 1;
                        cout << res << "\n";
                    }
                }
                else{
                    // ceil( (k_o ) /2 )
                    int res = (k_o +1 ) /2;
                    cout << res << "\n";
                }
            }
        }
        else{
            // Different component, need to connect to s's component
            // Assume that the component of i has k_o_i odd degrees
            // To make trail from i to s, need to:
            // Connect i's component to s's component
            // If k_o_i is even, connect to s's component with any odd
            // If k_o_i is odd, connect to s's component with any odd
            // The total odd degrees after connecting becomes k_o + k_o_i
            // Need trail from i to s: degrees of i and s must be odd
            // Thus, additions = ceil( (k_o +k_o_i -2 ) /2 ) +1
            // But to simplify, use ceil( (k_o +k_o_i ) /2 )
            int k_o_i = uf.odd_deg[fi];
            // After connecting, k_o_total = k_o + k_o_i
            // To have trail from i to s, need k_o_total to be 2
            // Thus, need to reduce k_o_total to2 by adding (k_o +k_o_i -2)/2
            // Plus 1 to connect
            // So total additions =1 + ceil( (k_o +k_o_i -2 ) /2 )
            // Handle cases where k_o +k_o_i <2
            if(k_o + k_o_i <2){
                // Need to make it 2
                // Number of additions to make k_o_total=2: ceil( (2 - (k_o +k_o_i)) /2 )
                // which is ceil( (2 -0)/2 )=1
                // So total additions =1 +1=2
                cout << 2 << "\n";
            }
            else{
                int needed = k_o + k_o_i -2;
                int res = 1 + ((needed +1)/2);
                cout << res << "\n";
            }
        }
    }
}
