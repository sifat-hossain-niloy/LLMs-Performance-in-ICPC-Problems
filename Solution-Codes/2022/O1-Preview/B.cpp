#include <bits/stdc++.h>
using namespace std;

/*
  We will implement the solution sketched above:
    1) Read n, and the permutation a of size n.
    2) Count #inversions in a => inv.
       Then NI = n*(n-1)/2 - inv.
    3) Build nextGreater[] in O(n):
         - We'll store indices in a stack, maintaining
           a decreasing sequence of labels a[idx].
         - For each position i from n-1 down to 0:
             while stack not empty and a[ stack.top() ] <= a[i], pop.
             if stack not empty, nextGreater[i] = stack.top(), else -1
             push i.
    4) Build a Segment Tree (or Sparse Table) for range maximum queries on array a.
    5) For each i from 0..n-1:
         let j = nextGreater[i].
         if j != -1:
           if max in a[i+1..j-1] < a[i], then PC++.
    6) result = n + NI - 0.5*PC
    7) print result with precision <= 1e-3
*/

// Fenwick (BIT) for counting inversions (values up to n).
// We'll compress the values first if needed, but here
// the permutation a is from 1..n, so no compression needed.
struct Fenwick {
    int n;
    vector<long long> fenw;
    Fenwick(int n) : n(n), fenw(n+1,0) {}
    void update(int i, long long val){
        for(; i<=n; i+=(i & -i)) fenw[i]+=val;
    }
    long long query(int i){ // sum from 1..i
        long long s=0; 
        for(; i>0; i-=(i & -i)) s += fenw[i];
        return s;
    }
};

static const int MAXN = 1000000;

// We'll build a Sparse Table for range-max queries in O(n log n).
// Then each query in O(1).
// Memory usage ~ n log n, which can be ~ 20 million for n=1e6 => borderline.
// We'll try to be memory-efficient (use int instead of long long for storing).
// Alternatively, we could do a Segment Tree.  Let's do Sparse Table for simplicity.

struct SparseTable {
    int n;
    int logval[MAXN+1];
    // st[k][i] = index of max in the interval of length 2^k starting at i
    // or we can store the actual max value. We'll store the value.
    static const int K = 20; // enough for n up to 1e6 (~log2(1e6)=20)
    int st[K][MAXN];  // st[0][i] = a[i], st[1][i] = max of length 2^1 from i, etc.

    SparseTable() { }

    void build(const vector<int> &arr){
        n = (int)arr.size();
        // precompute logs
        logval[1] = 0;
        for(int i=2; i<=n; i++){
            logval[i] = logval[i/2]+1;
        }
        // st[0][i] = arr[i],  i in [0..n-1]
        for(int i=0; i<n; i++){
            st[0][i] = arr[i];
        }
        // fill st[k][i]
        for(int k=1; (1<<k)<=n; k++){
            int length = (1<<k);
            int half = (length >>1);
            for(int i=0; i+length-1<n; i++){
                st[k][i] = max(st[k-1][i],
                               st[k-1][i+half]);
            }
        }
    }

    // query max on interval [L,R], inclusive
    int queryMax(int L, int R){
        if(L>R) return -1; // empty range => sentinel
        int length = R-L+1;
        int k = logval[length];
        int half = 1<<k;
        return max(st[k][L],
                   st[k][R - half + 1]);
    }
};


int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n; 
    cin >> n;
    vector<int> a(n);
    for(int i=0; i<n; i++){
        cin >> a[i];
    }

    // 1) Count #inversions via Fenwick.
    // a[i] in [1..n].
    Fenwick fenw(n);
    long long invCount = 0LL;
    for(int i=0; i<n; i++){
        // # of already-seen elements that are > a[i].
        // That is i - fenw.query(a[i]).
        long long bigger = i - fenw.query(a[i]);
        invCount += bigger;
        fenw.update(a[i], 1LL);
    }

    // total pairs = n*(n-1)/2
    long long totalPairs = (long long)n*(n-1)/2;
    long long NI = totalPairs - invCount;  // label-ascending & truck-order-ascending

    // 2) Build nextGreater array.
    vector<int> nxtG(n,-1);
    // We'll do a decreasing stack of indices by value a[idx].
    // Traverse from right to left:
    //   while stack top has a[st.top()] <= a[i], pop
    //   if stack not empty, nxtG[i] = st.top()
    //   push i
    {
        stack<int> st;
        for(int i=n-1; i>=0; i--){
            while(!st.empty() && a[st.top()] <= a[i]){
                st.pop();
            }
            if(!st.empty()){
                nxtG[i] = st.top();
            }
            st.push(i);
        }
    }

    // 3) Build SparseTable for range-max queries on a.
    static SparseTable rmq;
    rmq.build(a);

    // 4) Compute PC
    long long PC = 0;
    for(int i=0; i<n; i++){
        int j = nxtG[i];
        if(j==-1) continue;
        if(j == i+1){
            // then the subrange (i+1..j-1) is empty => automatically no bigger
            // => PC++
            PC++;
        } else {
            // range [i+1, j-1]
            int mx = rmq.queryMax(i+1, j-1);
            if(mx < a[i]){
                PC++;
            }
        }
    }

    // 5) final = n + NI - 0.5*PC
    // We should do all big sums in double or long long carefully.
    long double ans = (long double)n + (long double)NI - 0.5L*(long double)PC;

    cout << fixed << setprecision(4) << (long double)ans << "\n";

    return 0;
}
