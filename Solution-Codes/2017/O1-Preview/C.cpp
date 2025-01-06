#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <cstring>
using namespace std;

const int MAXN = 10005; // Adjust according to problem constraints
const int INF = INT_MAX;

struct Edge {
    int to, rev;
    int capacity;
    int cost;
};

vector<Edge> graph[MAXN];
int dist[MAXN], h[MAXN], prevv[MAXN], preve[MAXN];

void add_edge(int from, int to, int capacity, int cost) {
    graph[from].push_back((Edge){to, (int)graph[to].size(), capacity, cost});
    graph[to].push_back((Edge){from, (int)graph[from].size()-1, 0, -cost});
}

int min_cost_flow(int s, int t, int f, int V, int &flow_cost) {
    memset(h, 0, sizeof(h));
    int flow = 0;
    flow_cost = 0;
    while (f > 0) {
        priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;
        fill(dist, dist+V, INF);
        dist[s] = 0;
        pq.push(make_pair(0, s));
        while (!pq.empty()) {
            pair<int, int> p = pq.top(); pq.pop();
            int v = p.second;
            if (dist[v] < p.first) continue;
            for (int i = 0; i < graph[v].size(); i++) {
                Edge &e = graph[v][i];
                if (e.capacity > 0 && dist[e.to] > dist[v] + e.cost + h[v] - h[e.to]) {
                    dist[e.to] = dist[v] + e.cost + h[v] - h[e.to];
                    prevv[e.to] = v;
                    preve[e.to] = i;
                    pq.push(make_pair(dist[e.to], e.to));
                }
            }
        }
        if (dist[t] == INF) {
            return flow; // Cannot add more flow
        }
        for (int v = 0; v < V; v++) h[v] += dist[v];
        int d = f;
        for (int v = t; v != s; v = prevv[v]) {
            d = min(d, graph[prevv[v]][preve[v]].capacity);
        }
        f -= d;
        flow += d;
        flow_cost += d * h[t];
        for (int v = t; v != s; v = prevv[v]) {
            Edge &e = graph[prevv[v]][preve[v]];
            e.capacity -= d;
            graph[v][e.rev].capacity += d;
        }
    }
    return flow;
}

int main() {
    int r, c;
    cin >> r >> c;
    vector<vector<int> > h(r, vector<int>(c));
    vector<int> max_row(r, 0), max_col(c, 0);
    int total_crates = 0;
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            cin >> h[i][j];
            total_crates += h[i][j];
            max_row[i] = max(max_row[i], h[i][j]);
            max_col[j] = max(max_col[j], h[i][j]);
        }
    }

    int V = 2 + r + c + r * c; // S, T, rows, columns, piles
    int S = V - 2, T = V - 1;
    // Nodes:
    // 0 ~ r-1: Row nodes
    // r ~ r+c-1: Column nodes
    // r+c ~ r+c+r*c-1: Pile nodes

    // Edges from S to Row nodes
    for (int i = 0; i < r; i++) {
        add_edge(S, i, 1, 0);
    }
    // Edges from S to Column nodes
    for (int j = 0; j < c; j++) {
        add_edge(S, r + j, 1, 0);
    }

    // Edges from Row and Column nodes to Pile nodes
    int pile_base = r + c;
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            int pile_node = pile_base + i * c + j;
            if (h[i][j] >= max_row[i]) {
                int cost = h[i][j] - max_row[i];
                add_edge(i, pile_node, 1, cost);
            }
            if (h[i][j] >= max_col[j]) {
                int cost = h[i][j] - max_col[j];
                add_edge(r + j, pile_node, 1, cost);
            }
            // Edges from Pile nodes to T
            add_edge(pile_node, T, 2, 0); // Capacity 2 to allow both row and column max
        }
    }

    int flow_cost = 0;
    int required_flow = r + c;
    int flow = min_cost_flow(S, T, required_flow, V, flow_cost);

    int crates_stolen = total_crates - flow_cost;
    cout << crates_stolen << endl;
    return 0;
}
