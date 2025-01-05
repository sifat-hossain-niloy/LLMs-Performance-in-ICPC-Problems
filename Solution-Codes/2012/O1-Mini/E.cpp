#include <bits/stdc++.h>
using namespace std;

// Structure to solve the Set Cover problem
struct SetCoverSolver {
    int n;
    vector<bitset<80>> coverage;
    vector<vector<int>> covers;
    int best_count;
    vector<int> best_selected;

    SetCoverSolver(int n_, const vector<bitset<80>> &coverage_, const vector<vector<int>> &covers_) {
        n = n_;
        coverage = coverage_;
        covers = covers_;
        best_count = n;
        best_selected = vector<int>();
    }

    // Greedy algorithm to find an initial upper bound
    void compute_greedy() {
        bitset<80> covered;
        covered.reset();
        vector<int> selected;
        while (covered.count() < n) {
            int best_j = -1;
            int best_cover = -1;
            for (int j = 0; j < n; j++) {
                int new_cover = (int)((coverage[j] & ~covered).count());
                if (new_cover > best_cover) {
                    best_cover = new_cover;
                    best_j = j;
                }
            }
            if (best_j == -1) break; // No progress can be made
            selected.push_back(best_j);
            covered |= coverage[best_j];
        }
        if (selected.size() < best_count) {
            best_count = selected.size();
            best_selected = selected;
        }
    }

    // Recursive backtracking function
    void backtrack(bitset<80> &covered, int count, vector<int> &selected) {
        if (count >= best_count)
            return;
        if (covered.count() == n) {
            if (count < best_count) {
                best_count = count;
                best_selected = selected;
            }
            return;
        }

        // Choose the node with the fewest covering sets
        int u = -1;
        int min_covers = n + 1;
        for (int i = 0; i < n; i++) {
            if (!covered[i]) {
                if ((int)covers[i].size() < min_covers) {
                    min_covers = covers[i].size();
                    u = i;
                    if (min_covers == 0)
                        break;
                }
            }
        }

        if (u == -1)
            return;
        if (min_covers == 0)
            return; // Cannot cover this node

        // Iterate over the cells that cover u, sorted by descending coverage
        for (int j : covers[u]) {
            selected.push_back(j);
            bitset<80> new_covered = covered | coverage[j];
            // Compute remaining and maximum additional coverage
            int remaining = n - (int)new_covered.count();
            if (remaining == 0) {
                if (count + 1 < best_count) {
                    best_count = count + 1;
                    best_selected = selected;
                }
                selected.pop_back();
                continue;
            }
            int max_additional = 0;
            for (int k = 0; k < n; k++) {
                if (!new_covered[k]) {
                    int new_cover = (int)((coverage[k] & ~new_covered).count());
                    if (new_cover > max_additional)
                        max_additional = new_cover;
                }
            }
            if (max_additional == 0) {
                selected.pop_back();
                continue; // Cannot cover remaining nodes
            }
            int lower_bound = (remaining + max_additional - 1) / max_additional;
            if (count + 1 + lower_bound >= best_count) {
                selected.pop_back();
                continue;
            }
            backtrack(new_covered, count + 1, selected);
            selected.pop_back();
        }
    }

    // Solve the Set Cover problem
    void solve() {
        compute_greedy();
        bitset<80> covered;
        covered.reset();
        vector<int> selected;
        backtrack(covered, 0, selected);
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    int case_num = 1;
    while(cin >> n){
        vector<string> adj(n);
        for(int i=0;i<n;i++) cin >> adj[i];
        // Construct coverage sets
        vector<bitset<80>> coverage(n, bitset<80>());
        for(int j=0; j<n; j++){
            coverage[j].reset();
            coverage[j].set(j);
            for(int i=0; i<n; i++) if(adj[j][i] == '1') coverage[j].set(i);
        }
        // Construct covers[u] lists
        vector<vector<int>> covers(n, vector<int>());
        for(int u=0; u<n; u++) {
            for(int j=0; j<n; j++) {
                if(coverage[j].test(u)) covers[u].push_back(j);
            }
            // Sort covers[u] in descending order of coverage size
            sort(covers[u].begin(), covers[u].end(), [&](const int a, const int b) -> bool {
                return coverage[a].count() > coverage[b].count();
            });
        }
        // Initialize and solve
        SetCoverSolver solver(n, coverage, covers);
        solver.solve();
        // Output the result
        cout << "Case " << case_num++ << ": " << solver.best_count;
        for(int j=0; j<solver.best_selected.size(); j++) cout << " " << (solver.best_selected[j]+1);
        cout << "\n";
    }
}
