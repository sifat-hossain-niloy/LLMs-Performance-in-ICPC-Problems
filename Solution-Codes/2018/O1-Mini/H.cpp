#include <bits/stdc++.h>
using namespace std;

struct Interval {
    int y_min;
    int y_max;
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    long long w, h;
    cin >> n >> w >> h;
    vector<Interval> intervals(n);
    for(int i=0;i<n;i++){
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        intervals[i].y_min = min(y1, y2);
        intervals[i].y_max = max(y1, y2);
    }
    // Sort intervals by y_max
    sort(intervals.begin(), intervals.end(), [&](const Interval &a, const Interval &b) -> bool{
        if(a.y_max != b.y_max) return a.y_max < b.y_max;
        return a.y_min < b.y_min;
    });
    vector<int> cuts;
    int current_cut = -1;
    for(int i=0;i<n;i++){
        if(current_cut < intervals[i].y_min){
            current_cut = intervals[i].y_max;
            cuts.push_back(current_cut);
        }
    }
    // Output
    cout << cuts.size() << "\n";
    for(auto c: cuts){
        // Output as integers: (0, c) to (w, c)
        // To handle large w and c, use integer types
        // But c is int, w is long long
        cout << "0 " << c << " " << w << " " << c << "\n";
    }
}
