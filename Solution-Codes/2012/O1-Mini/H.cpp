#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-9;

struct Point {
    double x, y;
};

// Function to compute orientation
int orientation(const Point& p, const Point& q, const Point& r) {
    double val = (q.x - p.x)*(r.y - p.y) - (q.y - p.y)*(r.x - p.x);
    if (abs(val) < EPS) return 0; // colinear
    return (val > 0) ? 1 : 2; // clock or counterclock wise
}

// Function to check if point q lies on segment pr
bool on_segment(const Point& p, const Point& q, const Point& r) {
    if (q.x <= max(p.x, r.x) + EPS && q.x >= min(p.x, r.x) - EPS &&
        q.y <= max(p.y, r.y) + EPS && q.y >= min(p.y, r.y) - EPS)
        return true;
    return false;
}

// Function to check if two segments intersect
bool segments_intersect(const Point& p1, const Point& p2, const Point& q1, const Point& q2) {
    int o1 = orientation(p1, p2, q1);
    int o2 = orientation(p1, p2, q2);
    int o3 = orientation(q1, q2, p1);
    int o4 = orientation(q1, q2, p2);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special Cases
    if (o1 == 0 && on_segment(p1, q1, p2)) return true;
    if (o2 == 0 && on_segment(p1, q2, p2)) return true;
    if (o3 == 0 && on_segment(q1, p1, q2)) return true;
    if (o4 == 0 && on_segment(q1, p2, q2)) return true;

    return false;
}

// Function to check if loop S-P-Q-S intersects all walls
bool loop_covers_all_walls(const Point& S, const Point& P, const Point& Q, const vector<pair<Point, Point>>& walls) {
    // Define the loop segments
    pair<Point, Point> seg1 = {S, P};
    pair<Point, Point> seg2 = {P, Q};
    pair<Point, Point> seg3 = {Q, S};

    // For each wall, check if it is intersected by at least one of seg1, seg2, seg3
    for(auto &wall : walls){
        bool covered = false;
        // Check intersection with seg1
        if(segments_intersect(seg1.first, seg1.second, wall.first, wall.second)){
            covered = true;
        }
        // Check intersection with seg2
        if(!covered && segments_intersect(seg2.first, seg2.second, wall.first, wall.second)){
            covered = true;
        }
        // Check intersection with seg3
        if(!covered && segments_intersect(seg3.first, seg3.second, wall.first, wall.second)){
            covered = true;
        }
        if(!covered){
            return false;
        }
    }
    return true;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int N;
    int case_num = 1;
    while(cin >> N){
        Point S;
        cin >> S.x >> S.y;
        vector<Point> vertices(N);
        for(int i=0;i<N;i++) cin >> vertices[i].x >> vertices[i].y;
        // Create walls
        vector<pair<Point, Point>> walls(N);
        for(int i=0;i<N;i++) {
            walls[i] = {vertices[i], vertices[(i+1)%N]};
        }
        // Generate touch points: sample K points per edge
        int K = 10; // Number of samples per edge
        vector<Point> touch_points;
        for(int i=0;i<N;i++){
            Point p1 = vertices[i];
            Point p2 = vertices[(i+1)%N];
            for(int k=0; k<=K; k++){
                double t = (double)k / K;
                Point p;
                p.x = p1.x + t*(p2.x - p1.x);
                p.y = p1.y + t*(p2.y - p1.y);
                touch_points.push_back(p);
            }
        }
        // Remove duplicate points (vertices are included multiple times)
        // Not strictly necessary, but can speed up a bit
        // Alternatively, keep all points
        // Enumerate all pairs P, Q
        double min_total = 1e18;
        int M = touch_points.size();
        for(int i=0;i<M;i++){
            for(int j=i+1;j<M;j++){
                Point P = touch_points[i];
                Point Q = touch_points[j];
                if(loop_covers_all_walls(S, P, Q, walls)){
                    // Compute S-P + P-Q + Q-S
                    double dist_SP = sqrt( (S.x - P.x)*(S.x - P.x) + (S.y - P.y)*(S.y - P.y) );
                    double dist_PQ = sqrt( (P.x - Q.x)*(P.x - Q.x) + (P.y - Q.y)*(P.y - Q.y) );
                    double dist_QS = sqrt( (Q.x - S.x)*(Q.x - S.x) + (Q.y - S.y)*(Q.y - S.y) );
                    double total = dist_SP + dist_PQ + dist_QS;
                    if(total < min_total - EPS){
                        min_total = total;
                    }
                }
            }
        }
        // Additionally, check if using the same point P=Q can cover all walls
        // Only possible if all walls are intersected by S-P
        for(int i=0;i<M;i++){
            Point P = touch_points[i];
            // Check if S-P intersects all walls
            bool covers = true;
            pair<Point, Point> seg = {S, P};
            for(auto &wall : walls){
                if(!segments_intersect(seg.first, seg.second, wall.first, wall.second)){
                    covers = false;
                    break;
                }
            }
            if(covers){
                double dist_SP = sqrt( (S.x - P.x)*(S.x - P.x) + (S.y - P.y)*(S.y - P.y) );
                double total = dist_SP * 2.0;
                if(total < min_total - EPS){
                    min_total = total;
                }
            }
        }
        // If no pair found, min_total remains 1e18
        // But problem guarantees at least one path
        // Output the result
        printf("Case %d: %.2lf\n", case_num++, min_total);
    }
}
