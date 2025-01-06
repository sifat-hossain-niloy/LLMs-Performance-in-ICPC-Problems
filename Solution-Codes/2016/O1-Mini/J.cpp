#include <bits/stdc++.h>
using namespace std;

struct Point {
    int a;
    int b;
    int c;
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    cin >> n;
    vector<Point> points(n);
    int m =0;
    for(int i=0;i<n;i++){
        cin >> points[i].a >> points[i].b >> points[i].c;
        if(points[i].c ==1) m++;
    }
    if(m ==0){
        cout << 0;
        return 0;
    }
    // Function to find minimal window size in sorted order
    auto find_min_k = [&](const vector<Point> &sorted_points) -> int {
        int l=0, count=0;
        int min_k = n+1;
        for(int r=0;r<sorted_points.size();r++){
            if(sorted_points[r].c ==1){
                count++;
            }
            while(count == m){
                min_k = min(min_k, r - l +1);
                if(sorted_points[l].c ==1){
                    count--;
                }
                l++;
            }
        }
        return min_k;
    };
    // Define multiple sorted orders
    // 1. Sort by a ascending, then b ascending
    vector<Point> sorted_a = points;
    sort(sorted_a.begin(), sorted_a.end(), [&](const Point &x, const Point &y) -> bool {
        if(x.a != y.a) return x.a < y.a;
        return x.b < y.b;
    });
    // 2. Sort by b ascending, then a ascending
    vector<Point> sorted_b = points;
    sort(sorted_b.begin(), sorted_b.end(), [&](const Point &x, const Point &y) -> bool {
        if(x.b != y.b) return x.b < y.b;
        return x.a < y.a;
    });
    // 3. Sort by (a + b) ascending, then a ascending
    vector<Point> sorted_a_plus_b = points;
    sort(sorted_a_plus_b.begin(), sorted_a_plus_b.end(), [&](const Point &x, const Point &y) -> bool {
        long long sum_x = (long long)x.a + (long long)x.b;
        long long sum_y = (long long)y.a + (long long)y.b;
        if(sum_x != sum_y) return sum_x < sum_y;
        return x.a < y.a;
    });
    // 4. Sort by (a - b) ascending, then a ascending
    vector<Point> sorted_a_minus_b = points;
    sort(sorted_a_minus_b.begin(), sorted_a_minus_b.end(), [&](const Point &x, const Point &y) -> bool {
        long long diff_x = (long long)x.a - (long long)x.b;
        long long diff_y = (long long)y.a - (long long)y.b;
        if(diff_x != diff_y) return diff_x < diff_y;
        return x.a < y.a;
    });
    // 5. Sort by a * 1e6 + b to handle some mixed ordering
    vector<Point> sorted_mixed = points;
    sort(sorted_mixed.begin(), sorted_mixed.end(), [&](const Point &x, const Point &y) -> bool {
        long long key_x = (long long)x.a * 1000000 + (long long)x.b;
        long long key_y = (long long)y.a * 1000000 + (long long)y.b;
        if(key_x != key_y) return key_x < key_y;
        return x.a < y.a;
    });
    // Collect all sorted orders
    vector<vector<Point>> sorted_orders = {sorted_a, sorted_b, sorted_a_plus_b, sorted_a_minus_b, sorted_mixed};
    int answer = n+1;
    for(auto &sorted_points : sorted_orders){
        int current_k = find_min_k(sorted_points);
        answer = min(answer, current_k);
    }
    // Edge case: if all ci=1 points are the same, check k= m
    // Not necessary, since sliding window would capture it
    cout << answer;
}
