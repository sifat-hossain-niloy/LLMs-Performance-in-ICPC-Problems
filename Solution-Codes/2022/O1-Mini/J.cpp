#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAX = 100005;
const ll mod1 = 1000000007;
const ll mod2 = 1000000009;
const ll base1 = 911382629;
const ll base2 = 3571428571;

// Function to compute power arrays
void compute_powers(vector<ll> &pow1, vector<ll> &pow2, int n) {
    pow1[0] = 1;
    pow2[0] = 1;
    for(int i=1;i<=n;i++) {
        pow1[i] = (pow1[i-1] * base1) % mod1;
        pow2[i] = (pow2[i-1] * base2) % mod2;
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    string S;
    cin >> S;
    int n = S.size();
    // Compute powers
    vector<ll> pow1_arr(n+1, 1);
    vector<ll> pow2_arr(n+1, 1);
    compute_powers(pow1_arr, pow2_arr, n);
    // Compute Z-array
    vector<int> Z(n, 0);
    int l=0, r=0;
    for(int i=1;i<n;i++){
        if(i > r){
            l = r = i;
            while(r < n && S[r - l] == S[r]) r++;
            Z[i] = r - l;
            r--;
        }
        else{
            int k = i - l;
            if(Z[k] < r - i +1){
                Z[i] = Z[k];
            }
            else{
                l = i;
                while(r < n && S[r - l] == S[r]) r++;
                Z[i] = r - l;
                r--;
            }
        }
    }
    // Implement the stack-based compression
    string stack_str = "";
    vector<ll> hash1_stack;
    vector<ll> hash2_stack;
    hash1_stack.push_back(0);
    hash2_stack.push_back(0);
    for(int j=0; j<n; j++){
        // Push the character
        stack_str += S[j];
        // Update hashes
        ll new_hash1 = (hash1_stack.back() * base1 + (S[j]-'0'+1)) % mod1;
        ll new_hash2 = (hash2_stack.back() * base2 + (S[j]-'0'+1)) % mod2;
        hash1_stack.push_back(new_hash1);
        hash2_stack.push_back(new_hash2);
        // Now, try to replace duplicates
        while(true){
            int size = stack_str.size();
            if(size <2) break;
            bool replaced = false;
            // Iterate k from1 to size /2
            // To prioritize smaller k's first
            for(int k=1; k <= size/2; k++){
                int l1 = size - 2*k;
                int r1 = size - k -1;
                int l2 = size - k;
                int r2 = size -1;
                if(l1 <0) continue;
                // Compute hash for [l1, r1]
                ll hash1_a = (hash1_stack[r1 +1] - (hash1_stack[l1] * pow1_arr[k]) % mod1 + mod1) % mod1;
                ll hash1_b = (hash1_stack[r2 +1] - (hash1_stack[l2] * pow1_arr[k]) % mod1 + mod1) % mod1;
                if(hash1_a != hash1_b) continue;
                // Compute hash for [l1, r1] in second hash
                ll hash2_a = (hash2_stack[r1 +1] - (hash2_stack[l1] * pow2_arr[k]) % mod2 + mod2) % mod2;
                ll hash2_b = (hash2_stack[r2 +1] - (hash2_stack[l2] * pow2_arr[k]) % mod2 + mod2) % mod2;
                if(hash2_a != hash2_b) continue;
                // If both hashes match, perform replacement
                // Remove last k characters
                stack_str.erase(stack_str.end() - k, stack_str.end());
                // Remove last k hashes
                for(int m=0; m<k; m++) {
                    hash1_stack.pop_back();
                    hash2_stack.pop_back();
                }
                replaced = true;
                break; // After replacement, check again from k=1
            }
            if(!replaced) break;
        }
    }
    cout << stack_str;
}
