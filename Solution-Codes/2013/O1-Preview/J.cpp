#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>

const double EPS = 1e-9;
const double PI = acos(-1.0);

struct Point {
    double x, y;
};

struct Segment {
    Point p0, p1;
    bool isArc; // true if arc, false if line segment
};

double cross(const Point &a, const Point &b) {
    return a.x * b.y - a.y * b.x;
}

double dot(const Point &a, const Point &b) {
    return a.x * b.x + a.y * b.y;
}

double length(const Point &a) {
    return sqrt(a.x * a.x + a.y * a.y);
}

double angle(const Point &p) {
    double ang = atan2(p.y, p.x);
    if (ang < 0) ang += 2 * PI;
    return ang;
}

// Solve quadratic equation a*t^2 + b*t + c = 0
bool solveQuadratic(double a, double b, double c, double &t0, double &t1) {
    double D = b * b - 4 * a * c;
    if (D < -EPS) return false;
    if (D < 0) D = 0;
    else D = sqrt(D);
    t0 = (-b - D) / (2 * a);
    t1 = (-b + D) / (2 * a);
    return true;
}

bool insideCircle(const Point &p, double r) {
    return p.x * p.x + p.y * p.y <= r * r + EPS;
}

Point interpolate(const Point &p0, const Point &p1, double t) {
    return {p0.x + t * (p1.x - p0.x), p0.y + t * (p1.y - p0.y)};
}

// Compute intersection between line segment and circle
std::vector<double> circleIntersections(const Point &p0, const Point &p1, double r) {
    Point d = {p1.x - p0.x, p1.y - p0.y};
    double a = d.x * d.x + d.y * d.y;
    double b = 2 * (p0.x * d.x + p0.y * d.y);
    double c = p0.x * p0.x + p0.y * p0.y - r * r;
    double t0, t1;
    std::vector<double> ts;
    if (solveQuadratic(a, b, c, t0, t1)) {
        if (t0 >= -EPS && t0 <= 1 + EPS) ts.push_back(t0);
        if (t1 >= -EPS && t1 <= 1 + EPS && fabs(t1 - t0) > EPS) ts.push_back(t1);
    }
    return ts;
}

int main() {
    int n;
    double r;
    std::cin >> n >> r;
    std::vector<Point> poly(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> poly[i].x >> poly[i].y;
    }

    std::vector<Segment> segments;

    for (int i = 0; i < n; ++i) {
        Point p0 = poly[i];
        Point p1 = poly[(i + 1) % n];

        bool inside0 = insideCircle(p0, r);
        bool inside1 = insideCircle(p1, r);

        std::vector<double> ts = circleIntersections(p0, p1, r);
        std::vector<std::pair<double, Point>> intersections;

        for (double t : ts) {
            Point p = interpolate(p0, p1, t);
            intersections.push_back({t, p});
        }

        // Sort intersections by t
        std::sort(intersections.begin(), intersections.end());

        std::vector<Point> points;
        if (inside0) points.push_back(p0);
        for (auto &inter : intersections) {
            points.push_back(inter.second);
        }
        if (inside1) points.push_back(p1);

        for (size_t j = 0; j + 1 < points.size(); ++j) {
            segments.push_back({points[j], points[j + 1], false});
        }

        // Handle arcs
        if (points.size() == 2 && (!inside0 || !inside1)) {
            // Need to add arc between points[1] and points[0] along circle
            segments.push_back({points[1], points[0], true});
        }
    }

    // Compute total area
    double area = 0.0;
    for (auto &seg : segments) {
        if (!seg.isArc) {
            area += cross(seg.p0, seg.p1) / 2.0;
        } else {
            double theta1 = angle(seg.p0);
            double theta2 = angle(seg.p1);
            double delta = theta2 - theta1;
            if (delta <= -PI) delta += 2 * PI;
            else if (delta > PI) delta -= 2 * PI;
            area += 0.5 * r * r * delta;
        }
    }

    std::cout << std::fixed << std::setprecision(9) << fabs(area) << std::endl;

    return 0;
}
