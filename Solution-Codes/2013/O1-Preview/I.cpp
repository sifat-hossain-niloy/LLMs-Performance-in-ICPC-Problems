#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <climits>

using namespace std;

const int MAXN = 505;
const int LOGN = 10;

long long pond[MAXN][MAXN];
long long st[MAXN][MAXN][LOGN][LOGN];
int m, n;

void buildSparseTable(int m, int n) {
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            st[i][j][0][0] = pond[i][j];

    for (int x = 0; (1 << x) <= m; x++) {
        for (int y = 0; (1 << y) <= n; y++) {
            if (x == 0 && y == 0) continue;
            for (int i = 0; i + (1 << x) <= m; i++) {
                for (int j = 0; j + (1 << y) <= n; j++) {
                    if (x == 0) {
                        st[i][j][x][y] = max(st[i][j][x][y - 1], st[i][j + (1 << (y - 1))][x][y - 1]);
                    } else if (y == 0) {
                        st[i][j][x][y] = max(st[i][j][x - 1][y], st[i + (1 << (x - 1))][j][x - 1][y]);
                    } else {
                        long long m1 = max(st[i][j][x - 1][y - 1], st[i + (1 << (x - 1))][j][x - 1][y - 1]);
                        long long m2 = max(st[i][j + (1 << (y - 1))][x - 1][y - 1], st[i + (1 << (x - 1))][j + (1 << (y - 1))][x - 1][y - 1]);
                        st[i][j][x][y] = max(m1, m2);
                    }
                }
            }
        }
    }
}

long long query(int x1, int y1, int x2, int y2) {
    int kx = log2(x2 - x1 + 1);
    int ky = log2(y2 - y1 + 1);
    long long m1 = max(st[x1][y1][kx][ky], st[x2 - (1 << kx) + 1][y1][kx][ky]);
    long long m2 = max(st[x1][y2 - (1 << ky) + 1][kx][ky], st[x2 - (1 << kx) + 1][y2 - (1 << ky) + 1][kx][ky]);
    return max(m1, m2);
}

int main() {
    int a, b;
    cin >> a >> b >> m >> n;

    long long V_water_initial = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            cin >> pond[i][j];
            V_water_initial += pond[i][j];
        }
    }

    buildSparseTable(m, n);

    long long area_pond = (long long)m * n;
    long long max_volume = 0;

    for (int l = a; l >= 1; l--) {
        if (l > m) continue;
        for (int w = b; w >= 1; w--) {
            if (w > n) continue;

            long long denominator = (long long)l * w - area_pond;
            for (int i = 0; i + l <= m; i++) {
                for (int j = 0; j + w <= n; j++) {
                    long long D = query(i, j, i + l - 1, j + w - 1);

                    if (denominator == 0) {
                        if (D * area_pond < V_water_initial) {
                            long long h_max = 1000000000LL; // Set to a large value
                            long long volume = (long long)l * w * h_max;
                            if (volume > max_volume) {
                                max_volume = volume;
                            }
                        } else {
                            continue;
                        }
                    } else {
                        long long numerator = D * area_pond - V_water_initial;
                        long long h_max = numerator / denominator;
                        if (h_max >= 1) {
                            long long volume = (long long)l * w * h_max;
                            if (volume > max_volume) {
                                max_volume = volume;
                            }
                        }
                    }
                }
            }
        }
    }

    cout << max_volume << endl;

    return 0;
}
