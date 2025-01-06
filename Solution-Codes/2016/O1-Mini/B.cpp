#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<ll, int> pii;

// Dijkstra's algorithm
vector<ll> dijkstra(int start, int n, const vector<vector<pair<int, int>>>& adj) {
    const ll INF = 1e18;
    vector<ll> dist(n + 1, INF);
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, std::greater<pair<ll, int>>> pq;
    dist[start] = 0;
    pq.emplace(0, start);
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        for (auto &[v, w] : adj[u]) {
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                pq.emplace(dist[v], v);
            }
        }
    }
    return dist;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, b, s, r;
    cin >> n >> b >> s >> r;
    // Original graph
    vector<vector<pair<int, int>>> adj(n + 2, vector<pair<int, int>>());
    // Reversed graph
    vector<vector<pair<int, int>>> adj_rev(n + 2, vector<pair<int, int>>());
    for(int i=0;i<r;i++){
        int u, v, l;
        cin >> u >> v >> l;
        adj[u].emplace_back(v, l);
        adj_rev[v].emplace_back(u, l);
    }
    int HQ = b +1;
    // Compute d'_i: distance from HQ to i in original graph
    vector<ll> d_prime = dijkstra(HQ, n, adj);
    // Compute d_i: distance from i to HQ, which is distance from HQ to i in reversed graph
    vector<ll> d = dijkstra(HQ, n, adj_rev);
    // For branches 1 to b, compute c_i = d_i + d'_i
    vector<ll> c;
    c.reserve(b);
    for(int i=1;i<=b;i++) {
        c.push_back(d[i] + d_prime[i]);
    }
    // Sort branches in decreasing order of c_i
    vector<int> indices(b);
    for(int i=0;i<b;i++) indices[i]=i;
    sort(indices.begin(), indices.end(), [&](const int a, const int b_) -> bool {
        return c[a] > c[b_];
    });
    // Assign first s to separate groups
    // Each group has sum_c and m_g
    // Initialize priority queue with (sum_c, m_g)
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, std::greater<pair<ll, int>>> pq_groups;
    // To store group sums
    vector<pair<ll, int>> groups;
    for(int i=0;i<s;i++) {
        int idx = indices[i];
        groups.emplace_back(c[idx], 1);
    }
    // Sort the groups in increasing order of sum_c
    // Push them into the priority queue
    for(auto &[sum_c, m_g] : groups){
        pq_groups.emplace(sum_c, m_g);
    }
    // Assign remaining branches
    // Sort the remaining branches in increasing order of c_i
    vector<int> remaining;
    remaining.reserve(b - s);
    for(int i=s;i<b;i++) remaining.push_back(indices[i]);
    sort(remaining.begin(), remaining.end(), [&](const int a, const int b_) -> bool {
        return c[a] < c[b_];
    });
    for(auto idx : remaining){
        auto [sum_c, m_g] = pq_groups.top(); pq_groups.pop();
        sum_c += c[idx];
        m_g +=1;
        pq_groups.emplace(sum_c, m_g);
    }
    // Now, compute the total cost
    ll total =0;
    while(!pq_groups.empty()){
        auto [sum_c, m_g] = pq_groups.top(); pq_groups.pop();
        if(m_g >=2){
            total += (m_g -1) * sum_c;
        }
    }
    cout << total;
}
