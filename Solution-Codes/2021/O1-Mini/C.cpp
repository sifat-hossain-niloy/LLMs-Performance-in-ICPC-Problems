#include <bits/stdc++.h>
using namespace std;

typedef unsigned long long ull;

// Function to compute base^exp with a limit. Returns -1 if it exceeds limit.
ull pow_limit(ull base, ull exp, ull limit){
    if(exp ==0) return 1;
    ull res =1;
    for(ull i=0;i < exp;i++){
        if(res > limit / base){
            return (ull)(-1);
        }
        res *= base;
    }
    return res;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    ull n, m;
    cin >> n >> m;
    // Precompute log(m)
    double logm = log((double)m);
    // Iterate q from 2 upwards
    // Set q_max to 100 to cover possible small q
    // Since n <=1e6 and m <=1e18, q_max won't be too large
    bool found = false;
    ull answer_p=0, answer_q=0;
    for(ull q=2; q <= 100 && !found; q++){
        // Compute log(q) and log(q-1)
        double logq = log((double)q);
        double logqm1;
        if(q-1 ==1){
            logqm1 = 0.0;
        }
        else{
            logqm1 = log((double)(q-1));
        }
        // Check if n * log(q) <= log(m) + log(q-1)
        double cond = (double)n * logq;
        double target = logm + logqm1;
        if(cond > target + 1e-12){
            continue;
        }
        // Iterate p from 1 to q-1
        for(ull p=1; p < q && !found; p++){
            if(__gcd(p, q) !=1){
                continue;
            }
            // Compute q^n
            ull qn = pow_limit(q, n, m * q);
            if(qn == (ull)(-1)){
                continue;
            }
            // Compute (q - p)^n
            ull qp = q - p;
            if(qp ==0){
                // (q-p)^n =0
                // C = qn / p
                if(qn % p !=0){
                    continue;
                }
                ull C = qn / p;
                if(C ==0){
                    continue;
                }
                if(m % C !=0){
                    continue;
                }
                // Found
                answer_p = p;
                answer_q = q;
                found = true;
                break;
            }
            ull qpn = pow_limit(qp, n, qn);
            if(qpn == (ull)(-1)){
                continue;
            }
            if(qn < qpn){
                continue;
            }
            ull diff = qn - qpn;
            if(diff % p !=0){
                continue;
            }
            ull C = diff / p;
            if(C ==0){
                continue;
            }
            if(m % C !=0){
                continue;
            }
            // Found
            answer_p = p;
            answer_q = q;
            found = true;
            break;
        }
    }
    if(found){
        cout << answer_p << " " << answer_q;
    }
    else{
        cout << "impossible";
    }
}
