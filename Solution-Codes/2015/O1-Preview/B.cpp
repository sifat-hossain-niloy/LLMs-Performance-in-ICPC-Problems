#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdio>

using namespace std;

const double EPS = 1e-8;

struct Point {
    double x, y;

    Point operator+(const Point& p) const { return {x + p.x, y + p.y}; }
    Point operator-(const Point& p) const { return {x - p.x, y - p.y}; }
    Point operator*(double k) const { return {x * k, y * k}; }
    double cross(const Point& p) const { return x * p.y - y * p.x; }
    double dot(const Point& p) const { return x * p.x + y * p.y; }
};

typedef vector<Point> Polygon;

Point readPoint() {
    int x, y;
    cin >> x >> y;
    return {double(x), double(y)};
}

double polygonArea(const Polygon& poly) {
    double area = 0;
    int n = poly.size();
    for(int i = 0; i < n; ++i)
        area += poly[i].cross(poly[(i + 1) % n]);
    return fabs(area) / 2.0;
}

// Compute intersection of two convex polygons using Sutherland-Hodgman algorithm
Polygon polygonIntersect(const Polygon& poly1, const Polygon& poly2) {
    Polygon output = poly1;

    for(int i = 0; i < poly2.size(); ++i) {
        Point A = poly2[i];
        Point B = poly2[(i + 1) % poly2.size()];
        Polygon input = output;
        output.clear();

        for(int j = 0; j < input.size(); ++j) {
            Point P = input[j];
            Point Q = input[(j + 1) % input.size()];

            double cp1 = (B - A).cross(P - A);
            double cp2 = (B - A).cross(Q - A);

            if(cp1 >= -EPS) {
                if(cp2 >= -EPS) {
                    output.push_back(Q);
                } else {
                    double t = ((A - P).cross(B - A)) / ((Q - P).cross(B - A));
                    Point I = P + (Q - P) * t;
                    output.push_back(I);
                }
            } else if(cp2 >= -EPS) {
                double t = ((A - P).cross(B - A)) / ((Q - P).cross(B - A));
                Point I = P + (Q - P) * t;
                output.push_back(I);
                output.push_back(Q);
            }
        }

        if(output.empty())
            return {};
    }
    return output;
}

vector<double> eventTimes;

void computeEventTimes(const Polygon& polyA, const Point& v, const Polygon& polyB) {
    int nA = polyA.size();
    int nB = polyB.size();
    for(int i = 0; i < nA; ++i) {
        Point V0 = polyA[i];
        for(int j = 0; j < nB; ++j) {
            Point P0 = polyB[j];
            Point P1 = polyB[(j + 1) % nB];
            Point edge = P1 - P0;
            double D = v.cross(edge);
            if(fabs(D) < EPS)
                continue;
            double t = (P0 - V0).cross(edge) / D;
            if(t < -EPS)
                continue;
            Point Vt = V0 + v * t;
            double u = (Vt - P0).dot(edge) / edge.dot(edge);
            if(u >= -EPS && u <= 1 + EPS) {
                eventTimes.push_back(t);
            }
        }
    }
}

double findMaxArea(double t_left, double t_right, const Polygon& polyA0, const Point& v, const Polygon& polyB) {
    double max_area = 0;
    double max_time = t_left;
    for(int iter = 0; iter < 50; ++iter) {
        double t1 = (2 * t_left + t_right) / 3;
        double t2 = (t_left + 2 * t_right) / 3;

        Polygon polyA_t1;
        for(const auto& p : polyA0)
            polyA_t1.push_back(p + v * t1);
        Polygon inter_t1 = polygonIntersect(polyA_t1, polyB);
        double area_t1 = polygonArea(inter_t1);

        Polygon polyA_t2;
        for(const auto& p : polyA0)
            polyA_t2.push_back(p + v * t2);
        Polygon inter_t2 = polygonIntersect(polyA_t2, polyB);
        double area_t2 = polygonArea(inter_t2);

        if(area_t1 < area_t2) {
            t_left = t1;
            if(area_t2 > max_area) {
                max_area = area_t2;
                max_time = t2;
            }
        } else {
            t_right = t2;
            if(area_t1 > max_area) {
                max_area = area_t1;
                max_time = t1;
            }
        }
    }
    return max_time;
}

int main() {
    Polygon polyA0, polyB;
    Point vA, vB;
    int n;
    cin >> n;
    for(int i = 0; i < n; ++i)
        polyA0.push_back(readPoint());
    cin >> vA.x >> vA.y;

    cin >> n;
    for(int i = 0; i < n; ++i)
        polyB.push_back(readPoint());
    cin >> vB.x >> vB.y;

    Point v = {vA.x - vB.x, vA.y - vB.y};

    // Compute event times
    eventTimes.push_back(0.0);
    computeEventTimes(polyA0, v, polyB);
    // Also compute times for edges of A and vertices of B
    int nA = polyA0.size();
    int nB = polyB.size();
    for(int i = 0; i < nB; ++i) {
        Point V0 = polyB[i];
        for(int j = 0; j < nA; ++j) {
            Point P0 = polyA0[j];
            Point P1 = polyA0[(j + 1) % nA];
            Point edge = P1 - P0;
            Point v_neg = {-v.x, -v.y};
            double D = v_neg.cross(edge);
            if(fabs(D) < EPS)
                continue;
            double t = (P0 - V0).cross(edge) / D;
            if(t < -EPS)
                continue;
            Point Vt = V0 + v_neg * t;
            double u = (Vt - P0).dot(edge) / edge.dot(edge);
            if(u >= -EPS && u <= 1 + EPS) {
                eventTimes.push_back(t);
            }
        }
    }

    sort(eventTimes.begin(), eventTimes.end());
    // Remove duplicates
    eventTimes.erase(unique(eventTimes.begin(), eventTimes.end(), [](double a, double b) {
        return fabs(a - b) < EPS;
    }), eventTimes.end());

    double max_area = 0;
    double max_time = -1;
    for(int i = 0; i + 1 < eventTimes.size(); ++i) {
        double t_left = max(eventTimes[i], 0.0);
        double t_right = eventTimes[i + 1];
        if(t_right - t_left < EPS)
            continue;
        double t = findMaxArea(t_left, t_right, polyA0, v, polyB);
        // Compute area at t
        Polygon polyA_t;
        for(const auto& p : polyA0)
            polyA_t.push_back(p + v * t);
        Polygon inter = polygonIntersect(polyA_t, polyB);
        double area = polygonArea(inter);
        if(area > max_area + EPS) {
            max_area = area;
            max_time = t;
        } else if(fabs(area - max_area) < EPS && t < max_time) {
            max_time = t;
        }
    }

    // Check at event times
    for(double t : eventTimes) {
        if(t < -EPS)
            continue;
        Polygon polyA_t;
        for(const auto& p : polyA0)
            polyA_t.push_back(p + v * t);
        Polygon inter = polygonIntersect(polyA_t, polyB);
        double area = polygonArea(inter);
        if(area > max_area + EPS) {
            max_area = area;
            max_time = t;
        } else if(fabs(area - max_area) < EPS && t < max_time) {
            max_time = t;
        }
    }

    if(max_time < -EPS) {
        cout << "never" << endl;
    } else {
        printf("%.6f\n", max_time);
    }
    return 0;
}
