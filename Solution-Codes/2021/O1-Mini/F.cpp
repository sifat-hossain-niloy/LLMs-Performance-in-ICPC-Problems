#include <bits/stdc++.h>
using namespace std;

const double PI = acos(-1.0);
const double INF = 1e18;

// Structure for a point
struct Point {
    double x, y;
};

// Structure for a flight path
struct FlightPath {
    Point A;
    double Az;
    Point B;
    double Bz;
    // Direction vectors
    double ux, uy;
    double vx, vy;
    double length;
    double s_min, s_max;
};

// Function to compute dot product
double dot_product(double x1, double y1, double x2, double y2){
    return x1*x2 + y1*y2;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, m;
    cin >> n >> m;
    // Read islands
    vector<vector<Point>> islands(n);
    for(int i=0; i<n; ++i){
        int ni;
        cin >> ni;
        islands[i].resize(ni);
        for(int j=0; j<ni; ++j){
            cin >> islands[i][j].x >> islands[i][j].y;
        }
    }
    // Read flight paths
    vector<FlightPath> flights(m);
    for(int i=0; i<m; ++i){
        double x1, y1, z1, x2, y2, z2;
        cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
        flights[i].A = Point{ x1, y1 };
        flights[i].Az = z1;
        flights[i].B = Point{ x2, y2 };
        flights[i].Bz = z2;
        // Compute direction vectors
        double dx = x2 - x1;
        double dy = y2 - y1;
        double d_len = sqrt(dx*dx + dy*dy);
        if(d_len == 0){
            // Degenerate flight path, handle as impossible
            flights[i].ux = 1.0;
            flights[i].uy = 0.0;
            flights[i].vx = 0.0;
            flights[i].vy = 1.0;
        }
        else{
            flights[i].ux = dx / d_len;
            flights[i].uy = dy / d_len;
            flights[i].vx = -flights[i].uy;
            flights[i].vy = flights[i].ux;
        }
        flights[i].length = d_len;
        // Project A and B onto s axis
        // Let s=0 at A
        // So s_A =0, s_B = d_len
        flights[i].s_min = 0.0;
        flights[i].s_max = d_len;
    }
    // Read z coordinates already done
    // Now process each flight path's s range based on direction
    // Now process islands
    double final_theta = 0.0;
    bool possible = true;
    for(int i=0; i<n; ++i){
        double island_min_theta = INF;
        for(int j=0; j<m; ++j){
            // For flight path j and island i
            FlightPath &fp = flights[j];
            // Define coordinate system: u and v
            // Project island's polygon onto (s,t)
            // Find s_I_min and s_I_max
            // Also find the maximum |t| for each s
            // To do this, we need to handle polygon edges
            // First, collect all vertices in (s,t)
            int ni = islands[i].size();
            vector<pair<double, double>> proj_pts;
            for(int k=0; k<ni; ++k){
                // Vector from A to P
                double px = islands[i][k].x - fp.A.x;
                double py = islands[i][k].y - fp.A.y;
                double s = fp.ux * px + fp.uy * py;
                double t = fp.vx * px + fp.vy * py;
                proj_pts.emplace_back(s, t);
            }
            // Find s_I_min and s_I_max
            double s_I_min = INF, s_I_max = -INF;
            for(auto &[s, t] : proj_pts){
                s_I_min = min(s_I_min, s);
                s_I_max = max(s_I_max, s);
            }
            // Flight path's s range is [0, fp.length]
            if(s_I_min < 0.0 - 1e-9 || s_I_max > fp.length + 1e-9){
                // Flight path does not cover the island's s range
                continue;
            }
            // Now, prepare the list of edges in (s,t)
            vector<pair<pair<double, double>, pair<double, double>>> edges;
            for(int k=0; k<ni; ++k){
                int next = (k+1)%ni;
                double s1 = proj_pts[k].first;
                double t1 = proj_pts[k].second;
                double s2 = proj_pts[next].first;
                double t2 = proj_pts[next].second;
                edges.push_back({{s1, t1}, {s2, t2}});
            }
            // Now, define a function to get t_I(s)
            // Implemented by iterating over edges and finding max |t(s)|
            // To find max_f = max(t_I(s)/Q_z(s)) over s in [s_I_min, s_I_max]
            // Q_z(s) = Az + (Bz - Az) * (s / fp.length)
            // Handle fp.length ==0
            if(fp.length ==0){
                // Flight path is a point
                // Check if it covers the island's s range, which is s_I_min = s_I_max =0
                // And t_I(s)= max |t(P)| over P
                double Qz = fp.Az;
                if(Qz <=0){
                    continue;
                }
                double max_t = 0.0;
                for(auto &[s, t] : proj_pts){
                    max_t = max(max_t, abs(t));
                }
                double f = max_t / Qz;
                island_min_theta = min(island_min_theta, atan(f) * 180.0 / PI);
                continue;
            }
            // Now, sample s from s_I_min to s_I_max
            // To find max(t_I(s)/Q_z(s))
            // We'll sample 1000 points
            int samples = 1000;
            double max_f = 0.0;
            for(int s_idx=0; s_idx<=samples; ++s_idx){
                double s = s_I_min + (s_I_max - s_I_min) * s_idx / samples;
                // Find all edges that span s
                double current_max_t = 0.0;
                for(auto &edge : edges){
                    double s1 = edge.first.first;
                    double t1 = edge.first.second;
                    double s2 = edge.second.first;
                    double t2 = edge.second.second;
                    if(s1 == s2){
                        if(abs(s - s1) < 1e-9){
                            current_max_t = max(current_max_t, abs(t1));
                            current_max_t = max(current_max_t, abs(t2));
                        }
                        continue;
                    }
                    if(s < min(s1, s2) -1e-9 || s > max(s1, s2) +1e-9){
                        continue;
                    }
                    // Compute t(s) on this edge
                    double ratio = (s - s1) / (s2 - s1);
                    double t = t1 + ratio * (t2 - t1);
                    current_max_t = max(current_max_t, abs(t));
                }
                // Compute Q_z(s)
                double Qz = fp.Az + (fp.Bz - fp.Az) * (s / fp.length);
                if(Qz <=0){
                    // Cannot see anything from here
                    max_f = INF;
                    break;
                }
                double f = current_max_t / Qz;
                max_f = max(max_f, f);
            }
            if(max_f >= INF){
                // Cannot see
                continue;
            }
            // Now, required theta for this flight path and island is arctan(max_f)
            double theta = atan(max_f) * 180.0 / PI;
            island_min_theta = min(island_min_theta, theta);
        }
        if(island_min_theta == INF){
            // No flight path can cover this island
            possible = false;
            break;
        }
        // Update final_theta
        final_theta = max(final_theta, island_min_theta);
    }
    if(!possible){
        cout << "impossible\n";
    }
    else{
        // Check if final_theta is finite
        if(final_theta >= 90.0){
            // It should be possible with theta=90
            // But check if all islands are covered
            // Since we already checked, output final_theta
            // But as per sample Input 2, "impossible" is possible even with theta=90
            // So likely, need to ensure final_theta <=90
            // But since we used atan, which is <90, it's fine
            // Output with 9 decimal places
            cout << fixed << setprecision(9) << final_theta << "\n";
        }
        else{
            // Output with 9 decimal places
            cout << fixed << setprecision(9) << final_theta << "\n";
        }
    }
}
