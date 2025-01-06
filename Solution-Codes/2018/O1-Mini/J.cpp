#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int main(){
    ll m;
    string n_str;
    cin >> m >> n_str;
    ll n = 0;
    // Convert n_str to ll safely
    try {
        n = stoull(n_str);
    } catch(...) {
        // In case of conversion error, set n to 0
        n = 0;
    }
    if(m <=3){
        cout << 0;
    }
    else if(m ==4 || m ==5 || m ==7){
        // Check for overflow, though 4*1e15 fits in long long
        cout << 4*n;
    }
    else if(m ==6 || m ==8){
        cout << 2*n;
    }
    else{
        cout << 0;
    }
}
