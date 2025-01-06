#include <iostream>
#include <vector>
#include <set>
#include <cmath>
#include <algorithm>

const double EPS = 1e-8;

struct Transformation {
    int x, y; // Rotation point
    int sx, sy;
    int tx, ty;

    bool operator<(const Transformation& other) const {
        if (x != other.x) return x < other.x;
        if (y != other.y) return y < other.y;
        if (sx != other.sx) return sx < other.sx;
        if (sy != other.sy) return sy < other.sy;
        if (tx != other.tx) return tx < other.tx;
        return ty < other.ty;
    }
};

void applyRotation(double theta, const std::vector<std::pair<int, int>>& points, std::vector<std::pair<double, double>>& rotatedPoints) {
    double cos_theta = cos(theta);
    double sin_theta = sin(theta);
    for (size_t i = 0; i < points.size(); ++i) {
        double x = points[i].first * cos_theta - points[i].second * sin_theta;
        double y = points[i].first * sin_theta + points[i].second * cos_theta;

        // Snapping
        double xf = fabs(x);
        if (fabs(xf - (int)(xf) - 0.5) < EPS) {
            x = (x > 0) ? floor(x) + 1.0 : ceil(x) - 1.0;
        }
        double yf = fabs(y);
        if (fabs(yf - (int)(yf) - 0.5) < EPS) {
            y = (y > 0) ? floor(y) + 1.0 : ceil(y) - 1.0;
        }
        rotatedPoints[i] = std::make_pair(x, y);
    }
}

int main() {
    int caseNum = 1;
    while (true) {
        std::vector<std::pair<int, int>> initialPoints(3), finalPoints(3);
        bool allZero = true;
        for (int i = 0; i < 3; ++i) {
            std::cin >> initialPoints[i].first >> initialPoints[i].second;
            if (initialPoints[i].first != 0 || initialPoints[i].second != 0)
                allZero = false;
        }
        for (int i = 0; i < 3; ++i) {
            std::cin >> finalPoints[i].first >> finalPoints[i].second;
            if (finalPoints[i].first != 0 || finalPoints[i].second != 0)
                allZero = false;
        }
        if (allZero) break;

        // Generate all possible rotation points
        std::vector<std::pair<int, int>> rotationPoints;
        for (int x = -10; x <= 10; ++x) {
            for (int y = -10; y <= 10; ++y) {
                if (abs(x) != 10 && abs(y) != 10) continue;
                if (x == 0 && y == 0) continue;
                rotationPoints.emplace_back(x, y);
            }
        }

        std::set<Transformation> transformations;
        for (auto& rotPoint : rotationPoints) {
            int x = rotPoint.first;
            int y = rotPoint.second;
            double theta = atan2(y, x);

            std::vector<std::pair<double, double>> rotatedPoints(3);
            applyRotation(theta, initialPoints, rotatedPoints);

            // Consider all permutations
            std::vector<int> perm = {0, 1, 2};
            do {
                bool valid = true;
                int sx = 0, sy = 0, tx = 0, ty = 0;
                // Solve for sx, sy
                for (int i = 0; i < 3 && valid; ++i) {
                    int idx1 = i;
                    int idx2 = (i + 1) % 3;
                    double dx1 = rotatedPoints[idx1].first;
                    double dy1 = rotatedPoints[idx1].second;
                    double dx2 = rotatedPoints[idx2].first;
                    double dy2 = rotatedPoints[idx2].second;
                    int qx1 = finalPoints[perm[idx1]].first;
                    int qy1 = finalPoints[perm[idx1]].second;
                    int qx2 = finalPoints[perm[idx2]].first;
                    int qy2 = finalPoints[perm[idx2]].second;

                    double denomX = dx1 - dx2;
                    double denomY = dy1 - dy2;
                    if (fabs(denomX) < EPS) {
                        if (qx1 != qx2) { valid = false; break; }
                    } else {
                        double sxd = (qx1 - qx2) / denomX;
                        if (fabs(sxd - round(sxd)) > EPS || fabs(sxd) < EPS) { valid = false; break; }
                        if (sx == 0) sx = (int)round(sxd);
                        else if (sx != (int)round(sxd)) { valid = false; break; }
                    }
                    if (fabs(denomY) < EPS) {
                        if (qy1 != qy2) { valid = false; break; }
                    } else {
                        double syd = (qy1 - qy2) / denomY;
                        if (fabs(syd - round(syd)) > EPS || fabs(syd) < EPS) { valid = false; break; }
                        if (sy == 0) sy = (int)round(syd);
                        else if (sy != (int)round(syd)) { valid = false; break; }
                    }
                }
                if (!valid || sx == 0 || sy == 0) continue;

                // Solve for tx, ty
                double txd = finalPoints[perm[0]].first - sx * rotatedPoints[0].first;
                double tyd = finalPoints[perm[0]].second - sy * rotatedPoints[0].second;
                if (fabs(txd - round(txd)) > EPS || fabs(tyd - round(tyd)) > EPS) continue;
                tx = (int)round(txd);
                ty = (int)round(tyd);

                // Verify all points
                for (int i = 0; i < 3 && valid; ++i) {
                    double x_calc = sx * rotatedPoints[i].first + tx;
                    double y_calc = sy * rotatedPoints[i].second + ty;
                    if (fabs(x_calc - finalPoints[perm[i]].first) > EPS || fabs(y_calc - finalPoints[perm[i]].second) > EPS) {
                        valid = false;
                    }
                }
                if (valid) {
                    Transformation trans = {x, y, sx, sy, tx, ty};
                    transformations.insert(trans);
                }
            } while (std::next_permutation(perm.begin(), perm.end()));
        }

        std::cout << "Case " << caseNum++ << ": ";
        if (transformations.empty()) {
            std::cout << "no solution\n";
        } else if (transformations.size() == 1) {
            std::cout << "equivalent solutions\n";
        } else {
            std::cout << "inconsistent solutions\n";
        }
    }
    return 0;
}
