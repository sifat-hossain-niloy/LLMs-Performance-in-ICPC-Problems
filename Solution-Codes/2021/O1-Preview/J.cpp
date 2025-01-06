#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

struct Node {
    char type; // 'I', 'S', 'M'
    long long length;
    int parent; // For split outputs
    int left;   // For split node or merge node (first input)
    int right;  // For split node or merge node (second input)
    bool isLeftChild; // For split outputs: true if first output
};

int main() {
    long long m;
    int n, q;
    cin >> m >> n >> q;

    unordered_map<int, Node> nodes;

    // Initialize input node 1
    nodes[1] = {'I', m, -1, -1, -1, false};

    for (int i = 0; i < n; ++i) {
        string s;
        int x, y, z;
        cin >> s >> x >> y >> z;
        if (s == "S") {
            // Split node x splits into outputs y and z
            nodes[x].type = 'S';
            nodes[x].left = y;
            nodes[x].right = z;
            // Outputs y and z
            nodes[y] = {'-', 0, x, -1, -1, true}; // Left output
            nodes[z] = {'-', 0, x, -1, -1, false}; // Right output
        } else {
            // Merge node z merges inputs x and y
            nodes[z].type = 'M';
            nodes[z].left = x;
            nodes[z].right = y;
        }
    }

    // Compute lengths in topological order
    vector<int> order;
    vector<int> indegree(n + 2, 0);
    for (auto &p : nodes) {
        int id = p.first;
        Node &node = p.second;
        if (node.type == 'S') {
            indegree[node.left]++;
            indegree[node.right]++;
        } else if (node.type == 'M') {
            indegree[id]++;
        }
    }

    // Topological sort
    vector<int> stack;
    for (auto &p : nodes) {
        int id = p.first;
        if (indegree[id] == 0) {
            stack.push_back(id);
        }
    }

    while (!stack.empty()) {
        int u = stack.back();
        stack.pop_back();
        order.push_back(u);
        Node &node = nodes[u];
        if (node.type == 'S') {
            Node &leftChild = nodes[node.left];
            Node &rightChild = nodes[node.right];
            leftChild.type = '-';
            rightChild.type = '-';
            leftChild.length = (node.length + 1) / 2;
            rightChild.length = node.length / 2;
            if (--indegree[node.left] == 0) stack.push_back(node.left);
            if (--indegree[node.right] == 0) stack.push_back(node.right);
        } else if (node.type == 'M') {
            Node &leftInput = nodes[node.left];
            Node &rightInput = nodes[node.right];
            node.length = leftInput.length + rightInput.length;
            if (--indegree[node.left] == 0) stack.push_back(node.left);
            if (--indegree[node.right] == 0) stack.push_back(node.right);
        }
    }

    // Process queries
    for (int i = 0; i < q; ++i) {
        int x;
        long long k;
        cin >> x >> k;
        int node = x;
        long long idx = k;
        bool valid = true;
        while (node != 1) {
            Node &curNode = nodes[node];
            if (idx > curNode.length || idx <= 0) {
                valid = false;
                break;
            }
            if (curNode.type == '-') {
                // Output of a split node
                Node &parent = nodes[curNode.parent];
                if (curNode.isLeftChild) {
                    idx = 2 * idx - 1;
                } else {
                    idx = 2 * idx;
                }
                node = curNode.parent;
            } else if (curNode.type == 'M') {
                Node &leftInput = nodes[curNode.left];
                Node &rightInput = nodes[curNode.right];
                long long Lx = leftInput.length;
                long long Ly = rightInput.length;
                long long minL = min(Lx, Ly);
                if (idx <= 2 * minL) {
                    if (idx % 2 == 1) {
                        idx = (idx + 1) / 2;
                        node = curNode.left;
                    } else {
                        idx = idx / 2;
                        node = curNode.right;
                    }
                } else {
                    if (Lx > Ly) {
                        idx = idx - 2 * minL;
                        if (idx > Lx - minL) {
                            valid = false;
                            break;
                        }
                        idx = minL + idx;
                        node = curNode.left;
                    } else {
                        idx = idx - 2 * minL;
                        if (idx > Ly - minL) {
                            valid = false;
                            break;
                        }
                        idx = minL + idx;
                        node = curNode.right;
                    }
                }
            } else if (curNode.type == 'S') {
                // Should not reach here
                valid = false;
                break;
            } else {
                // Should not reach here
                valid = false;
                break;
            }
        }
        if (!valid || idx > m || idx <= 0) {
            cout << "none" << endl;
        } else {
            cout << idx << endl;
        }
    }
    return 0;
}
