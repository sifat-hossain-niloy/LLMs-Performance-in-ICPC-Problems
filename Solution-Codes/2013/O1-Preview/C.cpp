#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int to, rev;
    int cap;
};

const int MAXN = 25005;
vector<Edge> adj[MAXN];
int level[MAXN];
int ptr[MAXN];

void add_edge(int u, int v) {
    adj[u].push_back({v, (int)adj[v].size(), 1});
    adj[v].push_back({u, (int)adj[u].size() - 1, 0});
}

bool bfs(int s, int t) {
    fill(level, level + MAXN, -1);
    queue<int> q;
    q.push(s);
    level[s] = 0;
    while (!q.empty()) {
        int v = q.front(); q.pop();
        for (auto &e : adj[v]) {
            if (e.cap > 0 && level[e.to] == -1) {
                level[e.to] = level[v] + 1;
                q.push(e.to);
            }
        }
    }
    return level[t] != -1;
}

int dfs(int v, int t, int flow) {
    if (v == t || flow == 0) return flow;
    for (int &i = ptr[v]; i < adj[v].size(); ++i) {
        Edge &e = adj[v][i];
        if (level[e.to] == level[v] + 1 && e.cap > 0) {
            int pushed = dfs(e.to, t, min(flow, e.cap));
            if (pushed > 0) {
                e.cap -= pushed;
                adj[e.to][e.rev].cap += pushed;
                return pushed;
            }
        }
    }
    return 0;
}

int dinic(int s, int t) {
    int flow = 0;
    while (bfs(s, t)) {
        fill(ptr, ptr + MAXN, 0);
        while (int pushed = dfs(s, t, INT_MAX)) {
            flow += pushed;
        }
    }
    return flow;
}

typedef pair<int, int> PII;
const int INF = INT_MAX;
vector<PII> adj_dijkstra[MAXN];
int dist[MAXN];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, c;
    cin >> n >> m >> c;
    for (int i = 0; i < m; ++i) {
        int xi, yi, ti;
        cin >> xi >> yi >> ti;
        adj_dijkstra[xi].push_back({yi, ti});
        adj_dijkstra[yi].push_back({xi, ti});
    }

    vector<int> commuters(c);
    for (int i = 0; i < c; ++i) {
        cin >> commuters[i];
    }

    // Dijkstra from node 1
    fill(dist, dist + n + 1, INF);
    dist[1] = 0;
    priority_queue<PII, vector<PII>, greater<PII>> pq;
    pq.push({0, 1});
    while (!pq.empty()) {
        int d = pq.top().first;
        int v = pq.top().second;
        pq.pop();
        if (d != dist[v]) continue;
        for (auto &e : adj_dijkstra[v]) {
            int to = e.first;
            int w = e.second;
            if (dist[to] > dist[v] + w) {
                dist[to] = dist[v] + w;
                pq.push({dist[to], to});
            }
        }
    }

    // Build shortest path DAG
    for (int u = 1; u <= n; ++u) {
        for (auto &e : adj_dijkstra[u]) {
            int v = e.first;
            int w = e.second;
            if (dist[v] == dist[u] + w) {
                add_edge(u, v);
            }
        }
    }

    int S = 0; // Super-source
    int T = 1; // Sink (downtown)
    for (int s : commuters) {
        add_edge(S, s);
    }

    int maxFlow = dinic(S, T);

    cout << maxFlow << '\n';

    return 0;
}
