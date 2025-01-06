#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <cstring>
using namespace std;

const double g = 9.80665;
const double EPS = 1e-8;

struct Building {
    int x, y;
    double h;
    int id;
};

int dx, dy, w, v, lx, ly;
double heights[25][25];
int N;
Building buildings[500];
vector<int> adj[500];
int dist[500];

bool canJump(int i, int j) {
    if (i == j) return true; // same building
    Building &A = buildings[i];
    Building &B = buildings[j];
    double x0 = A.x * w + w / 2.0;
    double y0 = A.y * w + w / 2.0;
    double z0 = A.h;
    double x1 = B.x * w + w / 2.0;
    double y1 = B.y * w + w / 2.0;
    double z1 = B.h;
    double D = hypot(x1 - x0, y1 - y0);
    double deltaZ = z1 - z0;
    double K = (g * D * D) / (2 * v * v);
    double Dsq = D * D;
    double discriminant = Dsq - 4 * K * (K + deltaZ);
    if (discriminant < -EPS) return false; // No real solution
    double sqrtD = sqrt(max(0.0, discriminant));
    double tanTheta1 = (D + sqrtD) / (2 * K);
    double tanTheta2 = (D - sqrtD) / (2 * K);
    bool canReach = false;
    vector<double> tanThetas = { tanTheta1, tanTheta2 };
    for (double tanTheta : tanThetas) {
        double theta = atan(tanTheta);
        double cosTheta = cos(theta);
        double sinTheta = sin(theta);
        if (cosTheta <= 0) continue;
        double vh = v * cosTheta;
        double vv = v * sinTheta;
        double t_total = D / vh;
        // Line traversal to get buildings under the trajectory
        int x_grid0 = int(x0 / w);
        int y_grid0 = int(y0 / w);
        int x_grid1 = int(x1 / w);
        int y_grid1 = int(y1 / w);
        int x0_int = x_grid0, y0_int = y_grid0;
        int x1_int = x_grid1, y1_int = y_grid1;
        int dx_line = abs(x1_int - x0_int), sx = x0_int < x1_int ? 1 : -1;
        int dy_line = -abs(y1_int - y0_int), sy = y0_int < y1_int ? 1 : -1;
        int err = dx_line + dy_line, e2;
        int x = x0_int, y = y0_int;
        bool collision = false;
        while (true) {
            int grid_idx = y * dx + x;
            if (grid_idx < 0 || grid_idx >= N || (x < 0 || x >= dx || y < 0 || y >= dy)) break;
            int c = y * dx + x;
            if (c != i && c != j) {
                // Compute t when over this building
                double xc = x * w + w / 2.0;
                double yc = y * w + w / 2.0;
                double dx_c = xc - x0;
                double dy_c = yc - y0;
                double D_c = sqrt(dx_c * dx_c + dy_c * dy_c);
                double t_c = D_c / vh;
                double z_c = z0 + vv * t_c - 0.5 * g * t_c * t_c;
                if (z_c <= heights[y][x] + EPS) {
                    collision = true;
                    break;
                }
            }
            if (x == x1_int && y == y1_int) break;
            e2 = 2 * err;
            if (e2 >= dy_line) { err += dy_line; x += sx; }
            if (e2 <= dx_line) { err += dx_line; y += sy; }
        }
        if (!collision) {
            canReach = true;
            break;
        }
    }
    return canReach;
}

int main() {
    cin >> dx >> dy >> w >> v >> lx >> ly;
    for (int j = 0; j < dy; ++j) {
        for (int i = 0; i < dx; ++i) {
            cin >> heights[j][i];
        }
    }
    N = dx * dy;
    int start_id = -1;
    int idx = 0;
    for (int y = 0; y < dy; ++y) {
        for (int x = 0; x < dx; ++x) {
            buildings[idx] = { x, y, heights[y][x], idx };
            if (x + 1 == lx && y + 1 == ly) {
                start_id = idx;
            }
            ++idx;
        }
    }
    // Build adjacency list
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (canJump(i, j)) {
                adj[i].push_back(j);
            }
        }
    }
    memset(dist, -1, sizeof(dist));
    queue<int> q;
    q.push(start_id);
    dist[start_id] = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v_id : adj[u]) {
            if (dist[v_id] == -1) {
                dist[v_id] = dist[u] + 1;
                q.push(v_id);
            }
        }
    }
    // Output results
    for (int j = 0; j < dy; ++j) {
        for (int i = 0; i < dx; ++i) {
            int idx = j * dx + i;
            if (dist[idx] == -1) {
                cout << "X";
            } else {
                cout << dist[idx];
            }
            if (i != dx - 1) cout << " ";
        }
        cout << endl;
    }
    return 0;
}
