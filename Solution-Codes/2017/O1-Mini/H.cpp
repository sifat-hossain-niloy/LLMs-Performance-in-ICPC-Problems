#include <bits/stdc++.h>
using namespace std;

struct Job {
    long long a;
    long long b;
    long long latest_start;
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    long long t;
    cin >> n >> t;
    vector<Job> jobs(n);
    for(int i=0;i<n;i++){
        cin >> jobs[i].a >> jobs[i].b;
        jobs[i].latest_start = jobs[i].b - t;
    }
    // Sort jobs by latest_start ascending
    sort(jobs.begin(), jobs.end(), [&](const Job &x, const Job &y) -> bool {
        if(x.latest_start != y.latest_start)
            return x.latest_start < y.latest_start;
        return x.a < y.a;
    });
    long long last_end_time = 1e18;
    bool possible = true;
    for(int i=n-1;i>=0;i--){
        long long end_time = min(jobs[i].b, last_end_time);
        long long start_time = end_time - t;
        if(start_time < jobs[i].a){
            possible = false;
            break;
        }
        last_end_time = start_time;
    }
    if(possible) cout << "yes";
    else cout << "no";
}
