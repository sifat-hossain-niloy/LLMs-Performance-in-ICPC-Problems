#include <bits/stdc++.h>
using namespace std;

// Function to compute determinant of 3x3 matrix
long long det3(long long a1, long long b1, long long c1,
              long long a2, long long b2, long long c2,
              long long a3, long long b3, long long c3)
{
    return a1*(b2*c3 - b3*c2) - b1*(a2*c3 - a3*c2) + c1*(a2*b3 - a3*b2);
}

// Function to solve system of three equations
bool solve_system(const vector<tuple<int,int,int,int>>& subset, int& la, int& lb, int& lc)
{
    // Extract equations
    auto [a1, b1, c1, r1] = subset[0];
    auto [a2, b2, c2, r2] = subset[1];
    auto [a3, b3, c3, r3] = subset[2];
    
    // Compute determinant
    long long D = det3(a1, b1, c1,
                       a2, b2, c2,
                       a3, b3, c3);
    if (D == 0) return false;
    
    // Compute D_x, D_y, D_z
    long long D_x = det3(r1, b1, c1,
                         r2, b2, c2,
                         r3, b3, c3);
    long long D_y = det3(a1, r1, c1,
                         a2, r2, c2,
                         a3, r3, c3);
    long long D_z = det3(a1, b1, r1,
                         a2, b2, r2,
                         a3, b3, r3);
    
    // Check if D divides D_x, D_y, D_z
    if (D == 0) return false;
    if (D_x % D != 0 || D_y % D != 0 || D_z % D != 0) return false;
    
    la = D_x / D;
    lb = D_y / D;
    lc = D_z / D;
    
    // Ensure non-negative
    if (la < 0 || lb < 0 || lc < 0) return false;
    
    return true;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    // Read five questions and answers
    vector<tuple<int,int,int,int>> qa(5);
    for(int i=0;i<5;i++){
        int a, b, c, r;
        cin >> a >> b >> c >> r;
        qa[i] = make_tuple(a, b, c, r);
    }
    
    // Collect possible solutions
    set<tuple<int,int,int>> solutions;
    
    // Enumerate all C(5,3)=10 subsets
    for(int mask=0; mask<(1<<5); mask++){
        if(__builtin_popcount(mask) !=3) continue;
        vector<tuple<int,int,int,int>> subset;
        for(int i=0;i<5;i++) if(mask & (1<<i)) subset.push_back(qa[i]);
        int la, lb, lc;
        if(solve_system(subset, la, lb, lc)){
            // Now check how many equations are satisfied
            int count =0;
            for(int i=0;i<5;i++){
                int a = get<0>(qa[i]);
                int b = get<1>(qa[i]);
                int c = get<2>(qa[i]);
                int r = get<3>(qa[i]);
                if(a*la + b*lb + c*lc == r) count++;
            }
            if(count >=4){
                solutions.emplace(make_tuple(la, lb, lc));
            }
        }
    }
    
    if(solutions.empty()){
        // No solution found
        cout << "0 0 0\n";
        return 0;
    }
    
    // Assuming all solutions are same
    // Take the first one
    auto it = solutions.begin();
    cout << get<0>(*it) << " " << get<1>(*it) << " " << get<2>(*it) << "\n";
    
    return 0;
}
