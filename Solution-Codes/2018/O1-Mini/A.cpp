#include <bits/stdc++.h>
using namespace std;

struct Point {
    long long r;
    long long c;
    int index;
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    cin >> n;
    vector<Point> top(n);
    for(int i=0;i<n;i++){
        cin >> top[i].r >> top[i].c;
        top[i].index = i+1;
    }
    vector<Point> bottom(n);
    for(int i=0;i<n;i++){
        cin >> bottom[i].r >> bottom[i].c;
        bottom[i].index = i+1;
    }
    // Sort top-lefts by r ascending, then c ascending
    sort(top.begin(), top.end(), [&](const Point &a, const Point &b) -> bool{
        if(a.r != b.r) return a.r < b.r;
        return a.c < b.c;
    });
    // Sort bottom-rights by r ascending, then c ascending
    sort(bottom.begin(), bottom.end(), [&](const Point &a, const Point &b) -> bool{
        if(a.r != b.r) return a.r < b.r;
        return a.c < b.c;
    });
    // Check conditions
    bool ok = true;
    for(int i=0;i<n;i++){
        if(bottom[i].c < top[i].c){
            ok = false;
            break;
        }
        if(i>0 && bottom[i].c < bottom[i-1].c){
            ok = false;
            break;
        }
    }
    if(!ok){
        cout << "syntax error";
        return 0;
    }
    // Assign
    vector<int> assign(n+1, 0); // 1-based
    for(int i=0;i<n;i++){
        assign[top[i].index] = bottom[i].index;
    }
    // Output in original top-left order
    for(int i=1;i<=n;i++){
        cout << assign[i] << "\n";
    }
}
