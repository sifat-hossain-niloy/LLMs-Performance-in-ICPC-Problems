#include <bits/stdc++.h>
using namespace std;

// Function to convert a single hex character to 4-bit binary string
string hex_to_bin(char c) {
    string bin = "";
    switch(c) {
        case '0': bin = "0000"; break;
        case '1': bin = "0001"; break;
        case '2': bin = "0010"; break;
        case '3': bin = "0011"; break;
        case '4': bin = "0100"; break;
        case '5': bin = "0101"; break;
        case '6': bin = "0110"; break;
        case '7': bin = "0111"; break;
        case '8': bin = "1000"; break;
        case '9': bin = "1001"; break;
        case 'a': bin = "1010"; break;
        case 'b': bin = "1011"; break;
        case 'c': bin = "1100"; break;
        case 'd': bin = "1101"; break;
        case 'e': bin = "1110"; break;
        case 'f': bin = "1111"; break;
        default: bin = "0000"; break;
    }
    return bin;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    int H, W;
    int case_num = 1;
    
    // Mapping from number of holes to code
    // 0: 'W', 1: 'A', 2: 'K', 3: 'J', 4: 'S', 5: 'D'
    unordered_map<int, char> hole_to_code = {
        {0, 'W'},
        {1, 'A'},
        {2, 'K'},
        {3, 'J'},
        {4, 'S'},
        {5, 'D'}
    };
    
    while(cin >> H >> W){
        if(H == 0 && W == 0){
            break;
        }
        // Read H lines
        vector<string> hex_lines(H);
        for(int i=0;i<H;i++){
            cin >> hex_lines[i];
        }
        // Convert to binary grid
        int cols = W * 4;
        vector<vector<int>> grid(H, vector<int>(cols, 0));
        for(int i=0;i<H;i++){
            for(int j=0;j<W;j++){
                string bin = hex_to_bin(hex_lines[i][j]);
                for(int k=0; k<4; k++){
                    grid[i][j*4 + k] = bin[k] - '0';
                }
            }
        }
        // Initialize background visited
        vector<vector<bool>> background_visited(H, vector<bool>(cols, false));
        // BFS to mark background
        queue<pair<int, int>> q;
        // Add all white pixels on the border
        for(int i=0;i<H;i++){
            for(int j=0; j<cols; j++){
                if(i == 0 || i == H-1 || j == 0 || j == cols-1){
                    if(grid[i][j] == 0 && !background_visited[i][j]){
                        q.push({i,j});
                        background_visited[i][j] = true;
                    }
                }
            }
        }
        // Directions for 4-connected
        int dir_r[4] = {-1, 1, 0, 0};
        int dir_c[4] = {0, 0, -1, 1};
        while(!q.empty()){
            pair<int, int> current = q.front(); q.pop();
            int r = current.first;
            int c = current.second;
            for(int d=0; d<4; d++){
                int nr = r + dir_r[d];
                int nc = c + dir_c[d];
                if(nr >=0 && nr < H && nc >=0 && nc < cols){
                    if(grid[nr][nc] == 0 && !background_visited[nr][nc]){
                        background_visited[nr][nc] = true;
                        q.push({nr, nc});
                    }
                }
            }
        }
        // Now, find hieroglyphs
        vector<vector<bool>> hieroglyph_visited(H, vector<bool>(cols, false));
        vector<char> codes;
        for(int i=0;i<H;i++){
            for(int j=0; j<cols; j++){
                if(grid[i][j] == 1 && !hieroglyph_visited[i][j]){
                    // BFS to mark the hieroglyph
                    queue<pair<int, int>> hq;
                    hq.push({i,j});
                    hieroglyph_visited[i][j] = true;
                    // To find bounding rectangle
                    int min_r = i, max_r = i, min_c = j, max_c = j;
                    vector<pair<int, int>> pixels;
                    pixels.emplace_back(i,j);
                    while(!hq.empty()){
                        pair<int, int> current = hq.front(); hq.pop();
                        int r = current.first;
                        int c = current.second;
                        // Update bounding box
                        min_r = min(min_r, r);
                        max_r = max(max_r, r);
                        min_c = min(min_c, c);
                        max_c = max(max_c, c);
                        // Check neighbors
                        for(int d=0; d<4; d++){
                            int nr = r + dir_r[d];
                            int nc = c + dir_c[d];
                            if(nr >=0 && nr < H && nc >=0 && nc < cols){
                                if(grid[nr][nc] == 1 && !hieroglyph_visited[nr][nc]){
                                    hieroglyph_visited[nr][nc] = true;
                                    hq.push({nr, nc});
                                    pixels.emplace_back(nr, nc);
                                }
                            }
                        }
                    }
                    // Now, within bounding rectangle, find number of holes
                    // Holes are white regions not connected to background
                    // So, we need to find connected white regions within bounding box not background
                    // Initialize visited_inside
                    vector<vector<bool>> visited_inside(max_r - min_r +1, vector<bool>(max_c - min_c +1, false));
                    int hole_count = 0;
                    for(int r = min_r; r <= max_r; r++){
                        for(int c = min_c; c <= max_c; c++){
                            if(grid[r][c] == 0 && !background_visited[r][c] && !visited_inside[r - min_r][c - min_c]){
                                // Found a hole
                                hole_count++;
                                // BFS to mark this hole
                                queue<pair<int, int>> hq2;
                                hq2.push({r, c});
                                visited_inside[r - min_r][c - min_c] = true;
                                while(!hq2.empty()){
                                    pair<int, int> current2 = hq2.front(); hq2.pop();
                                    int cr = current2.first;
                                    int cc = current2.second;
                                    for(int d=0; d<4; d++){
                                        int nr = cr + dir_r[d];
                                        int nc = cc + dir_c[d];
                                        if(nr >= min_r && nr <= max_r && nc >= min_c && nc <= max_c){
                                            if(grid[nr][nc] == 0 && !background_visited[nr][nc] && !visited_inside[nr - min_r][nc - min_c]){
                                                visited_inside[nr - min_r][nc - min_c] = true;
                                                hq2.push({nr, nc});
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    // Map hole_count to code
                    if(hole_to_code.find(hole_count) != hole_to_code.end()){
                        codes.push_back(hole_to_code[hole_count]);
                    }
                    else{
                        // If hole_count is not in mapping, ignore or handle as needed
                        // For this problem, it's guaranteed to match one of the hieroglyphs
                        // So we can assume it's in the mapping
                        // But to be safe, assign 'W' if not found
                        codes.push_back('W');
                    }
                }
            }
        }
        // Sort the codes alphabetically
        sort(codes.begin(), codes.end());
        // Prepare the output string
        string output = "";
        for(auto c : codes){
            output += c;
        }
        // Print the result
        cout << "Case " << case_num++ << ": " << output << "\n";
    }
}
