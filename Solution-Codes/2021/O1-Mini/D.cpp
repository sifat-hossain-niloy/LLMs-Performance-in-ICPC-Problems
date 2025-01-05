#include <bits/stdc++.h>
using namespace std;

const double PI = acos(-1.0);
const double EPS = 1e-9;

// Point structure
struct Point {
    double x, y;
    Point() : x(0), y(0){}
    Point(double _x, double _y) : x(_x), y(_y){}
    Point operator-(const Point& other) const{
        return Point(x - other.x, y - other.y);
    }
    Point operator+(const Point& other) const{
        return Point(x + other.x, y + other.y);
    }
    Point operator*(double t) const{
        return Point(x * t, y * t);
    }
};

// Cross product
double cross(const Point& O, const Point& A, const Point& B){
    double ox = A.x - O.x, oy = A.y - O.y;
    double ax = B.x - O.x, ay = B.y - O.y;
    return ox * ay - oy * ax;
}

// Check if two segments (p1,q1) and (p2,q2) intersect
bool segments_intersect(const Point& p1, const Point& q1, const Point& p2, const Point& q2){
    auto orientation = [&](const Point& a, const Point& b, const Point& c) -> int{
        double val = (b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x);
        if (abs(val) < EPS) return 0; // colinear
        return (val > 0) ? 1 : 2; // clock or counter
    };
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);
    if (o1 != o2 && o3 != o4)
        return true;
    // Special Cases
    auto on_segment = [&](const Point& a, const Point& b, const Point& c) -> bool{
        return min(a.x, b.x) - EPS <= c.x && c.x <= max(a.x, b.x) + EPS &&
               min(a.y, b.y) - EPS <= c.y && c.y <= max(a.y, b.y) + EPS;
    };
    // Colinear cases
    if (o1 == 0 && on_segment(p1, q1, p2)) return true;
    if (o2 == 0 && on_segment(p1, q1, q2)) return true;
    if (o3 == 0 && on_segment(p2, q2, p1)) return true;
    if (o4 == 0 && on_segment(p2, q2, q1)) return true;
    return false;
}

// Point inside polygon using ray casting
bool point_inside_polygon(const Point& p, const vector<Point>& poly){
    bool inside = false;
    int n = poly.size();
    for(int i=0;i<n;i++){
        int j = (i+1)%n;
        if( ((poly[i].y > p.y) != (poly[j].y > p.y)) ){
            double intersect_x = (poly[j].x - poly[i].x) * (p.y - poly[i].y) / (poly[j].y - poly[i].y) + poly[i].x;
            if(p.x < intersect_x)
                inside = !inside;
        }
    }
    return inside;
}

// Check if the line segment P-Q is visible inside the polygon
bool is_visible(const Point& P, const Point& Q, const vector<Point>& poly){
    int n = poly.size();
    for(int i=0;i<n;i++){
        int j = (i+1)%n;
        // Check if segments P-Q and poly[i]-poly[j] intersect
        // Skip if they share a point
        // Since P and Q are strictly inside, they don't lie on polygon edges
        // So no need to skip any edge
        // However, floating point issues might require careful handling
        // So we check intersection excluding endpoints
        // To avoid counting touching at Q as intersection, we ensure that intersection is not at Q
        // Similarly for P
        // Implement robust intersection
        // Check if segments intersect at a point other than Q or P
        // First, quick rejection based on bounding boxes
        auto a = P;
        auto b = Q;
        auto c = poly[i];
        auto d = poly[j];
        // Check if segments (a,b) and (c,d) intersect
        // Exclude intersections at endpoints
        // Implement the segments_intersect function accordingly
        // To exclude endpoints, ensure that intersection point is not equal to any endpoint
        // Modify segments_intersect to handle this
        // For simplicity, allow intersections at endpoints, but in our problem P and Q are inside
        if(segments_intersect(a, b, c, d)){
            // Now check if the intersection is at P or Q
            // Compute if P == c or P == d or Q == c or Q == d
            bool shared = false;
            if( (abs(a.x - c.x) < EPS && abs(a.y - c.y) < EPS) ||
                (abs(a.x - d.x) < EPS && abs(a.y - d.y) < EPS) ||
                (abs(b.x - c.x) < EPS && abs(b.y - c.y) < EPS) ||
                (abs(b.x - d.x) < EPS && abs(b.y - d.y) < EPS) ){
                shared = true;
            }
            if(!shared){
                return false;
            }
        }
    }
    return true;
}

// Check if from P, at least half of the sculpture is visible
bool check_condition(const Point& P, const Point& S, const vector<Point>& poly){
    // Define a small radius for the sculpture
    double r = 1.0;
    // Number of samples
    int k = 2000;
    int visible = 0;
    for(int i=0;i<k;i++){
        double angle = 2.0 * PI * i / k;
        double x = S.x + r * cos(angle);
        double y = S.y + r * sin(angle);
        Point Q(x, y);
        // Check if Q is inside the polygon
        if(!point_inside_polygon(Q, poly)) continue;
        // Check if P can see Q
        if(is_visible(P, Q, poly)){
            visible++;
        }
    }
    return ((double)visible / k) >= 0.5;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    cin >> n;
    vector<Point> poly(n);
    for(int i=0;i<n;i++) cin >> poly[i].x >> poly[i].y;
    double xg, yg;
    cin >> xg >> yg;
    Point G(xg, yg);
    double xs, ys;
    cin >> xs >> ys;
    Point S(xs, ys);
    // Compute direction from G to S
    double dx = S.x - G.x;
    double dy = S.y - G.y;
    double dist_gs = sqrt(dx*dx + dy*dy);
    double dirx = dx / dist_gs;
    double diry = dy / dist_gs;
    // Binary search on t in [0, dist_gs]
    double left = 0.0;
    double right = dist_gs;
    double answer = dist_gs;
    for(int it=0; it<100; it++){
        double mid = (left + right) / 2.0;
        // Compute P = G + mid * direction
        Point P(G.x + dirx * mid, G.y + diry * mid);
        // Check if P is inside the polygon
        if(!point_inside_polygon(P, poly)){
            // If P is not inside, need to move further
            left = mid;
            continue;
        }
        // Check the visibility condition
        if(check_condition(P, S, poly)){
            answer = mid;
            right = mid;
        }
        else{
            left = mid;
        }
    }
    // Output with high precision
    cout << fixed << setprecision(12) << answer;
}
