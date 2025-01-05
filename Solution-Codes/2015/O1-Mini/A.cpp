#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    long long p, a, b, c, d, n;
    cin >> p >> a >> b >> c >> d >> n;
    const double two_pi = 2.0 * M_PI;
    double angle_sin, angle_cos;
    if(a == 0){
        angle_sin = b;
    }
    else{
        angle_sin = a * 1.0 + b;
        angle_sin = fmod(angle_sin, two_pi);
    }
    if(c == 0){
        angle_cos = d;
    }
    else{
        angle_cos = c * 1.0 + d;
        angle_cos = fmod(angle_cos, two_pi);
    }
    double sin_val, cos_val;
    if(a == 0){
        sin_val = sin(angle_sin);
    }
    else{
        sin_val = sin(angle_sin);
    }
    if(c == 0){
        cos_val = cos(angle_cos);
    }
    else{
        cos_val = cos(angle_cos);
    }
    double price = p * (sin_val + cos_val + 2.0);
    double current_max = price;
    double max_decline = 0.0;
    for(long long k=2; k<=n; ++k){
        if(a != 0){
            angle_sin += a;
            if(angle_sin >= two_pi){
                angle_sin = fmod(angle_sin, two_pi);
            }
            else if(angle_sin < 0){
                angle_sin = fmod(angle_sin, two_pi);
                if(angle_sin < 0) angle_sin += two_pi;
            }
            sin_val = sin(angle_sin);
        }
        // else sin_val remains the same
        if(c != 0){
            angle_cos += c;
            if(angle_cos >= two_pi){
                angle_cos = fmod(angle_cos, two_pi);
            }
            else if(angle_cos < 0){
                angle_cos = fmod(angle_cos, two_pi);
                if(angle_cos < 0) angle_cos += two_pi;
            }
            cos_val = cos(angle_cos);
        }
        // else cos_val remains the same
        price = p * (sin_val + cos_val + 2.0);
        if(current_max - price > max_decline){
            max_decline = current_max - price;
        }
        if(price > current_max){
            current_max = price;
        }
    }
    // Handle the case when n ==1
    if(n ==1){
        max_decline = 0.0;
    }
    // Output with 9 decimal digits
    printf("%.9lf\n", max_decline);
}
