#include <bits/stdc++.h>
using namespace std;

struct Tarp {
    int x1, y1, x2, y2;
    // Compressed x indices
    int cx1, cx2;
    // Index after sorting
    int index;
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    long long l, r;
    int n;
    cin >> l >> r >> n;
    vector<Tarp> tarps(n);
    vector<long long> xs;
    xs.push_back(l);
    xs.push_back(r);
    for(int i=0;i<n;i++){
        cin >> tarps[i].x1 >> tarps[i].y1 >> tarps[i].x2 >> tarps[i].y2;
        xs.push_back(tarps[i].x1);
        xs.push_back(tarps[i].x2);
        tarps[i].index = i;
    }
    // Sort and unique x's
    sort(xs.begin(), xs.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());
    int m = xs.size();
    // Function to get compressed index
    auto get_cx = [&](long long x) -> int {
        return lower_bound(xs.begin(), xs.end(), x) - xs.begin();
    };
    // Assign compressed x's
    for(int i=0;i<n;i++){
        tarps[i].cx1 = get_cx(tarps[i].x1);
        tarps[i].cx2 = get_cx(tarps[i].x2);
    }
    // Sort tarps in decreasing y order
    sort(tarps.begin(), tarps.end(), [&](const Tarp &a, const Tarp &b) -> bool{
        if(a.y2 != b.y2) return a.y2 > b.y2;
        return a.index < b.index;
    });
    // Initialize first_tarp[x] = -1 (ground)
    vector<int> first_tarp(m, -1);
    // Initialize union find
    vector<int> parent(m+1);
    for(int i=0;i<=m;i++) parent[i] = i;
    // Find with path compression
    function<int(int)> find_set = [&](int x) -> int{
        if(parent[x] != x){
            parent[x] = find_set(parent[x]);
        }
        return parent[x];
    };
    // Initialize next_tarp
    vector<int> next_tarp(n, -1); // -1 represents ground
    for(int i=0;i<n;i++){
        // Current tarp
        Tarp &tarp = tarps[i];
        int x1 = tarp.cx1;
        // next_tarp[tarp.index] = first_tarp[x1]
        next_tarp[tarp.index] = first_tarp[x1];
        // Set first_tarp[x] for [min(x1, x2), max(x1, x2)]
        int left = min(tarp.cx1, tarp.cx2);
        int right = max(tarp.cx1, tarp.cx2);
        int x = find_set(left);
        while(x <= right){
            first_tarp[x] = tarp.index;
            parent[x] = x+1;
            x = find_set(x);
        }
    }
    // Now, compute path_length
    // Sort tarps in increasing y order
    sort(tarps.begin(), tarps.end(), [&](const Tarp &a, const Tarp &b) -> bool{
        if(a.y2 != b.y2) return a.y2 < b.y2;
        return a.index < b.index;
    });
    // Create a map from tarp index to its position in sorted order
    // Not necessary, just process in sorted order
    // Initialize path_length
    vector<int> path_length(n, 0);
    for(int i=0;i<n;i++){
        Tarp &tarp = tarps[i];
        if(next_tarp[tarp.index] == -1){
            path_length[tarp.index] = 1;
        }
        else{
            path_length[tarp.index] = 1 + path_length[next_tarp[tarp.index]];
        }
    }
    // Now, find the compressed indices for l and r
    int cl = get_cx(l);
    int cr = get_cx(r);
    // Ensure cl <= cr
    if(cl > cr){
        swap(cl, cr);
    }
    // Iterate over x in [cl, cr] and find the minimal path_length
    // If any x has first_tarp[x] == -1, answer is 0
    // Else, find the minimal path_length[x]
    int answer = INT32_MAX;
    for(int x=cl; x<=cr; x++){
        if(first_tarp[x] == -1){
            answer = 0;
            break;
        }
        else{
            answer = min(answer, path_length[first_tarp[x]]);
        }
    }
    // If no tarps hit, answer remains 0
    // Else, answer is the minimal path_length
    // If answer was not updated, it means no x in [l, r] can reach the ground, but per problem constraints, it's possible
    // To handle n=0, if n==0, answer should be 0
    if(n ==0){
        cout << 0;
        return 0;
    }
    cout << answer;
}
