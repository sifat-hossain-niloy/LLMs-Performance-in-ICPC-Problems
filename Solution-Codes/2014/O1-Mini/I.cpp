#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, k;
    cin >> n >> k;
    vector<int> a(n);
    for(auto &x : a) cin >> x;
    if(k >= n /2){
        sort(a.begin(), a.end());
    }
    else{
        // Sort first k+1 elements
        vector<int> first_part(a.begin(), a.begin() + k +1);
        sort(first_part.begin(), first_part.end());
        // Assign first k sorted elements
        for(int i=0;i<k;i++) a[i] = first_part[i];
        // Sort last k+1 elements
        vector<int> last_part(a.begin() + n -k -1, a.end());
        sort(last_part.begin(), last_part.end());
        // Assign last k sorted elements
        for(int i=0;i<k;i++) a[n -k +i] = last_part[last_part.size() -k +i];
    }
    for(int i=0;i<n;i++){
        if(i >0) cout << ' ';
        cout << a[i];
    }
    cout << '\n';
}
