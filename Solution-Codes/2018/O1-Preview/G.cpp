#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <queue>
#include <iomanip>
#include <set>

using namespace std;

const double EPS = 1e-9;

struct Point {
    double x, y;
    int id;
    Point() {}
    Point(double x, double y) : x(x), y(y) {}
};

struct Edge {
    int a, b;
};

struct Triangle {
    int a, b, c;
};

bool operator<(const Point& a, const Point& b) {
    if (fabs(a.x - b.x) > EPS) return a.x < b.x;
    return a.y < b.y;
}

double cross(const Point& O, const Point& A, const Point& B) {
    return (A.x - O.x)*(B.y - O.y) - (A.y - O.y)*(B.x - O.x);
}

bool pointInPolygon(const vector<Point>& polygon, const Point& p) {
    int wn = 0;
    int n = polygon.size();
    for (int i = 0; i < n; ++i) {
        const Point& pi = polygon[i];
        const Point& pj = polygon[(i+1)%n];
        if (fabs(cross(pi, pj, p)) < EPS && min(pi.x, pj.x) <= p.x && p.x <= max(pi.x, pj.x) && min(pi.y, pj.y) <= p.y && p.y <= max(pi.y, pj.y))
            return true;
        int k = ((p.y - pi.y) > EPS) - ((p.y - pi.y) < -EPS);
        int l = ((pj.y - p.y) > EPS) - ((pj.y - p.y) < -EPS);
        double d = cross(pi, pj, p);
        if (k <= 0 && l > 0 && d > EPS) ++wn;
        if (l <= 0 && k > 0 && d < -EPS) --wn;
    }
    return wn != 0;
}

double distance(const Point& a, const Point& b) {
    return hypot(a.x - b.x, a.y - b.y);
}

// Delaunay triangulation using Bowyer-Watson algorithm
struct Circle {
    Point center;
    double radius;
};

bool inCircle(const Circle& c, const Point& p) {
    return distance(c.center, p) < c.radius - EPS;
}

Circle circumcircle(const Point& a, const Point& b, const Point& c) {
    double dA = a.x * a.x + a.y * a.y;
    double dB = b.x * b.x + b.y * b.y;
    double dC = c.x * c.x + c.y * c.y;
    double aux1 = (dA*(c.y - b.y) + dB*(a.y - c.y) + dC*(b.y - a.y));
    double aux2 = -(dA*(c.x - b.x) + dB*(a.x - c.x) + dC*(b.x - a.x));
    double div = (2*(a.x*(c.y - b.y) + b.x*(a.y - c.y) + c.x*(b.y - a.y)));
    if (fabs(div) < EPS) {
        return Circle{Point{1e9, 1e9}, 1e9};
    }
    Point center(aux1/div, aux2/div);
    double radius = distance(center, a);
    return Circle{center, radius};
}

vector<Triangle> delaunayTriangulation(vector<Point>& points) {
    // Super triangle
    double minX = points[0].x, minY = points[0].y;
    double maxX = minX, maxY = minY;
    for (const auto& p : points) {
        if (p.x < minX) minX = p.x;
        if (p.y < minY) minY = p.y;
        if (p.x > maxX) maxX = p.x;
        if (p.y > maxY) maxY = p.y;
    }
    double dx = maxX - minX;
    double dy = maxY - minY;
    double deltaMax = max(dx, dy) * 1000;
    Point p1(minX - deltaMax, minY - deltaMax);
    Point p2(minX + dx / 2, maxY + deltaMax);
    Point p3(maxX + deltaMax, minY - deltaMax);
    points.push_back(p1);
    points.push_back(p2);
    points.push_back(p3);
    vector<Triangle> triangles;
    triangles.push_back({(int)points.size() - 3, (int)points.size() - 2, (int)points.size() - 1});
    for (int i = 0; i < (int)points.size() - 3; ++i) {
        vector<Edge> polygon;
        vector<int> badTriangles;
        vector<Circle> circles;
        for (int j = 0; j < (int)triangles.size(); ++j) {
            auto& tri = triangles[j];
            Circle c = circumcircle(points[tri.a], points[tri.b], points[tri.c]);
            if (inCircle(c, points[i])) {
                badTriangles.push_back(j);
                polygon.push_back({tri.a, tri.b});
                polygon.push_back({tri.b, tri.c});
                polygon.push_back({tri.c, tri.a});
            }
        }
        set<pair<int, int>> edges;
        for (const auto& e : polygon) {
            int a = min(e.a, e.b);
            int b = max(e.a, e.b);
            if (edges.count({a, b})) edges.erase({a, b});
            else edges.insert({a, b});
        }
        for (int j = badTriangles.size() - 1; j >= 0; --j) {
            triangles.erase(triangles.begin() + badTriangles[j]);
        }
        for (const auto& e : edges) {
            triangles.push_back({e.first, e.second, i});
        }
    }
    vector<Triangle> result;
    for (const auto& tri : triangles) {
        if (tri.a < points.size() - 3 && tri.b < points.size() - 3 && tri.c < points.size() - 3) {
            result.push_back(tri);
        }
    }
    points.pop_back();
    points.pop_back();
    points.pop_back();
    return result;
}

int main() {
    int n;
    cin >> n;
    vector<Point> polygon(n);
    for (int i = 0; i < n; ++i) {
        cin >> polygon[i].x >> polygon[i].y;
        polygon[i].id = i;
    }
    vector<Point> points = polygon;
    auto triangles = delaunayTriangulation(points);
    double maxMinDist = 0.0;
    for (const auto& tri : triangles) {
        Point a = points[tri.a];
        Point b = points[tri.b];
        Point c = points[tri.c];
        Circle cc = circumcircle(a, b, c);
        if (pointInPolygon(polygon, cc.center)) {
            double minDist = distance(cc.center, a);
            if (minDist > maxMinDist) maxMinDist = minDist;
        }
    }
    cout << fixed << setprecision(9) << maxMinDist << endl;
    return 0;
}
