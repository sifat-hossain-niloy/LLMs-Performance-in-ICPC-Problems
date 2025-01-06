#include <bits/stdc++.h>
using namespace std;

struct Point {
    long long x, y;
};

// Function to check if two points are equal
bool point_equal(const Point& a, const Point& b) {
    return a.x == b.x && a.y == b.y;
}

// Function to compute orientation of ordered triplet (p, q, r)
// Returns 0 if colinear, 1 if clockwise, 2 if counterclockwise
int orientation(const Point& p, const Point& q, const Point& r) {
    long long val = (q.x - p.x)*(r.y - p.y) - (q.y - p.y)*(r.x - p.x);
    if (val == 0) return 0; // colinear
    return (val > 0) ? 1 : 2; // clockwise or counterclockwise
}

// Function to check if point q lies on segment pr
bool on_segment(const Point& p, const Point& q, const Point& r) {
    if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
        q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
        return true;
    return false;
}

// Function to check if two segments (p1,p2) and (q1,q2) intersect
bool do_segments_intersect(const Point& p1, const Point& p2, const Point& q1, const Point& q2) {
    int o1 = orientation(p1, p2, q1);
    int o2 = orientation(p1, p2, q2);
    int o3 = orientation(q1, q2, p1);
    int o4 = orientation(q1, q2, p2);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special Cases
    // p1, p2 and q1 are colinear and q1 lies on segment p1p2
    if (o1 == 0 && on_segment(p1, q1, p2)) return true;

    // p1, p2 and q2 are colinear and q2 lies on segment p1p2
    if (o2 == 0 && on_segment(p1, q2, p2)) return true;

    // q1, q2 and p1 are colinear and p1 lies on segment q1q2
    if (o3 == 0 && on_segment(q1, p1, q2)) return true;

    // q1, q2 and p2 are colinear and p2 lies on segment q1q2
    if (o4 == 0 && on_segment(q1, p2, q2)) return true;

    return false; // Doesn't fall in any of the above cases
}

// Function to check if two segments properly intersect (not just at shared endpoints)
bool segments_properly_intersect(const Point& p1, const Point& p2, const Point& q1, const Point& q2) {
    if (!do_segments_intersect(p1, p2, q1, q2))
        return false;
    // Check if they share a common endpoint
    if (point_equal(p1, q1) || point_equal(p1, q2) || point_equal(p2, q1) || point_equal(p2, q2))
        return false; // Intersection at shared endpoint is allowed
    return true; // Proper intersection
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    cin >> n;
    vector<Point> polygon(n);
    for(int i=0;i<n;i++) cin >> polygon[i].x >> polygon[i].y;
    double max_dist = 0.0;
    for(int i=0;i<n;i++){
        for(int j=i+1;j<n;j++){
            bool valid = true;
            for(int k=0; k<n; k++){
                int k1 = k;
                int k2 = (k+1)%n;
                // Check if segments (i,j) and (k1,k2) properly intersect
                if(segments_properly_intersect(polygon[i], polygon[j], polygon[k1], polygon[k2])){
                    valid = false;
                    break;
                }
            }
            if(valid){
                long long dx = polygon[i].x - polygon[j].x;
                long long dy = polygon[i].y - polygon[j].y;
                double dist = sqrt((double)dx*dx + (double)dy*dy);
                if(dist > max_dist) max_dist = dist;
            }
        }
    }
    // Output with at least 9 decimal places
    cout << fixed << setprecision(9) << max_dist;
}
