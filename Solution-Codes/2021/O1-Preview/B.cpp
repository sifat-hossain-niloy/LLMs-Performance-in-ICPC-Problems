#include <iostream>
#include <vector>
using namespace std;

int main() {
    int dx, dy, k;
    cin >> dx >> dy >> k;
    vector<vector<int>> cell(dx + 2, vector<int>(dy + 2, 2)); // 0: no molecule, 1: molecule, 2: unknown

    for (int w = 0; w < k; ++w) {
        int wx, wy, b;
        cin >> wx >> wy >> b;
        vector<pair<int, int>> boundaries(b);
        for (int i = 0; i < b; ++i) {
            int x, y;
            cin >> x >> y;
            boundaries[i] = {x, y};
            if (cell[x][y] == 0) {
                // Contradiction, but problem says data is consistent
            }
            cell[x][y] = 1;
        }
        for (auto [x, y] : boundaries) {
            int x_prev = x - wx;
            int y_prev = y - wy;
            if (x_prev >= 1 && x_prev <= dx && y_prev >= 1 && y_prev <= dy) {
                if (cell[x_prev][y_prev] == 1) {
                    // Contradiction
                }
                cell[x_prev][y_prev] = 0;
            }
        }
    }

    // Output minimal structure
    for (int y = 1; y <= dy; ++y) {
        for (int x = 1; x <= dx; ++x) {
            if (cell[x][y] == 2) cell[x][y] = 0;
            cout << (cell[x][y] == 1 ? '#' : '.');
        }
        cout << '\n';
    }

    cout << '\n';

    // Output maximal structure
    for (int y = 1; y <= dy; ++y) {
        for (int x = 1; x <= dx; ++x) {
            if (cell[x][y] == 2) cell[x][y] = 1;
            cout << (cell[x][y] == 1 ? '#' : '.');
        }
        cout << '\n';
    }

    return 0;
}
