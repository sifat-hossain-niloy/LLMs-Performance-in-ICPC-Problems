#include <bits/stdc++.h>
using namespace std;

// Function to solve three equations and return x,y,z if integer solution exists
bool solve_three(const vector<tuple<int, int, int, int>> &eq, int &x, int &y, int &z) {
    // Using Cramer's rule
    // Equation 1: a1 x + b1 y + c1 z = r1
    // Equation 2: a2 x + b2 y + c2 z = r2
    // Equation 3: a3 x + b3 y + c3 z = r3
    double a1, b1, c1, r1;
    double a2, b2, c2, r2;
    double a3, b3, c3, r3;
    tie(a1, b1, c1, r1) = eq[0];
    tie(a2, b2, c2, r2) = eq[1];
    tie(a3, b3, c3, r3) = eq[2];
    
    double D = a1*(b2*c3 - b3*c2) - b1*(a2*c3 - a3*c2) + c1*(a2*b3 - a3*b2);
    if (abs(D) < 1e-8) return false;
    double Dx = r1*(b2*c3 - b3*c2) - b1*(r2*c3 - r3*c2) + c1*(r2*b3 - r3*b2);
    double Dy = a1*(r2*c3 - r3*c2) - r1*(a2*c3 - a3*c2) + c1*(a2*r3 - a3*r2);
    double Dz = a1*(b2*r3 - b3*r2) - b1*(a2*r3 - a3*r2) + r1*(a2*b3 - a3*b2);
    
    double x_d = Dx / D;
    double y_d = Dy / D;
    double z_d = Dz / D;
    
    // Check if x,y,z are close to integers
    int xi = round(x_d);
    int yi = round(y_d);
    int zi = round(z_d);
    
    const double eps = 1e-4;
    if (abs(x_d - xi) > eps || abs(y_d - yi) > eps || abs(z_d - zi) > eps)
        return false;
    if (xi < 0 || yi < 0 || zi < 0)
        return false;
    x = xi;
    y = yi;
    z = zi;
    return true;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    // Read five queries and responses
    vector<tuple<int, int, int, int>> queries(5);
    for(int i=0;i<5;i++){
        int a, b, c, r;
        cin >> a >> b >> c >> r;
        queries[i] = make_tuple(a, b, c, r);
    }
    // Iterate through all combinations of three equations out of five
    // C(5,3)=10
    vector<int> indices = {0,1,2,3,4};
    // To store possible solutions
    vector<tuple<int,int,int>> solutions;
    for(int i=0;i<5;i++){
        for(int j=i+1;j<5;j++){
            for(int k=j+1;k<5;k++){
                vector<tuple<int,int,int,int>> subset = {queries[i], queries[j], queries[k]};
                int xi, yi, zi;
                if(solve_three(subset, xi, yi, zi)){
                    // Check how many equations are satisfied
                    int count =0;
                    for(int m=0;m<5;m++){
                        long long total = (long long)get<0>(queries[m]) * xi + (long long)get<1>(queries[m]) * yi + (long long)get<2>(queries[m]) * zi;
                        if(total == get<3>(queries[m])) count++;
                    }
                    if(count >=4){
                        solutions.emplace_back(xi, yi, zi);
                    }
                }
            }
        }
    }
    // Remove duplicates
    sort(solutions.begin(), solutions.end());
    solutions.erase(unique(solutions.begin(), solutions.end()), solutions.end());
    if(solutions.empty()){
        // No solution found
        // Depending on problem constraints, maybe output 0 0 0 or some default
        cout << "0 0 0\n";
    }
    else{
        // Output the first solution
        auto sol = solutions[0];
        cout << get<0>(sol) << " " << get<1>(sol) << " " << get<2>(sol) << "\n";
    }
}
