#include <bits/stdc++.h>
using namespace std;

struct Node {
    int child0;
    int child1;
    int count;
    Node() : child0(-1), child1(-1), count(0) {}
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int dx, dy;
    cin >> dx >> dy;
    // Initialize grid[y][x], 1-based indices
    vector<vector<char>> grid(dy+2, vector<char>(dx+2, '.'));
    for(int l=1; l<=dy; l++){
        string line;
        cin >> line;
        int y = dy - l +1;
        for(int x=1; x<=dx; x++){
            grid[y][x] = line[x-1];
        }
    }
    // Generate spiral order up to 500 steps
    vector<pair<int, int>> spiral_order;
    spiral_order.emplace_back(0,0); // step 1
    int x =0, y=0;
    int step_length =1;
    int direction =0;
    // Directions: right, up, left, down
    int dx_dir[4] = {1,0,-1,0};
    int dy_dir_order[4] = {0,1,0,-1};
    while(spiral_order.size() < 500){
        for(int i=0; i<2; i++){
            for(int s=1; s<=step_length; s++){
                x += dx_dir[direction];
                y += dy_dir_order[direction];
                spiral_order.emplace_back(x,y);
                if(spiral_order.size() >=500) break;
            }
            direction = (direction +1 )%4;
            if(spiral_order.size() >=500) break;
        }
        step_length +=1;
    }
    // Initialize starting cells
    vector<pair<int,int>> starting_cells;
    for(int y_start=1; y_start<=dy; y_start++){
        for(int x_start=1; x_start<=dx; x_start++){
            starting_cells.emplace_back(x_start, y_start);
        }
    }
    int N = starting_cells.size();
    // Initialize trie
    vector<Node> trie;
    trie.emplace_back();
    trie[0].count = N;
    // Initialize current nodes and uniqueness steps
    vector<int> current_nodes(N, 0);
    vector<int> uniqueness_steps(N, 0);
    // Initialize active list
    vector<int> active_list(N);
    for(int i=0; i<N; i++) active_list[i] = i;
    // Main loop
    int step =1;
    while(!active_list.empty() && step <= spiral_order.size()){
        pair<int, int> pos = spiral_order[step-1];
        vector<int> new_active_list;
        for(auto cell: active_list){
            int x_start = starting_cells[cell].first;
            int y_start = starting_cells[cell].second;
            int new_x = x_start + pos.first;
            int new_y = y_start + pos.second;
            int obs =0;
            if(new_x >=1 && new_x <=dx && new_y >=1 && new_y <=dy){
                obs = (grid[new_y][new_x] == 'X') ? 1 : 0;
            }
            int current_node = current_nodes[cell];
            int child;
            if(obs ==0){
                if(trie[current_node].child0 == -1){
                    trie.emplace_back();
                    trie[current_node].child0 = trie.size()-1;
                }
                child = trie[current_node].child0;
            }
            else{
                if(trie[current_node].child1 == -1){
                    trie.emplace_back();
                    trie[current_node].child1 = trie.size()-1;
                }
                child = trie[current_node].child1;
            }
            trie[child].count +=1;
            current_nodes[cell] = child;
            if(trie[child].count ==1){
                uniqueness_steps[cell] = step;
            }
            else{
                new_active_list.push_back(cell);
            }
        }
        active_list = move(new_active_list);
        step +=1;
    }
    // Now, all uniqueness_steps should be set
    // Compute average and max
    double sum_steps =0.0;
    int max_step_val =0;
    for(int i=0; i<N; i++){
        sum_steps += uniqueness_steps[i];
        if(uniqueness_steps[i] > max_step_val){
            max_step_val = uniqueness_steps[i];
        }
    }
    double average = sum_steps / N;
    // Collect cells with max_step_val
    vector<pair<int,int>> max_cells;
    for(int i=0; i<N; i++){
        if(uniqueness_steps[i] == max_step_val){
            max_cells.emplace_back(starting_cells[i].first, starting_cells[i].second);
        }
    }
    // Sort max_cells by y increasing, then x increasing
    sort(max_cells.begin(), max_cells.end(), [&](const pair<int,int> &a, const pair<int,int> &b)->bool{
        if(a.second != b.second) return a.second < b.second;
        return a.first < b.first;
    });
    // Output
    cout << fixed << setprecision(3) << average << "\n";
    cout << max_step_val << "\n";
    for(int i=0; i<max_cells.size(); i++){
        if(i >0) cout << " ";
        cout << "(" << max_cells[i].first << "," << max_cells[i].second << ")";
    }
    cout << "\n";
}
