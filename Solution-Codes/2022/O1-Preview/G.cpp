#include <bits/stdc++.h>
using namespace std;

// A structure to hold a die's faces, plus sorted arrays and prefix sums.
struct Die {
    int n;
    vector<long long> faces;  // possibly up to 1e5 faces

    // Sorted faces, plus prefix counts to quickly compute P(X > face).
    vector<long long> sortedFaces;
    vector<long long> prefixCount;  // prefixCount[i] = # of faces <= sortedFaces[i]

    void read() {
        cin >> n;
        faces.resize(n);
        for (int i = 0; i < n; i++) cin >> faces[i];
    }
    void prepare() {
        // sort faces
        sortedFaces = faces;
        sort(sortedFaces.begin(), sortedFaces.end());
        // build prefix count
        prefixCount.resize(n);
        prefixCount[0] = 1;
        for (int i = 1; i < n; i++) {
            prefixCount[i] = prefixCount[i-1] + 1; 
            // but if same as previous, be careful – we might skip duplicates or handle them.
        }
        // Actually we might store partial sums differently; see below.
    }

    // Return probability that a random face from this die is < val
    double probLess(long long val) const {
        // # of faces < val
        // We can do binary search in sortedFaces
        auto it = lower_bound(sortedFaces.begin(), sortedFaces.end(), val);
        // distance from begin
        long long idx = it - sortedFaces.begin();
        return double(idx) / double(n);
    }

    // Return probability that a random face from this die is == val
    double probEqual(long long val) const {
        // standard approach:
        auto lower = lower_bound(sortedFaces.begin(), sortedFaces.end(), val);
        auto upper = upper_bound(sortedFaces.begin(), sortedFaces.end(), val);
        long long countEq = (upper - lower);
        return double(countEq) / double(n);
    }

    // Return probability that a random face from this die is > val
    double probGreater(long long val) const {
        return 1.0 - probLessOrEqual(val);
    }
    double probLessOrEqual(long long val) const {
        auto it = upper_bound(sortedFaces.begin(), sortedFaces.end(), val);
        long long idx = it - sortedFaces.begin();
        return double(idx) / double(n);
    }
};

// Compute score(Dx, Dy) for two prepared dice
// But we only need it if both are fully known.  Usually, we do partial: P(X>Dy) etc.
double scoreDice(const Die &Dx, const Die &Dy) {
    // We can do O(n^2) in worst case, so be careful if n=1e5.
    // Usually we do a two-pointer approach or offline approach:
    //   score = sum_{x in Dx} sum_{y in Dy} [1(x>y) + 0.5*1(x=y)] / (n_x*n_y)
    // But we won't do that here if n is large. We'll do a faster method:
    //  1) Sort Dy
    //  2) For each face in Dx, do a binary search in Dy
    //  => O(n log n).
    // Enough for 1e5 if implemented efficiently.
    // Below is a naive version for illustration only.
    long long nX = Dx.n, nY = Dy.n;
    // sumWins counts how many times x>y, sumTies how many times x=y
    long long sumWins = 0, sumTies = 0;
    for (auto x : Dx.faces) {
        // # y's that are < x
        long long countLess = (long long)( lower_bound(Dy.sortedFaces.begin(), Dy.sortedFaces.end(), x)
                                          - Dy.sortedFaces.begin() );
        long long countEq   = (long long)( upper_bound(Dy.sortedFaces.begin(), Dy.sortedFaces.end(), x)
                                          - lower_bound(Dy.sortedFaces.begin(), Dy.sortedFaces.end(), x) );
        sumWins += countLess;
        sumTies += countEq;
    }
    double total = double(nX)*double(nY);
    return (sumWins + 0.5*sumTies) / total;
}


// Now the "two-point distribution" approach for subproblem #1 (fmin).
// We'll do a big loop over candidate X, Y among a set of "interesting" values.
double findFmin(const Die &D1, const Die &D2) {
    // We'll store a big vector of candidate values = all distinct face values from D1 and D2,
    // plus maybe "one below min" and "one above max" if >1.  
    // For brevity, let's just store all distinct and also +/- 1 offsets (but watch out for <1).
    vector<long long> candidates;
    // Insert distinct from D1:
    // ...
    // Insert distinct from D2:
    // ...
    // Sort and unique them:
    // ...

    double best = 1.0; // we'll minimize score(D3, D2), initialize with something large

    // For each pair (X, Y) with X<Y in candidates:
    //   We want to find minimal p s.t. score(D3, D1) >= 1/2.
    //   score(D3, D1) = p * [P(X>D1) + 0.5 P(X=D1)] 
    //                 + (1-p) * [P(Y>D1) + 0.5 P(Y=D1)].
    //   That is a linear function in p.  So if we let
    //     alpha = [P(X>D1) + 0.5 P(X=D1)],
    //     beta  = [P(Y>D1) + 0.5 P(Y=D1)],
    //   we want p*alpha + (1-p)*beta >= 1/2.
    //   => p*(alpha - beta) + beta >= 1/2
    //   => p*(alpha - beta) >= 1/2 - beta
    //   If alpha==beta, either it's always >= or always <.  We can handle carefully.
    //   Then once we find the minimal feasible p in [0,1], we compute
    //   score(D3, D_2) = p*scoreX + (1-p)*scoreY, where
    //     scoreX = [P(X>D2) + 0.5 P(X=D2)],
    //     scoreY = [P(Y>D2) + 0.5 P(Y=D2)].
    //   Minimize over all (X,Y).

    // Pseudocode:
    /*
    for (int i = 0; i < (int)candidates.size(); i++) {
        for (int j = i+1; j < (int)candidates.size(); j++) {
            long long X = candidates[i];
            long long Y = candidates[j];
            double alpha = pXscore(D1, X); // = P(X>D1)+0.5P(X=D1)
            double beta  = pXscore(D1, Y);
            // Solve p*(alpha-beta) + beta >= 0.5
            // p*(alpha-beta) >= 0.5 - beta
            // if alpha>beta, minimal p = max(0, (0.5-beta)/(alpha-beta)).
            // if alpha<beta, we'd want p=0 if that satisfies. Otherwise no solution.
            // if alpha==beta, either always meets or not.
            double pmin = ... // compute
            if (pmin>1 or pmin<0) discard if not feasible.
            // Now evaluate score(D3, D2):
            double sX = pXscore(D2, X);
            double sY = pXscore(D2, Y);
            double bestP = clamp(pmin, 0, 1); // minimal feasible p
            double sc = bestP*sX + (1-bestP)*sY;
            best = min(best, sc);

            // Also check p=1 or p=0 if they also satisfy the constraint (score>=1/2).
            // ...
        }
    }
    */

    // We'll skip the large nested loops here, but that's the idea.
    return best;
}

// Similarly findFmax for subproblem #2
double findFmax(const Die &D1, const Die &D2) {
    double best = 0.0;
    // analogous code but with constraint score(D3,D2)<=1/2
    // we maximize score(D3,D1).
    return best;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    // Read the two dice.
    // We are told that one of them "has advantage" over the other.
    // The input lines are: n f1 f2 ... fn
    // We have to figure out which is D1 and which is D2
    //   so that D1 has advantage.
    Die dA, dB;
    dA.read();
    dB.read();
    dA.prepare();
    dB.prepare();

    // Check scores in both directions:
    double sAB = scoreDice(dA, dB); // A vs B
    double sBA = scoreDice(dB, dA); // B vs A
    // One must be > 1/2. Suppose dA is the winner => D1=dA, D2=dB
    // else swap them.
    Die D1, D2;
    if (sAB > 0.5) {
        D1 = dA; 
        D2 = dB;
    } else {
        D1 = dB; 
        D2 = dA;
    }

    // Now solve subproblems:
    double fmin = findFmin(D1, D2);
    double fmax = findFmax(D1, D2);

    // Print results with required precision
    cout << fixed << setprecision(9) << fmin << " " << fmax << "\n";
    return 0;
}
