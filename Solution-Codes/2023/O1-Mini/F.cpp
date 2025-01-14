#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int h, w;
    cin >> h >> w;
    vector<string> start_grid(h);
    for(int i=0;i<h;i++) cin>>start_grid[i];
    string empty;
    getline(cin, empty); // read the empty line
    vector<string> target_grid(h);
    for(int i=0;i<h;i++) cin>>target_grid[i];
    
    // Function to check row alignment
    auto check_rows = [&](const vector<string> &grid) -> bool {
        for(int r=0;r<h;r++){
            int first = grid[r].find_first_of("abcdefghijklmnopqrstuvwxyz");
            if(first == string::npos){
                continue; // empty row, ok
            }
            int last = grid[r].find_last_of("abcdefghijklmnopqrstuvwxyz");
            // Check if aligned to left
            bool left = true;
            for(int c=0;c<last+1;c++){
                if(c < first) {
                    if(grid[r][c] != '.') { left = false; break; }
                }
                else{
                    if(grid[r][c] == '.') { left = false; break; }
                }
            }
            // Check if aligned to right
            bool right = true;
            for(int c=0;c<last+1;c++){
                if(c < first) {
                    if(grid[r][w-1-c] != '.') { right = false; break; }
                }
                else{
                    if(grid[r][w-1-c] == '.') { right = false; break; }
                }
            }
            // Alternatively, check contiguous and aligned to left or right
            // Check left alignment
            bool is_left = true;
            int cnt = 0;
            for(int c=0;c<w;c++) if(grid[r][c] != '.') cnt++;
            for(int c=0;c<cnt;c++) if(grid[r][c]=='.'){ is_left = false; break;}
            // Check right alignment
            bool is_right = true;
            for(int c=w-1; c>=w-cnt; c--) if(grid[r][c]=='.'){ is_right = false; break;}
            if(!(is_left || is_right)){
                return false;
            }
        }
        return true;
    };
    
    // Function to check column alignment
    auto check_cols = [&](const vector<string> &grid) -> bool {
        for(int c=0;c<w;c++){
            int first = -1;
            int last = -1;
            for(int r=0;r<h;r++) {
                if(grid[r][c] != '.'){
                    if(first == -1) first = r;
                    last = r;
                }
            }
            if(first == -1) continue; // empty column, ok
            // Check if aligned to up
            bool is_up = true;
            int cnt = 0;
            for(int r=0;r<h;r++) if(grid[r][c] != '.') cnt++;
            for(int r=0;r<cnt;r++) if(grid[r][c]=='.'){ is_up = false; break;}
            // Check if aligned to down
            bool is_down = true;
            for(int r=h-1; r>=h-cnt; r--) if(grid[r][c]=='.'){ is_down = false; break;}
            if(!(is_up || is_down)){
                return false;
            }
        }
        return true;
    };
    
    bool rows_ok = check_rows(target_grid);
    bool cols_ok = check_cols(target_grid);
    if(rows_ok && cols_ok){
        cout << "yes";
    }
    else{
        cout << "no";
    }
}
