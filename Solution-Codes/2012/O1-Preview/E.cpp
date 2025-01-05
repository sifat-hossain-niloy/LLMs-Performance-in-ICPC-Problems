#include <iostream>
#include <vector>
#include <bitset>
#include <string>
#include <algorithm>

using namespace std;

const int MAX_N = 75;

void compute_reachability(int n, vector<string>& control, vector<bitset<MAX_N>>& reachability) {
    for (int i = 0; i < n; ++i) {
        bitset<MAX_N> visited;
        vector<int> stack;
        stack.push_back(i);
        while (!stack.empty()) {
            int u = stack.back();
            stack.pop_back();
            if (visited[u]) continue;
            visited[u] = true;
            for (int v = 0; v < n; ++v) {
                if (control[u][v] == '1' && !visited[v]) {
                    stack.push_back(v);
                }
            }
        }
        // Exclude the cell itself unless it controls itself
        if (control[i][i] != '1') {
            visited[i] = false;
        }
        reachability[i] = visited;
    }
}

int main() {
    int n;
    int case_num = 1;
    while (cin >> n) {
        vector<string> control(n);
        for (int i = 0; i < n; ++i) {
            cin >> control[i];
        }
        vector<bitset<MAX_N>> reachability(n);
        compute_reachability(n, control, reachability);
        bitset<MAX_N> full_set;
        full_set.set();
        full_set >>= (MAX_N - n); // Only consider n bits

        int min_k = -1;
        vector<int> result_cells;

        // Try k from 1 up to 4 or 5
        int max_k = 4;
        bool found = false;
        for (int k = 1; k <= max_k && !found; ++k) {
            vector<int> indices(k);
            for (int i = 0; i < k; ++i) indices[i] = i;

            while (true) {
                bitset<MAX_N> union_set;
                for (int idx : indices) {
                    union_set |= reachability[idx];
                }
                if (union_set == full_set) {
                    min_k = k;
                    result_cells.clear();
                    for (int idx : indices) {
                        result_cells.push_back(idx + 1); // Convert to 1-based index
                    }
                    found = true;
                    break;
                }
                // Generate next combination
                int pos = k - 1;
                while (pos >= 0 && indices[pos] == n - k + pos) {
                    --pos;
                }
                if (pos < 0) break;
                ++indices[pos];
                for (int i = pos + 1; i < k; ++i) {
                    indices[i] = indices[i - 1] + 1;
                }
            }
        }
        // If not found, output all cells (worst case)
        if (!found) {
            min_k = n;
            result_cells.resize(n);
            for (int i = 0; i < n; ++i) {
                result_cells[i] = i + 1;
            }
        }
        // Output the result
        cout << "Case " << case_num << ": " << min_k;
        for (int idx : result_cells) {
            cout << " " << idx;
        }
        cout << endl;
        ++case_num;
    }
    return 0;
}
