#include <iostream>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

struct TrieNode {
    TrieNode* children[26];
    bool is_end;
    int count;

    TrieNode() {
        memset(children, 0, sizeof(children));
        is_end = false;
        count = 0;
    }
};

void dfs(TrieNode* node) {
    node->count = node->is_end ? 1 : 0;
    for (int i = 0; i < 26; ++i) {
        if (node->children[i]) {
            dfs(node->children[i]);
            node->count += node->children[i]->count;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<char> c(n + 1);
    vector<int> p(n + 1);
    vector<TrieNode*> node(n + 1);

    TrieNode* root = new TrieNode();
    node[0] = root;

    for (int i = 1; i <= n; ++i) {
        string s;
        int pi;
        cin >> c[i] >> pi;
        p[i] = pi;

        int idx = c[i] - 'A';
        TrieNode* parentNode = node[pi];
        if (!parentNode->children[idx]) {
            parentNode->children[idx] = new TrieNode();
        }
        node[i] = parentNode->children[idx];
        node[i]->is_end = true;
    }

    // Compute counts via post-order traversal
    dfs(root);

    // Handle queries
    for (int i = 0; i < k; ++i) {
        string s;
        cin >> s;
        TrieNode* current = root;
        bool found = true;
        for (int j = s.length() - 1; j >= 0; --j) { // Reverse traversal
            int idx = s[j] - 'A';
            if (!current->children[idx]) {
                found = false;
                break;
            }
            current = current->children[idx];
        }
        if (found) {
            cout << current->count << '\n';
        } else {
            cout << "0\n";
        }
    }

    return 0;
}
