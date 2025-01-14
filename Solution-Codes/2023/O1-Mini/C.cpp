#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    // Read D1
    int n1;
    cin >> n1;
    vector<int> D1(n1);
    for(auto &x: D1) cin >> x;
    sort(D1.begin(), D1.end());
    
    // Read D2
    int n2;
    cin >> n2;
    vector<int> D2(n2);
    for(auto &x: D2) cin >> x;
    sort(D2.begin(), D2.end());
    
    // Function to compute score(Da, Db) for a single x
    auto score = [&](const vector<int> &Da, int x, int n_a, int n_b) -> double {
        // E[Da > x] + 0.5 * E[Da ==x]
        int count_less = lower_bound(Da.begin(), Da.end(), x) - Da.begin();
        int count_eq = upper_bound(Da.begin(), Da.end(), x) - lower_bound(Da.begin(), Da.end(), x);
        double res = ((double)(count_less) + 0.5 * (double)(count_eq)) / (double)n_a;
        return res;
    };
    
    // Compute score(D1, D2)
    // Total pairs: n1 * n2
    // Count(D1 > D2) + 0.5 * Count(D1 == D2)
    ll total = (ll)n1 * n2;
    ll count_win =0;
    ll count_tie=0;
    for(auto x: D1){
        // Count D2 <x
        ll less = lower_bound(D2.begin(), D2.end(), x) - D2.begin();
        // Count D2 ==x
        ll first = lower_bound(D2.begin(), D2.end(), x) - D2.begin();
        ll last = upper_bound(D2.begin(), D2.end(), x) - D2.begin();
        ll equal = last - first;
        count_win += less;
        count_tie += equal;
    }
    double score_D1_D2 = ((double)count_win + 0.5 * (double)count_tie) / (double)total;
    
    // Determine which die is D1 (with advantage)
    // D1 is the die with advantage over D2
    // If score(D1,D2) >0.5, keep as is
    // Else, swap D1 and D2
    bool swap_flag = false;
    if(score_D1_D2 <= 0.5){
        swap(D1, D2);
        swap(n1, n2);
        swap_flag = true;
    }
    
    // Now, D1 has advantage over D2 (score(D1,D2) >0.5)
    
    // Part1: Find min score(D3,D2) where score(D3,D1)>=0.5
    // Which is finding the smallest x where 2 * count_f_less(x) + count_f_eq(x) >=n1
    // Then compute score(D3,D2)
    
    // Binary search for smallest x
    ll left = 1, right = 1000000000;
    ll answer_x_part1 = -1;
    while(left <= right){
        ll mid = left + (right - left)/2;
        // Count_f_less(x)= number of D1 <mid
        ll count_f_less = lower_bound(D1.begin(), D1.end(), (int)mid) - D1.begin();
        // Count_f_eq(x)= upper_bound - lower_bound
        ll first = lower_bound(D1.begin(), D1.end(), (int)mid) - D1.begin();
        ll last = upper_bound(D1.begin(), D1.end(), (int)mid) - D1.begin();
        ll count_f_eq = last - first;
        if(2 * count_f_less + count_f_eq >= n1){
            answer_x_part1 = mid;
            right = mid -1;
        }
        else{
            left = mid +1;
        }
    }
    
    double part1 = 1.0;
    if(answer_x_part1 != -1){
        // Compute score(D3,D2)
        // score(D3,D2) = [count_g_less +0.5 * count_g_eq] /n2
        ll count_g_less = lower_bound(D2.begin(), D2.end(), (int)answer_x_part1) - D2.begin();
        ll first_g = lower_bound(D2.begin(), D2.end(), (int)answer_x_part1) - D2.begin();
        ll last_g = upper_bound(D2.begin(), D2.end(), (int)answer_x_part1) - D2.begin();
        ll count_g_eq = last_g - first_g;
        part1 = ((double)count_g_less + 0.5 * (double)count_g_eq) / (double)n2;
    }
    
    // Part2: Find max score(D3,D1) where score(D3,D2)>=0.5
    // Which is finding the largest x where [2 * count_g_less(x) + count_g_eq(x)] >=n2
    // Then compute score(D3,D1)
    
    // Binary search for largest x
    left =1;
    right =1000000000;
    ll answer_x_part2 = -1;
    while(left <= right){
        ll mid = left + (right - left)/2;
        ll count_g_less = lower_bound(D2.begin(), D2.end(), (int)mid) - D2.begin();
        ll first_g = lower_bound(D2.begin(), D2.end(), (int)mid) - D2.begin();
        ll last_g = upper_bound(D2.begin(), D2.end(), (int)mid) - D2.begin();
        ll count_g_eq = last_g - first_g;
        if(2 * count_g_less + count_g_eq >= n2){
            answer_x_part2 = mid;
            left = mid +1;
        }
        else{
            right = mid -1;
        }
    }
    
    double part2 = 0.0;
    if(answer_x_part2 != -1){
        // Compute score(D3,D1)
        ll count_f_less = lower_bound(D1.begin(), D1.end(), (int)answer_x_part2) - D1.begin();
        ll first_f = lower_bound(D1.begin(), D1.end(), (int)answer_x_part2) - D1.begin();
        ll last_f = upper_bound(D1.begin(), D1.end(), (int)answer_x_part2) - D1.begin();
        ll count_f_eq = last_f - first_f;
        part2 = ((double)count_f_less + 0.5 * (double)count_f_eq) / (double)n1;
    }
    
    // Output with 9 decimal places
    cout << fixed << setprecision(9) << part1 << " " << part2;
}
