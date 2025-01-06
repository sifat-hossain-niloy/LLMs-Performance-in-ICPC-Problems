#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <set>

using namespace std;

struct Node {
    char val;
    Node* left;
    Node* right;
    Node(char v) : val(v), left(nullptr), right(nullptr) {}
};

string obs_pre, obs_in, obs_post;
map<string, string> functions; // Mapping of f1 to f6
map<pair<int, int>, int> memo;

vector<string> func_names = {"Pre", "In", "Post"};

// Function to find the index of a character in a string between l and r (inclusive)
int findIndex(const string& s, int l, int r, char c) {
    for (int i = l; i <= r; ++i) {
        if (s[i] == c) {
            return i;
        }
    }
    return -1;
}

// Function prototypes
Node* parse(const string& func, int l_pre, int r_pre, int l_in, int r_in, int l_post, int r_post);

// Parsing functions for Pre, In, and Post traversals
Node* parsePre(int l_pre, int r_pre, int l_in, int r_in, int l_post, int r_post);
Node* parseIn(int l_pre, int r_pre, int l_in, int r_in, int l_post, int r_post);
Node* parsePost(int l_pre, int r_pre, int l_in, int r_in, int l_post, int r_post);

// Function to perform the traversal and collect the output
void collectTraversal(Node* node, string& pre, string& in, string& post);

// Function to delete the tree and free memory
void deleteTree(Node* node);

// Function to generate all valid permutations of function assignments
void generate_permutations(vector<vector<string>>& permutations);

int main() {
    // Read observed outputs
    cin >> obs_pre >> obs_in >> obs_post;
    int n = obs_pre.size();

    vector<vector<string>> permutations;
    generate_permutations(permutations);

    vector<pair<vector<string>, vector<string>>> results;

    // For each permutation
    for (auto& perm : permutations) {
        // Assign functions f1 to f6 based on the permutation
        functions.clear();
        functions["f1"] = perm[0];
        functions["f2"] = perm[1];
        functions["f3"] = perm[2];
        functions["f4"] = perm[3];
        functions["f5"] = perm[4];
        functions["f6"] = perm[5];

        // Attempt to parse and construct the tree
        Node* root = parse("Pre", 0, n - 1, 0, n - 1, 0, n - 1);

        if (root != nullptr) {
            // Collect traversal outputs
            string pre_trav, in_trav, post_trav;
            collectTraversal(root, pre_trav, in_trav, post_trav);

            // Ensure we get the alphabetically first tree
            results.push_back({perm, {pre_trav, in_trav, post_trav}});
        }

        // Clean up
        deleteTree(root);
    }

    // Sort results according to the specified order
    sort(results.begin(), results.end(), [](const pair<vector<string>, vector<string>>& a,
                                            const pair<vector<string>, vector<string>>& b) {
        return a.first < b.first;
    });

    // Output the results
    for (auto& res : results) {
        // Print function assignments
        cout << res.first[0] << " " << res.first[1] << " " << res.first[2] << " "
             << res.first[3] << " " << res.first[4] << " " << res.first[5] << endl;
        // Print traversals
        cout << res.second[0] << endl;
        cout << res.second[1] << endl;
        cout << res.second[2] << endl;
    }

    return 0;
}

Node* parse(const string& func, int l_pre, int r_pre, int l_in, int r_in, int l_post, int r_post) {
    if (l_pre > r_pre || l_in > r_in || l_post > r_post)
        return nullptr;

    if ((r_pre - l_pre) != (r_in - l_in) || (r_pre - l_pre) != (r_post - l_post))
        return nullptr;

    if (func == "Pre") {
        return parsePre(l_pre, r_pre, l_in, r_in, l_post, r_post);
    } else if (func == "In") {
        return parseIn(l_pre, r_pre, l_in, r_in, l_post, r_post);
    } else if (func == "Post") {
        return parsePost(l_pre, r_pre, l_in, r_in, l_post, r_post);
    }
    return nullptr;
}

Node* parsePre(int l_pre, int r_pre, int l_in, int r_in, int l_post, int r_post) {
    if (l_pre > r_pre)
        return nullptr;

    char root_val = obs_pre[l_pre];
    int idx_in = findIndex(obs_in, l_in, r_in, root_val);
    if (idx_in == -1)
        return nullptr;

    int left_len = idx_in - l_in;
    int right_len = r_in - idx_in;

    Node* node = new Node(root_val);

    // Left subtree
    node->left = parse(functions["f1"], l_pre + 1, l_pre + left_len, l_in, idx_in - 1, l_post, l_post + left_len - 1);

    // Right subtree
    node->right = parse(functions["f2"], l_pre + left_len + 1, r_pre, idx_in + 1, r_in, l_post + left_len, r_post - 1);

    return node;
}

Node* parseIn(int l_pre, int r_pre, int l_in, int r_in, int l_post, int r_post) {
    if (l_in > r_in)
        return nullptr;

    // Try all possible positions for root to find the alphabetically first valid tree
    for (int idx_in = l_in; idx_in <= r_in; ++idx_in) {
        char root_val = obs_in[idx_in];
        int left_len = idx_in - l_in;
        int right_len = r_in - idx_in;

        Node* node = new Node(root_val);

        // Left subtree
        node->left = parse(functions["f3"], l_pre, l_pre + left_len - 1, l_in, idx_in - 1, l_post, l_post + left_len - 1);

        // Right subtree
        node->right = parse(functions["f4"], l_pre + left_len, r_pre - 1, idx_in + 1, r_in, l_post + left_len, r_post - 1);

        // Check root values in Pre and Post traversals
        if (l_pre <= r_pre && obs_pre[r_pre] != root_val)
            continue;
        if (l_post <= r_post && obs_post[r_post] != root_val)
            continue;

        return node;
    }
    return nullptr;
}

Node* parsePost(int l_pre, int r_pre, int l_in, int r_in, int l_post, int r_post) {
    if (l_post > r_post)
        return nullptr;

    char root_val = obs_post[r_post];
    int idx_in = findIndex(obs_in, l_in, r_in, root_val);
    if (idx_in == -1)
        return nullptr;

    int left_len = idx_in - l_in;
    int right_len = r_in - idx_in;

    Node* node = new Node(root_val);

    // Left subtree
    node->left = parse(functions["f5"], l_pre, l_pre + left_len - 1, l_in, idx_in - 1, l_post, l_post + left_len - 1);

    // Right subtree
    node->right = parse(functions["f6"], l_pre + left_len, r_pre - 1, idx_in + 1, r_in, l_post + left_len, r_post - 1);

    return node;
}

void collectTraversal(Node* node, string& pre, string& in, string& post) {
    if (!node)
        return;
    pre += node->val;
    collectTraversal(node->left, pre, in, post);
    in += node->val;
    collectTraversal(node->right, pre, in, post);
    post += node->val;
}

void deleteTree(Node* node) {
    if (!node)
        return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

void generate_permutations(vector<vector<string>>& permutations) {
    vector<string> calls = {"Pre", "Pre", "In", "In", "Post", "Post"};
    sort(calls.begin(), calls.end());
    do {
        permutations.push_back(calls);
    } while (next_permutation(calls.begin(), calls.end()));
}
