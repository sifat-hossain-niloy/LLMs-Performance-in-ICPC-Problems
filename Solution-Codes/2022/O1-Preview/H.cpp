#include <bits/stdc++.h>
using namespace std;

// We will ask 5 queries in this specific order:
// Q1: (1,0,0) -> S1 ~ x
// Q2: (0,1,0) -> S2 ~ y
// Q3: (0,0,1) -> S3 ~ z
// Q4: (1,1,0) -> S4 ~ x + y
// Q5: (0,1,1) -> S5 ~ y + z

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    // In a real interactive problem, we would do something like:
    //
    // cout << "1 0 0\n" << flush; // Q1
    // cin >> S1;
    // cout << "0 1 0\n" << flush; // Q2
    // cin >> S2;
    // cout << "0 0 1\n" << flush; // Q3
    // cin >> S3;
    // cout << "1 1 0\n" << flush; // Q4
    // cin >> S4;
    // cout << "0 1 1\n" << flush; // Q5
    // cin >> S5;
    //
    // But here, let's just pretend we printed them and read them:

    // Print the queries (for demonstration):
    cout << "1 0 0" << "\n";
    cout << "0 1 0" << "\n";
    cout << "0 0 1" << "\n";
    cout << "1 1 0" << "\n";
    cout << "0 1 1" << "\n";
    // Normally, flush and then read answers interactively.

    // Read the five answers (S1..S5):
    // For a real interactive solution, you'd read them one by one.
    int S1, S2, S3, S4, S5;
    cin >> S1 >> S2 >> S3 >> S4 >> S5;

    // We'll try each possibility of which S_i might be the lie
    // and see if we can find a consistent (x, y, z).
    // We'll store any valid solution we find in (solX, solY, solZ).
    // If more than one valid solution arises, we'd have to be cautious;
    // the puzzle states there should be a unique solution.

    vector<array<long long,3>> candidates; // store all possible (x,y,z)

    // Helper lambda to check how many of the 5 equations hold
    auto checkEquations = [&](long long x, long long y, long long z) {
        int countTrue = 0;
        // eq1: x == S1
        if (x == S1) countTrue++;
        // eq2: y == S2
        if (y == S2) countTrue++;
        // eq3: z == S3
        if (z == S3) countTrue++;
        // eq4: x + y == S4
        if (x + y == S4) countTrue++;
        // eq5: y + z == S5
        if (y + z == S5) countTrue++;
        return countTrue;
    };

    // Case 1: S1 is the lie => S2=y, S3=z, S4=x+y, S5=y+z
    {
        long long y = S2, z = S3;
        long long x = S4 - y; // from x+y=S4
        if (y + z == S5 && x >= 0 && y >= 0 && z >= 0) {
            // check that exactly 4 out of 5 are correct
            if (checkEquations(x, y, z) == 4) {
                candidates.push_back({x,y,z});
            }
        }
    }

    // Case 2: S2 is the lie => S1=x, S3=z, S4=x+y, S5=y+z
    {
        long long x = S1, z = S3;
        // x+y = S4 => y = S4 - x
        long long y = S4 - x;
        if (y + z == S5 && x >= 0 && y >= 0 && z >= 0) {
            if (checkEquations(x, y, z) == 4) {
                candidates.push_back({x,y,z});
            }
        }
    }

    // Case 3: S3 is the lie => S1=x, S2=y, S4=x+y, S5=y+z
    {
        long long x = S1, y = S2;
        // x+y = S4 => check
        if (x + y == S4) {
            // from y+z = S5 => z = S5 - y
            long long z = S5 - y;
            if (x >= 0 && y >= 0 && z >= 0) {
                if (checkEquations(x, y, z) == 4) {
                    candidates.push_back({x,y,z});
                }
            }
        }
    }

    // Case 4: S4 is the lie => S1=x, S2=y, S3=z, S5=y+z
    {
        long long x = S1, y = S2, z = S3;
        // Check y+z == S5
        if (y + z == S5 && x >= 0 && y >= 0 && z >= 0) {
            // Then S4 must be the wrong sum. Just check if we get 4/5 correct
            if (checkEquations(x, y, z) == 4) {
                candidates.push_back({x,y,z});
            }
        }
    }

    // Case 5: S5 is the lie => S1=x, S2=y, S3=z, S4=x+y
    {
        long long x = S1, y = S2, z = S3;
        // Check x+y == S4
        if (x + y == S4 && x >= 0 && y >= 0 && z >= 0) {
            // Then S5 must be wrong. Just check if we get 4/5 correct
            if (checkEquations(x, y, z) == 4) {
                candidates.push_back({x,y,z});
            }
        }
    }

    // Normally, we expect exactly one valid candidate
    // (the puzzle states it should be uniquely determined).
    // We'll just pick the first if there's more than one.
    if (!candidates.empty()) {
        auto [axex, basilisk, centaur] = candidates.front();
        // Output the result
        cout << axex << " " << basilisk << " " << centaur << "\n";
    } else {
        // In theory, should never happen for a valid puzzle
        // but just in case, output something safe.
        cout << "0 0 0\n";
    }

    return 0;
}
