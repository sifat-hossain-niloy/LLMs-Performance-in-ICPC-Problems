#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int m, n;
    cin >> m >> n;
    vector<pair<int, int>> producers(m);
    for(int i=0; i<m; i++) cin >> producers[i].second >> producers[i].first; // (di, pi)
    vector<pair<int, int>> consumers(n);
    for(int i=0; i<n; i++) cin >> consumers[i].second >> consumers[i].first; // (ej, qj)
    sort(producers.begin(), producers.end(), [&](const pair<int, int> &a, const pair<int, int> &b) -> bool{
        if(a.first != b.first) return a.first < b.first;
        return a.second < b.second;
    });
    sort(consumers.begin(), consumers.end(), [&](const pair<int, int> &a, const pair<int, int> &b) -> bool{
        if(a.first != b.first) return a.first < b.first;
        return a.second < b.second;
    });
    ll max_profit = 0;
    int pointer_p =0;
    ll min_pi = LLONG_MAX;
    int di_of_min_pi = 0;
    while(pointer_p < m && producers[pointer_p].first < 1){
        // Edge case where ej starts from 1
        if(producers[pointer_p].second < min_pi){
            min_pi = producers[pointer_p].second;
            di_of_min_pi = producers[pointer_p].first;
        }
        else if(producers[pointer_p].second == min_pi && producers[pointer_p].first > di_of_min_pi){
            di_of_min_pi = producers[pointer_p].first;
        }
        pointer_p++;
    }
    for(int i=0; i<n; i++){
        int ej = consumers[i].first;
        int qj = consumers[i].second;
        // Insert all producers with di < ej
        while(pointer_p < m && producers[pointer_p].first < ej){
            if(producers[pointer_p].second < min_pi){
                min_pi = producers[pointer_p].second;
                di_of_min_pi = producers[pointer_p].first;
            }
            else if(producers[pointer_p].second == min_pi && producers[pointer_p].first > di_of_min_pi){
                di_of_min_pi = producers[pointer_p].first;
            }
            pointer_p++;
        }
        if(min_pi < (ll)qj && di_of_min_pi < ej){
            ll profit = ((ll)qj - min_pi) * ((ll)ej - di_of_min_pi);
            if(profit > max_profit){
                max_profit = profit;
            }
        }
    }
    cout << max_profit;
}
