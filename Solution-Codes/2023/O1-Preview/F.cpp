#include <bits/stdc++.h>
using namespace std;

// We will store adjacency constraints as sets of chars (since color 'a'..'z').
static const int ALPH_SZ = 26;  // 26 possible tile colors

inline int toIdx(char c) {
    // map 'a'..'z' to 0..25, and '.' is invalid
    return c - 'a';
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int h, w;
    cin >> h >> w;

    vector<string> start(h), endB(h);  // store the start and end boards
    for(int i=0; i<h; i++){
        cin >> start[i];
    }

    // read the blank line (according to the problem statement there's one empty line)
    {
        string dummy;
        if(cin.peek() == '\n') cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    for(int i=0; i<h; i++){
        cin >> endB[i];
    }

    // --- 1) Count color frequencies ---
    array<int,ALPH_SZ> freqStart{}, freqEnd{};
    for(int i=0; i<h; i++){
        for(int j=0; j<w; j++){
            char c = start[i][j];
            if(c != '.') freqStart[toIdx(c)]++;
        }
    }
    for(int i=0; i<h; i++){
        for(int j=0; j<w; j++){
            char c = endB[i][j];
            if(c != '.') freqEnd[toIdx(c)]++;
        }
    }
    for(int c=0; c<ALPH_SZ; c++){
        if(freqStart[c] != freqEnd[c]){
            cout << "no\n";
            return 0;
        }
    }

    // --- 2) Handle single row or single column quickly ---
    if(h == 1){
        // We have exactly one row in start and end.
        // Let us gather the color sequence from left to right for the start:
        vector<char> startColors;
        for(int j=0; j<w; j++){
            if(start[0][j] != '.') {
                startColors.push_back(start[0][j]);
            }
        }

        // Similarly for the end arrangement:
        vector<char> endColors;
        for(int j=0; j<w; j++){
            if(endB[0][j] != '.') {
                endColors.push_back(endB[0][j]);
            }
        }

        // They must have the same multiset of colors (we already checked total freq),
        // and also the same left-to-right order (since you can't reorder them if
        // they remain in the same row).
        // But the entire block can either be aligned left or aligned right. 
        // Let's check if endColors == startColors as a sequence OR
        // endColors == startColors reversed? Actually, not reversed, but
        // the puzzle does NOT allow reversing the tile order in a single row,
        // because they cannot pass each other. So the order must remain identical.

        // So we just check if endColors == startColors as a vector of chars.
        if(endColors.size() != startColors.size()){
            cout << "no\n";
            return 0;
        }
        // The color sequence must match exactly:
        for(size_t i=0; i<startColors.size(); i++){
            if(startColors[i] != endColors[i]){
                cout << "no\n";
                return 0;
            }
        }

        // Finally, they must be contiguous in the end arrangement and at a boundary
        // (unless they are blocked by something else, but there's nothing else if it's a single row).
        // So in effect, if there are n tiles, in the end arrangement they occupy w consecutive cells
        // that start at col1=0 or col1=w-n. Let's check that quickly:
        int n = (int)startColors.size();
        // find the leftmost tile in end arrangement:
        int leftPos = 0;
        while(leftPos < w && endB[0][leftPos] == '.') leftPos++;
        if(leftPos == w && n>0){
            // no tiles found, but we expected n>0 => mismatch
            cout << (n==0 ? "yes\n":"no\n");
            return 0;
        }
        // find the rightmost tile:
        int rightPos = w-1;
        while(rightPos >= 0 && endB[0][rightPos] == '.') rightPos--;
        int blockLen = rightPos - leftPos + 1;
        if(blockLen != n){
            // The tiles are not exactly bunched in a contiguous block
            cout << "no\n";
            return 0;
        }

        // If we made it here, the arrangement is valid
        cout << "yes\n";
        return 0;
    }
    else if(w == 1){
        // Single column case.  Analogous logic:
        vector<char> startColors;
        for(int i=0; i<h; i++){
            if(start[i][0] != '.'){
                startColors.push_back(start[i][0]);
            }
        }
        vector<char> endColors;
        for(int i=0; i<h; i++){
            if(endB[i][0] != '.'){
                endColors.push_back(endB[i][0]);
            }
        }
        if(startColors.size() != endColors.size()){
            cout << "no\n";
            return 0;
        }
        for(size_t k=0; k<startColors.size(); k++){
            if(startColors[k] != endColors[k]){
                cout << "no\n";
                return 0;
            }
        }
        // Check contiguity in the end arrangement
        int n = (int)startColors.size();
        int topPos = 0;
        while(topPos < h && endB[topPos][0] == '.') topPos++;
        if(topPos == h && n>0){
            cout << (n==0?"yes\n":"no\n");
            return 0;
        }
        int botPos = h-1;
        while(botPos>=0 && endB[botPos][0]=='.') botPos--;
        int blockLen = botPos - topPos + 1;
        if(blockLen != n){
            cout << "no\n";
            return 0;
        }
        cout << "yes\n";
        return 0;
    }

    // --- 3) Now handle the general case: h>1, w>1 ---
    // Build adjacency constraints among colors (horizontal + vertical) from the start arrangement.
    // We'll represent these as sets (or vectors) of size 26, for each color 0..25.

    vector<unordered_set<int>> Hadj(ALPH_SZ), Vadj(ALPH_SZ);

    // horizontal constraints from the start
    for(int r=0; r<h; r++){
        // collect the colors in row r in left-to-right order
        vector<int> rowColors;
        rowColors.reserve(w);
        for(int c=0; c<w; c++){
            char tile = start[r][c];
            if(tile != '.') rowColors.push_back(toIdx(tile));
        }
        // for each pair in order, if they differ, record adjacency
        for(int i=0; i<(int)rowColors.size(); i++){
            for(int j=i+1; j<(int)rowColors.size(); j++){
                if(rowColors[i] != rowColors[j]){
                    // rowColors[i] must be to the left of rowColors[j] if they end up in same row
                    Hadj[rowColors[i]].insert(rowColors[j]);
                }
            }
        }
    }
    // vertical constraints from the start
    for(int c=0; c<w; c++){
        vector<int> colColors;
        colColors.reserve(h);
        for(int r=0; r<h; r++){
            char tile = start[r][c];
            if(tile != '.') colColors.push_back(toIdx(tile));
        }
        for(int i=0; i<(int)colColors.size(); i++){
            for(int j=i+1; j<(int)colColors.size(); j++){
                if(colColors[i] != colColors[j]){
                    Vadj[colColors[i]].insert(colColors[j]);
                }
            }
        }
    }

    // --- 4) Check the end arrangement for conflicts ---
    // horizontal check in the end arrangement
    for(int r=0; r<h; r++){
        // gather colors in row r
        vector<int> rowColors;
        for(int c=0; c<w; c++){
            if(endB[r][c] != '.'){
                rowColors.push_back(toIdx(endB[r][c]));
            }
        }
        // for each pair in the order they appear, if that contradicts adjacency => no
        // i.e. if we see rowColors[j] to the left of rowColors[i], but Hadj[rowColors[i]] says it must come before rowColors[j], conflict
        // More straightforwardly, we can just check every pair (i<j).  If rowColors[i] != rowColors[j] and
        // rowColors[j] is in Hadj[rowColors[i]], we have a violation (since that means rowColors[i] must be left of rowColors[j],
        // but we found rowColors[j] is to the left).
        for(int i=0; i<(int)rowColors.size(); i++){
            for(int j=i+1; j<(int)rowColors.size(); j++){
                if(rowColors[i] != rowColors[j]){
                    // if Hadj[rowColors[j]] contains rowColors[i], that means "rowColors[j] must be left of rowColors[i]"
                    // But in the final arrangement, rowColors[i] is left of rowColors[j].  That's not necessarily the correct direction...
                    // We have to be consistent with how we built Hadj.  Let's be careful:
                    //
                    // We said: Hadj[a].insert(b) means: "if a and b end up in the same row, 'a' must be to the LEFT of 'b'." 
                    // So if rowColors[i] = a, rowColors[j] = b, and we find b in Hadj[a], that is consistent: a must be left of b.  Good.
                    // The violation is if we find a in Hadj[b], meaning: "b must be left of a", but we see a is to the left of b.  Contradiction.
                    int a = rowColors[i];
                    int b = rowColors[j];
                    // If a is in Hadj[b], that means "b must be left of a"
                    // But we see 'a' to the left of 'b'. => conflict
                    if(Hadj[b].count(a)){
                        cout << "no\n";
                        return 0;
                    }
                }
            }
        }
    }

    // vertical check in the end arrangement
    for(int c=0; c<w; c++){
        vector<int> colColors;
        for(int r=0; r<h; r++){
            if(endB[r][c] != '.'){
                colColors.push_back(toIdx(endB[r][c]));
            }
        }
        for(int i=0; i<(int)colColors.size(); i++){
            for(int j=i+1; j<(int)colColors.size(); j++){
                if(colColors[i] != colColors[j]){
                    int a = colColors[i];
                    int b = colColors[j];
                    // If a is in Vadj[b], that means "b must be above a",
                    // but here we see a is above b => conflict
                    if(Vadj[b].count(a)){
                        cout << "no\n";
                        return 0;
                    }
                }
            }
        }
    }

    // If no conflicts found, answer yes
    cout << "yes\n";
    return 0;
}
