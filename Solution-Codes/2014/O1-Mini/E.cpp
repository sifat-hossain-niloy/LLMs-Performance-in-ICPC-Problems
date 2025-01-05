#include <bits/stdc++.h>
using namespace std;

int main(){
    int n;
    cin >> n;
    // Rooms are 1-indexed
    vector<vector<int>> adj(n+1);
    for(int i=1;i<=n;i++){
        int k;
        cin >> k;
        adj[i].reserve(k);
        for(int j=0; j<k; j++){
            int x;
            cin >> x;
            adj[i].push_back(x);
        }
    }
    // Initialize labels based on degree
    // label_map: key is the unique string, value is the label
    // Initialize label_map with degrees
    // To ensure unique labels, use a map from string to int
    // Start labels from 0
    // To handle multiple rooms with same degree, assign same label
    // First, collect degrees
    vector<int> labels(n+1, 0);
    map<int, int> degree_to_label;
    int current_label = 0;
    for(int i=1;i<=n;i++){
        int deg = adj[i].size();
        if(degree_to_label.find(deg) == degree_to_label.end()){
            degree_to_label[deg] = current_label++;
        }
        labels[i] = degree_to_label[deg];
    }
    bool changed = true;
    while(changed){
        changed = false;
        // Create a map from room to its signature
        // Signature is a sorted list of outgoing labels per incoming corridor
        // To generate signature, for each incoming corridor, collect the outgoing labels in order
        // Then sort all these lists and concatenate
        // Use a map from signature string to new label
        map<string, int> sig_to_label;
        int new_label = 0;
        vector<int> new_labels(n+1, 0);
        for(int i=1;i<=n;i++){
            int k = adj[i].size();
            if(k == 0){
                // Rooms with no exits
                new_labels[i] = 0;
                continue;
            }
            vector<string> lists;
            for(int j=0; j<k; j++){
                int incoming_corridor = adj[i][j];
                // To get the index of incoming corridor in adj[incoming_corridor]
                // Find where room i appears in adj[incoming_corridor]
                // Since corridors are bidirectional, but the input does not specify,
                // we assume corridors are directed as given.
                // Thus, to simulate entering via corridor j, we rotate the adj list
                // of room i so that j is the incoming corridor
                // and collect the outgoing corridors in order starting after incoming
                // Since we know the order is clockwise from incoming
                // So outgoing corridors are from (j+1) to (j+k-1) modulo k
                string s = "";
                for(int m=1; m<k; m++){
                    int idx = (j + m) % k;
                    int connected_room = adj[i][idx];
                    s += to_string(labels[connected_room]) + ",";
                }
                lists.push_back(s);
            }
            // Sort the lists
            sort(lists.begin(), lists.end());
            // Concatenate to form signature
            string signature = "";
            for(auto &s : lists){
                signature += s + "|";
            }
            // Assign a new label
            if(sig_to_label.find(signature) == sig_to_label.end()){
                sig_to_label[signature] = new_label++;
            }
            new_labels[i] = sig_to_label[signature];
        }
        // Check if labels have changed
        for(int i=1;i<=n;i++){
            if(labels[i] != new_labels[i]){
                changed = true;
                break;
            }
        }
        labels = new_labels;
    }
    // Now, group rooms by labels
    map<int, vector<int>> groups;
    for(int i=1;i<=n;i++) groups[labels[i]].push_back(i);
    // Collect groups with size >=2
    vector<vector<int>> result;
    for(auto &[lbl, vec] : groups){
        if(vec.size() >=2){
            result.emplace_back(vec);
        }
    }
    if(result.empty()){
        cout << "none";
    }
    else{
        // Sort the groups by their smallest room number
        sort(result.begin(), result.end(), [&](const vector<int> &a, const vector<int> &b) -> bool{
            return a[0] < b[0];
        });
        // For each group, sort the room numbers
        for(auto &group : result){
            sort(group.begin(), group.end());
        }
        // Now, sort again to ensure the order
        sort(result.begin(), result.end(), [&](const vector<int> &a, const vector<int> &b) -> bool{
            return a[0] < b[0];
        });
        // Print each group
        for(auto &group : result){
            for(int i=0;i<group.size();i++){
                if(i>0) cout << ' ';
                cout << group[i];
            }
            cout << '\n';
        }
    }
}
