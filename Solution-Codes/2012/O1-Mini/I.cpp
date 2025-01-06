#include <bits/stdc++.h>
using namespace std;

// Function to change direction based on mirror
int change_direction(int dir, char mirror){
    if(mirror == '/'){
        if(dir == 0) return 3; // R -> U
        if(dir == 1) return 2; // D -> L
        if(dir == 2) return 1; // L -> D
        if(dir == 3) return 0; // U -> R
    }
    else if(mirror == '\\'){
        if(dir == 0) return 1; // R -> D
        if(dir == 1) return 0; // D -> R
        if(dir == 2) return 3; // L -> U
        if(dir == 3) return 2; // U -> L
    }
    return dir;
}

// Function to simulate the beam path
pair<int, int> simulate_beam(const vector<vector<char>> &grid, int r, int c){
    int x = 1, y = 1;
    int dir = 0; // 0: Right, 1: Down, 2: Left, 3: Up

    while(x >=1 && x <=r && y >=1 && y <=c){
        char cell = grid[x][y];
        if(cell == '/' || cell == '\\'){
            dir = change_direction(dir, cell);
        }
        // Move to next cell based on direction
        if(dir == 0){
            y++;
        }
        else if(dir == 1){
            x++;
        }
        else if(dir == 2){
            y--;
        }
        else if(dir == 3){
            x--;
        }
    }
    return {x, y};
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int r, c, m, n;
    int case_num = 1;
    while(cin >> r >> c >> m >> n){
        // Initialize grid with '.'
        vector<vector<char>> grid(r+1, vector<char>(c+1, '.'));
        // Read '/' mirrors
        for(int i=0; i<m; ++i){
            int x, y;
            cin >> x >> y;
            grid[x][y] = '/';
        }
        // Read '\' mirrors
        for(int i=0; i<n; ++i){
            int x, y;
            cin >> x >> y;
            grid[x][y] = '\\';
        }
        // Simulate beam traversal
        pair<int, int> exit_point = simulate_beam(grid, r, c);
        if(exit_point.first == r && exit_point.second == c+1){
            cout << "Case " << case_num++ << ": 0\n";
            continue;
        }
        // Find all possible insertion positions
        vector<pair<int, int>> possible;
        for(int x=1; x<=r; ++x){
            for(int y=1; y<=c; ++y){
                if(grid[x][y] == '.'){
                    bool can_reach = false;
                    // Try inserting '/'
                    grid[x][y] = '/';
                    pair<int, int> temp_exit = simulate_beam(grid, r, c);
                    if(temp_exit.first == r && temp_exit.second == c+1){
                        can_reach = true;
                    }
                    // Try inserting '\'
                    grid[x][y] = '\\';
                    temp_exit = simulate_beam(grid, r, c);
                    if(temp_exit.first == r && temp_exit.second == c+1){
                        can_reach = true;
                    }
                    // Reset the cell
                    grid[x][y] = '.';
                    if(can_reach){
                        possible.emplace_back(x, y);
                    }
                }
            }
        }
        if(!possible.empty()){
            // Find the smallest (x,y)
            sort(possible.begin(), possible.end());
            // Count unique positions
            // Since each position is counted once, even if both '/' and '\' work
            int k = possible.size();
            cout << "Case " << case_num++ << ": " << k << " " << possible[0].first << " " << possible[0].second << "\n";
        }
        else{
            cout << "Case " << case_num++ << ": impossible\n";
        }
    }
}
