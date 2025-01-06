#include <bits/stdc++.h>
using namespace std;

struct Window {
    long long x, y, w, h;
    int order; // for ordering
};

bool contains(const Window& W, long long px, long long py){
    return (px >= W.x) && (px < W.x + W.w) && (py >= W.y) && (py < W.y + W.h);
}

bool overlaps(const Window& A, const Window& B){
    return (A.x < B.x + B.w) && (B.x < A.x + A.w) &&
           (A.y < B.y + B.h) && (B.y < A.y + A.h);
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    long long xmax, ymax;
    cin >> xmax >> ymax;
    cin.ignore(); // ignore the rest of the line
    string line;
    vector<Window> windows;
    int cmd_num = 0;
    int order_counter = 0;
    while(getline(cin, line)){
        if(line.empty()) continue;
        cmd_num++;
        // Parse the command
        stringstream ss(line);
        string cmd;
        ss >> cmd;
        if(cmd == "OPEN"){
            long long x, y, w, h;
            ss >> x >> y >> w >> h;
            // Check within screen
            bool fit = (x >=0 && y >=0 && x + w <= xmax && y + h <= ymax);
            // Check no overlap
            if(fit){
                for(auto &win: windows){
                    Window new_win = {x, y, w, h, -1};
                    if(overlaps(win, new_win)){
                        fit = false;
                        break;
                    }
                }
            }
            if(fit){
                Window new_win = {x, y, w, h, order_counter++};
                windows.push_back(new_win);
            }
            else{
                cout << "Command " << cmd_num << ": OPEN - window does not fit\n";
            }
        }
        else if(cmd == "CLOSE"){
            long long x, y;
            ss >> x >> y;
            int found = -1;
            for(int i=0;i<windows.size();i++){
                if(contains(windows[i], x, y)){
                    found = i;
                    break;
                }
            }
            if(found != -1){
                windows.erase(windows.begin()+found);
            }
            else{
                cout << "Command " << cmd_num << ": CLOSE - no window at given position\n";
            }
        }
        else if(cmd == "RESIZE"){
            long long x, y, w_new, h_new;
            ss >> x >> y >> w_new >> h_new;
            int found = -1;
            for(int i=0;i<windows.size();i++){
                if(contains(windows[i], x, y)){
                    found = i;
                    break;
                }
            }
            if(found == -1){
                cout << "Command " << cmd_num << ": RESIZE - no window at given position\n";
            }
            else{
                // Check new window within screen
                Window old_win = windows[found];
                Window new_win = {old_win.x, old_win.y, w_new, h_new, old_win.order};
                bool fit = (new_win.x >=0 && new_win.y >=0 && new_win.x + new_win.w <= xmax && new_win.y + new_win.h <= ymax);
                // Check no overlap with other windows
                if(fit){
                    for(int i=0;i<windows.size();i++){
                        if(i == found) continue;
                        if(overlaps(windows[i], new_win)){
                            fit = false;
                            break;
                        }
                    }
                }
                if(fit){
                    windows[found].w = w_new;
                    windows[found].h = h_new;
                }
                else{
                    cout << "Command " << cmd_num << ": RESIZE - window does not fit\n";
                }
            }
        }
        else if(cmd == "MOVE"){
            long long x, y, dx, dy;
            ss >> x >> y >> dx >> dy;
            int found = -1;
            for(int i=0;i<windows.size();i++){
                if(contains(windows[i], x, y)){
                    found = i;
                    break;
                }
            }
            if(found == -1){
                cout << "Command " << cmd_num << ": MOVE - no window at given position\n";
            }
            else{
                // Determine direction and d
                string direction;
                int dir = 0; // 1:right, -1:left, 2:down, -2:up
                long long d =0;
                if(dx !=0){
                    if(dx >0){
                        direction = "right";
                        dir = 1;
                    }
                    else{
                        direction = "left";
                        dir = -1;
                    }
                    d = abs(dx);
                }
                else{
                    if(dy >0){
                        direction = "down";
                        dir = 2;
                    }
                    else{
                        direction = "up";
                        dir = -2;
                    }
                    d = abs(dy);
                }
                if(d ==0){
                    // No movement
                    // Do nothing
                    continue;
                }
                // Collect to_move
                // Use indices
                vector<int> to_move;
                vector<bool> in_move(windows.size(), false);
                queue<int> q;
                to_move.push_back(found);
                in_move[found] = true;
                q.push(found);
                while(!q.empty()){
                    int current = q.front(); q.pop();
                    // Compute new position after moving by d in direction
                    Window shifted = windows[current];
                    if(dir ==1){
                        shifted.x += d;
                    }
                    else if(dir ==-1){
                        shifted.x -= d;
                    }
                    else if(dir ==2){
                        shifted.y += d;
                    }
                    else if(dir ==-2){
                        shifted.y -= d;
                    }
                    // Check overlap with other windows
                    for(int i=0;i<windows.size();i++){
                        if(in_move[i]) continue;
                        if(overlaps(shifted, windows[i])){
                            to_move.push_back(i);
                            in_move[i] = true;
                            q.push(i);
                        }
                    }
                }
                // Now, compute d'
                long long d_min_needed = 0;
                for(auto &i: to_move){
                    // For each window in to_move
                    // Check all windows not in to_move that overlap in direction
                    for(int j=0; j<windows.size(); j++){
                        if(in_move[j]) continue;
                        // Check overlapping in direction
                        bool overlap_dir = false;
                        if(dir ==1 || dir ==-1){
                            // horizontal overlap
                            // Check vertical overlap
                            if(!(windows[i].y + windows[i].h <= windows[j].y || windows[j].y + windows[j].h <= windows[i].y)){
                                overlap_dir = true;
                            }
                        }
                        else{
                            // vertical overlap
                            // Check horizontal overlap
                            if(!(windows[i].x + windows[i].w <= windows[j].x || windows[j].x + windows[j].w <= windows[i].x)){
                                overlap_dir = true;
                            }
                        }
                        if(overlap_dir){
                            // Depending on direction, compute d_min_needed
                            if(dir ==1){
                                // moving right
                                if(windows[i].x + windows[i].w > windows[j].x + windows[j].w){
                                    // No need to shift if already beyond
                                    if(windows[i].x + d + windows[i].w > windows[j].x + windows[j].w){
                                        // To not overlap, need to have windows[i].x +d >= windows[j].x +windows[j].w
                                        d_min_needed = max(d_min_needed, windows[j].x + windows[j].w - windows[i].x);
                                    }
                                }
                                else{
                                    d_min_needed = max(d_min_needed, windows[j].x + windows[j].w - windows[i].x);
                                }
                            }
                            else if(dir ==-1){
                                // moving left
                                if(windows[i].x < windows[j].x){
                                    if(windows[i].x - d < windows[j].x + windows[j].w){
                                        d_min_needed = max(d_min_needed, windows[i].x - (windows[j].x + windows[j].w));
                                    }
                                }
                                else{
                                    d_min_needed = max(d_min_needed, windows[j].x + windows[j].w - windows[i].x);
                                }
                            }
                            else if(dir ==2){
                                // moving down
                                if(windows[i].y + windows[i].h > windows[j].y + windows[j].h){
                                    if(windows[i].y + d >= windows[j].y + windows[j].h){
                                        d_min_needed = max(d_min_needed, windows[j].y + windows[j].h - windows[i].y);
                                    }
                                }
                                else{
                                    d_min_needed = max(d_min_needed, windows[j].y + windows[j].h - windows[i].y);
                                }
                            }
                            else if(dir ==-2){
                                // moving up
                                if(windows[i].y < windows[j].y){
                                    if(windows[i].y - d < windows[j].y + windows[j].h){
                                        d_min_needed = max(d_min_needed, windows[i].y - (windows[j].y + windows[j].h));
                                    }
                                }
                                else{
                                    d_min_needed = max(d_min_needed, windows[j].y + windows[j].h - windows[i].y);
                                }
                            }
                        }
                    }
                }
                // Compute d_min_needed correctly
                // To simplify, recompute d_min_needed as the maximum (W2.x +w2 - W.x) for moving right
                // and similar for other directions
                // Recompute based on direction
                d_min_needed = 0;
                for(auto &i: to_move){
                    for(int j=0; j<windows.size(); j++){
                        if(in_move[j]) continue;
                        // Check overlapping in direction
                        bool overlap_dir = false;
                        if(dir ==1 || dir ==-1){
                            // horizontal overlap
                            // Check vertical overlap
                            if(!(windows[i].y + windows[i].h <= windows[j].y || windows[j].y + windows[j].h <= windows[i].y)){
                                overlap_dir = true;
                            }
                        }
                        else{
                            // vertical overlap
                            // Check horizontal overlap
                            if(!(windows[i].x + windows[i].w <= windows[j].x || windows[j].x + windows[j].w <= windows[i].x)){
                                overlap_dir = true;
                            }
                        }
                        if(overlap_dir){
                            if(dir ==1){
                                // moving right
                                d_min_needed = max(d_min_needed, windows[j].x + windows[j].w - windows[i].x);
                            }
                            else if(dir ==-1){
                                // moving left
                                d_min_needed = max(d_min_needed, windows[i].x - (windows[j].x + windows[j].w));
                            }
                            else if(dir ==2){
                                // moving down
                                d_min_needed = max(d_min_needed, windows[j].y + windows[j].h - windows[i].y);
                            }
                            else if(dir ==-2){
                                // moving up
                                d_min_needed = max(d_min_needed, windows[i].y - (windows[j].y + windows[j].h));
                            }
                        }
                    }
                }
                // Now compute d'_max_screen
                long long d_max_screen = LLONG_MAX;
                for(auto &i: to_move){
                    if(dir ==1){
                        d_max_screen = min(d_max_screen, xmax - (windows[i].x + windows[i].w));
                    }
                    else if(dir ==-1){
                        d_max_screen = min(d_max_screen, windows[i].x);
                    }
                    else if(dir ==2){
                        d_max_screen = min(d_max_screen, ymax - (windows[i].y + windows[i].h));
                    }
                    else if(dir ==-2){
                        d_max_screen = min(d_max_screen, windows[i].y);
                    }
                }
                // Now decide d'
                long long d_prime = d;
                if(dir ==1){
                    // moving right
                    // Compute d_min_needed
                    long long current_d_min = 0;
                    for(auto &i: to_move){
                        for(int j=0; j<windows.size(); j++){
                            if(in_move[j]) continue;
                            // Check overlapping vertically
                            if(!(windows[i].y + windows[i].h <= windows[j].y || windows[j].y + windows[j].h <= windows[i].y)){
                                // Need W.x +d >= W2.x +w2
                                current_d_min = max(current_d_min, windows[j].x + windows[j].w - windows[i].x);
                            }
                        }
                    }
                    if(d >= current_d_min){
                        d_prime = min(d, d_max_screen);
                    }
                    else{
                        // Find the minimum (W2.x - W.x -w) for overlapping windows
                        long long d_limit = LLONG_MAX;
                        for(auto &i: to_move){
                            for(int j=0; j<windows.size(); j++){
                                if(in_move[j]) continue;
                                // Check overlapping vertically
                                if(!(windows[i].y + windows[i].h <= windows[j].y || windows[j].y + windows[j].h <= windows[i].y)){
                                    if(windows[j].x > windows[i].x + windows[i].w){
                                        d_limit = min(d_limit, windows[j].x - windows[i].x - windows[i].w);
                                    }
                                }
                            }
                        }
                        if(d_limit != LLONG_MAX){
                            d_prime = min(d, d_limit);
                        }
                        else{
                            d_prime = min(d, d_max_screen);
                        }
                    }
                }
                else if(dir ==-1){
                    // moving left
                    long long current_d_min = 0;
                    for(auto &i: to_move){
                        for(int j=0; j<windows.size(); j++){
                            if(in_move[j]) continue;
                            // Check overlapping vertically
                            if(!(windows[i].y + windows[i].h <= windows[j].y || windows[j].y + windows[j].h <= windows[i].y)){
                                // Need W.x -d >= W2.x +w2
                                current_d_min = max(current_d_min, windows[i].x - (windows[j].x + windows[j].w));
                            }
                        }
                    }
                    if(d >= current_d_min){
                        d_prime = min(d, d_max_screen);
                    }
                    else{
                        // Find the minimum (W.x - W2.x -w2) for overlapping windows
                        long long d_limit = LLONG_MAX;
                        for(auto &i: to_move){
                            for(int j=0; j<windows.size(); j++){
                                if(in_move[j]) continue;
                                // Check overlapping vertically
                                if(!(windows[i].y + windows[i].h <= windows[j].y || windows[j].y + windows[j].h <= windows[i].y)){
                                    if(windows[j].x + windows[j].w <= windows[i].x){
                                        d_limit = min(d_limit, windows[i].x - (windows[j].x + windows[j].w));
                                    }
                                }
                            }
                        }
                        if(d_limit != LLONG_MAX){
                            d_prime = min(d, d_limit);
                        }
                        else{
                            d_prime = min(d, d_max_screen);
                        }
                    }
                }
                else if(dir ==2){
                    // moving down
                    long long current_d_min = 0;
                    for(auto &i: to_move){
                        for(int j=0; j<windows.size(); j++){
                            if(in_move[j]) continue;
                            // Check overlapping horizontally
                            if(!(windows[i].x + windows[i].w <= windows[j].x || windows[j].x + windows[j].w <= windows[i].x)){
                                // Need W.y +d >= W2.y +w2
                                current_d_min = max(current_d_min, windows[j].y + windows[j].h - windows[i].y);
                            }
                        }
                    }
                    if(d >= current_d_min){
                        d_prime = min(d, d_max_screen);
                    }
                    else{
                        // Find the minimum (W2.y - W.y -h) for overlapping windows
                        long long d_limit = LLONG_MAX;
                        for(auto &i: to_move){
                            for(int j=0; j<windows.size(); j++){
                                if(in_move[j]) continue;
                                // Check overlapping horizontally
                                if(!(windows[i].x + windows[i].w <= windows[j].x || windows[j].x + windows[j].w <= windows[i].x)){
                                    if(windows[j].y > windows[i].y + windows[i].h){
                                        d_limit = min(d_limit, windows[j].y - windows[i].y - windows[i].h);
                                    }
                                }
                            }
                        }
                        if(d_limit != LLONG_MAX){
                            d_prime = min(d, d_limit);
                        }
                        else{
                            d_prime = min(d, d_max_screen);
                        }
                    }
                }
                else if(dir ==-2){
                    // moving up
                    long long current_d_min = 0;
                    for(auto &i: to_move){
                        for(int j=0; j<windows.size(); j++){
                            if(in_move[j]) continue;
                            // Check overlapping horizontally
                            if(!(windows[i].x + windows[i].w <= windows[j].x || windows[j].x + windows[j].w <= windows[i].x)){
                                // Need W.y -d >= W2.y +h2
                                current_d_min = max(current_d_min, windows[i].y - (windows[j].y + windows[j].h));
                            }
                        }
                    }
                    if(d >= current_d_min){
                        d_prime = min(d, d_max_screen);
                    }
                    else{
                        // Find the minimum (W.y - W2.y -h2) for overlapping windows
                        long long d_limit = LLONG_MAX;
                        for(auto &i: to_move){
                            for(int j=0; j<windows.size(); j++){
                                if(in_move[j]) continue;
                                // Check overlapping horizontally
                                if(!(windows[i].x + windows[i].w <= windows[j].x || windows[j].x + windows[j].w <= windows[i].x)){
                                    if(windows[j].y + windows[j].h <= windows[i].y){
                                        d_limit = min(d_limit, windows[i].y - (windows[j].y + windows[j].h));
                                    }
                                }
                            }
                        }
                        if(d_limit != LLONG_MAX){
                            d_prime = min(d, d_limit);
                        }
                        else{
                            d_prime = min(d, d_max_screen);
                        }
                    }
                }
                // Now, check if d' < d
                if(d_prime < d){
                    cout << "Command " << cmd_num << ": MOVE - moved " << d_prime << " instead of " << d << "\n";
                }
                // Now, move all windows in to_move by d_prime in direction
                for(auto &i: to_move){
                    if(dir ==1){
                        windows[i].x += d_prime;
                    }
                    else if(dir ==-1){
                        windows[i].x -= d_prime;
                    }
                    else if(dir ==2){
                        windows[i].y += d_prime;
                    }
                    else if(dir ==-2){
                        windows[i].y -= d_prime;
                    }
                }
            }
        }
    }
    // After all commands, output the windows
    cout << windows.size() << " window(s):\n";
    for(auto &win: windows){
        cout << win.x << " " << win.y << " " << win.w << " " << win.h << "\n";
    }
}
