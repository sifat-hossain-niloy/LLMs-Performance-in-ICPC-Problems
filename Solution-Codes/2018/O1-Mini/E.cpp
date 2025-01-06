#include <bits/stdc++.h>
using namespace std;

struct Building {
    double x;
    double y;
    int height;
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int dx, dy, w;
    double v;
    int lx, ly;
    cin >> dx >> dy >> w >> v >> lx >> ly;
    // Read heights
    vector<vector<int>> heights(dy, vector<int>(dx));
    for(int y=0; y<dy; y++){
        for(int x=0; x<dx; x++) cin >> heights[y][x];
    }
    // Compute positions
    int N = dx * dy;
    vector<Building> buildings(N);
    for(int y=0; y<dy; y++){
        for(int x=0; x<dx; x++){
            int idx = y*dx + x;
            double pos_x = x * w + w / 2.0;
            double pos_y = y * w + w / 2.0;
            buildings[idx].x = pos_x;
            buildings[idx].y = pos_y;
            buildings[idx].height = heights[y][x];
        }
    }
    // Start index
    int start = (ly-1)*dx + (lx-1);
    // Build graph
    double g_val = 9.80665;
    double half_diag = w * sqrt(2.0) / 2.0;
    vector<vector<int>> adj(N, vector<int>());
    for(int A=0; A<N; A++){
        double x_A = buildings[A].x;
        double y_A = buildings[A].y;
        int h_A = buildings[A].height;
        for(int B=0; B<N; B++){
            if(A == B) continue;
            double x_B = buildings[B].x;
            double y_B = buildings[B].y;
            int h_B = buildings[B].height;
            double delta_x = x_B - x_A;
            double delta_y = y_B - y_A;
            double d_sq = delta_x * delta_x + delta_y * delta_y;
            double d = sqrt(d_sq);
            double h = (double)(h_B - h_A);
            // Quadratic coefficients based on derived equation
            double A_quad = 4.0 * h * h + 4.0 * d * d;
            double B_coeff = 4.0 * h * g_val * d * d - 4.0 * v * v * d * d;
            double C_coeff = g_val * g_val * pow(d, 4.0);
            double D = B_coeff * B_coeff - 4.0 * A_quad * C_coeff;
            if(D < -1e-6) continue;
            if(D < 0.0) D = 0.0;
            double sqrt_D = sqrt(D);
            // Possible solutions for v_x^2
            vector<double> v_x2_s;
            double v_x2_1 = (-B_coeff + sqrt_D) / (2.0 * A_quad);
            double v_x2_2 = (-B_coeff - sqrt_D) / (2.0 * A_quad);
            v_x2_s.push_back(v_x2_1);
            v_x2_s.push_back(v_x2_2);
            bool jump_possible = false;
            for(auto v_x2 : v_x2_s){
                if(v_x2 <= 1e-6 || v_x2 > v * v + 1e-6) continue;
                double v_x = sqrt(v_x2);
                double v_y_sq = v * v - v_x2;
                if(v_y_sq < -1e-6) continue;
                double v_y = v_y_sq > 0.0 ? sqrt(v_y_sq) : 0.0;
                // Find buildings C on the path
                bool collision = false;
                for(int C=0; C<N; C++){
                    if(C == A || C == B) continue;
                    double x_C = buildings[C].x;
                    double y_C = buildings[C].y;
                    double AC_x = x_C - x_A;
                    double AC_y = y_C - y_A;
                    double s = (delta_x * AC_x + delta_y * AC_y) / d_sq;
                    if(s <= 1e-9 || s >= 1.0 - 1e-9) continue;
                    double distance = abs(delta_x * (y_A - y_C) - delta_y * (x_A - x_C)) / d;
                    if(distance > half_diag + 1e-9) continue;
                    // Compute y(x_C_along)
                    double x_C_along = s * d;
                    double y_traj = (v_y / v_x) * x_C_along - (g_val / (2.0 * v_x2)) * x_C_along * x_C_along;
                    double required_y = (double)buildings[C].height - (double)h_A;
                    if(y_traj <= required_y + 1e-9){
                        collision = true;
                        break;
                    }
                }
                if(!collision){
                    jump_possible = true;
                    break;
                }
            }
            if(jump_possible){
                adj[A].push_back(B);
            }
        }
    }
    // BFS
    vector<int> distance_N(N, -1);
    distance_N[start] = 0;
    queue<int> q;
    q.push(start);
    while(!q.empty()){
        int u = q.front(); q.pop();
        for(auto &v_neighbor : adj[u]){
            if(distance_N[v_neighbor] == -1){
                distance_N[v_neighbor] = distance_N[u] + 1;
                q.push(v_neighbor);
            }
        }
    }
    // Output
    for(int y=0; y<dy; y++){
        string line = "";
        for(int x=0; x<dx; x++){
            int idx = y*dx + x;
            if(x > 0) line += " ";
            if(idx == start){
                line += "0";
            }
            else if(distance_N[idx] != -1){
                line += to_string(distance_N[idx]);
            }
            else{
                line += "X";
            }
        }
        cout << line << "\n";
    }
}
