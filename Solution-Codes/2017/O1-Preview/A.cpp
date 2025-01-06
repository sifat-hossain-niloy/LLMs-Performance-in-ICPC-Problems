#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

struct Point {
    int x, y;
};

// Function to compute the orientation of the ordered triplet (p, q, r)
// Returns:
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(const Point &p, const Point &q, const Point &r) {
    long long val = (long long)(q.y - p.y) * (r.x - q.x) - (long long)(q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0; // colinear
    return (val > 0) ? 1 : 2; // clock or counterclockwise
}

// Function to check if point q lies on segment pr
bool onSegment(const Point &p, const Point &q, const Point &r) {
    if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
        q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
        return true;
    return false;
}

// Function to check if segments p1q1 and p2q2 intersect
bool doIntersect(const Point &p1, const Point &q1, const Point &p2, const Point &q2) {
    // Find the four orientations
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;
    // p1, q1 and q2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;
    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;
    // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false;
}

// Function to check if point p lies inside polygon poly[]
bool isInsidePolygon(const vector<Point> &poly, const Point &p) {
    int n = poly.size();
    if (n < 3) return false; // Polygon must have at least 3 vertices

    // Create a point for the line segment from p to infinity
    Point extreme = {1000000001, p.y + 1};

    // Count intersections of the above line with sides of polygon
    int count = 0, i = 0;
    do {
        int next = (i + 1) % n;

        // Check if the line segment from p to extreme intersects with the edge from poly[i] to poly[next]
        if (doIntersect(poly[i], poly[next], p, extreme)) {
            // If p is colinear with edge, check if it lies on edge
            if (orientation(poly[i], p, poly[next]) == 0)
                return onSegment(poly[i], p, poly[next]);
            count++;
        }
        i = next;
    } while (i != 0);

    // Point is inside if count is odd
    return (count % 2 == 1);
}

// Function to check if two vertices are visible to each other
bool areVisible(const vector<Point> &poly, int i, int j) {
    int n = poly.size();
    Point vi = poly[i];
    Point vj = poly[j];

    // Exclude edges adjacent to vi and vj
    vector<pair<Point, Point>> edges;
    for (int k = 0; k < n; ++k) {
        int k_next = (k + 1) % n;
        // Exclude edges incident to vi or vj
        if ((k == i) || (k_next == i) || (k == j) || (k_next == j))
            continue;
        edges.push_back({poly[k], poly[k_next]});
    }

    // Check for intersection with other edges
    for (const auto &edge : edges) {
        if (doIntersect(vi, vj, edge.first, edge.second))
            return false;
    }

    // Check if midpoint is inside the polygon
    Point mid;
    mid.x = (vi.x + vj.x) / 2;
    mid.y = (vi.y + vj.y) / 2;
    if (!isInsidePolygon(poly, mid))
        return false;

    return true;
}

double distance(const Point &p1, const Point &p2) {
    double dx = (double)p1.x - (double)p2.x;
    double dy = (double)p1.y - (double)p2.y;
    return sqrt(dx * dx + dy * dy);
}

int main() {
    int n;
    cin >> n;
    vector<Point> poly(n);
    for (int i = 0; i < n; ++i) {
        cin >> poly[i].x >> poly[i].y;
    }

    double maxLength = 0.0;

    // For all pairs of vertices
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (areVisible(poly, i, j)) {
                double len = distance(poly[i], poly[j]);
                if (len > maxLength) {
                    maxLength = len;
                }
            }
        }
    }

    cout << fixed << setprecision(9) << maxLength << endl;

    return 0;
}
