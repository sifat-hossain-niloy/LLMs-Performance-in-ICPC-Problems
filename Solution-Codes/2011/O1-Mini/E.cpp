#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int dx, dy, n, q;
    int case_num = 1;
    while(cin >> dx >> dy >> n >> q){
        if(dx ==0 and dy ==0 and n ==0 and q ==0) break;
        // Compute u_max and v_max
        int u_max = dx + dy;
        int v_max = dx + dy -1;
        // Initialize counts
        // counts and prefix_sum as 1D vectors
        // To save memory and speed, use vector<int>
        // Initialize counts to 0
        vector<int> counts((u_max +1) * (v_max +1), 0);
        // Read n coffee shops
        for(int i=0;i<n;i++){
            int x, y;
            cin >> x >> y;
            int u = x + y;
            int v = x - y + dy;
            if(u >=2 and u <= u_max and v >=1 and v <=v_max){
                counts[u * (v_max +1) + v]++;
            }
        }
        // Compute prefix_sum
        vector<int> prefix_sum((u_max +1) * (v_max +1), 0);
        for(int u=1; u <=u_max; u++){
            int u_offset = u * (v_max +1);
            int u_prev_offset = (u-1) * (v_max +1);
            for(int v=1; v <=v_max; v++){
                prefix_sum[u_offset + v] = counts[u_offset +v] 
                                            + prefix_sum[u_prev_offset +v] 
                                            + prefix_sum[u_offset +v -1] 
                                            - prefix_sum[u_prev_offset + v -1];
            }
        }
        // Read q queries
        vector<int> queries(q);
        for(int i=0;i<q;i++) cin >> queries[i];
        // Output
        cout << "Case " << case_num++ << ":\n";
        for(int i=0;i<q;i++){
            int m = queries[i];
            int max_count = -1;
            int best_x =1, best_y =1;
            for(int y=1; y <= dy; y++){
                for(int x=1; x <= dx; x++){
                    int u0 = x + y;
                    int v0 = x - y + dy;
                    // Compute u1, v1, u2, v2
                    int u1 = max(u0 - m, 2);
                    int v1 = max(v0 - m, 1);
                    int u2 = min(u0 + m, u_max);
                    int v2 = min(v0 + m, v_max);
                    // Compute sum using prefix_sum
                    int sum = prefix_sum[u2 * (v_max +1) + v2] 
                            - prefix_sum[(u1 -1) * (v_max +1) + v2] 
                            - prefix_sum[u2 * (v_max +1) + (v1 -1)] 
                            + prefix_sum[(u1 -1) * (v_max +1) + (v1 -1)];
                    if(sum > max_count){
                        max_count = sum;
                        best_x =x;
                        best_y = y;
                    }
                    else if(sum == max_count){
                        if(y < best_y || (y == best_y && x < best_x)){
                            best_x =x;
                            best_y = y;
                        }
                    }
                }
            }
            // Output the result for the query
            cout << max_count << " (" << best_x << "," << best_y << ")\n";
        }
    }
}
