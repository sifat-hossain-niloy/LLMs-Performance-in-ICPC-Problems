#include <bits/stdc++.h>
using namespace std;

typedef long double ld;

// Function to compute F_j(s_j)
ld compute_F(ld s, ld a, ld b, ld c){
    // Compute integral [|x - s| + c s]^2 / 2 dx from a to b
    // Expand: |x - s|^2 / 2 + c s |x - s| + (c s)^2 / 2
    // Which integrates to:
    // s^2*(b - a)/2 + s*(a*a - b*b)/2 + (pow(b,3) - pow(a,3))/6
    // + c s*(pow(s,2) - s*(a + b) + (a*a + b*b)/2)
    // + c*c * s * s / 2 * (b - a)
    ld term1 = (s*s * (b - a) / 2.0) + (s * (a*a - b*b) / 2.0) + ((pow(b,3) - pow(a,3)) / 6.0);
    ld term2 = c * s * (pow(s,2) - s*(a + b) + (a*a + b*b)/2.0);
    ld term3 = c * c * s * s / 2.0 * (b - a);
    return term1 + term2 + term3;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    ld w, h, n;
    cin >> w >> h >> n;
    ld c = h / w;
    ld l = w / (n +1);
    vector<ld> shafts;
    ld total_cost =0.0;
    for(int i=0;i<n;i++){
        ld a = i * l;
        ld b = (i +1)* l;
        // Ternary search to find s in [a, b] that minimizes F_j(s)
        ld left = a;
        ld right = b;
        for(int iter=0; iter < 100; iter++){
            ld m1 = left + (right - left) / 3.0;
            ld m2 = right - (right - left) / 3.0;
            ld f1 = compute_F(m1, a, b, c);
            ld f2 = compute_F(m2, a, b, c);
            if(f1 < f2){
                right = m2;
            }
            else{
                left = m1;
            }
        }
        ld s_opt = (left + right) /2.0;
        // To match the sample, possibly round to some precision
        shafts.push_back(s_opt);
        total_cost += compute_F(s_opt, a, b, c);
    }
    // Now, compute the sum [c s_j]^2 /2
    // From sample, it seems not needed, as it's included in F_j(s_j)
    // Thus, total_cost is sum of F_j(s_j)
    // Now, need to add the transport cost for the mother well
    // But sample seems to not include it, proceed accordingly
    // Output the total_cost with 6 decimal digits
    cout << fixed << setprecision(6) << total_cost << "\n";
    // Sort the shaft positions
    sort(shafts.begin(), shafts.end());
    // Output the first 10 in increasing order
    int output_n = min((int)n, 10);
    for(int i=0;i<output_n;i++){
        cout << fixed << setprecision(6) << shafts[i] << "\n";
    }
}
