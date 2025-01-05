#include <bits/stdc++.h>
using namespace std;

struct MatchPosition {
    int row;
    int col;
};

// Fast input
inline void fast_io() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
}

int main(){
    fast_io();
    int rp, cp;
    cin >> rp >> cp;
    // Read motif grid and store non-zero cells
    struct MotifCell {
        int mi;
        int mj;
        int value;
    };
    vector<MotifCell> motif_non_zero;
    vector<int> motif_grid(rp * cp);
    for(int i=0;i<rp;i++){
        for(int j=0;j<cp;j++){
            cin >> motif_grid[i * cp + j];
            if(motif_grid[i * cp + j] != 0){
                motif_non_zero.push_back(MotifCell{i, j, motif_grid[i * cp + j]});
            }
        }
    }
    int rq, cq;
    cin >> rq >> cq;
    // Read mosaic grid
    vector<int> mosaic_grid(rq * cq);
    for(int i=0;i<rq;i++){
        for(int j=0;j<cq;j++){
            cin >> mosaic_grid[i * cq + j];
        }
    }
    // If motif is larger than mosaic, no matches
    if(rp > rq || cp > cq){
        cout << "0\n";
        return 0;
    }
    // Find matches
    vector<MatchPosition> matches;
    int max_i = rq - rp;
    int max_j = cq - cp;
    for(int i=0;i<=max_i;i++){
        for(int j=0;j<=max_j;j++){
            bool match = true;
            for(auto &cell : motif_non_zero){
                int mosaic_val = mosaic_grid[(i + cell.mi) * cq + (j + cell.mj)];
                if(mosaic_val != cell.value){
                    match = false;
                    break;
                }
            }
            if(match){
                matches.push_back(MatchPosition{ i+1, j+1 });
            }
        }
    }
    // Output
    cout << matches.size() << "\n";
    for(auto &pos : matches){
        cout << pos.row << " " << pos.col << "\n";
    }
}
