#include <bits/stdc++.h>
using namespace std;

// Structure to hold a PLRR
struct PLRR {
    int k;
    vector<int> c;
    vector<int> a;
    vector<long long> generated;
};

// Function to generate the first 10 generated terms
vector<long long> generate_sequence(const vector<int>& c, const vector<int>& a, int num_terms=10){
    int k = c.size();
    vector<long long> buffer(a.begin(), a.end());
    vector<long long> generated;
    for(int t=0;t<num_terms;t++){
        long long next =0;
        for(int i=0;i<k;i++) next += (long long)c[i] * buffer[i];
        generated.push_back(next);
        // Update buffer
        buffer.erase(buffer.begin());
        buffer.push_back(next);
    }
    return generated;
}

// Recursive function to enumerate all PLRRs with sum =x and k fixed
void backtrack(int x, int k, int pos, int current_sum, vector<int>& c, vector<int>& a, vector<PLRR>& plrrs){
    if(pos ==k){
        if(current_sum ==x){
            PLRR pl;
            pl.k =k;
            pl.c =c;
            pl.a =a;
            pl.generated = generate_sequence(c, a, 10);
            plrrs.push_back(pl);
        }
        return;
    }
    // To ensure at least 1 for remaining positions
    for(int ci=1; ci<=x - current_sum - (k - pos -1); ci++){
        // For each c_i, a_i >=1
        // a_i can be from 1 to (x - current_sum)/ci
        int max_ai = (x - current_sum - (k - pos -1)) / ci;
        for(int ai=1; ai<=max_ai; ai++){
            c[pos] = ci;
            a[pos] = ai;
            backtrack(x, k, pos+1, current_sum + ci * ai, c, a, plrrs);
        }
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    long long n;
    cin >> n;
    // Find x such that T(x-1) <n <=T(x)
    // T(x) =x*(x+1)/2
    // Solve x*(x+1)/2 >=n
    // x = ceil( (-1 + sqrt(1 +8n)) /2 )
    double temp = (-1.0 + sqrt(1.0 + 8.0 * n)) /2.0;
    int x = ceil(temp);
    // Compute T(x-1)
    long long Tx_1 = ((long long)(x-1) * (x)) /2;
    long long y = n - Tx_1;
    // Now, enumerate all PLRRs with G1=x
    // and sort them by generated sequences lex,
    // then by coefficients lex
    // Then pick the y-th one
    vector<PLRR> plrrs;
    // To limit k, we can set k up to 10, since generating larger k would take too much time
    // But in reality, k can be up to x
    // However, y is up to T(x) which is x*(x+1)/2, but for x=44721, y=2140
    // Thus, proceed to enumerate k from 1 to x
    for(int k=1; k<=x; k++){
        vector<int> c(k,1);
        vector<int> a(k,1);
        backtrack(x, k, 0, 0, c, a, plrrs);
    }
    // Now, sort plrrs
    sort(plrrs.begin(), plrrs.end(), [&](const PLRR& a, const PLRR& b) -> bool{
        for(int i=0;i<10;i++){
            if(i >= a.generated.size() && i >= b.generated.size()) break;
            if(i >= a.generated.size()) return true;
            if(i >= b.generated.size()) return false;
            if(a.generated[i] != b.generated[i]) return a.generated[i] < b.generated[i];
        }
        // If generated sequences are same, sort by coefficients lex
        if(a.c.size() != b.c.size()) return a.c.size() < b.c.size();
        for(int i=0;i<a.c.size();i++){
            if(a.c[i] != b.c[i]) return a.c[i] < b.c[i];
        }
        return false;
    });
    // Now, pick the y-th one (1-based)
    if(y <= plrrs.size()){
        PLRR ans = plrrs[y-1];
        // Output
        cout << ans.k << "\n";
        for(int i=0;i<ans.c.size();i++) cout << ans.c[i] << (i<ans.c.size()-1?" ":"\n");
        for(int i=0;i<ans.a.size();i++) cout << ans.a[i] << (i<ans.a.size()-1?" ":"\n");
        for(int i=0;i<ans.generated.size();i++) cout << ans.generated[i] << (i<ans.generated.size()-1?" ":"\n");
    }
    return 0;
}
