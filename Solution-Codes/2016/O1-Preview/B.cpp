#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <functional>
#include <limits>
using namespace std;
typedef long long ll;
typedef pair<int, int> pii;

const ll INF = 1e18;

int n, b, s, r;
vector<vector<pii>> adj; // adjacency list (node, weight)
vector<ll> d_h_to; // distances from headquarters to nodes
vector<ll> d_to_h; // distances from nodes to headquarters

void dijkstra(int src, vector<ll>& dist) {
    dist.assign(n + 1, INF);
    dist[src] = 0;
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<>> pq;
    pq.push({0, src});
    while (!pq.empty()) {
        auto [d_u, u] = pq.top(); pq.pop();
        if (d_u > dist[u]) continue;
        for (auto [v, w] : adj[u]) {
            if (dist[v] > d_u + w) {
                dist[v] = d_u + w;
                pq.push({dist[v], v});
            }
        }
    }
}

int main() {
    cin >> n >> b >> s >> r;
    adj.resize(n + 1);
    for (int i = 0; i < r; ++i) {
        int u, v, l;
        cin >> u >> v >> l;
        adj[u].push_back({v, l});
    }
    // Compute distances from headquarters to all nodes
    dijkstra(b + 1, d_h_to);
    // Reverse the graph to compute distances from all nodes to headquarters
    vector<vector<pii>> adj_rev(n + 1);
    for (int u = 1; u <= n; ++u) {
        for (auto [v, w] : adj[u]) {
            adj_rev[v].push_back({u, w});
        }
    }
    swap(adj, adj_rev);
    dijkstra(b + 1, d_to_h);
    // Compute c_i = d_i_h + d_h_i
    vector<ll> c_i(b);
    for (int i = 1; i <= b; ++i) {
        ll d_i_h = d_to_h[i];
        ll d_h_i = d_h_to[i];
        if (d_i_h == INF || d_h_i == INF) {
            cout << "Cannot reach headquarters from branch " << i << endl;
            return 1;
        }
        c_i[i - 1] = d_i_h + d_h_i;
    }
    // Sort branches by c_i
    vector<ll> c_sorted = c_i;
    sort(c_sorted.begin(), c_sorted.end());
    ll min_total_cost = INF;
    for (int k1 = b - s + 1; k1 >= 1; --k1) {
        int b_remain = b - k1;
        int s_remain = s - 1;
        vector<int> group_sizes;
        group_sizes.push_back(k1);
        if (s_remain > 0) {
            int avg_k = b_remain / s_remain;
            int extra = b_remain % s_remain;
            for (int i = 0; i < s_remain; ++i) {
                int k_j = avg_k + (i < extra ? 1 : 0);
                group_sizes.push_back(k_j);
            }
        }
        ll total_cost = 0;
        int idx = 0;
        for (int gi = 0; gi < group_sizes.size(); ++gi) {
            int k_i = group_sizes[gi];
            ll sum_c_i = 0;
            for (int cnt = 0; cnt < k_i; ++cnt, ++idx) {
                sum_c_i += c_sorted[idx];
            }
            total_cost += (ll)(k_i - 1) * sum_c_i;
        }
        if (total_cost < min_total_cost) {
            min_total_cost = total_cost;
        }
    }
    cout << min_total_cost << endl;
    return 0;
}
