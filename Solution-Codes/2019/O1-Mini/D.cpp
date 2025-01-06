#include <bits/stdc++.h>
using namespace std;

struct Marker {
    char c; // 's' or 'e'
    int gene;
    int pos; // 1-based position in the original sequence
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    cin >> n;
    vector<Marker> markers(n);
    for(int i=0;i<n;i++){
        string s;
        cin >> s;
        markers[i].c = s[0];
        // Extract the gene type number
        markers[i].gene = stoi(s.substr(1));
        markers[i].pos = i+1;
    }
    // Find the maximum gene type id
    int max_gene = 0;
    for(int i=0;i<n;i++) {
        if(markers[i].gene > max_gene){
            max_gene = markers[i].gene;
        }
    }
    // Counting sort based on gene type
    // Initialize count array
    int k = max_gene;
    vector<int> count(k+2, 0); // gene types from 1 to k
    for(int i=0;i<n;i++) {
        count[markers[i].gene]++;
    }
    // Compute start indices
    vector<int> start(k+2, 0);
    for(int i=1;i<=k;i++) {
        start[i] = start[i-1] + count[i-1];
    }
    // Make a copy of start to use for placement
    vector<int> current_pos = start;
    // Create sorted_markers array
    vector<Marker> sorted_markers(n);
    for(int i=0;i<n;i++) {
        int gene = markers[i].gene;
        int pos = current_pos[gene]++;
        sorted_markers[pos] = markers[i];
    }
    // Now, for each gene type, process its markers
    // Initialize a counter for p from 1 to n
    // To save memory, use a vector of size n+1 (1-based)
    vector<int> counter(n+1, 0);
    for(int gene=1; gene<=k; gene++) {
        if(count[gene]==0) continue;
        int list_start = start[gene];
        int list_end = start[gene] + count[gene];
        int m_i = count[gene];
        // First, count number of 's' and 'e'
        int cnt_s=0, cnt_e=0;
        for(int j=list_start; j<list_end; j++) {
            if(sorted_markers[j].c == 's') cnt_s++;
            else cnt_e++;
        }
        if(cnt_s != cnt_e) continue;
        // Compute prefix sums and find min_balance, min_count, min_pos
        int balance =0;
        int min_balance =0;
        int min_count =0;
        int min_pos = -1;
        for(int j=list_start; j<list_end; j++) {
            if(sorted_markers[j].c == 's') balance +=1;
            else balance -=1;
            if(balance < min_balance){
                min_balance = balance;
                min_count =1;
                min_pos = j - list_start;
            }
            else if(balance == min_balance){
                min_count +=1;
            }
        }
        if(min_count !=1){
            continue;
        }
        // Find the starting marker index
        int starting_marker_index = min_pos +1;
        if(starting_marker_index == m_i){
            starting_marker_index =0;
        }
        // Get the global position p_i
        int p_i = sorted_markers[list_start + starting_marker_index].pos;
        // Increment the counter for p_i
        counter[p_i]++;
    }
    // Now, find the p with maximum counter[p]
    int best_p =1;
    int max_m = counter[1];
    for(int p=2; p<=n; p++) {
        if(counter[p] > max_m){
            max_m = counter[p];
            best_p = p;
        }
        else if(counter[p] == max_m && p < best_p){
            best_p = p;
        }
    }
    // If max_m is zero, choose p=1
    // But in the above loop, best_p is already set to 1
    // So no need to change
    cout << best_p << " " << max_m;
}
