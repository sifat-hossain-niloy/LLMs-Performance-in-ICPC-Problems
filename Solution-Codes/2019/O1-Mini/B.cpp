#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    ll h, alpha, beta;
    cin >> n >> h >> alpha >> beta;
    vector<pair<int, int>> points(n);
    for(int i=0;i<n;i++) cin >> points[i].first >> points[i].second;
    // Extract x and y
    vector<int> x(n), y(n);
    for(int i=0;i<n;i++){
        x[i] = points[i].first;
        y[i] = points[i].second;
    }
    // Build log table
    int LOG = 0;
    while((1<<LOG) <=n) LOG++;
    vector<int> log_table(n+1,0);
    for(int i=2;i<=n;i++) log_table[i] = log_table[i/2] +1;
    // Build sparse table
    vector<vector<int>> st(n, vector<int>(LOG, 0));
    for(int i=0;i<n;i++) st[i][0] = y[i];
    for(int j=1; j<LOG; j++){
        for(int i=0; i + (1<<j) <=n; i++){
            st[i][j] = max(st[i][j-1], st[i + (1<<(j-1))][j-1]);
        }
    }
    // Function to get max in [L,R]
    auto get_max = [&](int L, int R) -> int{
        int k = log_table[R - L +1];
        return max(st[L][k], st[R - (1<<k) +1][k]);
    };
    // Initialize dp
    const ll INF = 1e18;
    vector<ll> dp(n, INF);
    dp[0] = alpha * (h - (ll)y[0]);
    // Iterate over j
    for(int j=1; j<n; j++){
        // Iterate i from j-1 downto0
        for(int i=j-1; i>=0; i--){
            ll d = (ll)(x[j] - x[i]);
            // Get max_y in [i,j]
            int max_y = get_max(i,j);
            // Check 2*h >= 2*max_y +d
            if(2*h >= 2*(ll)max_y + d){
                // Update dp[j]
                ll cost = dp[i] + alpha * (h - (ll)y[j]) + beta * d * d;
                if(cost < dp[j]){
                    dp[j] = cost;
                }
            }
            else{
                // Since as i decreases, max_y increases, break
                break;
            }
        }
    }
    if(dp[n-1] < INF){
        cout << dp[n-1];
    }
    else{
        cout << "impossible";
    }
}
