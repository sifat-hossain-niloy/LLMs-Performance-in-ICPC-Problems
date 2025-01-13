#include <bits/stdc++.h>
using namespace std;

// We will use complex<long double> for a bit of extra precision vs. double.
typedef long double LD;
typedef complex<LD> Cplx;

// Precompute the 3rd roots of unity:  w^0, w^1, w^2.
// w = exp(2 pi i / 3) = -0.5 + i*(sqrt(3)/2)
static const LD PI = acosl(-1.0L);
static const Cplx w0 = Cplx(1.0L, 0.0L);
static const Cplx w1 = Cplx(cosl(2*PI/3), sinl(2*PI/3));  // -0.5 + i*0.866025...
static const Cplx w2 = Cplx(cosl(4*PI/3), sinl(4*PI/3));  // -0.5 - i*0.866025...

// Given a*b (mod 3), precompute to avoid small overhead.
int mul3[3][3];

// Map {0,1,2} to the corresponding w^0, w^1, w^2
Cplx powW[3] = { w0, w1, w2 };

// Convert configuration string (of A/E/I) => base-3 integer.
static int configToIndex(const string &s) {
    // A=0, I=1, E=2
    // s.length() <= 5
    int val = 0;
    for(char c : s) {
        val *= 3;
        if(c=='A') val += 0;
        else if(c=='I') val += 1;
        else if(c=='E') val += 2;
    }
    return val;
}

// Convert a base-3 vector-of-length-k into the +/0/- output string.
static string vectorToLever(int v, int k) {
    // v is 0..3^k-1 in base-3.  0->'0',1->'+',2->'-'
    // We'll build from right to left.
    // The j-th pyramid is the least significant trit in "v".
    // The output string's j-th character (left to right) must match that.
    // So we'll fill an array of length k from the LSD upwards, then reverse.
    string out(k, '?');
    for(int j = k-1; j >= 0; j--) {
        int trit = v % 3;
        v /= 3;
        if(trit == 0) out[j] = '0';
        if(trit == 1) out[j] = '+';
        if(trit == 2) out[j] = '-';
    }
    return out;
}

// Dot-product mod 3 of two k-digit base-3 indices c, v
// We'll decode them trit by trit.
static int dotMod3(int c, int v, int k) {
    int sum = 0;
    for(int i=0; i<k; i++) {
        int tc = c % 3;
        int tv = v % 3;
        c /= 3;  v /= 3;
        sum += mul3[tc][tv];
    }
    return sum % 3;
}

// Build the DFT of freq[]: for each c in [0..3^k-1], F[c] = sum_{x} freq[x]*w^{<c,x>}
static void buildDFT(const vector<long long> &freq, vector<Cplx> &F, int k) {
    int sz = (int)freq.size(); // = 3^k
    F.assign(sz, Cplx(0,0));
    for(int c=0; c<sz; c++){
        // We'll sum over x
        // <c,x> mod 3 => w^{<c,x>}
        // We can accumulate in a complex<LD>.
        Cplx sumval = Cplx(0,0);
        for(int x=0; x<sz; x++){
            if(freq[x] == 0) continue;
            int dp = dotMod3(c, x, k);
            sumval += (Cplx)freq[x] * powW[dp];
        }
        F[c] = sumval;
    }
}

// Try factoring out (1 + w^{<c, vCandidate>}) from F[c], for c=0..sz-1
// If success, we update F -> newF, and return true.  Otherwise false.
static bool tryFactor(
    vector<Cplx> &F,   // in/out
    int vCandidate,
    int &factorsLeft,  // in/out
    int k
) {
    int sz = (int)F.size(); // 3^k
    // factor(0) = 1 + w^{<0, vCandidate>} = 1 + w^0 = 2
    // We want F(0)/2 ~ 2^(factorsLeft-1).
    // Let's see what we have:
    LD desired0 = powl(2.0L, (long double)(factorsLeft-1));
    Cplx f0 = F[0];
    // If F[0] is near zero or doesn't match, fail
    if (abs(f0) < 1e-15L) return false;

    Cplx ratio0 = f0 / Cplx(2, 0);
    LD scale = 0.0L;
    // We'll check if |ratio0| is close to desired0, within some small rel error.
    LD gotVal = abs(ratio0);
    if (fabsl(gotVal - desired0) > 1e-7L * desired0) {
        return false;
    }
    // We'll define scale so that newF(0) = desired0 exactly
    scale = desired0 / gotVal;
    // The argument (phase) of ratio0 we also want to fix:
    // We'll multiply everything by conj(ratio0)/|ratio0| to "realign" it to be real+.
    // But an even simpler approach: we just check that ratio0 is not too "off" in angle.
    // Then scale is real.  We'll do an angle check:
    LD ang = arg(ratio0);
    // If the angle is large, fail:
    if (fabsl(ang) > 1e-7L) {
        // We allow a tiny tolerance near 0 angle, but not big phases.
        return false;
    }

    // If we pass so far, let's define factor(c) = 1 + w^{ <c,vCandidate> }.
    // Then newF(c) = F(c)/factor(c) * scale * exp(-i*ang) (but ang is ~0).
    // We'll just do F(c)/factor(c) * scale and require that the ratio is close to newF(0).
    // Then newF(0) = desired0 by design.

    // Precompute w^{ <c,vCandidate> } for c=0..sz-1 to avoid repeated dot computations.
    static vector<Cplx> facVal; // re-usable
    facVal.resize(sz);
    for(int c=0; c<sz; c++){
        int dp = dotMod3(c, vCandidate, k);
        facVal[c] = Cplx(1,0) + powW[dp];
    }

    // We'll do a two-pass check: 
    // 1) if for any c, facVal[c] ~ 0 but F[c]!=0 => fail (division by near-zero).
    // 2) compute newF(c) = F(c)/facVal[c], then check ratio to newF(0).

    // Step 1 check:
    for(int c=0; c<sz; c++){
        LD magfac = abs(facVal[c]);
        if(magfac < 1e-15L && abs(F[c]) > 1e-15L) {
            return false;
        }
    }

    // We'll define a temporary array newF to hold the result of the division.
    static vector<Cplx> newF;
    newF.resize(sz);
    // newF(0) = ratio0 * scale, that should be "desired0" in magnitude ~ 2^(factorsLeft-1).
    // We'll store it just to do the final copy if we succeed.
    Cplx newF0 = ratio0 * Cplx(scale,0);
    // We'll check that newF(0) is real & positive within tolerance:
    if (fabsl(arg(newF0)) > 1e-7L) {
        // Phase not near zero => fail
        return false;
    }

    // We'll do the fill:
    for(int c=0; c<sz; c++){
        newF[c] = (F[c] / facVal[c]) * Cplx(scale, 0);
    }

    // Now we want to check that for all c, newF(c) / newF(0) ~ 1 in magnitude,
    // i.e. within some small tolerance.  Because we want a perfect product of
    // (n-1) factors that also has no variation across c except for tiny float error.
    // So let's define r(c) = newF(c)/newF(0).  Check if |r(c) - 1| < 1e-7.
    for(int c=1; c<sz; c++){
        Cplx rr = newF[c] / newF0; 
        LD m = abs(rr - Cplx(1,0));
        if(m > 1e-7L) {
            return false;
        }
    }

    // If we get here => success. We'll copy newF back to F, store the factor.
    for(int c=0; c<sz; c++){
        F[c] = newF[c];
    }
    factorsLeft--;
    return true;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Prepare mul3 table
    for(int a=0;a<3;a++){
        for(int b=0;b<3;b++){
            mul3[a][b] = (a*b) % 3;
        }
    }

    int n,k,t;
    cin >> n >> k >> t;
    // We'll have freq of length 3^k
    int sz = 1;
    for(int i=0; i<k; i++) sz *= 3;

    vector<long long> freq(sz, 0LL);
    long long sumF = 0; // should end up = 2^n
    for(int i=0; i<t; i++){
        string s; 
        long long f;
        cin >> s >> f;
        int idx = configToIndex(s);
        freq[idx] += f;
        sumF += f;
    }
    // sumF should be 2^n (the total # subsets).
    // We don't specifically validate that because problem states it's correct input.

    // Build the DFT array F
    vector<Cplx> F;
    buildDFT(freq, F, k);

    // We now have F(c) = \sum_x freq[x]* w^{<c,x>}, and want to factor out n times
    // of (1 + w^{<c, v_i>}).  We'll store the chosen v_i in an array.
    vector<int> chosen; 
    chosen.reserve(n);

    int factorsLeft = n;
    // Repeat n times:
    for(int i=0; i<n; i++){
        bool found = false;
        // Try all vCandidate in [0..3^k - 1]
        for(int vCandidate=0; vCandidate<sz; vCandidate++){
            if(tryFactor(F, vCandidate, factorsLeft, k)){
                chosen.push_back(vCandidate);
                found = true;
                break;
            }
        }
        if(!found){
            // If for some reason we can't find a factor, 
            // (the problem guarantees at least one solution, so in theory shouldn't happen).
            // We'll just fill dummy "0" levers.
            // But let's break out anyway:
            cerr << "No factor found unexpectedly!\n";
            while((int)chosen.size()<n) chosen.push_back(0);
            break;
        }
    }

    // Now we have chosen n vectors.  Output them in the desired format.
    // Each chosen[i] is an integer in [0..3^k-1].
    // Convert to +/0/- for each of k pyramids:
    for(int i=0; i<n; i++){
        cout << vectorToLever(chosen[i], k) << "\n";
    }

    return 0;
}
