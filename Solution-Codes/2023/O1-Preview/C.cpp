#include <bits/stdc++.h>
using namespace std;

/*
   Helper functions to compute:
     countLess(D, x)  = # of faces in D strictly less than x
     countEqual(D, x) = # of faces in D exactly equal to x

   We'll store D in sorted order.  Then:
     countLess(D, x)  = index of first element >= x
     countEqual(D, x) = index of first element > x  -  index of first element >= x
*/
struct Die {
    int n;
    vector<long long> faces;  // sorted
};

long long countLess(const Die &D, long long x) {
    // number of elements in D.faces that are < x
    // can use std::lower_bound
    return (long long)(std::lower_bound(D.faces.begin(), D.faces.end(), x) - D.faces.begin());
}

long long countEqual(const Die &D, long long x) {
    // # of elements == x
    auto start = std::lower_bound(D.faces.begin(), D.faces.end(), x);
    auto end   = std::upper_bound(D.faces.begin(), D.faces.end(), x);
    return (long long)(end - start);
}

// score( {c}, D ) = ( #less(D,c) + 0.5 * #equal(D,c) ) / D.n
long double scoreSingleFaceVsDie(long long c, const Die &D) {
    long long lessCount  = countLess(D, c);
    long long equalCount = countEqual(D, c);
    long double val = (long double)lessCount 
                      + 0.5L * (long double)equalCount;
    val /= (long double)D.n;
    return val;
}

// We'll do a binary-search to find the smallest c >= 1 s.t. score({c},D) >= 0.5
// We'll assume c up to max(D)+1 is enough.
long long findSmallestC_forScoreAtLeastHalf(const Die &D) {
    long long lo = 1;
    long long hi = D.faces.back() + 1;  // c = maxFace(D)+1 definitely yields score=1
    while (lo < hi) {
        long long mid = (lo + hi) >> 1;
        long double sc = scoreSingleFaceVsDie(mid, D);
        if (sc >= 0.5L) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo; // smallest c
}

// We'll do a binary-search to find the largest c >= 1 s.t. score({c},D) <= 0.5
// If c is too large, the score is close to 1, so we shrink c.
long long findLargestC_forScoreAtMostHalf(const Die &D) {
    long long lo = 1;
    long long hi = D.faces.back() + 1;  // for c > maxFace(D), score=1, so won't be <=0.5
    // But it's possible no c in [1..max+1] satisfies. We'll handle that carefully.
    // We'll do a "reverse" binary search:
    while (lo < hi) {
        long long mid = (lo + hi + 1) >> 1;  // upper mid
        long double sc = scoreSingleFaceVsDie(mid, D);
        if (sc <= 0.5L) {
            // mid is feasible, try bigger
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    // lo might be 1 if nothing bigger is feasible
    // It's possible that even c=1 has score({1},D) > 0.5 => then lo=0 in logic, but we clamped c>=1
    // We'll check feasibility at the end:
    if (scoreSingleFaceVsDie(lo, D) > 0.5L) {
        // meaning no integer >=1 satisfies score <= 0.5
        // then effectively there's "no valid c", but we must return lo=0 or 1?
        // We'll return 0 to indicate "no feasible c" (the caller can check).
        return 0;
    }
    return lo;
}

// A function to compute the pair of dice-scores we need:
//
//   1) min( score(D3, D2) ) over all D3 s.t. score(D3, D1) >= 0.5
//      -> single-face approach: pick c1 = smallest c s.t. score({c}, D1) >= 0.5
//         then compute score({c1}, D2).
//
//   2) max( score(D3, D1) ) over all D3 s.t. score(D2, D3) >= 0.5
//      -> equivalently score(D3, D2) <= 0.5
//         single-face approach: pick c2 = largest c s.t. score({c}, D2) <= 0.5
//         then compute score({c2}, D1).
//
// We'll output (lowestScore, highestScore).
//
pair<long double, long double> solveIntransitive(const Die &D1, const Die &D2) {
    // 1) find c1
    long long c1 = findSmallestC_forScoreAtLeastHalf(D1);
    long double lowestScore;
    {
        long double sc = scoreSingleFaceVsDie(c1, D2);
        lowestScore = sc;
    }
    // 2) find c2
    long long c2 = findLargestC_forScoreAtMostHalf(D2);
    long double highestScore = 0.0L;
    if (c2 > 0) {
        // c2=0 means no feasible single-face die
        long double sc = scoreSingleFaceVsDie(c2, D1);
        highestScore = sc;
    }
    return make_pair(lowestScore, highestScore);
}

// A function to compute score(D1, D2) quickly (for deciding which is advantage).
long double scoreDiceVsDice(const Die &A, const Die &B) {
    // naive O(nA*nB) is too big (10^10) for n=10^5
    // Instead, we use a two-pointer or prefix-sums approach.
    // Sort A, B => already sorted.
    // We'll count pairs (a,b) with a<b, a=b, a>b in O(nA + nB).
    // Then final score = (#(a>b) + 0.5 * #(a=b)) / (nA*nB).
    // Let i, j be indices into A,B.
    long long i=0, j=0;
    long long countLess=0;   // # of pairs (a,b) with a<b
    long long countEqual=0;  // # of pairs (a,b) with a=b
    long long nA = A.n, nB = B.n;
    while (i<nA && j<nB) {
        if (A.faces[i] < B.faces[j]) {
            // a < b => all b's at j => that contributes (nB - j) ??? Actually we want pairs a<b, so:
            // Wait, we want to move in sorted order.  We'll do it systematically:
            // If A[i] < B[j], then for that a, it is < B[j], B[j+1], ... ??? Actually no, we only can increment i or j carefully.
            // Easiest is the standard "merge" trick to count how many B's are less/ equal/ bigger.
            // A[i] < B[j], so all B[k] with k >= j => A[i] < B[k]? Not necessarily for k>j, B[k] could be bigger than B[j].
            // Actually simpler approach:
            // - For each A[i], find how many B are strictly less, how many are equal, how many are bigger. Then multiply by that face's count?? But we have huge n.
            // We'll do 2-pointer counting:
            //   If A[i] < B[j], then A[i] < B[j], B[j+1],..., B[nB-1]? Actually that means A[i] < B[j] and also < B[j+1] if B[j+1]>=B[j]. But we don't want to assume it's strictly sorted or something. It's simpler to do the well-known 2-pointer approach for "count inversions" style:
            // Let’s adapt a standard approach:
            //   We'll keep a pointer j into B.  For each i in A, we move j while B[j] <= A[i], etc.
            // Actually let's do a direct approach for "score(A,B) = sum_{a in A} sum_{b in B} [1 if a>b, 0.5 if a=b, 0 else] / (nA*nB)". We'll count # of (a,b) with a>b, # with a=b, # with a<b in O(nA + nB).
            //   - We'll iterate from smallest to largest among both sets.  For consecutive runs of equal values, we do the cross product.
            
            // Let's do it more straightforwardly:
            // We'll group A's identical faces into (value, countA) and B's identical faces into (value, countB). Then we merge these lists.
            // That approach is simpler to implement. Let's break out of this code for now and do it outside with "compressed" arrays.
            
            // For simplicity, let's do it outside to avoid confusion.
        }
        // We'll do it in a more direct function after we "compress" (value -> frequency).
    }
    return 0.0L; // placeholder
}

// We'll implement a function to compress a die's sorted faces into (val, freq) pairs
vector<pair<long long, long long>> compressFaces(const Die &D) {
    vector<pair<long long,long long>> comp;
    for (int i=0; i<D.n; ) {
        long long v = D.faces[i];
        long long cnt = 0;
        while (i<D.n && D.faces[i]==v) {
            i++; cnt++;
        }
        comp.push_back({v, cnt});
    }
    return comp;
}

// Now a function to compute score(A,B) in O(nA + nB) if A,B are compressed
long double computeScore(const vector<pair<long long,long long>> &A,
                         const vector<pair<long long,long long>> &B,
                         long long nA, long long nB)
{
    // We'll keep pointers i,j. Let A[i].first = aVal, A[i].second = freqA
    // similarly B[j].first = bVal, B[j].second = freqB.
    // We'll move in ascending order of aVal,bVal.
    // We'll maintain a running total of how many B-values are strictly less or equal to the current aVal as we go.
    long long sumPairsGreater = 0; // count of (a,b) with a>b
    long long sumPairsEqual   = 0; // count of (a,b) with a=b
    
    int i=0, j=0;
    int NA = (int)A.size(), NB = (int)B.size();
    
    // We'll also keep track of how many B-faces we've passed so far
    // to know # of bVal < current aVal.
    long long bCountSoFar = 0;      // sum of freqB for bVal < current aVal
    long long bCountSoFarEq = 0;    // freq of bVal == current aVal (we'll handle that carefully)
    
    while (i<NA && j<NB) {
        long long aVal = A[i].first;
        long long bVal = B[j].first;
        long long freqA = A[i].second;
        long long freqB = B[j].second;
        if (aVal < bVal) {
            // all these freqA faces are < bVal, so for them bVal is bigger
            // meaning aVal < bVal => aVal can't equal or exceed bVal
            // so # pairs (a,b) with a>b is freqA * bCountSoFar
            // # pairs (a,b) with a=b is freqA * bCountSoFarEq?
            // Actually aVal != that bVal, so no equals there. We'll handle equals only when aVal==bVal.
            
            sumPairsGreater += freqA * (bCountSoFar); // a> bVal' where bVal' < aVal, not correct?
            // Wait, we want # of (a,b) with a>b.  If aVal < bVal, then aVal is not > bVal. 
            // We should do: # of (a,b) with a > b means we want to count how many bVal < aVal.
            // We'll keep a cumulative count of how many B's are strictly < aVal. That is bCountSoFar in the sense that for all B's with bVal < aVal, we add freqA*freqB-those to sumPairsGreater. 
            sumPairsGreater += freqA * (bCountSoFar);
            // no equals, because aVal < bVal => no eq. 
            
            // Then we advance i, since we've done all pairs for A[i] w.r.t. everything up to j
            i++;
        }
        else if (aVal > bVal) {
            // then we incorporate B[j]'s effect into bCountSoFar, because for future aVal >= bVal
            bCountSoFar += freqB; 
            j++;
        }
        else {
            // aVal == bVal
            // so pairs (a,b) with a>b is freqA * bCountSoFar
            sumPairsGreater += freqA * bCountSoFar;
            // pairs (a,b) with a=b is freqA*freqB
            sumPairsEqual   += freqA * freqB;
            // now we move both
            i++;
            j++;
        }
    }
    // if i<NA but j==NB => that means we've consumed all B-values
    // all subsequent A[i].first > all B[j].first? Not necessarily. We just know j=NB => we've exhausted the B list.
    // but we do know how many B faces in total => nB. We also know how many we have in bCountSoFar => all B are presumably <= last bVal we processed
    while (i<NA) {
        long long freqA = A[i].second;
        // all B values we encountered so far are < or = something. 
        // That means aVal is definitely > all those bCountSoFar B's. We add freqA * bCountSoFar to sumPairsGreater.
        // We can't have any more equals because j=NB => no new bVal to match exactly. 
        sumPairsGreater += freqA * bCountSoFar;
        i++;
    }
    // no need to do while (j<NB) because that just means we have leftover B-values smaller or bigger? 
    // Actually if j<NB => that means next bVal hasn't been processed. If aVal was exhausted, that means we won't add anything new to sumPairsGreater. 
    // done
    
    // sumPairsGreater = # of (a,b) with a>b
    // sumPairsEqual   = # of (a,b) with a=b
    // total pairs = nA*nB
    long double totalPairs = (long double)nA * (long double)nB;
    long double sc = (long double)sumPairsGreater + 0.5L*(long double)sumPairsEqual;
    sc /= totalPairs;
    return sc;
}

// We'll parse the input.  The problem states: one of the two dice lines has advantage over the other.
// We have to figure out which is D1 and which is D2 from the data (since the statement says "The die with advantage is D1").
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Read the two lines as A, B
    Die A, B;
    cin >> A.n;
    A.faces.resize(A.n);
    for (int i=0; i<A.n; i++){
        cin >> A.faces[i];
    }
    cin >> B.n;
    B.faces.resize(B.n);
    for (int i=0; i<B.n; i++){
        cin >> B.faces[i];
    }
    sort(A.faces.begin(), A.faces.end());
    sort(B.faces.begin(), B.faces.end());

    // Compute score(A,B) and score(B,A)
    // We'll do the compressed approach:
    auto compA = compressFaces(A);
    auto compB = compressFaces(B);
    long long nA = A.n, nB = B.n;
    long double sAB = computeScore(compA, compB, nA, nB);
    long double sBA = 1.0L - sAB; // since score(A,B) + score(B,A) = 1 always

    // We know exactly one is > 0.5, so decide which is D1 (the advantage die) and which is D2.
    Die D1, D2;
    if (sAB > 0.5L) {
        // A has advantage over B
        D1 = A; 
        D2 = B;
    } else {
        // B has advantage over A
        D1 = B;
        D2 = A;
    }
    // Now solve
    //  - "lowestScore" = among all D3 with score(D3,D1)>=0.5, min( score(D3,D2) )
    //  - "highestScore"= among all D3 with score(D2,D3)<=0.5, max( score(D3,D1) )
    // Implemented in solveIntransitive:
    // But first we must ensure D1.faces is sorted ascending, D2.faces sorted ascending (they already are).
    pair<long double,long double> ans = solveIntransitive(D1, D2);

    // Output with the required precision
    cout << fixed << setprecision(9) 
         << (long double)ans.first << " " 
         << (long double)ans.second << "\n";
    return 0;
}
