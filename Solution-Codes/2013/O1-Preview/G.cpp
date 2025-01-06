#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <set>
#include <limits>
using namespace std;

struct Point {
    double x, y;
};

struct Segment {
    Point p1, p2;
};

bool point_in_polygon(Point pt, const vector<Point>& poly) {
    int n = poly.size();
    int cnt = 0;
    for (int i = 0; i < n; i++) {
        Point a = poly[i];
        Point b = poly[(i + 1) % n];
        if ((a.y > pt.y) != (b.y > pt.y)) {
            double x_intersect = (b.x - a.x) * (pt.y - a.y) / (b.y - a.y) + a.x;
            if (pt.x < x_intersect)
                cnt++;
        }
    }
    return cnt % 2 == 1;
}

bool on_segment(Point p, Point q, Point r) {
    return q.x <= max(p.x, r.x) + 1e-8 && q.x >= min(p.x, r.x) - 1e-8 &&
           q.y <= max(p.y, r.y) + 1e-8 && q.y >= min(p.y, r.y) - 1e-8;
}

int orientation(Point p, Point q, Point r) {
    double val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (abs(val) < 1e-8) return 0; // colinear
    return (val > 0) ? 1 : 2; // clock or counterclockwise
}

bool segments_intersect(Point p1, Point q1, Point p2, Point q2) {
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);
    // General case
    if (o1 != o2 && o3 != o4)
        return true;
    // Special Cases
    if (o1 == 0 && on_segment(p1, p2, q1)) return true;
    if (o2 == 0 && on_segment(p1, q2, q1)) return true;
    if (o3 == 0 && on_segment(p2, p1, q2)) return true;
    if (o4 == 0 && on_segment(p2, q1, q2)) return true;
    return false;
}

bool rectangle_polygon_overlap(double rx1, double ry1, double rx2, double ry2, const vector<Point>& poly) {
    // Check if any corner of the rectangle is inside the polygon
    vector<Point> rect = {{rx1, ry1}, {rx2, ry1}, {rx2, ry2}, {rx1, ry2}};
    for (const auto& corner : rect) {
        if (point_in_polygon(corner, poly))
            return true;
    }
    // Check if any vertex of the polygon is inside the rectangle
    for (const auto& vertex : poly) {
        if (vertex.x >= rx1 - 1e-8 && vertex.x <= rx2 + 1e-8 &&
            vertex.y >= ry1 - 1e-8 && vertex.y <= ry2 + 1e-8)
            return true;
    }
    // Check if any edge of the polygon intersects with any edge of the rectangle
    int n = poly.size();
    vector<Segment> rect_edges = {
        {{rx1, ry1}, {rx2, ry1}},
        {{rx2, ry1}, {rx2, ry2}},
        {{rx2, ry2}, {rx1, ry2}},
        {{rx1, ry2}, {rx1, ry1}}
    };
    for (int i = 0; i < n; i++) {
        Segment poly_edge = {poly[i], poly[(i + 1) % n]};
        for (const auto& rect_edge : rect_edges) {
            if (segments_intersect(poly_edge.p1, poly_edge.p2, rect_edge.p1, rect_edge.p2))
                return true;
        }
    }
    return false;
}

int main() {
    int n, xs, ys;
    cin >> n >> xs >> ys;
    vector<Point> poly(n);
    set<int> x_rems, y_rems;
    for (int i = 0; i < n; i++) {
        int xi, yi;
        cin >> xi >> yi;
        poly[i] = {double(xi), double(yi)};
        x_rems.insert(xi % xs);
        y_rems.insert(yi % ys);
    }
    set<int> x_shifts, y_shifts;
    for (int xr : x_rems) {
        int shift = (xs - xr % xs) % xs;
        x_shifts.insert(shift);
    }
    for (int yr : y_rems) {
        int shift = (ys - yr % ys) % ys;
        y_shifts.insert(shift);
    }
    double xmin = numeric_limits<double>::max(), xmax = numeric_limits<double>::lowest();
    double ymin = numeric_limits<double>::max(), ymax = numeric_limits<double>::lowest();
    for (const auto& p : poly) {
        xmin = min(xmin, p.x);
        xmax = max(xmax, p.x);
        ymin = min(ymin, p.y);
        ymax = max(ymax, p.y);
    }
    int min_tiles = numeric_limits<int>::max();
    for (int x_shift : x_shifts) {
        for (int y_shift : y_shifts) {
            int tiles = 0;
            int x_start = int(floor((xmin - x_shift) / xs));
            int x_end = int(floor((xmax - x_shift) / xs));
            int y_start = int(floor((ymin - y_shift) / ys));
            int y_end = int(floor((ymax - y_shift) / ys));
            for (int i = x_start; i <= x_end; i++) {
                for (int j = y_start; j <= y_end; j++) {
                    double rx1 = x_shift + i * xs;
                    double ry1 = y_shift + j * ys;
                    double rx2 = rx1 + xs;
                    double ry2 = ry1 + ys;
                    if (rectangle_polygon_overlap(rx1, ry1, rx2, ry2, poly))
                        tiles++;
                }
            }
            min_tiles = min(min_tiles, tiles);
        }
    }
    cout << min_tiles << endl;
    return 0;
}
