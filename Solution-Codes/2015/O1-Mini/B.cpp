#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-9;
const double INF = 1e5;

struct Point {
    double x, y;
    Point() : x(0), y(0) {}
    Point(double x_, double y_) : x(x_), y(y_) {}
    Point operator-(const Point& p) const { return Point(x - p.x, y - p.y); }
    Point operator+(const Point& p) const { return Point(x + p.x, y + p.y); }
    Point operator*(double s) const { return Point(x * s, y * s); }
};

// Cross product of OA and OB vectors
double cross_product(const Point& O, const Point& A, const Point& B) {
    double ox = O.x, oy = O.y;
    double ax = A.x - ox, ay = A.y - oy;
    double bx = B.x - ox, by = B.y - oy;
    return ax * by - ay * bx;
}

// Compute cross product of vectors a and b
double cross(const Point& a, const Point& b) {
    return a.x * b.y - a.y * b.x;
}

// Check if two doubles are equal within epsilon
bool equal_double(double a, double b) {
    return fabs(a - b) < EPS;
}

// Get intersection point of lines AB and CD
bool get_line_intersection(const Point& A, const Point& B, const Point& C, const Point& D, Point& intersect) {
    Point E = B - A;
    Point F = D - C;
    double denom = cross(E, F);
    if (fabs(denom) < EPS) {
        return false; // Parallel or coincident
    }
    Point CA = A - C;
    double s = cross(CA, F) / denom;
    intersect = A + E * s;
    return true;
}

// Sutherland-Hodgman polygon clipping
vector<Point> polygon_clip(const vector<Point>& subject, const vector<Point>& clip) {
    vector<Point> output = subject;
    int clip_size = clip.size();
    for(int i=0; i < clip_size; ++i){
        vector<Point> input = output;
        output.clear();
        if(input.empty()) break;
        Point C = clip[i];
        Point D = clip[(i+1)%clip_size];
        int n = input.size();
        for(int j=0; j < n; ++j){
            Point S = input[j];
            Point E = input[(j+1)%n];
            // Inside test: (D - C) x (P - C) <= 0
            auto is_inside = [&](const Point& P) -> bool {
                double cp = cross(D - C, P - C);
                return cp <= EPS;
            };
            bool inside_S = is_inside(S);
            bool inside_E = is_inside(E);
            Point intersect;
            if(inside_E){
                if(!inside_S){
                    if(get_line_intersection(S, E, C, D, intersect)){
                        output.push_back(intersect);
                    }
                }
                output.push_back(E);
            }
            else{
                if(inside_S){
                    if(get_line_intersection(S, E, C, D, intersect)){
                        output.push_back(intersect);
                    }
                }
            }
        }
    }
    // Remove duplicate consecutive points
    vector<Point> final_output;
    for(auto &p : output){
        if(final_output.empty() || !(equal_double(p.x, final_output.back().x) && equal_double(p.y, final_output.back().y))){
            final_output.push_back(p);
        }
    }
    // Remove last point if it's the same as the first
    if(final_output.size() >1 && equal_double(final_output[0].x, final_output.back().x) && equal_double(final_output[0].y, final_output.back().y)){
        final_output.pop_back();
    }
    return final_output;
}

// Compute area of polygon
double polygon_area(const vector<Point>& poly){
    if(poly.size() <3) return 0.0;
    double area =0.0;
    int n = poly.size();
    for(int i=0;i<n;i++){
        area += (poly[i].x * poly[(i+1)%n].y - poly[(i+1)%n].x * poly[i].y);
    }
    return fabs(area)/2.0;
}

// Check if two polygons touch or overlap
bool polygons_touch_or_overlap(const vector<Point>& poly1, const vector<Point>& poly2){
    vector<Point> intersection = polygon_clip(poly1, poly2);
    return !intersection.empty();
}

// Move polygon by velocity (vx, vy) at time t
vector<Point> get_polygon_at_time(const vector<Point>& poly, double t, double vx, double vy){
    vector<Point> moved;
    moved.reserve(poly.size());
    for(auto &p : poly){
        moved.emplace_back(Point(p.x + vx * t, p.y + vy * t));
    }
    return moved;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    // Read first polygon
    int n1;
    cin >> n1;
    vector<Point> A_poly(n1);
    for(int i=0;i<n1;i++) cin >> A_poly[i].x >> A_poly[i].y;
    double vxA, vyA;
    cin >> vxA >> vyA;
    // Read second polygon
    int n2;
    cin >> n2;
    vector<Point> B_poly(n2);
    for(int i=0;i<n2;i++) cin >> B_poly[i].x >> B_poly[i].y;
    double vxB, vyB;
    cin >> vxB >> vyB;
    // Define function to compute area at time t
    auto compute_area = [&](double t) -> double {
        vector<Point> moved_A = get_polygon_at_time(A_poly, t, vxA, vyA);
        vector<Point> moved_B = get_polygon_at_time(B_poly, t, vxB, vyB);
        vector<Point> intersection = polygon_clip(moved_A, moved_B);
        if(intersection.empty()) return 0.0;
        return polygon_area(intersection);
    };
    // Ternary search to find maximum area
    double left = 0.0, right = INF;
    for(int i=0;i<100;i++){
        double t1 = left + (right - left)/3.0;
        double t2 = right - (right - left)/3.0;
        double area1 = compute_area(t1);
        double area2 = compute_area(t2);
        if(area1 < area2){
            left = t1;
        }
        else{
            right = t2;
        }
    }
    double t_best = (left + right)/2.0;
    double max_area = compute_area(t_best);
    if(max_area > EPS){
        // Now find the earliest t where area is approximately max_area
        // Perform a binary search to find earliest t where area(t) >= max_area - EPS
        double low = 0.0, high = t_best;
        for(int i=0;i<100;i++){
            double mid = (low + high)/2.0;
            double area_mid = compute_area(mid);
            if(area_mid >= max_area - 1e-6){
                high = mid;
            }
            else{
                low = mid;
            }
        }
        // The earliest t is high
        printf("%.6lf\n", high);
    }
    else{
        // Find earliest t where polygons touch or overlap
        // Binary search from 0 to INF
        double low = 0.0, high = INF;
        bool found = false;
        for(int i=0;i<100;i++){
            double mid = (low + high)/2.0;
            vector<Point> moved_A = get_polygon_at_time(A_poly, mid, vxA, vyA);
            vector<Point> moved_B = get_polygon_at_time(B_poly, mid, vxB, vyB);
            if(polygons_touch_or_overlap(moved_A, moved_B)){
                high = mid;
                found = true;
            }
            else{
                low = mid;
            }
        }
        // Check if they touch at high
        vector<Point> final_A = get_polygon_at_time(A_poly, high, vxA, vyA);
        vector<Point> final_B = get_polygon_at_time(B_poly, high, vxB, vyB);
        if(polygons_touch_or_overlap(final_A, final_B) && high <= INF - 1.0){
            printf("%.6lf\n", high);
        }
        else{
            printf("never\n");
        }
    }
}
