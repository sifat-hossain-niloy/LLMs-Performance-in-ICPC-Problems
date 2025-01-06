#include <bits/stdc++.h>
using namespace std;

typedef unsigned long long ull;

bool check(ull y, ull l, ull b){
    vector<int> digits;
    ull temp = y;
    if(temp == 0){
        digits.push_back(0);
    }
    while(temp >0){
        ull d = temp % b;
        if(d >9){
            return false;
        }
        digits.push_back(d);
        temp /= b;
    }
    // Now digits are least to most, reverse to get most to least
    reverse(digits.begin(), digits.end());
    // Compute the decimal number
    __int128 num = 0;
    for(auto d: digits){
        num = num *10 + d;
        // Since l <=1e18 and num can go up to roughly 1e18, no overflow issues
        if(num >= l){
            break;
        }
    }
    return num >= l;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    ull y, l;
    cin >> y >> l;
    ull left = 2;
    ull right = y;
    ull best = 10; // base10 is always valid since digits <=9 and y >=l
    // But to handle cases where y <10, adjust
    if(y <10){
        best = y;
    }
    // Binary search
    while(left <= right){
        ull mid = left + (right - left)/2;
        if(check(y, l, mid)){
            if(mid > best){
                best = mid;
            }
            left = mid +1;
        }
        else{
            right = mid -1;
        }
    }
    // Ensure that base10 is considered
    if(y >= l){
        best = max(best, (ull)10);
    }
    cout << best;
}
