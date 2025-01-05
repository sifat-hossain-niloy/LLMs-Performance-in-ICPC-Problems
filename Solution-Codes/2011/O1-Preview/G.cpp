#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>
using namespace std;

// Function to check if four sides can form a quadrilateral
bool canFormQuadrilateral(double a, double b, double c, double d) {
    double sides[4] = { a, b, c, d };
    sort(sides, sides + 4);
    return sides[0] + sides[1] + sides[2] > sides[3];
}

// Function to check if three sides can form a triangle
bool canFormTriangle(double a, double b, double c) {
    double sides[3] = { a, b, c };
    sort(sides, sides + 3);
    return sides[0] + sides[1] > sides[2];
}

// Function to compute area of quadrilateral using Brahmagupta's formula
double quadrilateralArea(double a, double b, double c, double d) {
    double s = (a + b + c + d) / 2.0;
    double areaSquared = (s - a) * (s - b) * (s - c) * (s - d);
    if (areaSquared <= 0) return 0;
    return sqrt(areaSquared);
}

// Function to compute area of triangle using Heron's formula
double triangleArea(double a, double b, double c) {
    double s = (a + b + c) / 2.0;
    double areaSquared = s * (s - a) * (s - b) * (s - c);
    if (areaSquared <= 0) return 0;
    return sqrt(areaSquared);
}

int main() {
    int caseNumber = 1;
    while (true) {
        int n;
        cin >> n;
        if (n == 0) break;
        vector<double> segments(n);
        for (int i = 0; i < n; ++i) {
            cin >> segments[i];
        }

        // Sort segments in decreasing order
        sort(segments.rbegin(), segments.rend());
        vector<bool> used(n, false);
        double totalArea = 0.0;

        // Try to form quadrilaterals
        for (int i = 0; i + 3 < n; ++i) {
            if (used[i]) continue;
            for (int j = i + 1; j + 2 < n; ++j) {
                if (used[j]) continue;
                for (int k = j + 1; k + 1 < n; ++k) {
                    if (used[k]) continue;
                    for (int l = k + 1; l < n; ++l) {
                        if (used[l]) continue;
                        double a = segments[i], b = segments[j], c = segments[k], d = segments[l];
                        if (canFormQuadrilateral(a, b, c, d)) {
                            double area = quadrilateralArea(a, b, c, d);
                            if (area > 0) {
                                totalArea += area;
                                used[i] = used[j] = used[k] = used[l] = true;
                                goto nextQuadrilateral; // Break to outer loop to pick next quadrilateral
                            }
                        }
                    }
                }
            }
            nextQuadrilateral:;
        }

        // Try to form triangles
        for (int i = 0; i + 2 < n; ++i) {
            if (used[i]) continue;
            for (int j = i + 1; j + 1 < n; ++j) {
                if (used[j]) continue;
                for (int k = j + 1; k < n; ++k) {
                    if (used[k]) continue;
                    double a = segments[i], b = segments[j], c = segments[k];
                    if (canFormTriangle(a, b, c)) {
                        double area = triangleArea(a, b, c);
                        if (area > 0) {
                            totalArea += area;
                            used[i] = used[j] = used[k] = true;
                            goto nextTriangle; // Break to outer loop to pick next triangle
                        }
                    }
                }
            }
            nextTriangle:;
        }

        cout << fixed << setprecision(6);
        cout << "Case " << caseNumber++ << ": " << totalArea << endl;
    }
    return 0;
}
