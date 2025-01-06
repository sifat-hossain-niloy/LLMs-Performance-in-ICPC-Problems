#include <iostream>
#include <vector>
#include <cmath>
#include <queue>
#include <cstdio>
#include <algorithm>
#include <cstring>

using namespace std;

const double PI = acos(-1.0);
const double EARTH_RADIUS = 6371.0; // in km

struct Airport {
    int id;
    double lat, lon;
    double x, y, z; // 3D Cartesian coordinates
};

struct Edge {
    int to;
    double weight;
};

struct KDNode {
    Airport *airport;
    KDNode *left, *right;
    int axis;
};

bool compareX(const Airport *a, const Airport *b) { return a->x < b->x; }
bool compareY(const Airport *a, const Airport *b) { return a->y < b->y; }
bool compareZ(const Airport *a, const Airport *b) { return a->z < b->z; }

KDNode* buildKDTree(vector<Airport*> &points, int depth) {
    if (points.empty()) return nullptr;
    int axis = depth % 3;
    if (axis == 0) sort(points.begin(), points.end(), compareX);
    else if (axis == 1) sort(points.begin(), points.end(), compareY);
    else sort(points.begin(), points.end(), compareZ);
    int median = points.size() / 2;
    KDNode *node = new KDNode();
    node->airport = points[median];
    vector<Airport*> leftPoints(points.begin(), points.begin() + median);
    vector<Airport*> rightPoints(points.begin() + median + 1, points.end());
    node->left = buildKDTree(leftPoints, depth + 1);
    node->right = buildKDTree(rightPoints, depth + 1);
    node->axis = axis;
    return node;
}

double sqr(double x) { return x * x; }

double distanceSquared(Airport *a, double x, double y, double z) {
    return sqr(a->x - x) + sqr(a->y - y) + sqr(a->z - z);
}

void nearestNeighbor(KDNode *node, double x, double y, double z, Airport *&bestAirport, double &bestDistSq, int depth) {
    if (!node) return;
    double d = distanceSquared(node->airport, x, y, z);
    if (d < bestDistSq) {
        bestDistSq = d;
        bestAirport = node->airport;
    }
    int axis = node->axis;
    double diff;
    if (axis == 0) diff = x - node->airport->x;
    else if (axis == 1) diff = y - node->airport->y;
    else diff = z - node->airport->z;
    KDNode *near = diff < 0 ? node->left : node->right;
    KDNode *far = diff < 0 ? node->right : node->left;
    nearestNeighbor(near, x, y, z, bestAirport, bestDistSq, depth + 1);
    if (diff * diff < bestDistSq) {
        nearestNeighbor(far, x, y, z, bestAirport, bestDistSq, depth + 1);
    }
}

void latLonToXYZ(double lat, double lon, double &x, double &y, double &z) {
    lat = lat * PI / 180.0;
    lon = lon * PI / 180.0;
    x = cos(lat) * cos(lon);
    y = cos(lat) * sin(lon);
    z = sin(lat);
}

double greatCircleDistance(Airport *a, Airport *b) {
    double angle = acos(a->x * b->x + a->y * b->y + a->z * b->z);
    return EARTH_RADIUS * angle;
}

void sampleGreatCircle(Airport *a, Airport *b, int numSamples, vector<pair<double, double>> &samples) {
    double angle = acos(a->x * b->x + a->y * b->y + a->z * b->z);
    if (angle == 0) {
        samples.push_back({a->x, a->y}); // Only one sample needed
        return;
    }
    double sinAngle = sin(angle);
    for (int i = 1; i < numSamples; ++i) {
        double t = (double)i / numSamples;
        double A = sin((1 - t) * angle) / sinAngle;
        double B = sin(t * angle) / sinAngle;
        double x = A * a->x + B * b->x;
        double y = A * a->y + B * b->y;
        double z = A * a->z + B * b->z;
        samples.push_back({x, y});
    }
}

int main() {
    int testCase = 1;
    while (true) {
        int N;
        double R;
        if (!(cin >> N >> R)) break;
        vector<Airport> airports(N);
        vector<Airport*> airportPtrs(N);
        for (int i = 0; i < N; ++i) {
            double lon, lat;
            cin >> lon >> lat;
            airports[i].id = i;
            airports[i].lat = lat;
            airports[i].
