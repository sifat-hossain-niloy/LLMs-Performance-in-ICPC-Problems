#include <bits/stdc++.h>
using namespace std;

struct Point {
    double x, y;
    Point() : x(0), y(0){}
    Point(double _x, double _y) : x(_x), y(_y){}
};

// Function to check if a point is inside the circle
bool is_inside(const Point &P, double r_sq) {
    return (P.x * P.x + P.y * P.y) <= r_sq + 1e-8;
}

// Function to compute intersection points between segment P0-P1 and circle with radius r
vector<Point> get_circle_intersections(const Point &P0, const Point &P1, double r) {
    vector<Point> intersections;
    double dx = P1.x - P0.x;
    double dy = P1.y - P0.y;
    double A = dx * dx + dy * dy;
    double B = 2.0 * (P0.x * dx + P0.y * dy);
    double C = P0.x * P0.x + P0.y * P0.y - r * r;
    double discriminant = B * B - 4 * A * C;
    if (discriminant < -1e-8) return intersections;
    if (abs(discriminant) < 1e-8) {
        double t = -B / (2.0 * A);
        if (t >= -1e-8 && t <= 1.0 + 1e-8) {
            double x = P0.x + t * dx;
            double y = P0.y + t * dy;
            intersections.emplace_back(x, y);
        }
    }
    else {
        double sqrtD = sqrt(discriminant);
        double t1 = (-B + sqrtD) / (2.0 * A);
        double t2 = (-B - sqrtD) / (2.0 * A);
        if (t1 >= -1e-8 && t1 <= 1.0 + 1e-8) {
            double x = P0.x + t1 * dx;
            double y = P0.y + t1 * dy;
            intersections.emplace_back(x, y);
        }
        if (t2 >= -1e-8 && t2 <= 1.0 + 1e-8) {
            double x = P0.x + t2 * dx;
            double y = P0.y + t2 * dy;
            intersections.emplace_back(x, y);
        }
    }
    return intersections;
}

// Function to compute angle from A to B in counter-clockwise direction
double angle_between_ccw(const Point &A, const Point &B) {
    double thetaA = atan2(A.y, A.x);
    double thetaB = atan2(B.y, B.x);
    double theta = thetaB - thetaA;
    if (theta < 0) theta += 2.0 * M_PI;
    return theta;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    double r;
    cin >> n >> r;
    vector<Point> points(n);
    for(int i=0;i<n;i++) cin >> points[i].x >> points[i].y;
    double total_area = 0.0;
    double r_sq = r * r;
    for(int i=0;i<n;i++){
        Point P0 = points[i];
        Point P1 = points[(i+1)%n];
        bool d0 = is_inside(P0, r_sq);
        bool d1 = is_inside(P1, r_sq);
        vector<Point> intersections = get_circle_intersections(P0, P1, r);
        if(d0 && d1){
            // Both inside
            total_area += (P0.x * P1.y - P1.x * P0.y) / 2.0;
        }
        else if(!d0 && !d1){
            // Both outside
            if(intersections.size() == 2){
                Point I1 = intersections[0];
                Point I2 = intersections[1];
                double theta = angle_between_ccw(I1, I2);
                double sector_area = 0.5 * r * r * (theta - sin(theta));
                total_area += sector_area;
            }
            // else: both outside and no intersection, do nothing
        }
        else{
            // One inside, one outside
            if(d0 && !d1){
                // P0 inside, P1 outside
                if(intersections.size() >=1){
                    Point I = intersections[0];
                    total_area += (P0.x * I.y - I.x * P0.y) / 2.0;
                    // Compute angle from I to P0
                    double thetaA = atan2(I.y, I.x);
                    double thetaB = atan2(P0.y, P0.x);
                    double theta = thetaB - thetaA;
                    if(theta < 0) theta += 2.0 * M_PI;
                    double sector_area = 0.5 * r * r * (theta - sin(theta));
                    total_area += sector_area;
                }
            }
            else if(!d0 && d1){
                // P0 outside, P1 inside
                if(intersections.size() >=1){
                    Point I = intersections[0];
                    total_area += (I.x * P1.y - P1.x * I.y) / 2.0;
                    // Compute angle from P1 to I
                    double thetaA = atan2(P1.y, P1.x);
                    double thetaB = atan2(I.y, I.x);
                    double theta = thetaB - thetaA;
                    if(theta < 0) theta += 2.0 * M_PI;
                    double sector_area = 0.5 * r * r * (theta - sin(theta));
                    total_area += sector_area;
                }
            }
        }
    }
    // Since we are dealing with a semicircle (y >=0), but the polygon is already in y >=0, no adjustment needed
    // However, ensure that the area is positive
    total_area = abs(total_area);
    // Output with sufficient precision
    cout << fixed << setprecision(9) << total_area;
}
