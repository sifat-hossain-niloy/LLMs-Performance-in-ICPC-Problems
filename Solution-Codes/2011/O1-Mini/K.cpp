#include <bits/stdc++.h>
using namespace std;

struct Point {
    double x, y;
};

// Cross product of OA and OB vectors
double cross_product(const Point& O, const Point& A, const Point& B){
    double dx1 = A.x - O.x;
    double dy1 = A.y - O.y;
    double dx2 = B.x - O.x;
    double dy2 = B.y - O.y;
    return dx1 * dy2 - dy1 * dx2;
}

// Convex Hull using Andrew's Monotone Chain Algorithm
vector<Point> convex_hull(vector<Point> pts){
    int n = pts.size();
    if(n == 0) return {};
    // Sort the points
    sort(pts.begin(), pts.end(), [&](const Point& a, const Point& b) -> bool{
        if(fabs(a.x - b.x) > 1e-9) return a.x < b.x;
        return a.y < b.y;
    });
    // Build lower hull
    vector<Point> lower;
    for(auto &p : pts){
        while(lower.size() >= 2 && cross_product(lower[lower.size()-2], lower[lower.size()-1], p) <= 0){
            lower.pop_back();
        }
        lower.push_back(p);
    }
    // Build upper hull
    vector<Point> upper;
    for(int i = pts.size()-1; i >=0; --i){
        auto &p = pts[i];
        while(upper.size() >= 2 && cross_product(upper[upper.size()-2], upper[upper.size()-1], p) <= 0){
            upper.pop_back();
        }
        upper.push_back(p);
    }
    // Concatenate lower and upper to get full hull
    // Remove the last point of each because it's duplicated
    lower.pop_back();
    upper.pop_back();
    vector<Point> hull = lower;
    for(auto &p : upper){
        hull.push_back(p);
    }
    return hull;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    int case_num = 1;
    while(cin >> n && n != 0){
        vector<Point> pts(n);
        for(int i=0;i<n;i++) cin >> pts[i].x >> pts[i].y;
        // Compute convex hull
        vector<Point> hull = convex_hull(pts);
        int m = hull.size();
        if(m == 0){
            // Degenerate case, all points same
            printf("Case %d: 0.00\n", case_num++);
            continue;
        }
        // Compute minimal width using rotating calipers
        double min_width = 1e18;
        for(int i=0;i<m;i++){
            // Edge from hull[i] to hull[(i+1)%m]
            Point A = hull[i];
            Point B = hull[(i+1)%m];
            // Compute the normal vector
            double dx = B.x - A.x;
            double dy = B.y - A.y;
            // Normal vector ( -dy, dx )
            double nx = -dy;
            double ny = dx;
            // Normalize the normal vector
            double norm = sqrt(nx * nx + ny * ny);
            if(norm == 0) continue; // Degenerate edge
            double ux = nx / norm;
            double uy = ny / norm;
            // Project all points on the normal vector
            double proj_min = 1e18, proj_max = -1e18;
            for(auto &p : hull){
                double proj = p.x * ux + p.y * uy;
                if(proj < proj_min) proj_min = proj;
                if(proj > proj_max) proj_max = proj;
            }
            double width = proj_max - proj_min;
            if(width < min_width){
                min_width = width;
            }
        }
        // Round up to nearest 0.01
        double width_rounded = ceil((min_width * 100.0) - 1e-9) / 100.0;
        // Ensure two decimal places
        printf("Case %d: %.2lf\n", case_num++, width_rounded);
    }
}
