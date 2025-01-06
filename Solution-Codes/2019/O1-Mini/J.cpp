#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int p, h;
    cin >> p >> h;
    vector<vector<ll>> scores(p, vector<ll>(h));
    vector<ll> unique_l;
    for(int i=0;i<p;i++){
        for(int j=0;j<h;j++){
            cin >> scores[i][j];
            unique_l.push_back(scores[i][j]);
        }
    }
    // Sort and remove duplicates
    sort(unique_l.begin(), unique_l.end());
    unique_l.erase(unique(unique_l.begin(), unique_l.end()), unique_l.end());
    int N = unique_l.size();
    // Sort scores and compute prefix sums
    vector<vector<ll>> sorted_scores(p, vector<ll>(h));
    vector<vector<ll>> prefix_sum(p, vector<ll>(h+1, 0));
    for(int i=0;i<p;i++){
        sorted_scores[i] = scores[i];
        sort(sorted_scores[i].begin(), sorted_scores[i].end());
        for(int j=0;j<h;j++){
            prefix_sum[i][j+1] = prefix_sum[i][j] + sorted_scores[i][j];
        }
    }
    // Precompute S_i(l) for all players and all l's
    vector<vector<ll>> S(p, vector<ll>(N, 0));
    for(int i=0;i<p;i++){
        int m =0;
        for(int l_idx=0; l_idx<N; l_idx++){
            ll l = unique_l[l_idx];
            while(m < h && sorted_scores[i][m] <= l){
                m++;
            }
            S[i][l_idx] = prefix_sum[i][m] + (ll)(h - m) * l;
        }
    }
    // Initialize max_count
    vector<int> max_count(p, 0);
    // Temporary vector for sorting
    vector<pair<ll, int>> tmp(p);
    for(int l_idx=0; l_idx<N; l_idx++){
        for(int i=0;i<p;i++) tmp[i] = {S[i][l_idx], i};
        sort(tmp.begin(), tmp.end());
        int j=0;
        while(j < p){
            ll current_S = tmp[j].first;
            int k = j;
            while(k < p && tmp[k].first == current_S){
                k++;
            }
            int count = p - k;
            for(int m=j; m<k; m++){
                int player_id = tmp[m].second;
                if(count > max_count[player_id]){
                    max_count[player_id] = count;
                }
            }
            j = k;
        }
    }
    // Compute minimal ranks
    for(int i=0;i<p;i++){
        ll minimal_rank = 1 + (ll)(p-1) - (ll)(max_count[i]);
        cout << minimal_rank << "\n";
    }
}
