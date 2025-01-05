#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll INF = 1e18;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int case_num =1;
    while(true){
        int n;
        cin>>n;
        if(n==-1) break;
        // Initialize minimal t_i for four quadrants
        ll t1 = INF, t2 = INF, t3 = INF, t4 = INF;
        for(int i=0;i<n;i++){
            ll x, y;
            cin>>x>>y;
            // Determine quadrants
            bool q1=false, q2=false, q3=false, q4=false;
            if(x <0 && y <0){
                q1 = true;
            }
            if(x <0 && y >0){
                q2 = true;
            }
            if(x >0 && y >0){
                q3 = true;
            }
            if(x >0 && y <0){
                q4 = true;
            }
            if(x ==0){
                if(y >0){
                    q2 = true;
                    q3 = true;
                }
                if(y <0){
                    q1 = true;
                    q4 = true;
                }
            }
            if(y ==0){
                if(x >0){
                    q3 = true;
                    q4 = true;
                }
                if(x <0){
                    q1 = true;
                    q2 = true;
                }
            }
            // Compute t_i
            ll absx = abs(x);
            ll absy = abs(y);
            ll ceilx = (absx +1)/2;
            ll ceily = (absy +1)/2;
            ll ti = max(ceilx, ceily);
            // Assign to quadrants
            if(q1){
                t1 = min(t1, ti);
            }
            if(q2){
                t2 = min(t2, ti);
            }
            if(q3){
                t3 = min(t3, ti);
            }
            if(q4){
                t4 = min(t4, ti);
            }
        }
        // Check if all four quadrants have at least one mummy
        if(t1 == INF || t2 == INF || t3 == INF || t4 == INF){
            cout<<"Case "<<case_num<<": never\n";
        }
        else{
            ll t = max({t1, t2, t3, t4});
            cout<<"Case "<<case_num<<": "<<t+1<<"\n";
        }
        case_num++;
    }
}
