#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <cstring>
#include <algorithm>

using namespace std;

const int MAXN = 1000; // Maximum number of nodes
const int INF = 1e9;

struct Edge {
    int to, rev;
    int capacity;
};

vector<Edge> graph[MAXN];
int level[MAXN];
int iter[MAXN];
map<char, int> ring_map;
int node_count;

void add_edge(int from, int to, int capacity) {
    graph[from].push_back((Edge){to, (int)graph[to].size(), capacity});
    graph[to].push_back((Edge){from, (int)graph[from].size() - 1, 0});
}

void bfs(int s) {
    memset(level, -1, sizeof(level));
    queue<int> que;
    level[s] = 0;
    que.push(s);
    while (!que.empty()) {
        int v = que.front(); que.pop();
        for (auto &e : graph[v]) {
            if (e.capacity > 0 && level[e.to] < 0) {
                level[e.to] = level[v] + 1;
                que.push(e.to);
            }
        }
    }
}

int dfs(int v, int t, int upTo) {
    if (v == t) return upTo;
    for (int &i = iter[v]; i < graph[v].size(); i++) {
        Edge &e = graph[v][i];
        if (e.capacity > 0 && level[v] < level[e.to]) {
            int d = dfs(e.to, t, min(upTo, e.capacity));
            if (d > 0) {
                e.capacity -= d;
                graph[e.to][e.rev].capacity += d;
                return d;
            }
        }
    }
    return 0;
}

int max_flow(int s, int t) {
    int flow = 0;
    while (true) {
        bfs(s);
        if (level[t] < 0) break;
        memset(iter, 0, sizeof(iter));
        int f;
        while ((f = dfs(s, t, INF)) > 0)
            flow += f;
    }
    return flow;
}

bool visited[MAXN];

void dfs_cut(int v) {
    visited[v] = true;
    for (auto &e : graph[v]) {
        if (e.capacity > 0 && !visited[e.to]) {
            dfs_cut(e.to);
        }
    }
}

int main() {
    int case_num = 1;
    string line;
    while (getline(cin, line)) {
        if (line == "0") continue;
        // Clear previous data
        ring_map.clear();
        for (int i = 0; i < MAXN; i++) graph[i].clear();
        node_count = 2; // 0 for source, 1 for sink

        map<int, vector<char>> ring_keys;
        map<pair<int, int>, bool> ring_connections;

        // Read input
        do {
            if (line == "0") break;
            char a = line[0], b = line[1];
            if (islower(a) && islower(b)) {
                // Ring to Ring connection
                int ra, rb;
                if (ring_map.count(a)) ra = ring_map[a];
                else ring_map[a] = ra = node_count++;
                if (ring_map.count(b)) rb = ring_map[b];
                else ring_map[b] = node_count++;
                ring_connections[make_pair(min(ra, rb), max(ra, rb))] = true;
            } else if (islower(a) != islower(b)) {
                // Key attached to Ring
                char ring_char = islower(a) ? a : b;
                char key_char = islower(a) ? b : a;
                int ring_node;
                if (ring_map.count(ring_char)) ring_node = ring_map[ring_char];
                else ring_map[ring_char] = ring_node = node_count++;
                ring_keys[ring_node].push_back(key_char);
            }
        } while (getline(cin, line));

        int source = 0, sink = 1;

        // Build the graph
        for (auto &it : ring_map) {
            int ring_node = it.second;
            int c_brenda = 0, c_adam = 0;
            for (char key_char : ring_keys[ring_node]) {
                if (key_char >= 'A' && key_char <= 'M') {
                    c_brenda += 2; // Cost to move Brenda's key: 2 key operations
                } else {
                    c_adam += 2; // Cost to move Adam's key: 2 key operations
                }
            }
            // From Source to Ring (Brenda's keys)
            add_edge(source, ring_node, c_brenda);
            // From Ring to Sink (Adam's keys)
            add_edge(ring_node, sink, c_adam);
        }

        // Ring to Ring connections
        for (auto &conn : ring_connections) {
            int ra = conn.first.first;
            int rb = conn.first.second;
            // Edges with capacity 2 (ring operations)
            add_edge(ra, rb, 2);
            add_edge(rb, ra, 2);
        }

        // Calculate max flow (which gives us min cut)
        int flow = max_flow(source, sink);

        // Determine rings assigned to Brenda (reachable from source)
        memset(visited, 0, sizeof(visited));
        dfs_cut(source);

        int key_ops = 0, ring_ops = 0;

        // Key operations
        for (auto &it : ring_map) {
            int ring_node = it.second;
            if (visited[ring_node]) {
                // Ring assigned to Brenda
                for (char key_char : ring_keys[ring_node]) {
                    if (key_char >= 'N' && key_char <= 'Z') {
                        key_ops += 2; // Move Adam's key from Brenda's ring
                    }
                }
            } else {
                // Ring assigned to Adam
                for (char key_char : ring_keys[ring_node]) {
                    if (key_char >= 'A' && key_char <= 'M') {
                        key_ops += 2; // Move Brenda's key from Adam's ring
                    }
                }
            }
        }

        // Ring operations
        for (int i = 0; i < node_count; i++) {
            for (auto &e : graph[i]) {
                if (e.to >= node_count || e.to <= 1) continue;
                if (i < e.to && ((visited[i] && !visited[e.to]) || (!visited[i] && visited[e.to]))) {
                    // Edge crosses the cut
                    ring_ops += 2;
                }
            }
        }

        if (key_ops == 0 || ring_ops == 0 || key_ops + ring_ops > 0)
            cout << "Case " << case_num++ << ": " << key_ops << " " << ring_ops << endl;
        else
            cout << "Case " << case_num++ << ": impossible" << endl;
    }
    return 0;
}
