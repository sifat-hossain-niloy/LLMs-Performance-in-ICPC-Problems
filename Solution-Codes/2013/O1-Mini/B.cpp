#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    double x_input, p_input;
    cin >> x_input >> p_input;
    double x = x_input / 100.0;
    double p = p_input / 100.0;
    double max_E = 0.0;
    int max_n = 100000; // Upper limit for n
    int stop_threshold = 1000; // Number of iterations with no improvement to stop
    int consecutive_non_improve = 0;
    for(int n=1; n<=max_n; n++){
        // Compute binomial probabilities
        vector<double> prob(n+1, 0.0);
        prob[0] = pow(1.0 - p, n);
        for(int W=1; W<=n; W++){
            prob[W] = prob[W-1] * (n - W +1) / W * p / (1.0 - p);
        }
        // Compute E(n)
        double E =0.0;
        int ceil_n2 = (n+1)/2;
        for(int W=0; W<=n; W++){
            if(W < ceil_n2){
                double profit = 2.0 * W - n + x * (n - 2.0 * W);
                E += profit * prob[W];
            }
            else{
                double profit = 2.0 * W - n;
                E += profit * prob[W];
            }
        }
        // Update max_E
        if(E > max_E + 1e-12){
            max_E = E;
            consecutive_non_improve =0;
        }
        else{
            consecutive_non_improve++;
            if(consecutive_non_improve >= stop_threshold){
                break;
            }
        }
    }
    // Output with precision up to 1e-6
    cout << fixed << setprecision(10) << max_E;
}
