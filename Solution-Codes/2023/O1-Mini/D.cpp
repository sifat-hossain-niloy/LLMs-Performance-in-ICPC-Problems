#include <bits/stdc++.h>
using namespace std;

struct Point2D {
    double x, y;
};

struct Point3D {
    double x, y, z;
};

// Function to compute base square and apex
void compute_pyramid(int x1, int y1, int x2, int y2, double h, vector<Point2D> &base, Point3D &apex)
{
    double dx = x2 - x1;
    double dy = y2 - y1;
    // Left perpendicular is (-dy, dx)
    double perp_x = -dy;
    double perp_y = dx;
    // C = B + left_perp
    double x3 = x2 + perp_x;
    double y3 = y2 + perp_y;
    // D = A + left_perp
    double x4 = x1 + perp_x;
    double y4 = y1 + perp_y;
    base.clear();
    base.push_back(Point2D{(double)x1, (double)y1});
    base.push_back(Point2D{(double)x2, (double)y2});
    base.push_back(Point2D{x3, y3});
    base.push_back(Point2D{x4, y4});
    // Compute center
    double center_x = (x1 + x2 + x3 + x4) / 4.0;
    double center_y = (y1 + y2 + y3 + y4) / 4.0;
    apex = Point3D{center_x, center_y, h};
}

// Function to compute distance between two 3D points
double distance3D(const Point3D &a, const Point3D &b)
{
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    double dz = a.z - b.z;
    return sqrt(dx*dx + dy*dy + dz*dz);
}

// Function to compute distance between two 2D points
double distance2D(const Point2D &a, const Point2D &b)
{
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx*dx + dy*dy);
}

// Function to compute closest point on segment to a point
Point2D closest_point_on_segment(const Point2D &p, const Point2D &a, const Point2D &b)
{
    double dx = b.x - a.x;
    double dy = b.y - a.y;
    if(dx == 0 && dy == 0)
        return a;
    double t = ((p.x - a.x)*dx + (p.y - a.y)*dy) / (dx*dx + dy*dy);
    t = max(0.0, min(1.0, t));
    return Point2D{ a.x + t * dx, a.y + t * dy };
}

// Function to compute minimal distance between two squares
double minimal_distance_between_squares(const vector<Point2D> &base1, const vector<Point2D> &base2, Point2D &p_closest, Point2D &q_closest)
{
    double min_dist = 1e20;
    // Enumerate edges of base1
    vector<pair<Point2D, Point2D>> edges1;
    for(int i=0;i<4;i++){
        edges1.emplace_back(make_pair(base1[i], base1[(i+1)%4]));
    }
    // Enumerate edges of base2
    vector<pair<Point2D, Point2D>> edges2;
    for(int i=0;i<4;i++){
        edges2.emplace_back(make_pair(base2[i], base2[(i+1)%4]));
    }
    // For each vertex in base1, find closest point on base2
    for(int i=0;i<4;i++){
        Point2D p = base1[i];
        for(int j=0;j<4;j++){
            Point2D a = base2[j];
            Point2D b = base2[(j+1)%4];
            Point2D q = closest_point_on_segment(p, a, b);
            double dist = distance2D(p, q);
            if(dist < min_dist - 1e-12){
                min_dist = dist;
                p_closest = p;
                q_closest = q;
            }
        }
    }
    // For each vertex in base2, find closest point on base1
    for(int i=0;i<4;i++){
        Point2D q = base2[i];
        for(int j=0;j<4;j++){
            Point2D a = base1[j];
            Point2D b = base1[(j+1)%4];
            Point2D p = closest_point_on_segment(q, a, b);
            double dist = distance2D(p, q);
            if(dist < min_dist - 1e-12){
                min_dist = dist;
                p_closest = p;
                q_closest = q;
            }
        }
    }
    return min_dist;
}

// Function to perform ternary search on edge2 to find q that minimizes |p - q| + |apex2 - q|
double ternary_search_q_on_edge(const pair<Point2D, Point2D> &edge2, const Point2D &p, const Point3D &apex2, double &q_x, double &q_y)
{
    double left = 0.0, right = 1.0;
    double best_f = 1e20;
    double best_x = 0.0, best_y = 0.0;
    for(int it=0; it<100; it++){
        double t1 = left + (right - left) / 3.0;
        double t2 = right - (right - left) / 3.0;
        Point2D q1 = Point2D{edge2.first.x + t1 * (edge2.second.x - edge2.first.x),
                            edge2.first.y + t1 * (edge2.second.y - edge2.first.y)};
        Point2D q2 = Point2D{edge2.first.x + t2 * (edge2.second.x - edge2.first.x),
                            edge2.first.y + t2 * (edge2.second.y - edge2.first.y)};
        double d_p_q1 = distance2D(p, q1);
        double d_apex2_q1 = sqrt( (apex2.x - q1.x)*(apex2.x - q1.x) + (apex2.y - q1.y)*(apex2.y - q1.y) + (apex2.z)*(apex2.z) );
        double f1 = d_p_q1 + d_apex2_q1;
        double d_p_q2 = distance2D(p, q2);
        double d_apex2_q2 = sqrt( (apex2.x - q2.x)*(apex2.x - q2.x) + (apex2.y - q2.y)*(apex2.y - q2.y) + (apex2.z)*(apex2.z) );
        double f2 = d_p_q2 + d_apex2_q2;
        if(f1 < f2){
            right = t2;
            if(f1 < best_f){
                best_f = f1;
                best_x = q1.x;
                best_y = q1.y;
            }
        }
        else{
            left = t1;
            if(f2 < best_f){
                best_f = f2;
                best_x = q2.x;
                best_y = q2.y;
            }
        }
    }
    q_x = best_x;
    q_y = best_y;
    return best_f;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    // Read first pyramid
    int x1, y1, x2, y2;
    double h1;
    cin >> x1 >> y1 >> x2 >> y2 >> h1;
    vector<Point2D> base1;
    Point3D apex1;
    compute_pyramid(x1, y1, x2, y2, h1, base1, apex1);
    // Read second pyramid
    int x3, y3, x4, y4;
    double h2;
    cin >> x3 >> y3 >> x4 >> y4 >> h2;
    vector<Point2D> base2;
    Point3D apex2;
    compute_pyramid(x3, y3, x4, y4, h2, base2, apex2);
    // Initialize minimal distance
    double min_distance = 1e20;
    // Enumerate all vertex pairs
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            Point2D p = base1[i];
            Point2D q = base2[j];
            double d1 = sqrt( (apex1.x - p.x)*(apex1.x - p.x) + (apex1.y - p.y)*(apex1.y - p.y) + (apex1.z)*(apex1.z) );
            double d2 = distance2D(p, q);
            double d3 = sqrt( (apex2.x - q.x)*(apex2.x - q.x) + (apex2.y - q.y)*(apex2.y - q.y) + (apex2.z)*(apex2.z) );
            double total = d1 + d2 + d3;
            if(total < min_distance - 1e-12){
                min_distance = total;
            }
        }
    }
    // Enumerate all edge pairs and perform ternary search
    // Enumerate edges of base1
    vector<pair<Point2D, Point2D>> edges1;
    for(int i=0;i<4;i++) edges1.emplace_back(make_pair(base1[i], base1[(i+1)%4]));
    // Enumerate edges of base2
    vector<pair<Point2D, Point2D>> edges2;
    for(int i=0;i<4;i++) edges2.emplace_back(make_pair(base2[i], base2[(i+1)%4]));
    // For each edge pair
    for(auto &edge1 : edges1){
        for(auto &edge2 : edges2){
            // Sample points on edge1
            int samples = 100;
            for(int s=0; s<=samples; s++){
                double t_p = (double)s / samples;
                Point2D p = Point2D{ edge1.first.x + t_p * (edge1.second.x - edge1.first.x),
                                     edge1.first.y + t_p * (edge1.second.y - edge1.first.y) };
                // Find q on edge2 that minimizes |p - q| + |apex2 - q|
                double q_x, q_y;
                double f = ternary_search_q_on_edge(edge2, p, apex2, q_x, q_y);
                // Compute |apex1 - p|
                double d1 = sqrt( (apex1.x - p.x)*(apex1.x - p.x) + (apex1.y - p.y)*(apex1.y - p.y) + (apex1.z)*(apex1.z) );
                // Compute |p - q|
                double d2 = sqrt( (p.x - q_x)*(p.x - q_x) + (p.y - q_y)*(p.y - q_y) );
                // Compute |apex2 - q|
                double d3 = sqrt( (apex2.x - q_x)*(apex2.x - q_x) + (apex2.y - q_y)*(apex2.y - q_y) + (apex2.z)*(apex2.z) );
                double total = d1 + d2 + d3;
                if(total < min_distance - 1e-12){
                    min_distance = total;
                }
            }
        }
    }
    // Similarly, perform the reverse: iterate q on edge2 and find p on edge1
    for(auto &edge2 : edges2){
        for(auto &edge1 : edges1){
            // Sample points on edge2
            int samples = 100;
            for(int s=0; s<=samples; s++){
                double t_q = (double)s / samples;
                Point2D q = Point2D{ edge2.first.x + t_q * (edge2.second.x - edge2.first.x),
                                     edge2.first.y + t_q * (edge2.second.y - edge2.first.y) };
                // Find p on edge1 that minimizes |apex1 - p| + |p - q|
                // This is similar to finding p that minimizes |apex1 - p| + |p - q|
                // Which is the projection of apex1 onto the line to q, constrained on edge1
                // Implement ternary search
                auto ternary_search_p_on_edge = [&](const pair<Point2D, Point2D> &edge1_inner, const Point2D &q_point, double &p_x, double &p_y) -> double {
                    double left = 0.0, right = 1.0;
                    double best_f = 1e20;
                    double best_px = 0.0, best_py = 0.0;
                    for(int it=0; it<100; it++){
                        double t1 = left + (right - left) / 3.0;
                        double t2 = right - (right - left) / 3.0;
                        Point2D p1 = Point2D{edge1_inner.first.x + t1 * (edge1_inner.second.x - edge1_inner.first.x),
                                            edge1_inner.first.y + t1 * (edge1_inner.second.y - edge1_inner.first.y)};
                        Point2D p2 = Point2D{edge1_inner.first.x + t2 * (edge1_inner.second.x - edge1_inner.first.x),
                                            edge1_inner.first.y + t2 * (edge1_inner.second.y - edge1_inner.first.y)};
                        double f1 = sqrt( (apex1.x - p1.x)*(apex1.x - p1.x) + (apex1.y - p1.y)*(apex1.y - p1.y) + (apex1.z)*(apex1.z) ) + 
                                     sqrt( (p1.x - q_point.x)*(p1.x - q_point.x) + (p1.y - q_point.y)*(p1.y - q_point.y) );
                        double f2 = sqrt( (apex1.x - p2.x)*(apex1.x - p2.x) + (apex1.y - p2.y)*(apex1.y - p2.y) + (apex1.z)*(apex1.z) ) + 
                                     sqrt( (p2.x - q_point.x)*(p2.x - q_point.x) + (p2.y - q_point.y)*(p2.y - q_point.y) );
                        if(f1 < f2){
                            right = t2;
                            if(f1 < best_f){
                                best_f = f1;
                                best_px = p1.x;
                                best_py = p1.y;
                            }
                        }
                        else{
                            left = t1;
                            if(f2 < best_f){
                                best_f = f2;
                                best_px = p2.x;
                                best_py = p2.y;
                            }
                        }
                    }
                    p_x = best_px;
                    p_y = best_py;
                    return best_f;
                };
                double p_x, p_y;
                double f = ternary_search_p_on_edge(edge1, q, p_x, p_y);
                // Compute |apex1 - p|
                double d1 = sqrt( (apex1.x - p_x)*(apex1.x - p_x) + (apex1.y - p_y)*(apex1.y - p_y) + (apex1.z)*(apex1.z) );
                // Compute |p - q|
                double d2 = sqrt( (p_x - q.x)*(p_x - q.x) + (p_y - q.y)*(p_y - q.y) );
                // Compute |apex2 - q|
                double d3 = sqrt( (apex2.x - q.x)*(apex2.x - q.x) + (apex2.y - q.y)*(apex2.y - q.y) + (apex2.z)*(apex2.z) );
                double total = d1 + d2 + d3;
                if(total < min_distance - 1e-12){
                    min_distance = total;
                }
            }
        }
    }
    // Additionally, consider the closest pair
    Point2D p_c, q_c;
    minimal_distance_between_squares(base1, base2, p_c, q_c);
    // Compute |apex1 - p_c| + |p_c - q_c| + |apex2 - q_c|
    double d1 = sqrt( (apex1.x - p_c.x)*(apex1.x - p_c.x) + (apex1.y - p_c.y)*(apex1.y - p_c.y) + (apex1.z)*(apex1.z) );
    double d2 = distance2D(p_c, q_c);
    double d3 = sqrt( (apex2.x - q_c.x)*(apex2.x - q_c.x) + (apex2.y - q_c.y)*(apex2.y - q_c.y) + (apex2.z)*(apex2.z) );
    double total = d1 + d2 + d3;
    if(total < min_distance - 1e-12){
        min_distance = total;
    }
    // Output the minimal distance with 9 decimal places
    cout << fixed << setprecision(9) << min_distance;
}
