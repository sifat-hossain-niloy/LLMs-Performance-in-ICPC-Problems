#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <algorithm>

using namespace std;

struct Node {
    int downstream;    // Index of downstream node (0 for sea)
    int64_t distance;  // Distance to downstream node
    vector<int> upstreams; // Indices of upstream nodes
    unordered_set<int> rivers; // Set of river indices that can reach this node
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<string> riverNames(n); // Names of rivers in order
    vector<Node> nodes(n + m + 1); // Nodes: 0 is sea, 1 to n+m are sources and confluences
    unordered_map<string, int> riverIndex; // Map river name to index

    // Read sources
    for (int i = 1; i <= n; ++i) {
        string name;
        int c;
        int64_t d;
        cin >> name >> c >> d;
        riverNames[i - 1] = name;
        riverIndex[name] = i; // Map river name to its index
        nodes[i].downstream = c ? n + c : 0;
        nodes[i].distance = d;
        nodes[nodes[i].downstream].upstreams.push_back(i);
    }

    // Read confluences
    for (int i = n + 1; i <= n + m; ++i) {
        int c;
        int64_t d;
        cin >> c >> d;
        nodes[i].downstream = c ? n + c : 0;
        nodes[i].distance = d;
        nodes[nodes[i].downstream].upstreams.push_back(i);
    }

    // Build the set of rivers that can reach each confluence
    // We proceed in reverse topological order (from leaves to root)
    vector<int> order;
    vector<bool> visited(n + m + 1, false);
    function<void(int)> dfs = [&](int u) {
        visited[u] = true;
        for (int v : nodes[u].upstreams) {
            if (!visited[v]) dfs(v);
        }
        order.push_back(u);
    };
    dfs(0);

    reverse(order.begin(), order.end());

    for (int u : order) {
        if (u == 0) continue;
        if (u <= n) {
            // Source node
            nodes[u].rivers.insert(u);
        } else {
            // Confluence node
            for (int v : nodes[u].upstreams) {
                nodes[u].rivers.insert(nodes[v].rivers.begin(), nodes[v].rivers.end());
            }
        }
    }

    // For each river, compute its maximum possible length
    vector<int64_t> maxLengths(n);
    for (int i = 1; i <= n; ++i) {
        int u = i;
        int64_t totalLength = 0;
        while (nodes[u].downstream != 0) {
            totalLength += nodes[u].distance;
            int d = nodes[u].downstream;
            if (nodes[d].rivers.count(i)) {
                u = d;
            } else {
                break;
            }
        }
        // Add distance to sea if any
        if (nodes[u].downstream == 0) {
            totalLength += nodes[u].distance;
        }
        maxLengths[i - 1] = totalLength;
    }

    // Assign ranks
    vector<int64_t> sortedLengths = maxLengths;
    sort(sortedLengths.begin(), sortedLengths.end(), greater<int64_t>());

    unordered_map<int64_t, int> lengthToRank;
    int rank = 1;
    for (size_t i = 0; i < sortedLengths.size(); ++i) {
        if (i > 0 && sortedLengths[i] != sortedLengths[i - 1]) {
            rank = i + 1;
        }
        lengthToRank[sortedLengths[i]] = rank;
    }

    // Output results
    for (int i = 0; i < n; ++i) {
        cout << riverNames[i] << " " << lengthToRank[maxLengths[i]] << "\n";
    }

    return 0;
}
