#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

using namespace std;

// Define the Node structure for the binary tree
struct Node {
    char val;
    Node* left;
    Node* right;
    Node(char v) : val(v), left(NULL), right(NULL) {}
};

// Function to parse the sequences and construct the tree
Node* parse(string func, const string& s_pre, const string& s_in, const string& s_post,
            map<string, string>& functions) {
    // Base cases and validation checks
    // ...

    if (func == "Pre") {
        // Implement parsing logic for Pre traversal
        // ...
    } else if (func == "In") {
        // Implement parsing logic for In traversal
        // ...
    } else if (func == "Post") {
        // Implement parsing logic for Post traversal
        // ...
    }

    // Return the constructed subtree or NULL if parsing fails
}

// Function to generate all permutations of function assignments
void generate_permutations(vector<vector<string>>& permutations) {
    vector<string> calls = {"Pre", "Pre", "In", "In", "Post", "Post"};
    sort(calls.begin(), calls.end());
    do {
        permutations.push_back(calls);
    } while (next_permutation(calls.begin(), calls.end()));
}

// Main function
int main() {
    string obs_pre, obs_in, obs_post;
    cin >> obs_pre >> obs_in >> obs_post;

    vector<vector<string>> permutations;
    generate_permutations(permutations);

    vector<pair<vector<string>, vector<string>>> results;

    for (auto& perm : permutations) {
        map<string, string> functions;
        // Assign functions f1 to f6 based on the permutation
        functions["f1"] = perm[0];
        functions["f2"] = perm[1];
        functions["f3"] = perm[2];
        functions["f4"] = perm[3];
        functions["f5"] = perm[4];
        functions["f6"] = perm[5];

        // Attempt to parse and construct the tree
        Node* root = parse("Pre", obs_pre, obs_in, obs_post, functions);

        if (root != NULL) {
            // Collect traversal outputs
            vector<string> traversals;
            // Get pre-order, in-order, and post-order traversals
            // ...

            // Store the result
            results.push_back({perm, traversals});
        }
    }

    // Sort results according to the specified order
    // ...

    // Output the results
    for (auto& res : results) {
        // Print function assignments and traversal outputs
        // ...
    }

    return 0;
}
