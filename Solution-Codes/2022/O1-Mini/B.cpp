#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

// BIT implementation for counting inversions
struct BIT {
    int n;
    vector<int> tree;
    BIT(int size) : n(size), tree(size+1, 0) {}
    void update(int idx) {
        while(idx <=n){
            tree[idx] +=1;
            idx += idx & -idx;
        }
    }
    int query(int idx) const {
        int res=0;
        int x = const_cast<BIT*>(this)->n;
        int idx_copy = idx;
        BIT* self = const_cast<BIT*>(this);
        while(idx >0){
            res += self->tree[idx];
            idx -= idx & -idx;
        }
        return res;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    ll n;
    cin>>n;
    vector<int> a(n);
    for(auto &x:a) cin>>x;
    // Compute inversions
    // Since a[i] are from1 ton, no need to compress
    BIT bit(n);
    ll inv=0;
    for(int i=0;i<n;i++){
        // Number of elements already in BIT greater than a[i]
        inv += (i) - bit.query(a[i]);
        bit.update(a[i]);
    }
    // Compute E=moves =n + (n*(n-1)/2 - inv)/2
    double E = (double)n + ((double)(n)*(n-1)/2.0 - (double)inv)/2.0;
    // Print with 3 decimal digits
    cout<<fixed<<setprecision(3)<<E<<"\n";
}
