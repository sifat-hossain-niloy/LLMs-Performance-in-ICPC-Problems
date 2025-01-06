#include <bits/stdc++.h>
using namespace std;

struct Tile {
    int p;
    int h;
    int idx;
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    cin >> n;
    vector<int> back_p(n);
    for(auto &x: back_p) cin >> x;
    vector<int> back_h(n);
    for(auto &x: back_h) cin >> x;
    vector<int> front_p(n);
    for(auto &x: front_p) cin >> x;
    vector<int> front_h(n);
    for(auto &x: front_h) cin >> x;
    // Create back tiles
    vector<Tile> back_tiles(n);
    for(int i=0;i<n;i++){
        back_tiles[i] = Tile{back_p[i], back_h[i], i+1};
    }
    // Sort back tiles by p ascending, h ascending
    sort(back_tiles.begin(), back_tiles.end(), [&](const Tile &a, const Tile &b) -> bool{
        if(a.p != b.p) return a.p < b.p;
        return a.h < b.h;
    });
    // Create front tiles
    vector<Tile> front_tiles(n);
    for(int i=0;i<n;i++){
        front_tiles[i] = Tile{front_p[i], front_h[i], i+1};
    }
    // Sort front tiles by p ascending, h descending
    sort(front_tiles.begin(), front_tiles.end(), [&](const Tile &a, const Tile &b) -> bool{
        if(a.p != b.p) return a.p < b.p;
        return a.h > b.h;
    });
    // Check for each i, back_h > front_h
    bool possible = true;
    for(int i=0;i<n;i++){
        if(back_tiles[i].h <= front_tiles[i].h){
            possible = false;
            break;
        }
    }
    if(!possible){
        cout << "impossible";
    }
    else{
        // Output back tile indices
        for(int i=0;i<n;i++){
            cout << back_tiles[i].idx;
            if(i != n-1) cout << ' ';
        }
        cout << '\n';
        // Output front tile indices
        for(int i=0;i<n;i++){
            cout << front_tiles[i].idx;
            if(i != n-1) cout << ' ';
        }
    }
}
