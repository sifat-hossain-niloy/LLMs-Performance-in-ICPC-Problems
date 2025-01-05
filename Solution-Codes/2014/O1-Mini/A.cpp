#include <bits/stdc++.h>
using namespace std;

int main(){
    int n;
    cin>>n;
    vector<int> move_list;
    if(n%2==1){
        // n is odd
        int f = 2*n -2;
        move_list.push_back(f);
        for(int j=1; j<n; j++){
            if(j%2==1){
                f = move_list[j-1] - n;
                if(f <1){
                    f =0;
                }
            }
            else{
                f = 3*(j/2 +1);
            }
            move_list.push_back(f);
        }
    }
    else{
        // n is even
        int f = 2*n -6;
        move_list.push_back(f);
        for(int j=1; j<n; j++){
            if(j%2==1){
                f = move_list[j-1] - (n-1);
                if(f <1){
                    f =0;
                }
            }
            else{
                f = move_list[j-1] + (n+3);
                if(f >2*n-1){
                    f =0;
                }
            }
            move_list.push_back(f);
        }
    }
    // Now, generate the move output
    for(int i=0;i<move_list.size();i++){
        int f = move_list[i];
        int t;
        if(i==0){
            t = -1;
        }
        else{
            t = move_list[i-1];
        }
        cout<<f<<" to "<<t<<'\n';
    }
}
