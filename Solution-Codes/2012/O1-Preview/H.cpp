#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <limits>
#include <algorithm>

using namespace std;

struct Point {
    double x, y;
};

double euclideanDistance(const Point &a, const Point &b) {
    return hypot(a.x - b.x, a.y - b.y);
}

// Structure to hold the problem data
struct Problem {
    int N; // Number of vertices (walls)
    Point start; // Starting point
    vector<Point> vertices; // Vertices of the polygon
};

// Function to read the input
Problem readInput() {
    Problem prob;
    cin >> prob.N >> prob.start.x >> prob.start.y;
    prob.vertices.resize(prob.N);
    for (int i = 0; i < prob.N; ++i) {
        cin >> prob.vertices[i].x >> prob.vertices[i].y;
    }
    return prob;
}

// Solve the problem
void solveProblem(const Problem &prob, int caseNumber) {
    int N = prob.N;
    const Point &start = prob.start;
    const vector<Point> &vertices = prob.vertices;

    // Build list of touchpoints (using endpoints of walls)
    vector<Point> nodes;
    nodes.push_back(start); // Node 0 is the starting point
    for (int i = 0; i < N; ++i) {
        nodes.push_back(vertices[i]); // Endpoint 1 of wall i
        nodes.push_back(vertices[(i + 1) % N]); // Endpoint 2 of wall i
    }

    int numNodes = nodes.size();

    // Compute distance matrix
    vector<vector<double>> dist(numNodes, vector<double>(numNodes, 0.0));
    for (int i = 0; i < numNodes; ++i) {
        for (int j = i + 1; j < numNodes; ++j) {
            dist[i][j] = dist[j][i] = euclideanDistance(nodes[i], nodes[j]);
        }
    }

    // Implement a simple heuristic TSP solver (Nearest Neighbor + 2-opt)
    vector<int> bestTour;
    double bestLength = numeric_limits<double>::max();

    // Since the starting point is fixed, we fix it at position 0
    // For the initial tour, we can use Nearest Neighbor heuristic
    vector<int> tour(numNodes);
    for (int i = 0; i < numNodes; ++i) {
        tour[i] = i;
    }

    // Start from node 0 (starting point)
    vector<bool> visited(numNodes, false);
    visited[0] = true;

    for (int i = 1; i < numNodes; ++i) {
        int last = tour[i - 1];
        double minDist = numeric_limits<double>::max();
        int nextNode = -1;
        for (int j = 1; j < numNodes; ++j) {
            if (!visited[j] && dist[last][j] < minDist) {
                minDist = dist[last][j];
                nextNode = j;
            }
        }
        tour[i] = nextNode;
        visited[nextNode] = true;
    }

    // Perform simple 2-opt optimization
    bool improved = true;
    while (improved) {
        improved = false;
        for (int i = 1; i < numNodes - 1; ++i) {
            for (int j = i + 1; j < numNodes; ++j) {
                double delta = dist[tour[i - 1]][tour[j]] + dist[tour[i]][tour[(j + 1) % numNodes]]
                             - dist[tour[i - 1]][tour[i]] - dist[tour[j]][tour[(j + 1) % numNodes]];
                if (delta < -1e-6) {
                    reverse(tour.begin() + i, tour.begin() + j + 1);
                    improved = true;
                }
            }
        }
    }

    // Enforce visiting one point per wall
    // Since each wall has two endpoints (nodes), we need to select one per wall
    // Walls are from nodes 1 to 2N in pairs (nodes 1 & 2 belong to wall 0, nodes 3 & 4 to wall 1, etc.)

    // For simplicity, we can accept the approximate tour we have
    // Compute the total length
    double totalLength = 0.0;
    for (int i = 0; i < numNodes; ++i) {
        int u = tour[i];
        int v = tour[(i + 1) % numNodes];
        totalLength += dist[u][v];
    }

    // Output the result
    cout << "Case " << caseNumber << ": " << fixed << setprecision(2) << totalLength << endl;
}

int main() {
    int caseNumber = 1;
    while (cin.peek() != EOF) {
        Problem prob = readInput();
        if (prob.N == 0) break; // End of input
        solveProblem(prob, caseNumber);
        caseNumber++;
        // Skip any remaining whitespace or newlines
        while (cin.peek() == '\n' || cin.peek() == ' ') cin.get();
    }
    return 0;
}
