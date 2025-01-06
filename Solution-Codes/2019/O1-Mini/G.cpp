#include <bits/stdc++.h>
using namespace std;

// Fast IO
struct FastIO {
    static const int S = 1 << 20;
    char inbuf[S];
    char outbuf[S];
    int inpos, inlen, outpos;
    FastIO() : inpos(0), inlen(0), outpos(0) {}
    char getChar() {
        if (inpos == inlen) {
            inlen = fread(inbuf, 1, S, stdin);
            inpos = 0;
            if (inlen == 0) return EOF;
        }
        return inbuf[inpos++];
    }
    void putChar(char c) {
        if (outpos == S) {
            fwrite(outbuf, 1, S, stdout);
            outpos = 0;
        }
        outbuf[outpos++] = c;
    }
    void flush() {
        fwrite(outbuf, 1, outpos, stdout);
        outpos = 0;
    }
    bool readInt(int &x) {
        x = 0;
        char c;
        bool neg = false;
        while ((c = getChar()) != EOF && !isdigit(c) && c != '-');
        if (c == EOF) return false;
        if (c == '-') {
            neg = true;
            c = getChar();
        }
        do {
            x = x * 10 + (c - '0');
            c = getChar();
        } while (isdigit(c));
        if (neg) x = -x;
        return true;
    }
    bool readString(string &s) {
        s.clear();
        char c;
        while ((c = getChar()) != EOF && !isupper(c));
        if (c == EOF) return false;
        s += c;
        while ((c = getChar()) != EOF && isupper(c)) {
            s += c;
        }
        return true;
    }
    void writeInt(int x) {
        if (x == 0) {
            putChar('0');
            putChar('\n');
            return;
        }
        string s;
        if (x < 0) {
            putChar('-');
            x = -x;
        }
        while (x > 0) {
            s += '0' + (x % 10);
            x /= 10;
        }
        for (int i = s.size() - 1; i >= 0; --i) putChar(s[i]);
        putChar('\n');
    }
} io;

// Aho-Corasick automaton
struct TrieNode {
    int next[26];
    int fail;
    vector<int> output;
    TrieNode() : fail(0) {
        for(int i=0;i<26;i++) next[i] = -1;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    // Read n and k
    int n, k;
    // Using FastIO to read n and k
    io.readInt(n);
    io.readInt(k);
    // Build the family tree
    // To save memory and time, use a single array to store children
    // and a separate array to store the starting index of each node's children
    // But for simplicity, use vector<vector<int>> with pre-reserved space
    vector<vector<int>> family_tree(n+1);
    // To store the character for each node
    string c(n+1, 'A'); // 1-based indexing
    int p;
    int root = 1;
    for(int i=1;i<=n;i++){
        string s;
        // Read character and p_i
        io.readString(s);
        io.readInt(p);
        c[i] = s[0];
        if(p != 0){
            family_tree[p].push_back(i);
        }
        else{
            root = i;
        }
    }
    // Read k query strings
    // Store queries in order
    // To map query index to its count
    // Initialize counts to 0
    vector<int> counts(k, 0);
    // Build Aho-Corasick automaton
    vector<TrieNode> trie;
    trie.emplace_back(); // state 0
    // For each query, insert reversed string into trie
    for(int j=0; j<k; j++){
        string s;
        io.readString(s);
        // Reverse the string
        reverse(s.begin(), s.end());
        int state = 0;
        for(char ch : s){
            int idx = ch - 'A';
            if(trie[state].next[idx] == -1){
                trie[state].next[idx] = trie.size();
                trie.emplace_back();
            }
            state = trie[state].next[idx];
        }
        trie[state].output.push_back(j);
    }
    // Build failure links
    queue<int> q;
    // Initialize the queue with depth 1 states
    for(int i=0; i<26; i++){
        if(trie[0].next[i] != -1){
            trie[trie[0].next[i]].fail = 0;
            q.push(trie[0].next[i]);
        }
        else{
            trie[0].next[i] = 0;
        }
    }
    // BFS to build failure links
    while(!q.empty()){
        int current = q.front();
        q.pop();
        for(int i=0; i<26; i++){
            if(trie[current].next[i] != -1){
                int child = trie[current].next[i];
                // Set failure link for child
                int fail = trie[current].fail;
                while(trie[fail].next[i] == -1 && fail != 0){
                    fail = trie[fail].fail;
                }
                if(trie[fail].next[i] != -1){
                    trie[child].fail = trie[fail].next[i];
                }
                else{
                    trie[child].fail = 0;
                }
                // Merge output
                for(auto &pattern : trie[trie[child].fail].output){
                    trie[child].output.push_back(pattern);
                }
                q.push(child);
            }
            else{
                trie[current].next[i] = trie[trie[current].fail].next[i];
            }
        }
    }
    // Traverse the family tree using iterative DFS
    // Each stack element contains (node, current_state)
    // Use a vector as a stack
    vector<pair<int, int>> stack_dfs;
    stack_dfs.emplace_back(root, 0);
    while(!stack_dfs.empty()){
        pair<int, int> current = stack_dfs.back();
        stack_dfs.pop_back();
        int node = current.first;
        int state = current.second;
        // Transition with c[node]
        int idx = c[node] - 'A';
        state = trie[state].next[idx];
        // Increment counts for all patterns ending here
        for(auto &pattern_j : trie[state].output){
            counts[pattern_j]++;
        }
        // Add children to the stack
        for(auto &child : family_tree[node]){
            stack_dfs.emplace_back(child, state);
        }
    }
    // Output the counts
    for(int j=0; j<k; j++){
        io.writeInt(counts[j]);
    }
    io.flush();
}
