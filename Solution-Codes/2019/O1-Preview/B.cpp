#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>

using namespace std;

const long long INF = 1e18;

struct SegmentTree {
    int n;
    vector<int> maxy;

    SegmentTree(const vector<int>& ys) {
        n = ys.size();
        maxy.resize(4 * n);
        build(ys, 1, 0, n - 1);
    }

    void build(const vector<int>& ys, int v, int tl, int tr) {
        if (tl == tr) {
            maxy[v] = ys[tl];
        } else {
            int tm = (tl + tr) / 2;
            build(ys, 2 * v, tl, tm);
            build(ys, 2 * v + 1, tm + 1, tr);
            maxy[v] = max(maxy[2 * v], maxy[2 * v + 1]);
        }
    }

    int query_max(int v, int tl, int tr, int l, int r) {
        if (l > r) return 0;
        if (l == tl && r == tr) {
            return maxy[v];
        }
        int tm = (tl + tr) / 2;
        return max(query_max(2 * v, tl, tm, l, min(r, tm)),
                   query_max(2 * v + 1, tm + 1, tr, max(l, tm + 1), r));
    }

    int query_max(int l, int r) {
        return query_max(1, 0, n - 1, l, r);
    }
};

struct Edge {
    int to;
    long long cost;
};

int main() {
    int n, h, alpha, beta;
    cin >> n >> h >> alpha >> beta;
    vector<int> x(n), y(n);
    for (int i = 0; i < n; ++i) {
        cin >> x[i] >> y[i];
    }

    // Build segment tree over y
    SegmentTree seg_tree(y);

    // Build the graph
    vector<vector<Edge>> graph(n);

    for (int i = 0; i < n; ++i) {
        // Check if we can build an arch to i+1
        if (i + 1 < n) {
            int delta_x = x[i + 1] - x[i];
            double required_max_y = h - delta_x / 2.0;
            int max_y = max(y[i], y[i + 1]);
            if (max_y <= required_max_y) {
                graph[i].push_back({i + 1, alpha * (h - y[i + 1]) + beta * (long long)(delta_x) * delta_x});
            }
        }

        // Find the furthest position we can reach from i
        int lo = i, hi = n - 1, furthest = i;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            int delta_x = x[mid] - x[i];
            double required_max_y = h - delta_x / 2.0;
            int max_y = seg_tree.query_max(i, mid);
            if (max_y <= required_max_y) {
                furthest = mid;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
        if (furthest > i) {
            int delta_x = x[furthest] - x[i];
            graph[i].push_back({furthest, alpha * (h - y[furthest]) + beta * (long long)(delta_x) * delta_x});
        }
    }

    // Dijkstra's algorithm
    vector<long long> dist(n, INF);
    dist[0] = alpha * (h - y[0]);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    pq.push({dist[0], 0});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        for (auto& edge : graph[u]) {
            int v = edge.to;
            long long cost = edge.cost;
            if (dist[v] > dist[u] + cost) {
                dist[v] = dist[u] + cost;
                pq.push({dist[v], v});
            }
        }
    }

    if (dist[n - 1] == INF) {
        cout << "impossible\n";
    } else {
        cout << dist[n - 1] << "\n";
    }

    return 0;
}
