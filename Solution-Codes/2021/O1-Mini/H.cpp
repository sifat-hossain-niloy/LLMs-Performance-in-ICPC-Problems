#include <bits/stdc++.h>
using namespace std;

struct StringInfo {
    int index;
    int balance;
    int min_prefix;
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    cin >> n;
    vector<StringInfo> group1;
    vector<StringInfo> group2;
    for(int i=1; i<=n; i++){
        string s;
        cin >> s;
        int balance =0;
        int min_pref =0;
        for(char c: s){
            if(c == '('){
                balance +=1;
            }
            else{
                balance -=1;
            }
            if(balance < min_pref){
                min_pref = balance;
            }
        }
        StringInfo si;
        si.index = i;
        si.balance = balance;
        si.min_prefix = min_pref;
        if(si.balance >=0){
            group1.push_back(si);
        }
        else{
            group2.push_back(si);
        }
    }
    // Sort group1 in descending order of min_prefix
    sort(group1.begin(), group1.end(), [&](const StringInfo &a, const StringInfo &b) -> bool{
        return a.min_prefix > b.min_prefix;
    });
    // Sort group2 in ascending order of (min_prefix - balance)
    sort(group2.begin(), group2.end(), [&](const StringInfo &a, const StringInfo &b) -> bool{
        long long key_a = (long long)a.min_prefix - (long long)a.balance;
        long long key_b = (long long)b.min_prefix - (long long)b.balance;
        return key_a < key_b;
    });
    // Concatenate group1 and group2
    vector<StringInfo> order;
    order.reserve(n);
    for(auto &s: group1){
        order.push_back(s);
    }
    for(auto &s: group2){
        order.push_back(s);
    }
    // Simulate
    long long c =0;
    bool ok = true;
    for(auto &s: order){
        if(c + s.min_prefix <0){
            ok = false;
            break;
        }
        c += s.balance;
    }
    if(ok && c ==0){
        for(auto &s: order){
            cout << s.index << "\n";
        }
    }
    else{
        cout << "impossible";
    }
}
