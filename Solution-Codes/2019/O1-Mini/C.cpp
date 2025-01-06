#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    char c;
    int n;
    cin >> c >> n;
    vector<string> moves(n);
    for(int i=0;i<n;i++) cin>>moves[i];
    // Initialize boards
    char board[33];
    for(int i=1;i<=32;i++) board[i] = '.';
    char initial_board[33];
    char final_board[33];
    // Define enemy based on player
    auto get_enemy = [&](char player) -> char{
        if(player == 'B') return 'w';
        else return 'b';
    };
    // Define player piece
    auto get_player_piece = [&](char player) -> char{
        if(player == 'B') return 'b';
        else return 'w';
    };
    // Define promotion row
    auto get_promotion_row = [&](char player) -> int{
        if(player == 'B') return 8;
        else return 1;
    };
    // Define square_to_rc and rc_to_square
    auto square_to_rc = [&](int s) -> pair<int,int>{
        int r = (s-1)/4 +1;
        int pos_in_row = (s-1)%4;
        int c;
        if(r %2 ==1){
            c = 2 + pos_in_row*2;
        }
        else{
            c =1 + pos_in_row*2;
        }
        return {r, c};
    };
    auto rc_to_square = [&](int r, int c) -> int{
        if(r <1 || r >8 || c <1 || c >8) return -1;
        if(r %2 ==1){
            if(c %2 !=0 || c <2 || c >8) return -1;
            return (r-1)*4 + (c / 2);
        }
        else{
            if(c %2 !=1 || c <1 || c >7) return -1;
            return (r-1)*4 + ((c +1)/2);
        }
    };
    // Function to get jumped square
    auto get_jumped_square = [&](int x, int y) -> int{
        pair<int,int> rx = square_to_rc(x);
        pair<int,int> ry = square_to_rc(y);
        int rz_r = (rx.first + ry.first)/2;
        int rz_c = (rx.second + ry.second)/2;
        return rc_to_square(rz_r, rz_c);
    };
    // Current player
    char player = c;
    for(int i=0;i<n;i++){
        string move = moves[i];
        // Parse the move
        // Check if it's a simple move or jump
        if(move.find('-') != string::npos){
            // Simple move
            int dash = move.find('-');
            int a = stoi(move.substr(0, dash));
            int b = stoi(move.substr(dash+1));
            // Place player's piece at a if not present
            if(board[a] == '.'){
                board[a] = (player == 'B') ? 'b' : 'w';
            }
            // Ensure b is empty (already '.')
            // Ensure no enemy pieces on the board
            char enemy = get_enemy(player);
            bool has_enemy = false;
            for(int s=1;s<=32;s++) if(board[s]==enemy || board[s]==toupper(enemy)){
                has_enemy = true;
                break;
            }
            // If has enemy, it's invalid, but since a solution exists, assume no enemy pieces
            // Thus, do nothing
            // Now, if this is the first move, save the initial board
            if(i ==0){
                for(int s=1;s<=32;s++) initial_board[s] = board[s];
            }
            // Perform the move
            char piece = board[a];
            board[a] = '.';
            board[b] = piece;
            // Check promotion
            pair<int,int> rc = square_to_rc(b);
            int r = rc.first;
            if(player == 'B' && r ==8 && board[b] == 'b'){
                board[b] = 'B';
            }
            if(player == 'W' && r ==1 && board[b] == 'w'){
                board[b] = 'W';
            }
        }
        else{
            // Jump move
            // Split by 'x'
            vector<int> path;
            int pos =0;
            while(pos < move.size()){
                int x = move.find('x', pos);
                if(x == string::npos){
                    path.push_back(stoi(move.substr(pos)));
                    break;
                }
                else{
                    path.push_back(stoi(move.substr(pos, x-pos)));
                    pos = x+1;
                }
            }
            // Now path has a, b1, b2, ..., bk
            int a = path[0];
            // Place player's piece at a if not present
            if(board[a] == '.'){
                board[a] = (player == 'B') ? 'b' : 'w';
            }
            // Place enemy pieces at jumped squares
            char enemy = get_enemy(player);
            for(int j=1; j<path.size();j++){
                int x = path[j-1];
                int y = path[j];
                int z = get_jumped_square(x,y);
                if(z == -1) continue; // invalid, but assume correct
                if(board[z] == '.'){
                    board[z] = enemy;
                }
            }
            // Now, if this is the first move, save the initial board
            if(i ==0){
                for(int s=1;s<=32;s++) initial_board[s] = board[s];
            }
            // Perform the jumps
            int current_pos = a;
            for(int j=1; j<path.size();j++){
                int y = path[j];
                int z = get_jumped_square(current_pos, y);
                board[z] = '.'; // capture
                char piece = board[current_pos];
                board[current_pos] = '.';
                board[y] = piece;
                current_pos = y;
            }
            // Check promotion
            pair<int,int> rc = square_to_rc(current_pos);
            int r = rc.first;
            if(player == 'B' && r ==8 && board[current_pos] == 'b'){
                board[current_pos] = 'B';
            }
            if(player == 'W' && r ==1 && board[current_pos] == 'w'){
                board[current_pos] = 'W';
            }
        }
        // Switch player
        player = (player == 'B') ? 'W' : 'B';
    }
    // After all moves, copy to final_board
    for(int s=1;s<=32;s++) final_board[s] = board[s];
    // Now, output the boards
    for(int r=1;r<=8;r++){
        string initial_row = "";
        string final_row = "";
        for(int c=1;c<=8;c++){
            if( (r + c) %2 ==0 ){
                initial_row += '-';
                final_row += '-';
            }
            else{
                int s =0;
                if(r %2 ==1){
                    if(c %2 !=0 || c <2 || c >8){
                        // invalid, but assume correct
                        s = -1;
                    }
                    else{
                        s = (r-1)*4 + (c / 2);
                    }
                }
                else{
                    if(c %2 !=1 || c <1 || c >7){
                        // invalid
                        s = -1;
                    }
                    else{
                        s = (r-1)*4 + ((c +1)/2);
                    }
                }
                if(s >=1 && s <=32){
                    char ip = initial_board[s];
                    char fp = final_board[s];
                    if(ip == '.') initial_row += '.';
                    else initial_row += ip;
                    if(fp == '.') final_row += '.';
                    else final_row += fp;
                }
                else{
                    // invalid
                    initial_row += '-';
                    final_row += '-';
                }
            }
        }
        cout<<initial_row<<" "<<final_row<<'\n';
    }
}
