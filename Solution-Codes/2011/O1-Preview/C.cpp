#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstdio>

using namespace std;

const int MAX_H = 205;
const int MAX_W = 205;

int H, W;
int image[MAX_H][MAX_W];
bool visited[MAX_H][MAX_W];
int hole_count[MAX_H * MAX_W];
char hieroglyph_codes[MAX_H * MAX_W];

int dx[4] = {-1, 0, 1, 0}; // Directions: up, right, down, left
int dy[4] = {0, 1, 0, -1};

// Map number of holes to hieroglyph code
char hole_to_code(int holes) {
    switch (holes) {
        case 0: return 'W';
        case 1: return 'A';
        case 2: return 'K';
        case 3: return 'J';
        case 4: return 'S';
        case 5: return 'D';
        default: return '?';
    }
}

// Flood fill for background (white pixels connected to border)
void fill_background(int x, int y) {
    queue<pair<int, int>> q;
    q.push({x, y});
    visited[x][y] = true;
    while (!q.empty()) {
        int cx = q.front().first;
        int cy = q.front().second;
        q.pop();
        for (int dir = 0; dir < 4; ++dir) {
            int nx = cx + dx[dir];
            int ny = cy + dy[dir];
            if (nx >= 0 && nx < H && ny >= 0 && ny < W * 4 && !visited[nx][ny] && image[nx][ny] == 0) {
                visited[nx][ny] = true;
                q.push({nx, ny});
            }
        }
    }
}

// Flood fill for black regions (hieroglyphs)
void fill_black(int x, int y, int label) {
    queue<pair<int, int>> q;
    q.push({x, y});
    visited[x][y] = true;
    image[x][y] = label;
    while (!q.empty()) {
        int cx = q.front().first;
        int cy = q.front().second;
        q.pop();
        for (int dir = 0; dir < 4; ++dir) {
            int nx = cx + dx[dir];
            int ny = cy + dy[dir];
            if (nx >= 0 && nx < H && ny >= 0 && ny < W * 4 && !visited[nx][ny]) {
                if (image[nx][ny] == 1) {
                    visited[nx][ny] = true;
                    image[nx][ny] = label;
                    q.push({nx, ny});
                }
            }
        }
    }
}

// Flood fill for holes inside a hieroglyph
void fill_hole(int x, int y, int label) {
    queue<pair<int, int>> q;
    q.push({x, y});
    visited[x][y] = true;
    while (!q.empty()) {
        int cx = q.front().first;
        int cy = q.front().second;
        q.pop();
        for (int dir = 0; dir < 4; ++dir) {
            int nx = cx + dx[dir];
            int ny = cy + dy[dir];
            if (nx >= 0 && nx < H && ny >= 0 && ny < W * 4 && !visited[nx][ny]) {
                if (image[nx][ny] == 0) {
                    visited[nx][ny] = true;
                    q.push({nx, ny});
                }
            }
        }
    }
    hole_count[label - 2]++;
}

int main() {
    int case_num = 1;
    while (cin >> H >> W) {
        if (H == 0 && W == 0)
            break;
        // Clear data
        memset(image, 0, sizeof(image));
        memset(visited, false, sizeof(visited));
        memset(hole_count, 0, sizeof(hole_count));
        vector<char> result_codes;

        // Read image data
        for (int i = 0; i < H; ++i) {
            string line;
            cin >> line;
            int idx = 0;
            for (char ch : line) {
                int val;
                if (isdigit(ch))
                    val = ch - '0';
                else
                    val = ch - 'a' + 10;
                // Convert to 4 bits
                for (int k = 3; k >= 0; --k) {
                    image[i][idx++] = (val >> k) & 1;
                }
            }
        }

        // Mark the background
        for (int i = 0; i < H; ++i) {
            if (!visited[i][0] && image[i][0] == 0)
                fill_background(i, 0);
            if (!visited[i][W * 4 - 1] && image[i][W * 4 - 1] == 0)
                fill_background(i, W * 4 - 1);
        }
        for (int j = 0; j < W * 4; ++j) {
            if (!visited[0][j] && image[0][j] == 0)
                fill_background(0, j);
            if (!visited[H - 1][j] && image[H - 1][j] == 0)
                fill_background(H - 1, j);
        }

        // Label black regions (hieroglyphs)
        int label = 2;
        for (int i = 0; i < H; ++i) {
            for (int j = 0; j < W * 4; ++j) {
                if (!visited[i][j] && image[i][j] == 1) {
                    fill_black(i, j, label++);
                }
            }
        }

        // Reset visited array for hole detection
        memset(visited, false, sizeof(visited));

        // Count holes within each hieroglyph
        for (int i = 0; i < H; ++i) {
            for (int j = 0; j < W * 4; ++j) {
                if (!visited[i][j] && image[i][j] == 0) {
                    // Check adjacent pixels to find the enclosing hieroglyph label
                    int parent_label = -1;
                    for (int dir = 0; dir < 4; ++dir) {
                        int ni = i + dx[dir];
                        int nj = j + dy[dir];
                        if (ni >= 0 && ni < H && nj >= 0 && nj < W * 4) {
                            if (image[ni][nj] >= 2) {
                                parent_label = image[ni][nj];
                                break;
                            }
                        }
                    }
                    if (parent_label != -1) {
                        fill_hole(i, j, parent_label);
                    } else {
                        // This should not happen if background is correctly marked
                    }
                }
            }
        }

        // Map hole counts to hieroglyph codes and collect results
        for (int l = 2; l < label; ++l) {
            char code = hole_to_code(hole_count[l - 2]);
            result_codes.push_back(code);
        }

        // Sort and print the results
        sort(result_codes.begin(), result_codes.end());
        cout << "Case " << case_num++ << ": ";
        for (char c : result_codes)
            cout << c;
        cout << endl;
    }
    return 0;
}
