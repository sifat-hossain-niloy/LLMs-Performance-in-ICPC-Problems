#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    int N, M;
    int case_num =1;
    while(cin >> N >> M){
        vector<int> TI(N);
        vector<int> BL(M);
        for(auto &x: TI) cin >> x;
        for(auto &x: BL) cin >> x;
        sort(TI.begin(), TI.end());
        sort(BL.begin(), BL.end());
        int TI_ptr =0;
        int BL_ptr =0;
        long long t=0;
        bool isTI = true;
        while(true){
            if(isTI){
                if(BL_ptr >= M){
                    break;
                }
                // Find first TI[k] > BL[BL_ptr]
                // upper_bound returns iterator to first TI[k] > BL[BL_ptr]
                auto it = upper_bound(TI.begin()+TI_ptr, TI.end(), BL[BL_ptr]);
                if(it != TI.end()){
                    t++;
                    BL_ptr++;
                    isTI = false;
                }
                else{
                    break;
                }
            }
            else{
                if(TI_ptr >= N){
                    break;
                }
                // Find first BL[k] > TI[TI_ptr]
                auto it = upper_bound(BL.begin()+BL_ptr, BL.end(), TI[TI_ptr]);
                if(it != BL.end()){
                    t++;
                    TI_ptr++;
                    isTI = true;
                }
                else{
                    break;
                }
            }
        }
        // Determine winner
        string winner;
        if(t %2 ==1){
            winner = "Takeover Incorporated";
        }
        else{
            winner = "Buyout Limited";
        }
        cout << "Case " << case_num++ << ": " << winner << "\n";
    }
}
