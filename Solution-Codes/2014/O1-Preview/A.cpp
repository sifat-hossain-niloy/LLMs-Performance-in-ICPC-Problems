#include <iostream>
using namespace std;

int main() {
    int n;
    cin >> n;

    int t = -1; // Starting target position
    for (int i = 0; i < n; ++i) {
        int f;
        if (i % 2 == 0) {
            f = 2 * (n - i) - 2; // Even iteration: move from the end
        } else {
            f = 3 + 3 * (i / 2); // Odd iteration: move from the start
        }
        cout << f << " to " << t << endl;
        t += 3; // Move target position for the next iteration
    }

    return 0;
}
