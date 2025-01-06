#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int m, n;
    cin >> m >> n;
    // Initialize reachability
    bool reach[26][26];
    for(int i=0;i<26;i++) {
        for(int j=0;j<26;j++) {
            reach[i][j] = false;
        }
        reach[i][i] = true;
    }
    // Read translations
    for(int i=0;i<m;i++){
        char a, b;
        cin >> a >> b;
        int ai = a - 'a';
        int bi = b - 'a';
        reach[ai][bi] = true;
    }
    // Floyd-Warshall
    for(int k=0;k<26;k++){
        for(int i=0;i<26;i++){
            if(reach[i][k]){
                for(int j=0;j<26;j++){
                    if(reach[k][j]){
                        if(!reach[i][j]){
                            reach[i][j] = true;
                        }
                    }
                }
            }
        }
    }
    // Process word pairs
    for(int i=0;i<n;i++){
        string w1, w2;
        cin >> w1 >> w2;
        if(w1.length() != w2.length()){
            cout << "no\n";
            continue;
        }
        bool ok = true;
        for(int j=0; j<w1.length(); j++){
            int from = w1[j] - 'a';
            int to = w2[j] - 'a';
            if(!reach[from][to]){
                ok = false;
                break;
            }
        }
        if(ok){
            cout << "yes\n";
        }
        else{
            cout << "no\n";
        }
    }
}
