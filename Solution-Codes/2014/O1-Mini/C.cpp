#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-9;
const double INF = 1e18;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    cin >> n;
    vector<pair<long long, long long>> points(n);
    for(auto &p: points) cin >> p.first >> p.second;
    
    // Find support points on x-axis
    vector<pair<long long, long long>> support;
    for(auto &p: points){
        if(p.second ==0){
            support.emplace_back(p);
        }
    }
    if(support.size() <2){
        // At least two support points as per problem statement
    }
    // Find x_min and x_max
    double x_min = 1e20, x_max = -1e20;
    for(auto &p: support){
        x_min = min(x_min, (double)p.first);
        x_max = max(x_max, (double)p.first);
    }
    // Compute area and centroid
    double shoelace =0.0;
    double centroid_sum =0.0;
    for(int i=0;i<n;i++){
        int j=(i+1)%n;
        double cross = (double)points[i].first * points[j].second - (double)points[j].first * points[i].second;
        shoelace += cross;
        centroid_sum += (points[i].first + points[j].first) * cross;
    }
    double area = abs(shoelace)/2.0;
    if(area < EPS){
        // Degenerate polygon, but n >=3, assume valid
    }
    double C_x = centroid_sum / (6.0 * area);
    // Weight attachment point
    double xw = (double)points[0].first;
    // Compute A, B, C, D
    double W_c = area;
    double A = (x_min - C_x) * W_c;
    double B = xw - x_min;
    double C = (C_x - x_max) * W_c;
    double D = x_max - xw;
    // Initialize bounds
    double lower = 0.0;
    double upper = INF;
    bool unstable = false;
    // First inequality: A <= B * W_w
    if(abs(B) > EPS){
        if(B > 0){
            double tmp = A / B;
            lower = max(lower, tmp);
        }
        else{
            double tmp = A / B;
            upper = min(upper, tmp);
        }
    }
    else{
        if(A > EPS){
            unstable = true;
        }
    }
    if(!unstable){
        // Second inequality: C <= D * W_w
        if(abs(D) > EPS){
            if(D > 0){
                double tmp = C / D;
                lower = max(lower, tmp);
            }
            else{
                double tmp = C / D;
                upper = min(upper, tmp);
            }
        }
        else{
            if(C > EPS){
                unstable = true;
            }
        }
    }
    // Apply W_w >=0
    lower = max(lower, 0.0);
    // Check if upper is still >= lower
    if(!unstable){
        if(lower - upper > EPS){
            unstable = true;
        }
    }
    if(!unstable){
        // Determine if upper is infinity
        bool is_inf = false;
        if(upper >= 1e17){
            is_inf = true;
        }
        // Adjust lower and upper
        double final_lower = lower;
        double final_upper = upper;
        // If is_inf, ignore upper
        // Else, floor the upper
        // For lower, ceil
        // To handle precision, subtract EPS for ceil and add EPS for floor
        // Also, ensure final_lower >=0
        if(is_inf){
            // Determine a'
            double a_real = final_lower;
            long long a_prime = ceil(a_real - EPS);
            a_prime = max(a_prime, (long long)0);
            // Output "a' .. inf"
            cout << a_prime << " .. inf\n";
            return 0;
        }
        else{
            // finite upper
            double a_real = final_lower;
            double b_real = final_upper;
            long long a_prime = ceil(a_real - EPS);
            long long b_prime = floor(b_real + EPS);
            // W_w must be >=0
            a_prime = max(a_prime, (long long)0);
            if(a_prime > b_prime){
                unstable = true;
            }
            if(!unstable){
                cout << a_prime << " .. " << b_prime << "\n";
                return 0;
            }
        }
    }
    if(unstable){
        cout << "unstable\n";
    }
    return 0;
}
