#include <iostream>
#include <cstdio>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <algorithm>
#include <cstring>

using namespace std;

struct State {
    int x, y, dir;
    State(int _x, int _y, int _dir) : x(_x), y(_y), dir(_dir) {}
    bool operator<(const State& other) const {
        if (x != other.x) return x < other.x;
        if (y != other.y) return y < other.y;
        return dir < other.dir;
    }
};

int T;
int r, c, m, n;
char grid[105][105]; // grid[1..r][1..c]
int dx[4] = { -1, +1, +1, -1 }; // NE, SE, SW, NW
int dy[4] = { +1, +1, -1, -1 };
map<char, int> mirrorType; // '/' or '\'

int oppositeDir[4] = { 2, 3, 0, 1 }; // For cycle detection

bool simulate(int start_x, int start_y, int start_dir) {
    set<State> visited;
    int x = start_x, y = start_y, dir = start_dir;
    while (true) {
        State s(x, y, dir);
        if (visited.count(s)) {
            return false; // Loop detected
        }
        visited.insert(s);

        x += dx[dir];
        y += dy[dir];

        if (x == 0 || y == 0 || x == r + 1 || y == c + 1) {
            // Laser has exited the grid
            if (x == r + 1 && y == c + 1) {
                return true; // Laser reaches exit point
            } else {
                return false; // Laser exits elsewhere
            }
        }

        char mirror = grid[x][y];
        if (mirror == '/' || mirror == '\\') {
            // Update direction based on mirror
            if (mirror == '/') {
                if (dir == 0) dir = 2;
                else if (dir == 1) dir = 3;
                else if (dir == 2) dir = 0;
                else if (dir == 3) dir = 1;
            } else { // mirror == '\\'
                if (dir == 0) dir = 3;
                else if (dir == 1) dir = 2;
                else if (dir == 2) dir = 1;
                else if (dir == 3) dir = 0;
            }
        }
    }
}

int main() {
    int case_num = 1;
    while (scanf("%d %d %d %d", &r, &c, &m, &n) == 4) {
        memset(grid, 0, sizeof(grid));
        for (int i = 0; i < m; ++i) {
            int x, y;
            scanf("%d %d", &x, &y);
            grid[x][y] = '/';
        }
        for (int i = 0; i < n; ++i) {
            int x, y;
            scanf("%d %d", &x, &y);
            grid[x][y] = '\\';
        }

        bool canOpenWithoutMirror = simulate(0, 0, 1); // Start from (0,0) moving SE
        if (canOpenWithoutMirror) {
            printf("Case %d: 0\n", case_num++);
            continue;
        }

        vector<pair<int, int>> positions;
        for (int x = 1; x <= r; ++x) {
            for (int y = 1; y <= c; ++y) {
                if (grid[x][y] == 0) {
                    // Try inserting '/' mirror
                    grid[x][y] = '/';
                    if (simulate(0, 0, 1)) {
                        positions.emplace_back(x, y);
                    }
                    grid[x][y] = 0; // Remove mirror

                    // Try inserting '\' mirror
                    grid[x][y] = '\\';
                    if (simulate(0, 0, 1)) {
                        positions.emplace_back(x, y);
                    }
                    grid[x][y] = 0; // Remove mirror
                }
            }
        }

        if (positions.empty()) {
            printf("Case %d: impossible\n", case_num++);
        } else {
            sort(positions.begin(), positions.end());
            int k = positions.size();
            printf("Case %d: %d %d %d\n", case_num++, k, positions[0].first, positions[0].second);
        }
    }
    return 0;
}
