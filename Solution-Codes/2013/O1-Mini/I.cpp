#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int a, b, m, n;
    cin >> a >> b >> m >> n;
    // Read the depth grid
    vector<vector<int>> d(m, vector<int>(n));
    for(auto &row: d) for(auto &x: row) cin >> x;
    // Initialize maximum volume
    ll max_V = 0;
    // Temporary grid for row sliding mins
    vector<vector<int>> temp(m, vector<int>(n, 0));
    // Function to process one orientation
    auto process = [&](int limit_w, int limit_l) -> void{
        for(int l=1; l<=limit_l; l++){
            // Step 1: Row sliding min for window size l
            for(int row=0; row<m; row++){
                deque<int> dq;
                for(int c=0; c<n; c++){
                    while(!dq.empty() && d[row][c] <= d[row][dq.back()]) dq.pop_back();
                    dq.push_back(c);
                    while(!dq.empty() && dq.front() < c - l +1) dq.pop_front();
                    if(c >= l-1){
                        temp[row][c - l +1] = d[row][dq.front()];
                    }
                }
            }
            // Step 2: Column sliding min for window size w
            // To store the current max d_min for each w
            vector<int> current_max_w(limit_w +1, 0);
            for(int c=0; c <= n -l; c++){
                deque<int> dq;
                for(int r=0; r<m; r++){
                    while(!dq.empty() && temp[r][c] <= temp[dq.back()][c]) dq.pop_back();
                    dq.push_back(r);
                    while(!dq.empty() && dq.front() < r - limit_w +1) dq.pop_front();
                    if(r >= limit_w -1){
                        int w = limit_w;
                        int current_min = temp[dq.front()][c];
                        current_max_w[w] = max(current_max_w[w], current_min);
                    }
                }
                // To handle all possible w from 1 to limit_w
                // Reinitialize deque for smaller window sizes
                deque<int> dq_small;
                for(int w=1; w<=limit_w; w++){
                    // For each w, slide the window
                    // Not efficient, but keeping it simple
                    // Reuse the deque
                    // This part can be optimized further
                }
            }
            // Now, compute for each possible w, find the maximum d_min
            // Implement column sliding min for all possible w
            for(int w=1; w<=limit_w; w++){
                // For each column, compute sliding min with window size w
                // and keep the maximum d_min
                // To optimize, reuse deques
                // Initialize maximum d_min for this (w,l)
                int max_d_min = 0;
                for(int c=0; c <= n -l; c++){
                    deque<int> dq;
                    for(int r=0; r<m; r++){
                        while(!dq.empty() && temp[r][c] <= temp[dq.back()][c]) dq.pop_back();
                        dq.push_back(r);
                        while(!dq.empty() && dq.front() < r - w +1) dq.pop_front();
                        if(r >=w-1){
                            int current_min = temp[dq.front()][c];
                            max_d_min = max(max_d_min, current_min);
                        }
                    }
                }
                if(m*n - w*l <=0) continue;
                // Compute h
                ll numerator = (ll)max_d_min * m * n -1;
                ll denominator = (ll)m * n - (ll)w * l;
                if(denominator <=0) continue;
                ll h = numerator / denominator;
                if(h <=0) continue;
                ll V = h * (ll)w * l;
                max_V = max(max_V, V);
            }
        }
    };
    // Process normal orientation
    process(min(a, m), min(b, n));
    // Process swapped orientation
    if(a != b){
        process(min(b, m), min(a, n));
    }
    cout << max_V;
}
