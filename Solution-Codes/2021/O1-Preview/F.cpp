#include <iostream>
#include <cmath>
#include <vector>
#include <limits>
#include <iomanip>

struct Point3D {
    double x, y, z;
};

struct FlightPath {
    Point3D p0, p1;
};

double computeTheta(const Point3D& Q, const FlightPath& flight) {
    // Vector V = P1 - P0
    Point3D V = {flight.p1.x - flight.p0.x, flight.p1.y - flight.p0.y, flight.p1.z - flight.p0.z};
    // Vector from P0 to Q
    Point3D QP0 = {Q.x - flight.p0.x, Q.y - flight.p0.y, Q.z - flight.p0.z};
    // Dot product V ⋅ V
    double V_dot_V = V.x * V.x + V.y * V.y + V.z * V.z;
    // Dot product QP0 ⋅ V
    double QP0_dot_V = QP0.x * V.x + QP0.y * V.y + QP0.z * V.z;
    // Parameter s0
    double s0 = QP0_dot_V / V_dot_V;
    // Clamp s0 to [0,1]
    if (s0 < 0.0) s0 = 0.0;
    if (s0 > 1.0) s0 = 1.0;
    // Point P(s0) on the flight path
    Point3D P = {flight.p0.x + s0 * V.x, flight.p0.y + s0 * V.y, flight.p0.z + s0 * V.z};
    // Horizontal distance D
    double D = std::sqrt((Q.x - P.x) * (Q.x - P.x) + (Q.y - P.y) * (Q.y - P.y));
    // Altitude H
    double H = P.z - Q.z;
    // If H <= 0, impossible to observe
    if (H <= 0.0) return std::numeric_limits<double>::infinity();
    // Angle theta in radians
    double theta = std::atan2(D, H);
    // Convert to degrees
    theta = theta * (180.0 / M_PI);
    return theta;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    std::vector<std::vector<Point3D>> islands(n);
    for (int i = 0; i < n; ++i) {
        int ni;
        std::cin >> ni;
        islands[i].resize(ni);
        for (int j = 0; j < ni; ++j) {
            std::cin >> islands[i][j].x >> islands[i][j].y;
            islands[i][j].z = 0.0; // Ground level
        }
    }
    std::vector<FlightPath> flights(m);
    for (int i = 0; i < m; ++i) {
        std::cin >> flights[i].p0.x >> flights[i].p0.y >> flights[i].p0.z
                 >> flights[i].p1.x >> flights[i].p1.y >> flights[i].p1.z;
    }
    bool impossible = false;
    double overall_theta = 0.0;
    for (int i = 0; i < n; ++i) {
        double min_theta = std::numeric_limits<double>::infinity();
        for (int j = 0; j < m; ++j) {
            double max_theta = 0.0;
            bool can_observe = true;
            for (const auto& Q : islands[i]) {
                double theta = computeTheta(Q, flights[j]);
                if (theta == std::numeric_limits<double>::infinity()) {
                    can_observe = false;
                    break;
                }
                if (theta > max_theta) max_theta = theta;
            }
            if (can_observe && max_theta < min_theta) {
                min_theta = max_theta;
            }
        }
        if (min_theta == std::numeric_limits<double>::infinity()) {
            impossible = true;
            break;
        }
        if (min_theta > overall_theta) overall_theta = min_theta;
    }
    if (impossible) {
        std::cout << "impossible" << std::endl;
    } else {
        std::cout << std::fixed << std::setprecision(9) << overall_theta << std::endl;
    }
    return 0;
}
