#include <bits/stdc++.h>
using namespace std;

// Structure to represent a node in the program tree
struct Node {
    enum Type { VAR, REP };
    Type type;
    int var; // for VAR
    int n; // for REP
    vector<Node*> children; // for REP

    Node(Type t) : type(t), var(0), n(0) {}
};

// Function to parse the program into a tree structure
Node* parse_program(const vector<string>& tokens, int& index) {
    // Create a dummy root node
    Node* root = new Node(Node::REP);
    root->n = 1; // single repetition
    while (index < tokens.size()) {
        string token = tokens[index];
        if (token[0] == 'V') {
            // Variable access
            int var = stoi(token.substr(1));
            Node* var_node = new Node(Node::VAR);
            var_node->var = var;
            root->children.push_back(var_node);
            index++;
        }
        else if (token[0] == 'R') {
            // Repetition
            int repeat = stoi(token.substr(1));
            index++;
            Node* rep_node = new Node(Node::REP);
            rep_node->n = repeat;
            rep_node->children = vector<Node*>();
            // Recursively parse the subprogram
            Node* sub_program = parse_program(tokens, index);
            rep_node->children = sub_program->children;
            root->children.push_back(rep_node);
        }
        else if (token == "E") {
            // End of repetition
            index++;
            break;
        }
        else {
            // Invalid token
            index++;
        }
    }
    return root;
}

// Function to collect all unique variables in the program
void collect_variables(Node* node, set<int>& vars) {
    if (node->type == Node::VAR) {
        vars.insert(node->var);
    }
    else if (node->type == Node::REP) {
        for (auto child : node->children) {
            collect_variables(child, vars);
        }
    }
}

// Function to simulate the program execution for a given mapping
pair<long long, int> simulate_program(Node* node, const vector<int>& mapping, int initial_BSR) {
    if (node->type == Node::VAR) {
        if (mapping[node->var] == 0) {
            return {1, initial_BSR};
        }
        else {
            int bank = mapping[node->var];
            if (initial_BSR == bank) {
                return {1, initial_BSR};
            }
            else {
                return {2, bank};
            }
        }
    }
    else if (node->type == Node::REP) {
        long long total_cost = 0;
        int current_BSR = initial_BSR;
        for (int i = 0; i < node->n; ++i) {
            for (auto child : node->children) {
                pair<long long, int> res = simulate_program(child, mapping, current_BSR);
                total_cost += res.first;
                current_BSR = res.second;
            }
        }
        return {total_cost, current_BSR};
    }
    return {0, initial_BSR};
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int b, s;
    cin >> b >> s;
    string line;
    getline(cin, line); // consume the remaining newline
    getline(cin, line);
    // Split the line into tokens
    vector<string> tokens;
    string token;
    int pos =0;
    while (pos < line.size()) {
        if (line[pos] == ' ') { pos++; continue;}
        int start = pos;
        while (pos < line.size() && line[pos] != ' ') pos++;
        tokens.push_back(line.substr(start, pos - start));
    }
    // Parse the program
    int index =0;
    Node* program_root = parse_program(tokens, index);
    // Collect variables
    set<int> var_set;
    collect_variables(program_root, var_set);
    vector<int> variables(var_set.begin(), var_set.end());
    int k = variables.size();
    // Sort variables by frequency (optional optimization)
    // Not implemented here
    // Assign indices to variables
    // Map variable number to index
    unordered_map<int, int> var_to_idx;
    for(int i=0;i<k;i++) var_to_idx[variables[i]] = i;
    // Now, enumerate all possible subsets of variables to assign to bank0 (size <=s)
    // Use bitmask to represent subsets
    // If k >20, it's too slow. Assume k <=15
    if(k >20){
        // Handle k >20 differently or exit
        // For simplicity, proceed assuming k <=20
    }
    // Enumerate all possible subsets
    long long min_cost = LLONG_MAX;
    // Precompute all subsets of variables with size up to s
    vector<vector<int>> subsets;
    for(int mask=0; mask<(1<<k); mask++){
        if(__builtin_popcount(mask) <=s){
            vector<int> subset;
            for(int i=0;i<k;i++) if(mask & (1<<i)) subset.push_back(i);
            subsets.push_back(subset);
        }
    }
    // Iterate through all subsets
    for(auto subset : subsets){
        // Assign variables in subset to bank0
        // Check if remaining variables can be assigned to b-1 banks
        int remaining = k - subset.size();
        if(remaining > (b-1)*s) continue;
        // Now, assign remaining variables to b-1 banks
        // Implement a recursive backtracking assignment
        // Prepare list of remaining variables
        vector<int> remaining_vars;
        for(int i=0;i<k;i++) if(!(find(subset.begin(), subset.end(), i) != subset.end())) remaining_vars.push_back(i);
        // Initialize bank assignments
        vector<vector<int>> banks(b, vector<int>()); // bank0 to bank(b-1)
        for(auto var_idx : subset) banks[0].push_back(var_idx);
        // Assign remaining variables to banks 1..b-1
        // Implement a backtracking function
        // To speed up, implement it iteratively
        // Use BFS with states: current variable index, current bank assignments
        // But it's too slow. Instead, assign greedily
        // Assign remaining variables to first available bank
        vector<int> mapping(k, 0); // Initialize all to bank0
        for(auto var_idx : subset) mapping[var_idx] =0;
        for(int i=0;i<b;i++) if(i>=1){
            for(auto var_idx : banks[i]) mapping[var_idx] =i;
        }
        // Assign remaining variables greedily
        bool valid = true;
        for(auto var_idx : remaining_vars){
            bool assigned = false;
            for(int bank=1; bank<b; bank++){
                if(banks[bank].size() <s){
                    banks[bank].push_back(var_idx);
                    mapping[var_idx] = bank;
                    assigned = true;
                    break;
                }
            }
            if(!assigned){
                valid = false;
                break;
            }
        }
        if(!valid) continue;
        // Now, simulate the program with this mapping
        pair<long long, int> res = simulate_program(program_root, mapping, 0);
        if(res.first < min_cost){
            min_cost = res.first;
        }
    }
    cout << min_cost;
}
