#include <bits/stdc++.h>
using namespace std;

struct Point {
    double x, y;
};

// Function to determine the orientation of ordered triplet (p, q, r)
// Returns 0 if colinear, 1 if clockwise, 2 if counterclockwise
int orientation(const Point& p, const Point& q, const Point& r) {
    double val = (q.x - p.x)*(r.y - p.y) - (q.y - p.y)*(r.x - p.x);
    if (abs(val) < 1e-9) return 0; // colinear
    return (val > 0) ? 1 : 2; // clock or counter
}

// Function to check if point q lies on segment pr
bool on_segment(const Point& p, const Point& q, const Point& r) {
    if (q.x <= max(p.x, r.x)+1e-9 && q.x >= min(p.x, r.x)-1e-9 &&
        q.y <= max(p.y, r.y)+1e-9 && q.y >= min(p.y, r.y)-1e-9)
        return true;
    return false;
}

// Function to check if segments p1q1 and p2q2 intersect
bool segments_intersect(const Point& p1, const Point& q1, const Point& p2, const Point& q2) {
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);
    
    // General case
    if (o1 != o2 && o3 != o4)
        return true;
    
    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && on_segment(p1, p2, q1)) return true;
    // p1, q1 and q2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && on_segment(p1, q2, q1)) return true;
    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && on_segment(p2, p1, q2)) return true;
    // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && on_segment(p2, q1, q2)) return true;
    
    return false;
}

// Function to check if a point is inside a polygon (including boundary)
bool point_inside_polygon(double x, double y, const vector<Point>& poly) {
    int n = poly.size();
    bool inside = false;
    for(int i = 0; i < n; ++i){
        int j = (i + n -1) % n;
        if(((poly[i].y > y) != (poly[j].y > y))){
            double intersect_x = (poly[j].x - poly[i].x) * (y - poly[i].y) / (poly[j].y - poly[i].y) + poly[i].x;
            if(x <= intersect_x + 1e-9)
                inside = !inside;
        }
    }
    return inside;
}

// Function to check if tile intersects polygon
bool tile_intersects_polygon(double xmin, double xmax, double ymin, double ymax, const vector<Point>& poly) {
    // 1. Check if any polygon vertex is inside rectangle
    for(auto &p : poly){
        if(p.x >= xmin - 1e-9 && p.x <= xmax + 1e-9 &&
           p.y >= ymin - 1e-9 && p.y <= ymax + 1e-9){
            return true;
        }
    }
    // 2. Check if any rectangle corner is inside polygon
    vector<Point> corners = {Point{xmin, ymin}, Point{xmin, ymax}, Point{xmax, ymin}, Point{xmax, ymax}};
    for(auto &c : corners){
        // Check if point is inside polygon
        // First check boundary
        bool on_edge = false;
        int n = poly.size();
        for(int i = 0; i < n; ++i){
            int j = (i +1) % n;
            // Check if c is on segment poly[i]-poly[j]
            double dx1 = poly[j].x - poly[i].x;
            double dy1 = poly[j].y - poly[i].y;
            double dx2 = c.x - poly[i].x;
            double dy2 = c.y - poly[i].y;
            double cross = dx1 * dy2 - dy1 * dx2;
            if(abs(cross) < 1e-9){
                // Check if within segment
                if(c.x >= min(poly[i].x, poly[j].x) -1e-9 && c.x <= max(poly[i].x, poly[j].x)+1e-9 &&
                   c.y >= min(poly[i].y, poly[j].y) -1e-9 && c.y <= max(poly[i].y, poly[j].y)+1e-9){
                    on_edge = true;
                    break;
                }
            }
        }
        if(on_edge || point_inside_polygon(c.x, c.y, poly)){
            return true;
        }
    }
    // 3. Check if any polygon edge intersects any rectangle edge
    // Define rectangle edges
    vector<pair<Point, Point>> rect_edges = {
        {Point{xmin, ymin}, Point{xmax, ymin}},
        {Point{xmax, ymin}, Point{xmax, ymax}},
        {Point{xmax, ymax}, Point{xmin, ymax}},
        {Point{xmin, ymax}, Point{xmin, ymin}}
    };
    int n = poly.size();
    for(int i = 0; i < n; ++i){
        int j = (i +1) % n;
        Point p1 = poly[i];
        Point p2 = poly[j];
        for(auto &e : rect_edges){
            Point q1 = e.first;
            Point q2 = e.second;
            if(segments_intersect(p1, p2, q1, q2)){
                return true;
            }
        }
    }
    // No intersection
    return false;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, xs, ys;
    cin >> n >> xs >> ys;
    vector<Point> poly(n);
    for(int i =0; i < n; ++i){
        cin >> poly[i].x >> poly[i].y;
    }
    // Collect unique dx and dy candidates
    vector<int> dx_candidates;
    vector<int> dy_candidates;
    for(int i =0; i < n; ++i){
        int dx = poly[i].x % xs;
        dx_candidates.push_back(dx);
        int dy = poly[i].y % ys;
        dy_candidates.push_back(dy);
    }
    // Remove duplicates
    sort(dx_candidates.begin(), dx_candidates.end());
    dx_candidates.erase(unique(dx_candidates.begin(), dx_candidates.end()), dx_candidates.end());
    sort(dy_candidates.begin(), dy_candidates.end());
    dy_candidates.erase(unique(dy_candidates.begin(), dy_candidates.end()), dy_candidates.end());
    // Find polygon bounding box
    double min_x = poly[0].x, max_x = poly[0].x;
    double min_y = poly[0].y, max_y = poly[0].y;
    for(auto &p : poly){
        min_x = min(min_x, (double)p.x);
        max_x = max(max_x, (double)p.x);
        min_y = min(min_y, (double)p.y);
        max_y = max(max_y, (double)p.y);
    }
    int min_count = INT32_MAX;
    for(auto dx : dx_candidates){
        for(auto dy : dy_candidates){
            // Determine tile indices
            double shifted_min_x = min_x - dx;
            double shifted_max_x = max_x - dx;
            double shifted_min_y = min_y - dy;
            double shifted_max_y = max_y - dy;
            int i_min = floor(shifted_min_x / xs);
            int i_max = floor(shifted_max_x / xs);
            int j_min = floor(shifted_min_y / ys);
            int j_max = floor(shifted_max_y / ys);
            // Iterate through tiles
            int count = 0;
            for(int i = i_min; i <= i_max; ++i){
                for(int j = j_min; j <= j_max; ++j){
                    double tile_xmin = dx + i * (double)xs;
                    double tile_xmax = dx + (i +1) * (double)xs;
                    double tile_ymin = dy + j * (double)ys;
                    double tile_ymax = dy + (j +1) * (double)ys;
                    if(tile_intersects_polygon(tile_xmin, tile_xmax, tile_ymin, tile_ymax, poly)){
                        count++;
                    }
                }
            }
            min_count = min(min_count, count);
        }
    }
    cout << min_count;
}
