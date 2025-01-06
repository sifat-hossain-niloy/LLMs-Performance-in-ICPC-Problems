#include <bits/stdc++.h>
using namespace std;

const double PI = acos(-1.0);

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    int case_num =1;
    while(cin >> n){
        if(n ==0) break;
        vector<int> s(n);
        for(auto &x: s) cin >> x;
        sort(s.begin(), s.end());
        // Remove largest segments until polygon inequality is satisfied
        while(s.size() >=3){
            long long sum =0;
            for(int i=0;i<s.size()-1;i++) sum += s[i];
            if(sum > s.back()) break;
            s.pop_back();
        }
        double area =0.0;
        if(s.size() >=3){
            // Find R such that sum(arcsin(s_i / (2 R))) = PI
            double left = 0.0;
            for(auto x: s) left = max(left, x / 2.0);
            double right = left * 1e6;
            // Binary search
            for(int iter=0; iter < 100; iter++){
                double mid = (left + right)/2.0;
                double sum_angles =0.0;
                bool invalid = false;
                for(auto x: s){
                    double ratio = x / (2.0 * mid);
                    if(ratio >1.0){
                        invalid = true;
                        break;
                    }
                    sum_angles += asin(ratio);
                }
                if(invalid || sum_angles > PI){
                    left = mid;
                }
                else{
                    right = mid;
                }
            }
            double R = left;
            // Compute area
            for(auto x: s){
                double ratio = x / (2.0 * R);
                // Clamp ratio to [0,1] due to floating errors
                if(ratio >1.0) ratio =1.0;
                if(ratio < -1.0) ratio = -1.0;
                double term = (x * R / 2.0) * sqrt(1.0 - ratio * ratio);
                area += term;
            }
        }
        // Output
        printf("Case %d: %.6lf\n", case_num++, area);
    }
}
