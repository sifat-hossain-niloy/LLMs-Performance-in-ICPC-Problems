#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-8;

struct Point {
    double x, y;
    Point(double x=0, double y=0): x(x), y(y) {}
    Point operator-(const Point& p) const { return Point(x - p.x, y - p.y); }
    Point operator+(const Point& p) const { return Point(x + p.x, y + p.y); }
    Point operator*(double c) const { return Point(x * c, y * c); }
    Point rotate(double theta) const {
        return Point(x * cos(theta) - y * sin(theta),
                     x * sin(theta) + y * cos(theta));
    }
};

struct Edge {
    Point p1, p2;
    double length;
};

typedef vector<Point> Polygon;

double cross(const Point& a, const Point& b) {
    return a.x * b.y - a.y * b.x;
}

double dot(const Point& a, const Point& b) {
    return a.x * b.x + a.y * b.y;
}

double angle(const Point& a) {
    return atan2(a.y, a.x);
}

bool equal(const Point& a, const Point& b) {
    return fabs(a.x - b.x) < EPS && fabs(a.y - b.y) < EPS;
}

void computeEdges(const Polygon& poly, vector<Edge>& edges) {
    int n = poly.size();
    edges.resize(n);
    for (int i = 0; i < n; ++i) {
        Point p1 = poly[i];
        Point p2 = poly[(i + 1) % n];
        edges[i] = {p1, p2, hypot(p2.x - p1.x, p2.y - p1.y)};
    }
}

void rotatePolygon(Polygon& poly, double theta) {
    for (auto& p : poly) {
        p = p.rotate(theta);
    }
}

void translatePolygon(Polygon& poly, const Point& t) {
    for (auto& p : poly) {
        p = p + t;
    }
}

bool checkPolygonsOverlap(const Polygon& poly1, const Polygon& poly2) {
    // Using Separating Axis Theorem (SAT)
    auto projectPolygon = [](const Polygon& poly, const Point& axis, double& minP, double& maxP) {
        minP = maxP = dot(poly[0], axis);
        for (const auto& p : poly) {
            double proj = dot(p, axis);
            if (proj < minP) minP = proj;
            if (proj > maxP) maxP = proj;
        }
    };

    auto getAxes = [](const Polygon& poly, vector<Point>& axes) {
        int n = poly.size();
        axes.resize(n);
        for (int i = 0; i < n; ++i) {
            Point p1 = poly[i];
            Point p2 = poly[(i + 1) % n];
            Point edge = p2 - p1;
            axes[i] = Point(-edge.y, edge.x); // Normal vector
            double len = hypot(axes[i].x, axes[i].y);
            axes[i].x /= len;
            axes[i].y /= len;
        }
    };

    vector<Point> axes1, axes2;
    getAxes(poly1, axes1);
    getAxes(poly2, axes2);

    for (const auto& axis : axes1) {
        double minA, maxA, minB, maxB;
        projectPolygon(poly1, axis, minA, maxA);
        projectPolygon(poly2, axis, minB, maxB);
        if (maxA < minB + EPS || maxB < minA + EPS) {
            return false; // No overlap on this axis
        }
    }

    for (const auto& axis : axes2) {
        double minA, maxA, minB, maxB;
        projectPolygon(poly1, axis, minA, maxA);
        projectPolygon(poly2, axis, minB, maxB);
        if (maxA < minB + EPS || maxB < minA + EPS) {
            return false; // No overlap on this axis
        }
    }

    return true; // Polygons overlap
}

int main() {
    int n1, n2;
    cin >> n1;
    Polygon poly1(n1);
    for (int i = 0; i < n1; ++i) {
        int x, y;
        cin >> x >> y;
        poly1[i] = Point(x, y);
    }
    cin >> n2;
    Polygon poly2(n2);
    for (int i = 0; i < n2; ++i) {
        int x, y;
        cin >> x >> y;
        poly2[i] = Point(x, y);
    }

    vector<Edge> edges1, edges2;
    computeEdges(poly1, edges1);
    computeEdges(poly2, edges2);

    double maxCommonLength = 0.0;

    for (int i1 = 0; i1 < n1; ++i1) {
        Edge e1 = edges1[i1];
        Point v1 = e1.p2 - e1.p1;
        double len1 = hypot(v1.x, v1.y);

        for (int i2 = 0; i2 < n2; ++i2) {
            Edge e2 = edges2[i2];
            Point v2 = e2.p2 - e2.p1;
            double len2 = hypot(v2.x, v2.y);

            // Compute rotation angle
            double angle1 = atan2(v1.y, v1.x);
            double angle2 = atan2(-v2.y, -v2.x); // Reverse direction
            double theta = angle1 - angle2;

            // Rotate and translate polygon2
            Polygon poly2_transformed = poly2;
            rotatePolygon(poly2_transformed, theta);

            Point q2_rotated = poly2_transformed[(i2 + 1) % n2];
            Point translation = e1.p1 - q2_rotated;
            translatePolygon(poly2_transformed, translation);

            // Update edges of transformed polygon2
            vector<Edge> edges2_transformed;
            computeEdges(poly2_transformed, edges2_transformed);

            // Edge matching
            double commonLength = 0.0;
            int k = 0;
            for (; k < n1 && k < n2; ++k) {
                int idx1 = (i1 + k) % n1;
                int idx2 = (i2 - k + n2) % n2;

                Edge e1_k = edges1[idx1];
                Edge e2_k = edges2_transformed[idx2];

                if (!equal(e1_k.p1, e2_k.p2) || !equal(e1_k.p2, e2_k.p1)) {
                    break; // Edges do not coincide
                }
                commonLength += e1_k.length;
            }

            if (commonLength > maxCommonLength) {
                // Check for overlap
                if (!checkPolygonsOverlap(poly1, poly2_transformed)) {
                    maxCommonLength = commonLength;
                }
            }
        }
    }

    cout << fixed << setprecision(9) << maxCommonLength << endl;
    return 0;
}
