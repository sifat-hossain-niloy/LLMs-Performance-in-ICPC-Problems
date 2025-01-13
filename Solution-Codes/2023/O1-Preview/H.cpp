#include <bits/stdc++.h>
using namespace std;

/*
  We'll implement the greedy strategy described above.

  Steps:
   1) Read n.
   2) Read the intervals [b_i, e_i].
   3) If b_1 == 0 => impossible immediately.
   4) current = 0, i = 0 (indexing from 0..n-1 in the code).
   5) While i < n:
        - Let b = b[i], e = e[i].
        - If b <= current < e => impossible (we're forced to sleep inside an activity).
        - k = b - current (must be > 0).
        - Sleep [current, current + k].
        - Next cycle start = current + 3*k.
        - Mark all subsequent activities whose b_j < (current + 3*k) as "covered" in awake time. So increment i for all that.
        - current = current + 3*k.
        - If current >= b_n (i.e. we've passed or reached the start of the last activity),
          then we can't schedule new sleeps. We'll see if we've already covered all activities. If not => impossible.
     endWhile
   6) If we covered all activities => output the sleeps. Otherwise => impossible.
*/

static const long long INF = (long long)1e18;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<long long> b(n), e(n);
    for(int i=0; i<n; i++){
        cin >> b[i] >> e[i];
    }

    // If the first activity starts at time 0, we have no room to sleep >0 minutes at time 0.
    if(b[0] == 0){
        cout << "impossible\n";
        return 0;
    }

    // We'll store the sleeps as pairs (s_i, t_i).
    vector<pair<long long,long long>> sleeps;
    sleeps.reserve(n + 10); // just in case

    long long current = 0;  // time we start the next sleep
    int i = 0;              // index into activities

    while(i < n){
        // Check if the next activity [b[i], e[i]) starts at or before 'current'
        // If so, see if that means an overlap:
        if(b[i] <= current && current < e[i]){
            // The next activity is already in progress at 'current',
            // so we must sleep at 'current' which intersects the activity => impossible
            cout << "impossible\n";
            return 0;
        }

        // We want to sleep up until the next activity start b[i].
        long long gap = b[i] - current; // this is our k
        if(gap <= 0){
            // No positive integer length for sleep => impossible
            cout << "impossible\n";
            return 0;
        }

        // We'll sleep [current, current+gap).
        long long s = current;
        long long t = current + gap;
        sleeps.push_back({s, t});

        // Now forced+optional awake covers [t, t + 2*gap).
        // So the earliest we might start the *next* sleep cycle is t + 2*gap = s + 3*gap
        long long nextCycle = s + 3LL*gap;

        // Skip all activities whose b_j < nextCycle (i.e. they start in the awake portion).
        while(i < n && b[i] < nextCycle){
            i++;
        }

        // Advance 'current'
        current = nextCycle;

        // If we've reached or passed the start of the last activity,
        // but still have some activities left, check feasibility:
        if(current >= b[n-1] && i < n){
            // If we haven't skipped them all, then we can't schedule new sleeps
            // (because sleeping at or beyond b_n is disallowed),
            // so we must be "awake" for the last activities. But are they covered?
            // Actually, we only skip an activity if it started < current. If some
            // b[i] >= current, we can't skip it -> impossible.
            // Let's see if the next unskipped b[i] is < current => skip. Otherwise impossible.
            // But let's do that check in the while loop below.
            break;
        }
    }

    // If i >= n, we've successfully covered all activities.
    if(i >= n){
        // All activities covered.
        // Now we just need to check that all sleep intervals end by b_n:
        // i.e. for each (s,t), we need t <= b_n.  Let's enforce the problem's final constraint.
        long long lastB = b[n-1]; // the start of the last activity
        for(auto &pp: sleeps){
            if(pp.second > lastB){
                // That means we ended a sleep beyond b_n => not allowed
                cout << "impossible\n";
                return 0;
            }
        }
        // Otherwise good
        cout << sleeps.size() << "\n";
        for(auto &pp: sleeps){
            cout << pp.first << " " << pp.second << "\n";
        }
        return 0;
    }
    else {
        // i < n means we still have unskipped activities. Let's see if they are automatically covered:
        // Our last 'current' might be >= b[n-1], so let's skip again if b[i] < current
        while(i < n && b[i] < current){
            i++;
        }
        if(i >= n){
            // all covered
            // again check the 't <= b_n' condition
            long long lastB = b[n-1];
            for(auto &pp: sleeps){
                if(pp.second > lastB){
                    cout << "impossible\n";
                    return 0;
                }
            }
            cout << sleeps.size() << "\n";
            for(auto &pp: sleeps){
                cout << pp.first << " " << pp.second << "\n";
            }
            return 0;
        }
        else {
            // There's still at least one activity that starts at or after 'current',
            // and we cannot place any more sleeps if current >= b_n.
            // => impossible
            cout << "impossible\n";
            return 0;
        }
    }
}
