#include <bits/stdc++.h>
using namespace std;

struct Segment {
    double start_time;
    double end_time;
    double x_start;
    double y_start;
    double v_x;
    double v_y;
};

// Function to read a path and populate the segments
void read_path(vector<Segment>& path_segments, double& total_time) {
    int n;
    cin >> n;
    vector<pair<double, double>> points(n);
    for(int i = 0; i < n; ++i){
        cin >> points[i].first >> points[i].second;
    }
    total_time = 0.0;
    for(int i = 1; i < n; ++i){
        double delta_x = points[i].first - points[i-1].first;
        double delta_y = points[i].second - points[i-1].second;
        double L = sqrt(delta_x * delta_x + delta_y * delta_y);
        double v_x = delta_x / L;
        double v_y = delta_y / L;
        Segment s;
        s.start_time = total_time;
        s.end_time = total_time + L;
        s.x_start = points[i-1].first;
        s.y_start = points[i-1].second;
        s.v_x = v_x;
        s.v_y = v_y;
        path_segments.push_back(s);
        total_time += L;
    }
}

// Function to check if a given D is feasible
bool feasible(double D, const vector<Segment>& m_segments, const vector<Segment>& n_segments, double T_m, double T_n){
    if(D > T_n + 1e-9) return false;
    size_t m_idx = 0;
    size_t n_idx = 0;
    size_t n_size = n_segments.size();
    while(m_idx < m_segments.size()){
        const Segment& s_m = m_segments[m_idx];
        double t_p_start = s_m.start_time;
        double t_p_end = s_m.end_time;
        t_p_end = min(t_p_end, T_n - D);
        if(t_p_start > t_p_end + 1e-9){
            m_idx++;
            continue;
        }
        double t_d_start = t_p_start + D;
        double t_d_end = t_p_end + D;
        // Find the starting Nadia's segment that covers t_d_start
        while(n_idx < n_segments.size() && n_segments[n_idx].end_time <= t_d_start + 1e-9){
            n_idx++;
        }
        size_t current_n = n_idx;
        while(current_n < n_segments.size() && n_segments[current_n].start_time < t_d_end - 1e-9){
            const Segment& s_n = n_segments[current_n];
            double overlap_t_d_start = max(t_d_start, s_n.start_time);
            double overlap_t_d_end = min(t_d_end, s_n.end_time);
            if(overlap_t_d_start > overlap_t_d_end - 1e-9){
                current_n++;
                continue;
            }
            double overlap_p_start = overlap_t_d_start - D;
            double overlap_p_end = overlap_t_d_end - D;
            // Compute A and B
            double A_x = s_m.x_start - s_n.x_start - s_n.v_x * D + s_n.v_x * s_n.start_time - s_m.v_x * s_m.start_time;
            double A_y = s_m.y_start - s_n.y_start - s_n.v_y * D + s_n.v_y * s_n.start_time - s_m.v_y * s_m.start_time;
            double B_x = s_m.v_x - s_n.v_x;
            double B_y = s_m.v_y - s_n.v_y;
            double a_coef = B_x * B_x + B_y * B_y;
            double b_coef = 2.0 * (A_x * B_x + A_y * B_y);
            double c_coef = A_x * A_x + A_y * A_y - D * D;
            bool found = false;
            const double EPS = 1e-12;
            if(a_coef <= EPS){
                if(abs(b_coef) <= EPS){
                    if(c_coef <= EPS){
                        // All t_p in [overlap_p_start, overlap_p_end] satisfy
                        return true;
                    }
                }
                else{
                    double t_star = -c_coef / b_coef;
                    if(b_coef > EPS){
                        if(overlap_p_start <= t_star + 1e-9){
                            // There exists t_p in [overlap_p_start, min(t_star, overlap_p_end)]
                            // which satisfies t_p <= t_star
                            // Check if overlap_p_start <= t_star
                            double upper = min(t_star, overlap_p_end);
                            if(upper >= overlap_p_start - 1e-9){
                                return true;
                            }
                        }
                    }
                    else if(b_coef < -EPS){
                        if(overlap_p_end >= t_star - 1e-9){
                            // There exists t_p in [max(t_star, overlap_p_start), overlap_p_end]
                            double lower = max(t_star, overlap_p_start);
                            if(lower <= overlap_p_end + 1e-9){
                                return true;
                            }
                        }
                    }
                }
            }
            else{
                double delta = b_coef * b_coef - 4.0 * a_coef * c_coef;
                if(delta >= -EPS){
                    if(delta < 0.0) delta = 0.0;
                    double sqrt_delta = sqrt(delta);
                    double t1 = (-b_coef - sqrt_delta) / (2.0 * a_coef);
                    double t2 = (-b_coef + sqrt_delta) / (2.0 * a_coef);
                    if(t1 > t2){
                        swap(t1, t2);
                    }
                    double inter_start = max(t1, overlap_p_start);
                    double inter_end = min(t2, overlap_p_end);
                    if(inter_start <= inter_end + 1e-9){
                        return true;
                    }
                }
            }
            current_n++;
        }
        m_idx++;
    }
    return false;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    vector<Segment> m_segments;
    vector<Segment> n_segments;
    double T_m, T_n;
    // Read Misha's path
    read_path(m_segments, T_m);
    // Read Nadia's path
    read_path(n_segments, T_n);
    // Binary search for minimal D
    double low = 0.0;
    double high = T_n;
    double res = -1.0;
    for(int i = 0; i < 100; ++i){
        double mid = (low + high) / 2.0;
        if(feasible(mid, m_segments, n_segments, T_m, T_n)){
            res = mid;
            high = mid;
        }
        else{
            low = mid;
        }
    }
    // After binary search, check if res is feasible
    // To handle precision, set res to high and check
    double final_D = high;
    // Check if final_D is feasible
    // To avoid floating point issues, perform one more check
    auto feasible_final = feasible(final_D, m_segments, n_segments, T_m, T_n);
    if(feasible_final){
        // Print with 5 decimal digits
        cout << fixed << setprecision(5) << final_D << "\n";
    }
    else{
        cout << "impossible\n";
    }
}
