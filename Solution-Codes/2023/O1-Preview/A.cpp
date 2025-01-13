#include <bits/stdc++.h>
using namespace std;

/*
  This illustrates the solving step *after* you've already
  asked queries and read back the 5 answers R1..R5.

  In a real interactive solution:
   1. You would print your 5 questions.
   2. For each question, read the integer answer from input.
   3. Then call the solve() logic to deduce x, y, z.
*/

// Uncomment for interactive platforms as needed:
// int main() {
//     // We'll do the ask here, but typically you'd do:
//     // cout << "1 0 0" << endl; flush output
//     // cin >> R1;
//     // ...
//     // Then call solve(R1, R2, R3, R4, R5);
//     // return 0;
// }

struct Triple {
    long long x, y, z;
};

// Helper: check nonnegative + within some large bound (e.g. 1e9 or 1e6).
bool validXYZ(long long x, long long y, long long z) {
    if (x < 0 || y < 0 || z < 0) return false;
    // problem statement suggests up to 1e5 or so is possible.
    // We can be generous:
    if (x > 1000000 || y > 1000000 || z > 1000000) return false;
    return true;
}

// We try to see if (x,y,z) from "the 4 truths" indeed
// contradicts the "lieEquation". Depending on the index of
// the lieEquation, we compare.
bool contradictsLie(int lieIdx, long long x, long long y, long long z,
                    long long R1, long long R2, long long R3,
                    long long R4, long long R5) {
    switch (lieIdx) {
      case 1: return (x == R1); // we WANT a contradiction, so if x == R1, it fails
      case 2: return (y == R2);
      case 3: return (z == R3);
      case 4: return (x + y + z == R4);
      case 5: return (2*x + 3*y + 5*z == R5);
    }
    return false; // should never happen
}

// Solve given 5 answers R1..R5 from queries:
//   Q1: (1,0,0)
//   Q2: (0,1,0)
//   Q3: (0,0,1)
//   Q4: (1,1,1)
//   Q5: (2,3,5)
// Exactly one of R1..R5 is a lie, figure out x,y,z.
Triple solveOnce(long long R1, long long R2, long long R3,
                 long long R4, long long R5)
{
    // We'll store the final answer here:
    Triple result{-1, -1, -1};

    // We try each possibility "i" that could be the lie:
    for (int i = 1; i <= 5; i++) {
        // We'll solve for x,y,z using the other 4 equations.

        // We'll do it by cases for simplicity:

        // If i = 1 is the lie, then we trust R2, R3, R4, R5
        // from which: y=R2, z=R3, x=R4 - y - z => check eqn5.
        if (i == 1) {
            long long y = R2;
            long long z = R3;
            long long x = R4 - y - z;
            if (!validXYZ(x,y,z)) continue;
            // Now check eqn5 must hold: 2x + 3y + 5z == R5
            if (2*x + 3*y + 5*z == R5) {
                // also check that it *contradicts* eqn1
                // i.e. x != R1
                if (x != R1) {
                    // We found a consistent solution
                    result = {x,y,z};
                    break;
                }
            }
        }

        // If i = 2 is the lie, then trust R1, R3, R4, R5
        // => x=R1, z=R3, y=R4 - x - z => check eqn5
        else if (i == 2) {
            long long x = R1;
            long long z = R3;
            long long y = R4 - x - z;
            if (!validXYZ(x,y,z)) continue;
            if (2*x + 3*y + 5*z == R5) {
                if (y != R2) {
                    result = {x,y,z};
                    break;
                }
            }
        }

        // If i = 3 is the lie, then trust R1, R2, R4, R5
        // => x=R1, y=R2, z=R4 - x - y => check eqn5
        else if (i == 3) {
            long long x = R1;
            long long y = R2;
            long long z = R4 - x - y;
            if (!validXYZ(x,y,z)) continue;
            if (2*x + 3*y + 5*z == R5) {
                if (z != R3) {
                    result = {x,y,z};
                    break;
                }
            }
        }

        // If i = 4 is the lie, then trust R1, R2, R3, R5
        // => x=R1, y=R2, z=R3, check eqn5 => 2x + 3y + 5z == R5
        // => if that holds, check that (x+y+z) != R4
        else if (i == 4) {
            long long x = R1;
            long long y = R2;
            long long z = R3;
            if (!validXYZ(x,y,z)) continue;
            if (2*x + 3*y + 5*z == R5) {
                if ((x + y + z) != R4) {
                    result = {x,y,z};
                    break;
                }
            }
        }

        // If i = 5 is the lie, then trust R1, R2, R3, R4
        // => x=R1, y=R2, z=R3, check eqn4 => x+y+z == R4
        // => if that holds, check that eqn5 is contradicted
        else {
            long long x = R1;
            long long y = R2;
            long long z = R3;
            if (!validXYZ(x,y,z)) continue;
            if ((x + y + z) == R4) {
                // must contradict eqn5 => 2x+3y+5z != R5
                if (2*x + 3*y + 5*z != R5) {
                    result = {x,y,z};
                    break;
                }
            }
        }
    }

    return result;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // In an actual interactive problem, you would:
    // 1) Print your 5 queries:
    //    cout << "1 0 0\n" << flush;
    //    cin >> R1;
    //    ... etc ...
    // Here, we'll just assume we've read them.

    // Suppose we've read these 5 answers from the sphinx:
    long long R1, R2, R3, R4, R5;
    cin >> R1 >> R2 >> R3 >> R4 >> R5;

    // Now solve:
    Triple ans = solveOnce(R1, R2, R3, R4, R5);

    // Output the solution:
    cout << ans.x << " " << ans.y << " " << ans.z << "\n";
    return 0;
}
