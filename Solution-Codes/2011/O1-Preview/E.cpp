#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

const int MAX_DX = 1000;
const int MAX_DY = 1000;
const int MAX_N = 500000;
const int SHIFT = 1000; // To make y' non-negative
const int MAX_XP = MAX_DX + MAX_DY + 2; // x' ranges from 2 to 2000
const int MAX_YP = 2 * MAX_DX + SHIFT + 2; // y' ranges from -999+SHIFT to 999+SHIFT

int dx, dy, n, q;
vector<pair<int, int>> coffee_shops;
vector<int> queries;
int diff[MAX_XP][MAX_YP];
long long cum[MAX_XP][MAX_YP];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int case_num = 0;
    while (cin >> dx >> dy >> n >> q) {
        if (dx == 0 && dy == 0 && n == 0 && q == 0) break;
        case_num++;
        coffee_shops.clear();
        queries.clear();

        for (int i = 0; i < n; i++) {
            int xi, yi;
            cin >> xi >> yi;
            coffee_shops.emplace_back(xi, yi);
        }
        for (int i = 0; i < q; i++) {
            int m;
            cin >> m;
            queries.push_back(m);
        }

        cout << "Case " << case_num << ":\n";

        for (int m_idx = 0; m_idx < q; m_idx++) {
            int m = queries[m_idx];

            memset(diff, 0, sizeof(diff));
            memset(cum, 0, sizeof(cum));

            // Process each coffee shop
            for (const auto& shop : coffee_shops) {
                int x = shop.first;
                int y = shop.second;

                int xp = x + y;
                int yp = x - y + SHIFT;

                int x1 = max(0, xp - m);
                int x2 = min(MAX_XP - 2, xp + m);
                int y1 = max(0, yp - m);
                int y2 = min(MAX_YP - 2, yp + m);

                diff[x1][y1] += 1;
                diff[x1][y2 + 1] -=1;
                diff[x2 + 1][y1] -=1;
                diff[x2 + 1][y2 + 1] +=1;
            }

            // Compute 2D prefix sums
            for (int i = 0; i < MAX_XP; i++) {
                for (int j = 0; j < MAX_YP; j++) {
                    if (i > 0) cum[i][j] += cum[i - 1][j];
                    if (j > 0) cum[i][j] += cum[i][j - 1];
                    if (i > 0 && j > 0) cum[i][j] -= cum[i - 1][j - 1];
                    cum[i][j] += diff[i][j];
                }
            }

            // Find the position with the maximum count
            long long max_count = -1;
            int opt_x = -1, opt_y = -1;

            for (int i = 0; i < MAX_XP; i++) {
                for (int j = 0; j < MAX_YP; j++) {
                    long long count = cum[i][j];
                    if (count >= max_count) {
                        // Map back to original coordinates
                        int x_prime = i;
                        int y_prime = j;

                        int x = (x_prime + y_prime - SHIFT) / 2;
                        int y = (x_prime - y_prime + SHIFT) / 2;

                        // Check if x and y are integers and within bounds
                        if ((x_prime + y_prime - SHIFT) % 2 != 0) continue;
                        if ((x_prime - y_prime + SHIFT) % 2 != 0) continue;
                        if (x < 1 || x > dx || y < 1 || y > dy) continue;

                        if (count > max_count) {
                            max_count = count;
                            opt_x = x;
                            opt_y = y;
                        } else if (count == max_count) {
                            if (y < opt_y || (y == opt_y && x < opt_x)) {
                                opt_x = x;
                                opt_y = y;
                            }
                        }
                    }
                }
            }

            cout << max_count << " (" << opt_x << "," << opt_y << ")\n";
        }
    }

    return 0;
}
