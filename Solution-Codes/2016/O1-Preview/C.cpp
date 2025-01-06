#include <iostream>
#include <string>
#include <set>
using namespace std;

struct Node {
    int value;
    Node* left;
    Node* right;
};

Node* insert(Node* root, int value) {
    if (root == nullptr) {
        Node* node = new Node();
        node->value = value;
        node->left = node->right = nullptr;
        return node;
    } else {
        if (value < root->value) {
            root->left = insert(root->left, value);
        } else {
            root->right = insert(root->right, value);
        }
        return root;
    }
}

void serialize(Node* root, string& s) {
    if (root == nullptr) {
        s += "N";
        return;
    }
    s += "X";
    serialize(root->left, s);
    serialize(root->right, s);
}

void deleteTree(Node* root) {
    if (root == nullptr) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    int n, k;
    cin >> n >> k;
    set<string> treeShapes;

    for (int i = 0; i < n; ++i) {
        Node* root = nullptr;
        for (int j = 0; j < k; ++j) {
            int value;
            cin >> value;
            root = insert(root, value);
        }
        string s;
        serialize(root, s);
        treeShapes.insert(s);
        deleteTree(root);
    }

    cout << treeShapes.size() << endl;
    return 0;
}
