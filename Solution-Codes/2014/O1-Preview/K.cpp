#include <iostream>
using namespace std;

int main() {
    int n;
    cin >> n;
    long long total = 0;
    for (int i = 0; i < n; ++i) {
        long long k;
        cin >> k;
        if (k % 2 == 0) {
            // Even number of invitations, person invites no one
            total += 0;
        } else {
            // Odd number of invitations, person invites k + 1 people
            total += k + 1;
        }
    }
    cout << total << endl;
    return 0;
}
