#include <bits/stdc++.h>
using namespace std;

/*
  Bridging the Gap

  We implement the solution as explained:

  1) Read n, c, and the n crossing times.
  2) Sort them ascending.
  3) If c >= n, answer = t[n-1].
  4) Else if c == 2, use the classical 2-person puzzle approach.
  5) Otherwise (c >= 3 and c < n), use the "groups of c" strategy.
*/

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, c;
    cin >> n >> c;
    vector<long long> t(n);
    for(int i=0; i<n; i++){
        cin >> t[i];
    }

    // Sort ascending
    sort(t.begin(), t.end());

    // If all can cross at once:
    if(c >= n){
        // The time = slowest among them
        cout << t[n-1] << "\n";
        return 0;
    }

    // If c == 2, use the standard 2-person puzzle solution:
    if(c == 2){
        long long ans = 0;
        int left = n; 
        // We'll label them as 1-based for convenience, but watch out in code:
        // We'll treat t[i] as i-based with i in [0..n-1].
        // So t[0] = t1, t[1] = t2, ...
        // We'll just do index math carefully.

        while(left > 3){
            // Strategy A: t[1]+2*t[2]+t[left] in 1-based logic
            // But in 0-based:
            //   t[0] = t1, t[1] = t2, t[left-1] = t_left
            // costA = t[1] + 2*t[0] + t[left-1]? That looks reversed, be careful.
            //
            // Actually the well-known formula is:
            //   costA = t[1] + t[0] + t[left-1] + t[1]
            // or costA = (t[1] + t[0] + t[left-1] + t[1])?
            // We must match the puzzle steps. Let's do it carefully:
            //
            // Strategy A:
            // 1) t[0] & t[1] go right => cost t[1]
            // 2) t[0] comes left => cost t[0]
            // 3) t[left-2] & t[left-1] go right => cost t[left-1]
            // 4) t[1] comes left => cost t[1]
            // total = t[1] + t[0] + t[left-1] + t[1] = t[left-1] + 2*t[1] + t[0]
            long long costA = t[1] + t[0] + t[left-1] + t[1];

            // Strategy B:
            // 1) t[0] & t[left-1] go right => cost t[left-1]
            // 2) t[0] goes left => cost t[0]
            // 3) t[0] & t[left-2] go right => cost t[left-2]
            // 4) t[0] goes left => cost t[0]
            // total = t[left-1] + t[left-2] + 2*t[0]
            long long costB = t[left-1] + t[left-2] + 2LL*t[0];

            ans += min(costA, costB);
            left -= 2; // we've successfully sent the 2 slowest across
        }

        // Now handle the final small group (left <= 3)
        if(left == 3){
            // e.g. t[0], t[1], t[2] => cost = t[0] + t[1] + t[2]
            ans += (t[0] + t[1] + t[2]);
        } 
        else if(left == 2){
            // just cost = t[1]
            ans += t[1];
        }
        else {
            // left == 1
            ans += t[0];
        }

        cout << ans << "\n";
        return 0;
    }

    // Now handle the case: 2 < c < n
    // We'll use the "send c at a time" approach
    long long total = 0;
    // i will point to the index of the slowest person *still* on the left side
    int i = n - 1;  // (0-based index of t)
    while(i + 1 > c){
        // Cross with group that includes t[0] and t[i], cost = t[i]
        total += t[i];       // crossing c people, slowest is t[i]
        total += t[0];       // fastest (t[0]) returns
        i -= (c - 1);        // effectively moved (c - 1) slowest across
    }
    // Now <= c remain on the left:
    total += t[i];  // final crossing for those i+1 people
    cout << total << "\n";

    return 0;
}
