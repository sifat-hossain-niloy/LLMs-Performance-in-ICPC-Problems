#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const int INF = 1e9;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, m;
    int case_num =1;
    while(cin >> n >> m){
        // Initialize distance matrix
        vector<vector<int>> dist(n, vector<int>(n, INF));
        for(int i=0;i<n;i++) dist[i][i]=0;
        // Read roads
        for(int i=0;i<m;i++){
            int u, v, t;
            cin >> u >> v >> t;
            if(t < dist[u][v]){
                dist[u][v] = t;
                dist[v][u] = t;
            }
        }
        // Floyd-Warshall
        for(int k=0;k<n;k++){
            for(int i=0;i<n;i++){
                for(int j=0;j<n;j++){
                    if(dist[i][k] < INF && dist[k][j] < INF){
                        if(dist[i][j] > dist[i][k] + dist[k][j]){
                            dist[i][j] = dist[i][k] + dist[k][j];
                        }
                    }
                }
            }
        }
        // Identify hotels
        int h = n-2;
        vector<int> hotels;
        for(int i=1;i<=n-2;i++) hotels.push_back(i);
        // If no hotels, just go from 0 to n-1 and back
        if(h ==0){
            if(dist[0][n-1] == INF){
                // Impossible
                cout << "Case " << case_num++ << ": " << "impossible\n";
            }
            else{
                cout << "Case " << case_num++ << ": " << dist[0][n-1]*2 << "\n";
            }
            continue;
        }
        // If h is too big, skip (but as per problem likely h<=10)
        if(h > 10){
            // Implement a heuristic or skip
            // Here, we implement up to h=10
            // If h>10, output -1 or similar
            // But problem likely has h<=10
            // For safety, proceed but it may be too slow
        }
        // Enumerate all permutations
        // Initialize minimal sum to INF
        ll min_sum = 1e18;
        // Sort the hotels for next_permutation
        sort(hotels.begin(), hotels.end());
        do{
            // Outgoing path: 0 -> h1 -> h2 -> ... -> hk -> n-1
            // Incoming path: n-1 -> h1 -> h2 -> ... -> hk -> 0
            // Compute the sum as:
            // distance[0][h1} + distance[n-1][h1} + distance[hk][n-1} + distance[hk][0} + 2 * sum(distance[h_i][h_{i+1}})
            ll sum =0;
            int h1 = hotels[0];
            int hk = hotels[h-1];
            if(dist[0][h1] == INF || dist[n-1][h1] == INF || dist[hk][n-1] == INF || dist[hk][0] == INF){
                // Impossible permutation
                continue;
            }
            sum += dist[0][h1];
            sum += dist[n-1][h1];
            sum += dist[hk][n-1];
            sum += dist[hk][0];
            bool valid = true;
            ll inter_sum =0;
            for(int i=0;i<h-1;i++){
                int from = hotels[i];
                int to = hotels[i+1];
                if(dist[from][to] == INF){
                    valid = false;
                    break;
                }
                inter_sum += 2LL * dist[from][to];
            }
            if(!valid){
                continue;
            }
            sum += inter_sum;
            if(sum < min_sum){
                min_sum = sum;
            }
        }while(next_permutation(hotels.begin(), hotels.end()));
        // Output
        cout << "Case " << case_num++ << ": " << min_sum << "\n";
    }
}
