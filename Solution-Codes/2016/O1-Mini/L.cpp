#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

struct Drive {
    ll a;
    ll b;
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    cin >> n;
    vector<Drive> drives(n);
    for(int i=0;i<n;i++) cin >> drives[i].a >> drives[i].b;
    // Sort drives in descending order of b_i
    sort(drives.begin(), drives.end(), [&](const Drive &x, const Drive &y) -> bool{
        if(x.b != y.b) return x.b > y.b;
        else return x.a > y.a; // optional, to handle ties
    });
    ll prefix = 0;
    ll E = 0;
    for(int i=0;i<n;i++){
        ll req = drives[i].a - prefix;
        if(req > E){
            E = req;
        }
        prefix += drives[i].b;
    }
    if(E <0) E=0;
    cout << E;
}
