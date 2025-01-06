#include <bits/stdc++.h>
using namespace std;

struct Wind {
    int wx, wy;
    vector<char> boundary; // 1 if boundary, 0 otherwise
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int dx, dy, k;
    cin >> dx >> dy >> k;
    vector<Wind> winds(k, Wind());
    for(int i=0;i<k;i++){
        cin >> winds[i].wx >> winds[i].wy;
        int b;
        cin >> b;
        winds[i].boundary.assign(dx * dy, 0);
        for(int j=0; j<b; j++){
            int x, y;
            cin >> x >> y;
            if(x <1 || x > dx || y <1 || y > dy){
                // invalid, but problem states input is consistent
                continue;
            }
            int index = (y-1)*dx + (x-1);
            winds[i].boundary[index] =1;
        }
    }
    // Min assignment
    vector<char> grid_min(dx * dy, 0);
    // Initialize grid_min and set to 1 for boundaries
    // Also set to 0 for (x - wx, y - wy) for boundaries
    // and enqueue boundaries
    queue<int> q_min;
    for(int w=0; w<k; w++){
        for(int index=0; index<dx*dy; index++){
            if(winds[w].boundary[index]){
                if(grid_min[index]==0){
                    grid_min[index]=1;
                    q_min.push(index);
                }
                // Compute (x - wx, y - wy)
                int x = index % dx +1;
                int y = index / dx +1;
                int x_prev = x - winds[w].wx;
                int y_prev = y - winds[w].wy;
                if(x_prev >=1 && x_prev <=dx && y_prev >=1 && y_prev <=dy){
                    int index_prev = (y_prev -1)*dx + (x_prev -1);
                    if(grid_min[index_prev]==1){
                        grid_min[index_prev]=0;
                        // No need to enqueue for min assignment when setting to 0
                    }
                }
            }
        }
    }
    // Enqueue all boundary positions
    for(int w=0; w<k; w++){
        for(int index=0; index<dx*dy; index++){
            if(winds[w].boundary[index]){
                q_min.push(index);
            }
        }
    }
    // BFS for min assignment
    while(!q_min.empty()){
        int index = q_min.front();
        q_min.pop();
        int x = index % dx +1;
        int y = index / dx +1;
        for(int w=0; w<k; w++){
            if(!winds[w].boundary[index]){
                int x_prev = x - winds[w].wx;
                int y_prev = y - winds[w].wy;
                if(x_prev >=1 && x_prev <=dx && y_prev >=1 && y_prev <=dy){
                    int index_prev = (y_prev -1)*dx + (x_prev -1);
                    if(grid_min[index_prev]==0){
                        grid_min[index_prev]=1;
                        q_min.push(index_prev);
                    }
                }
            }
        }
    }
    // Max assignment
    vector<char> grid_max(dx * dy, 1);
    queue<int> q_max;
    for(int w=0; w<k; w++){
        for(int index=0; index<dx*dy; index++){
            if(winds[w].boundary[index]){
                // Molecule(x,y) is already 1
                // Set (x - wx, y - wy) to 0
                int x = index % dx +1;
                int y = index / dx +1;
                int x_prev = x - winds[w].wx;
                int y_prev = y - winds[w].wy;
                if(x_prev >=1 && x_prev <=dx && y_prev >=1 && y_prev <=dy){
                    int index_prev = (y_prev -1)*dx + (x_prev -1);
                    if(grid_max[index_prev]==1){
                        grid_max[index_prev]=0;
                        q_max.push(index_prev);
                    }
                }
            }
        }
    }
    // BFS for max assignment
    while(!q_max.empty()){
        int index = q_max.front();
        q_max.pop();
        int x = index % dx +1;
        int y = index / dx +1;
        for(int w=0; w<k; w++){
            int a = x + winds[w].wx;
            int b = y + winds[w].wy;
            if(a >=1 && a <=dx && b >=1 && b <=dy){
                int index_ab = (b -1)*dx + (a -1);
                if(!winds[w].boundary[index_ab]){
                    if(grid_max[index_ab]==1){
                        grid_max[index_ab]=0;
                        q_max.push(index_ab);
                    }
                }
            }
        }
    }
    // Now, output grid_min
    // The top-left corner is (1,1), which corresponds to y=1
    // So iterate y from 1 to dy
    for(int y=1; y<=dy; y++){
        string line;
        for(int x=1; x<=dx; x++){
            int index = (y-1)*dx + (x-1);
            if(grid_min[index]){
                line += '#';
            }
            else{
                line += '.';
            }
        }
        cout << line << '\n';
    }
    cout << '\n';
    // Output grid_max
    for(int y=1; y<=dy; y++){
        string line;
        for(int x=1; x<=dx; x++){
            int index = (y-1)*dx + (x-1);
            if(grid_max[index]){
                line += '#';
            }
            else{
                line += '.';
            }
        }
        cout << line << '\n';
    }
}
