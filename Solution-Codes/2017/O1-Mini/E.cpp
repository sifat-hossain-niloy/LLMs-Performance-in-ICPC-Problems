#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    double t;
    cin >> n >> t;
    vector<pair<int, int>> segments(n);
    double min_s = 1e18;
    for(int i=0;i<n;i++){
        cin >> segments[i].first >> segments[i].second;
        if(segments[i].second < min_s){
            min_s = segments[i].second;
        }
    }
    // Find c_low
    double c_low = -1e12;
    for(int i=0;i<n;i++){
        if(segments[i].second + c_low <= 0){
            // c must be > -s_i
            c_low = max(c_low, (double)(-segments[i].second) + 1e-12);
        }
    }
    // To ensure c_low is at least -min_s + 1e-8
    c_low = -min_s + 1e-8;
    // c_high can be set to 1e12
    double c_high = 1e12;
    // Binary search
    for(int iter=0; iter<100; iter++){
        double mid = (c_low + c_high) / 2.0;
        double total = 0.0;
        bool valid = true;
        for(int i=0;i<n;i++){
            double speed = segments[i].second + mid;
            if(speed <= 0){
                valid = false;
                break;
            }
            total += segments[i].first / speed;
        }
        if(!valid){
            // c is too low
            c_low = mid;
            continue;
        }
        if(total > t){
            // Need to increase c
            c_low = mid;
        }
        else{
            // Need to decrease c
            c_high = mid;
        }
    }
    // After binary search, take c_high as the answer
    // To ensure f(c_high) <=0 and f(c_low) >=0
    // We can take (c_low + c_high)/2
    double c = (c_low + c_high) / 2.0;
    // To ensure precision, print with 12 decimal digits
    cout << fixed << setprecision(10) << c;
}
