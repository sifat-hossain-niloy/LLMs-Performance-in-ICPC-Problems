#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-9;

struct Point {
    double x, y;
    Point() : x(0), y(0) {}
    Point(double _x, double _y): x(_x), y(_y) {}
    Point operator-(const Point& p) const { return Point(x - p.x, y - p.y); }
    Point operator+(const Point& p) const { return Point(x + p.x, y + p.y); }
    double dot(const Point& p) const { return x * p.x + y * p.y; }
    double cross(const Point& p) const { return x * p.y - y * p.x; }
};

// Rotate point p by angle theta around origin
Point rotate_point(const Point& p, double theta) {
    double cos_theta = cos(theta);
    double sin_theta = sin(theta);
    double x_new = p.x * cos_theta - p.y * sin_theta;
    double y_new = p.x * sin_theta + p.y * cos_theta;
    return Point(x_new, y_new);
}

// Check orientation of ordered triplet (p, q, r)
// Returns 0 if collinear, 1 if clockwise, 2 if counterclockwise
int orientation(const Point& p, const Point& q, const Point& r) {
    double val = (q - p).cross(r - p);
    if (fabs(val) < EPS) return 0; // collinear
    return (val > 0) ? 1 : 2; // clockwise or counterclockwise
}

// Check if point r lies on segment pq
bool on_segment(const Point& p, const Point& q, const Point& r) {
    if (min(p.x, q.x) - EPS <= r.x && r.x <= max(p.x, q.x) + EPS &&
        min(p.y, q.y) - EPS <= r.y && r.y <= max(p.y, q.y) + EPS)
        return true;
    return false;
}

// Check if segments p1p2 and q1q2 intersect
bool segments_intersect(const Point& p1, const Point& p2, const Point& q1, const Point& q2) {
    int o1 = orientation(p1, p2, q1);
    int o2 = orientation(p1, p2, q2);
    int o3 = orientation(q1, q2, p1);
    int o4 = orientation(q1, q2, p2);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special Cases
    if (o1 == 0 && on_segment(p1, p2, q1)) return true;
    if (o2 == 0 && on_segment(p1, p2, q2)) return true;
    if (o3 == 0 && on_segment(q1, q2, p1)) return true;
    if (o4 == 0 && on_segment(q1, q2, p2)) return true;

    return false;
}

// Check if two segments are collinear
bool are_segments_collinear(const Point& p1, const Point& p2, const Point& q1, const Point& q2) {
    int o1 = orientation(p1, p2, q1);
    int o2 = orientation(p1, p2, q2);
    return (o1 == 0 && o2 == 0);
}

// Compute overlapping length of two collinear segments p1p2 and q1q2
double overlapping_length(const Point& p1, const Point& p2, const Point& q1, const Point& q2) {
    // Direction vector of p1p2
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    double norm = sqrt(dx * dx + dy * dy);
    if (norm < EPS) return 0.0;
    // Unit direction vector
    double ux = dx / norm;
    double uy = dy / norm;

    // Project q1 and q2 onto p1p2
    double t1 = ( (q1.x - p1.x) * ux + (q1.y - p1.y) * uy );
    double t2 = ( (q2.x - p1.x) * ux + (q2.y - p1.y) * uy );

    // Order t1 and t2
    if (t1 > t2) swap(t1, t2);

    // Segment p1p2 is from 0 to norm
    double overlap_min = max(t1, (double)0);
    double overlap_max = min(t2, norm);
    double overlap = max(0.0, overlap_max - overlap_min);
    return overlap;
}

// Check if point is inside polygon using ray casting
bool is_point_inside_polygon(const Point& p, const vector<Point>& poly) {
    int n = poly.size();
    bool inside = false;
    for(int i = 0; i < n; ++i){
        int j = (i + 1) % n;
        const Point& a = poly[i];
        const Point& b = poly[j];
        // Check if point is on the edge
        // Not necessary for overlap check, since overlapping edges are handled separately
        // Ray casting algorithm
        if (((a.y > p.y) != (b.y > p.y))) {
            double x_intersect = (b.x - a.x) * (p.y - a.y) / (b.y - a.y) + a.x;
            if (p.x < x_intersect)
                inside = !inside;
        }
    }
    return inside;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    // Read first polygon
    int n1;
    cin >> n1;
    vector<Point> P1(n1);
    for(int i = 0; i < n1; ++i){
        cin >> P1[i].x >> P1[i].y;
    }
    // Read second polygon
    int n2;
    cin >> n2;
    vector<Point> P2(n2);
    for(int i = 0; i < n2; ++i){
        cin >> P2[i].x >> P2[i].y;
    }
    double max_overlap = 0.0;
    // Iterate over all edge pairs
    for(int i = 0; i < n1; ++i){
        int i_next = (i + 1) % n1;
        Point p1 = P1[i];
        Point p2 = P1[i_next];
        // Compute angle of edge p1p2
        double angle1 = atan2(p2.y - p1.y, p2.x - p1.x);
        for(int j = 0; j < n2; ++j){
            int j_next = (j + 1) % n2;
            Point q1 = P2[j];
            Point q2 = P2[j_next];
            // Compute angle of edge q1q2
            double angle2 = atan2(q2.y - q1.y, q2.x - q1.x);
            // Compute rotation angle
            double theta = angle1 - angle2;
            // Rotate P2 by theta
            vector<Point> P2_rot(n2);
            for(int k = 0; k < n2; ++k){
                P2_rot[k] = rotate_point(P2[k], theta);
            }
            // Compute translation: map rotated q1 to p1
            Point rotated_q1 = P2_rot[j];
            double dx = p1.x - rotated_q1.x;
            double dy = p1.y - rotated_q1.y;
            // Translate P2_rot by (dx, dy)
            vector<Point> P2_trans(n2);
            for(int k = 0; k < n2; ++k){
                P2_trans[k] = Point(P2_rot[k].x + dx, P2_rot[k].y + dy);
            }
            // Check if interiors overlap
            bool overlap = false;
            // Check edge intersections
            for(int a = 0; a < n1 && !overlap; ++a){
                int a_next = (a + 1) % n1;
                Point pa1 = P1[a];
                Point pa2 = P1[a_next];
                for(int b = 0; b < n2 && !overlap; ++b){
                    int b_next = (b + 1) % n2;
                    Point pb1 = P2_trans[b];
                    Point pb2 = P2_trans[b_next];
                    if(segments_intersect(pa1, pa2, pb1, pb2)){
                        overlap = true;
                    }
                }
            }
            if(!overlap){
                // Check if one polygon is inside the other
                // Check if any vertex of P2_trans is inside P1
                // Or any vertex of P1 is inside P2_trans
                // First check if any vertex of P2_trans is inside P1
                bool p2_inside_p1 = false;
                for(int k = 0; k < n2 && !p2_inside_p1; ++k){
                    // To avoid counting boundary points as inside, use a small epsilon
                    // But since overlapping boundaries are handled separately, it's okay
                    if(is_point_inside_polygon(P2_trans[k], P1)){
                        p2_inside_p1 = true;
                    }
                }
                if(!p2_inside_p1){
                    // Check if any vertex of P1 is inside P2_trans
                    bool p1_inside_p2 = false;
                    for(int k = 0; k < n1 && !p1_inside_p2; ++k){
                        if(is_point_inside_polygon(P1[k], P2_trans)){
                            p1_inside_p2 = true;
                        }
                    }
                    if(p1_inside_p2){
                        overlap = true;
                    }
                }
            }
            if(overlap){
                continue;
            }
            // Compute overlapping boundary length
            double current_overlap = 0.0;
            for(int a = 0; a < n1; ++a){
                int a_next = (a + 1) % n1;
                Point pa1 = P1[a];
                Point pa2 = P1[a_next];
                for(int b = 0; b < n2; ++b){
                    int b_next = (b + 1) % n2;
                    Point pb1 = P2_trans[b];
                    Point pb2 = P2_trans[b_next];
                    if(are_segments_collinear(pa1, pa2, pb1, pb2)){
                        double ol = overlapping_length(pa1, pa2, pb1, pb2);
                        current_overlap += ol;
                    }
                }
            }
            if(current_overlap > max_overlap + 1e-9){
                max_overlap = current_overlap;
            }
        }
    }
    // Additionally, consider the reverse alignment (rotating 180 degrees)
    // Because in some cases, rotating the second polygon might give better overlap
    // But since we align edge directions, this is already handled
    // So no need to handle separately
    // Print the result with sufficient precision
    cout << fixed << setprecision(10) << max_overlap;
}
