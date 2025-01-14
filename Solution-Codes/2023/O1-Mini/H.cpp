#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    cin >> n;
    vector<pair<ll, ll>> activities(n);
    for(int i=0;i<n;i++) cin >> activities[i].first >> activities[i].second;
    // Collect gaps
    vector<pair<ll, ll>> gaps;
    ll last_end = 0;
    for(int i=0;i<n;i++){
        ll b = activities[i].first;
        ll e = activities[i].second;
        if(last_end < b){
            gaps.emplace_back(last_end, b);
        }
        last_end = e;
    }
    // The end time is the end of the last activity
    ll en = activities[n-1].second;
    // Check if first gap starts at 0
    if(gaps.empty() || gaps[0].first !=0){
        cout << "impossible\n";
        return 0;
    }
    // Schedule sleep periods
    vector<pair<ll, ll>> sleep_periods;
    ll earliest_available_time =0;
    for(int i=0;i<gaps.size();i++){
        ll g_start = gaps[i].first;
        ll g_end = gaps[i].second;
        ll s = max(g_start, earliest_available_time);
        if(s >=g_end) continue;
        ll next_gap_start;
        if(i+1 < gaps.size()){
            next_gap_start = gaps[i+1].first;
        }
        else{
            next_gap_start = en;
        }
        ll k_i = min(g_end -s, next_gap_start -s);
        if(k_i >=1){
            sleep_periods.emplace_back(s, s +k_i);
            earliest_available_time = s + 2 *k_i;
        }
        // else cannot schedule sleep period here
    }
    // Check that the first sleep period starts at0
    if(sleep_periods.empty() || sleep_periods[0].first !=0){
        cout << "impossible\n";
        return 0;
    }
    // Now, check that all sleep periods do not overlap activities
    // and sleep periods are within gaps
    // Since we scheduled them within gaps, it's already ensured
    // Also, sleep periods must not go beyond en
    bool valid=true;
    for(auto &[s, t]: sleep_periods){
        if(t > en){
            valid=false;
            break;
        }
    }
    if(valid){
        cout << sleep_periods.size() << "\n";
        for(auto &[s, t]: sleep_periods){
            cout << s << " " << t << "\n";
        }
    }
    else{
        cout << "impossible\n";
    }
}
