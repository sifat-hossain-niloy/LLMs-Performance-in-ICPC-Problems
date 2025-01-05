#include <bits/stdc++.h>
using namespace std;

// Structure to represent a node in the BST
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

// Function to insert a value into the BST
void insertBST(TreeNode* &root, int v) {
    if (root == nullptr) {
        root = new TreeNode(v);
        return;
    }
    if (v < root->val)
        insertBST(root->left, v);
    else
        insertBST(root->right, v);
}

// Function to serialize the tree structure into a string
string serialize(TreeNode* root) {
    if (root == nullptr)
        return "null";
    return "(" + serialize(root->left) + "," + serialize(root->right) + ")";
}

// Function to delete the tree and free memory
void deleteTree(TreeNode* root) {
    if (root == nullptr)
        return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, k;
    cin >> n >> k;
    set<string> tree_shapes;
    for(int i=0; i<n; ++i){
        vector<int> sequence(k);
        for(int j=0; j<k; ++j){
            cin >> sequence[j];
        }
        TreeNode* root = nullptr;
        for(int j=0; j<k; ++j){
            insertBST(root, sequence[j]);
        }
        string s = serialize(root);
        tree_shapes.insert(s);
        // Optional: delete the tree to free memory
        deleteTree(root);
    }
    cout << tree_shapes.size();
}
