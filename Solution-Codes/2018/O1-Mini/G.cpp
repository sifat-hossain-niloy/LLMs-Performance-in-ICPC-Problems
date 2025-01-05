#include <bits/stdc++.h>
using namespace std;

struct Point {
    double x, y;
};

// Function to compute the centroid of a polygon
Point compute_centroid(const vector<Point>& polygon) {
    double A = 0.0;
    double Cx = 0.0, Cy = 0.0;
    int n = polygon.size();
    for(int i=0;i<n;i++) {
        int j = (i+1)%n;
        double cross = polygon[i].x * polygon[j].y - polygon[j].x * polygon[i].y;
        A += cross;
        Cx += (polygon[i].x + polygon[j].x) * cross;
        Cy += (polygon[i].y + polygon[j].y) * cross;
    }
    A *= 0.5;
    if (fabs(A) < 1e-9) return {polygon[0].x, polygon[0].y};
    Cx /= (6.0 * A);
    Cy /= (6.0 * A);
    return {Cx, Cy};
}

// Function to check if a point is inside a polygon using ray casting
bool point_in_polygon(const Point& p, const vector<Point>& polygon) {
    bool inside = false;
    int n = polygon.size();
    for(int i=0;i<n;i++) {
        int j = (i+1)%n;
        if (((polygon[i].y > p.y) != (polygon[j].y > p.y))) {
            double intersect_x = (polygon[j].x - polygon[i].x) * (p.y - polygon[i].y) / (polygon[j].y - polygon[i].y) + polygon[i].x;
            if (p.x < intersect_x)
                inside = !inside;
        }
    }
    return inside;
}

// Function to compute the minimum distance from a point to all vertices
double min_distance(const Point& p, const vector<Point>& vertices) {
    double min_dist = 1e20;
    for(auto &v : vertices){
        double dx = p.x - v.x;
        double dy = p.y - v.y;
        double dist = sqrt(dx*dx + dy*dy);
        if(dist < min_dist){
            min_dist = dist;
            if(min_dist == 0.0) break;
        }
    }
    return min_dist;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    cin >> n;
    vector<Point> polygon(n);
    for(int i=0;i<n;i++) cin >> polygon[i].x >> polygon[i].y;
    
    // Precompute all vertices
    vector<Point> vertices = polygon;
    
    // Compute centroid
    Point centroid = compute_centroid(polygon);
    
    // Directions for hill climbing (8 directions)
    vector<pair<double, double>> directions;
    for(int d=0; d<8; d++) {
        double angle = d * M_PI / 4.0;
        directions.emplace_back(cos(angle), sin(angle));
    }
    
    // Function to perform hill climbing from a starting point
    auto hill_climb = [&](Point start) -> double {
        Point current = start;
        double current_dist = min_distance(current, vertices);
        double step = 1e5;
        double eps = 1e-7;
        while(step > eps){
            bool improved = false;
            double best_dist = current_dist;
            Point best_point = current;
            for(auto &[dx, dy] : directions){
                Point next;
                next.x = current.x + dx * step;
                next.y = current.y + dy * step;
                if(point_in_polygon(next, polygon)){
                    double dist = min_distance(next, vertices);
                    if(dist > best_dist + 1e-12){
                        best_dist = dist;
                        best_point = next;
                        improved = true;
                    }
                }
            }
            if(improved){
                current = best_point;
                current_dist = best_dist;
            }
            else{
                step *= 0.5;
            }
        }
        return current_dist;
    };
    
    // Initialize maximum min distance
    double max_min_dist = 0.0;
    
    // Start hill climbing from centroid
    if(point_in_polygon(centroid, polygon)){
        max_min_dist = max(max_min_dist, hill_climb(centroid));
    }
    
    // Start hill climbing from multiple points: midpoints of edges
    for(int i=0;i<n;i++){
        int j = (i+1)%n;
        Point mid;
        mid.x = (polygon[i].x + polygon[j].x) / 2.0;
        mid.y = (polygon[i].y + polygon[j].y) / 2.0;
        if(point_in_polygon(mid, polygon)){
            max_min_dist = max(max_min_dist, hill_climb(mid));
        }
    }
    
    // Additionally, sample some random points inside the polygon
    // to avoid missing global maxima
    // Use deterministic sampling for reproducibility
    int sample_count = 100;
    srand(42);
    for(int s=0;s<sample_count;s++){
        // Generate random points within bounding box
        // Using rejection sampling
        double min_x = 1e20, max_x = -1e20, min_y = 1e20, max_y = -1e20;
        for(auto &p : polygon){
            min_x = min(min_x, p.x);
            max_x = max(max_x, p.x);
            min_y = min(min_y, p.y);
            max_y = max(max_y, p.y);
        }
        for(int tries=0; tries<1000; tries++){
            double rx = ((double)rand() / RAND_MAX) * (max_x - min_x) + min_x;
            double ry = ((double)rand() / RAND_MAX) * (max_y - min_y) + min_y;
            Point random_p = {rx, ry};
            if(point_in_polygon(random_p, polygon)){
                max_min_dist = max(max_min_dist, hill_climb(random_p));
                break;
            }
        }
    }
    
    // Output the result with sufficient precision
    cout << fixed << setprecision(9) << max_min_dist;
}
