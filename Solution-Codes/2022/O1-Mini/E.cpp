#include <bits/stdc++.h>
using namespace std;

struct Point {
    double x, y;
    Point(double x_=0, double y_=0) : x(x_), y(y_) {}
    Point operator+(const Point& other) const {return Point(x + other.x, y + other.y);}
    Point operator-(const Point& other) const {return Point(x - other.x, y - other.y);}
    Point operator*(double t) const {return Point(x * t, y * t);}
    double distance(const Point& other) const {
        double dx = x - other.x, dy = y - other.y;
        return sqrt(dx*dx + dy*dy);
    }
};

// Function to compute the square base given two points and ensure left side
vector<Point> compute_base(double x1, double y1, double x2, double y2) {
    Point A(x1, y1), B(x2, y2);
    double dx = B.x - A.x;
    double dy = B.y - A.y;
    // Left perpendicular vector
    double px = -dy;
    double py = dx;
    // Normalize to the length of AB
    double length = sqrt(dx*dx + dy*dy);
    // Since it's a square, the perpendicular should have the same length as AB
    Point C = Point(B.x + px, B.y + py);
    Point D = Point(A.x + px, A.y + py);
    return {A, B, C, D};
}

// Function to perform ternary search on a line segment to find q that minimizes |p - q| + |B - q|
double ternary_search(const Point& p, const Point& B_point, const Point& start, const Point& end) {
    auto f = [&](double t) -> double {
        double x = start.x + (end.x - start.x) * t;
        double y = start.y + (end.y - start.y) * t;
        Point q(x, y);
        return p.distance(q) + B_point.distance(q);
    };
    double l = 0.0, r = 1.0;
    for(int i=0;i<50;i++) { // sufficient iterations for precision
        double m1 = l + (r - l) / 3.0;
        double m2 = r - (r - l) / 3.0;
        double f1 = f(m1);
        double f2 = f(m2);
        if(f1 < f2) r = m2;
        else l = m1;
    }
    double t = (l + r) / 2.0;
    double x = start.x + (end.x - start.x) * t;
    double y = start.y + (end.y - start.y) * t;
    Point q(x, y);
    return p.distance(q) + B_point.distance(q);
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    // Read first pyramid
    double x1, y1, x2, y2, h1;
    cin >> x1 >> y1 >> x2 >> y2 >> h1;
    vector<Point> base1 = compute_base(x1, y1, x2, y2);
    // Compute center of base1
    double cx1 = (base1[0].x + base1[2].x) / 2.0;
    double cy1 = (base1[0].y + base1[2].y) / 2.0;
    // Apex1
    Point A(cx1, cy1);
    // Read second pyramid
    double x3, y3, x4, y4, h2;
    cin >> x3 >> y3 >> x4 >> y4 >> h2;
    vector<Point> base2 = compute_base(x3, y3, x4, y4);
    // Compute center of base2
    double cx2 = (base2[0].x + base2[2].x) / 2.0;
    double cy2 = (base2[0].y + base2[2].y) / 2.0;
    // Apex2
    Point B(cx2, cy2);
    // Read the second line (assuming two pyramids, but problem statement says two lines)
    // If there are two pyramids, read the second line as second pyramid
    // But in sample input, there are two lines, so adjust
    // But in the problem statement, it says two lines, each for a pyramid
    // Thus, adjust the reading
    // Reset and read properly
    cin.clear();
    cin.seekg(0);
    // Read first pyramid
    cin >> x1 >> y1 >> x2 >> y2 >> h1;
    base1 = compute_base(x1, y1, x2, y2);
    cx1 = (base1[0].x + base1[2].x) / 2.0;
    cy1 = (base1[0].y + base1[2].y) / 2.0;
    Point apex1(cx1, cy1);
    // Read second pyramid
    cin >> x3 >> y3 >> x4 >> y4 >> h2;
    base2 = compute_base(x3, y3, x4, y4);
    cx2 = (base2[0].x + base2[2].x) / 2.0;
    cy2 = (base2[0].y + base2[2].y) / 2.0;
    Point apex2(cx2, cy2);
    // Apex coordinates in 3D
    // But since we're working in 2D plane, handle heights separately
    // The distance from apex to base point is sqrt((x - cx)^2 + (y - cy)^2 + h^2)
    // Thus, |A - p| = sqrt((p.x - cx1)^2 + (p.y - cy1)^2 + h1^2)
    // Similarly, |B - q| = sqrt((q.x - cx2)^2 + (q.y - cy2)^2 + h2^2)
    // Iterate over base1's perimeter
    // Define base1 edges
    vector<pair<Point, Point>> edges1;
    for(int i=0;i<4;i++) edges1.emplace_back(make_pair(base1[i], base1[(i+1)%4]));
    // Define base2 edges
    vector<pair<Point, Point>> edges2;
    for(int i=0;i<4;i++) edges2.emplace_back(make_pair(base2[i], base2[(i+1)%4]));
    double min_total = 1e18;
    // Iterate p on base1's perimeter
    int samples = 100; // samples per edge
    for(auto &[start1, end1] : edges1){
        for(int s=0; s<=samples; s++){
            double t = (double)s / samples;
            double px = start1.x + (end1.x - start1.x) * t;
            double py = start1.y + (end1.y - start1.y) * t;
            Point p(px, py);
            // Compute |A - p|
            double distA_p = sqrt( (p.x - cx1)*(p.x - cx1) + (p.y - cy1)*(p.y - cy1) + h1*h1 );
            // Find minimal |p - q| + |B - q| over q on base2's perimeter
            double min_pq_Bq = 1e18;
            for(auto &[start2, end2] : edges2){
                // Perform ternary search on this edge
                double f = ternary_search(p, apex2, start2, end2);
                if(f < min_pq_Bq) min_pq_Bq = f;
            }
            double total = distA_p + min_pq_Bq;
            if(total < min_total) min_total = total;
        }
    }
    // Output with sufficient precision
    cout << fixed << setprecision(10) << min_total;
}
