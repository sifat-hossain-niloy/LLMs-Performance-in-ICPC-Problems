#include <bits/stdc++.h>
using namespace std;

const double PI = acos(-1.0);
const double RE = 6371.0; // Earth's radius in kilometers
const double INF = 1e18;

// Structure to hold airport information
struct Airport {
    double lon_deg;
    double lat_deg;
    double lon_rad;
    double lat_rad;
    double x, y, z;
};

// Function to convert degrees to radians and compute unit vector
void convert_airport(Airport &airport) {
    airport.lon_rad = airport.lon_deg * PI / 180.0;
    airport.lat_rad = airport.lat_deg * PI / 180.0;
    airport.x = cos(airport.lat_rad) * cos(airport.lon_rad);
    airport.y = cos(airport.lat_rad) * sin(airport.lon_rad);
    airport.z = sin(airport.lat_rad);
}

// Function to compute angular distance using haversine formula
double angular_distance(const Airport &a, const Airport &b) {
    double delta_lat = b.lat_rad - a.lat_rad;
    double delta_lon = b.lon_rad - a.lon_rad;
    double sin_dlat = sin(delta_lat / 2.0);
    double sin_dlon = sin(delta_lon / 2.0);
    double a_hav = sin_dlat * sin_dlat + cos(a.lat_rad) * cos(b.lat_rad) * sin_dlon * sin_dlon;
    double c = 2.0 * asin(sqrt(a_hav));
    return c;
}

// Function to compute initial bearing from a to b
double initial_bearing(const Airport &a, const Airport &b) {
    double delta_lon = b.lon_rad - a.lon_rad;
    double y = sin(delta_lon) * cos(b.lat_rad);
    double x = cos(a.lat_rad) * sin(b.lat_rad) - sin(a.lat_rad) * cos(b.lat_rad) * cos(delta_lon);
    double theta = atan2(y, x);
    if (theta < 0.0) theta += 2.0 * PI;
    return theta;
}

// Function to compute minimal distance from C to AB
double minimal_distance(const Airport &A, const Airport &B, const Airport &C) {
    // Compute cross product N = A x B, normalized
    double Nx = A.y * B.z - A.z * B.y;
    double Ny = A.z * B.x - A.x * B.z;
    double Nz = A.x * B.y - A.y * B.x;
    double norm_N = sqrt(Nx * Nx + Ny * Ny + Nz * Nz);
    if (norm_N == 0.0) { // A and B are the same point
        return angular_distance(A, C);
    }
    Nx /= norm_N;
    Ny /= norm_N;
    Nz /= norm_N;

    // Compute cross-track distance delta_xt = asin(|N . C|)
    double cross = Nx * C.x + Ny * C.y + Nz * C.z;
    double d_xt = asin(abs(cross));

    // Compute angular distance from A to C
    double d_AC = angular_distance(A, C);

    // Compute along-track distance d_at_p = acos( cos(d_AC) / cos(d_xt) )
    double cos_d_at_p = cos(d_AC) / cos(d_xt);
    // Clamp to [-1, 1] to avoid numerical issues
    if (cos_d_at_p > 1.0) cos_d_at_p = 1.0;
    if (cos_d_at_p < -1.0) cos_d_at_p = -1.0;
    double d_at_p = acos(cos_d_at_p);

    double d_AB = angular_distance(A, B);

    if (d_at_p < 0.0 || d_at_p > d_AB) {
        double d_BC = angular_distance(B, C);
        return min(d_AC, d_BC);
    }
    else {
        return abs(d_xt);
    }
}

// Function to check if AB is fully covered by R-radius circles around airports
bool is_covered(const Airport &A, const Airport &B, const vector<Airport> &airports, double R_ang) {
    double d_AB = angular_distance(A, B);
    vector<pair<double, double>> intervals;

    for (const Airport &C : airports) {
        double d_xt = minimal_distance(A, B, C);
        if (d_xt > R_ang + 1e-12) continue;

        // Compute along-track distance d_at_p
        double d_AC = angular_distance(A, C);
        double cos_d_at_p = cos(d_AC) / cos(d_xt);
        if (cos_d_at_p > 1.0) cos_d_at_p = 1.0;
        if (cos_d_at_p < -1.0) cos_d_at_p = -1.0;
        double d_at_p = acos(cos_d_at_p);

        // Compute delta_t = acos( cos(R_ang) / cos(d_xt) )
        double ratio = cos(R_ang) / cos(d_xt);
        if (ratio > 1.0) ratio = 1.0;
        if (ratio < -1.0) ratio = -1.0;
        double delta_t = acos(ratio);

        // Compute coverage interval
        double coverage_start = d_at_p - delta_t;
        double coverage_end = d_at_p + delta_t;

        // Clamp to [0, d_AB]
        if (coverage_start < 0.0) coverage_start = 0.0;
        if (coverage_end > d_AB) coverage_end = d_AB;

        intervals.emplace_back(coverage_start, coverage_end);
    }

    // Sort intervals by start
    sort(intervals.begin(), intervals.end());

    // Merge intervals and check coverage
    double current_end = 0.0;
    for (const auto &interval : intervals) {
        if (interval.first > current_end + 1e-12) {
            return false;
        }
        current_end = max(current_end, interval.second);
        if (current_end >= d_AB - 1e-12) {
            break;
        }
    }

    return (current_end >= d_AB - 1e-12);
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int N, R;
    int case_num = 1;
    while(cin >> N >> R){
        vector<Airport> airports(N);
        for(int i=0;i<N;i++){
            cin >> airports[i].lon_deg >> airports[i].lat_deg;
            convert_airport(airports[i]);
        }
        double R_ang = (double)R / RE;
        // Precompute all AB pairs that satisfy coverage condition
        vector<vector<pair<int, double>>> adj(N, vector<pair<int, double>>());
        for(int i=0;i<N;i++){
            for(int j=i+1; j<N; j++){
                double d_AB = angular_distance(airports[i], airports[j]);
                double distance_km = d_AB * RE;
                if(is_covered(airports[i], airports[j], airports, R_ang)){
                    adj[i].emplace_back(j, distance_km);
                    adj[j].emplace_back(i, distance_km);
                }
            }
        }
        // Sort adjacency lists by distance
        for(int i=0;i<N;i++){
            sort(adj[i].begin(), adj[i].end(), [&](const pair<int, double> &a, const pair<int, double> &b)->bool{
                return a.second < b.second;
            });
        }
        // Read Q queries
        int Q;
        cin >> Q;
        cout << "Case " << case_num++ << ":\n";
        while(Q--){
            int s, t;
            double c;
            cin >> s >> t >> c;
            s--; t--; // Convert to 0-based index
            // Dijkstra's algorithm
            vector<double> dist(N, INF);
            dist[s] = 0.0;
            priority_queue<pair<double, int>, vector<pair<double, int>>, std::greater<pair<double, int>>> pq;
            pq.emplace(0.0, s);
            while(!pq.empty()){
                auto [current_distance, u] = pq.top(); pq.pop();
                if(u == t){
                    break;
                }
                if(current_distance > dist[u] + 1e-9){
                    continue;
                }
                for(auto &[v, d_uv] : adj[u]){
                    if(d_uv > c + 1e-9){
                        break; // Since sorted, no need to check further
                    }
                    if(dist[v] > dist[u] + d_uv + 1e-9){
                        dist[v] = dist[u] + d_uv;
                        pq.emplace(dist[v], v);
                    }
                }
            }
            if(dist[t] < INF/2){
                // Round to three decimal places
                printf("%.3lf\n", dist[t]);
            }
            else{
                printf("impossible\n");
            }
        }
    }
}
