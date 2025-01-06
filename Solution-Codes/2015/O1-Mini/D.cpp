#include <bits/stdc++.h>
using namespace std;

struct Event {
    double z;
    int type; // 0 for 'start', 1 for 'end'
    double zc;
    double r;
};

// Structure to store interval information
struct Interval {
    double z_start;
    double z_end;
    double a;
    double b;
    double c;
    double d;
    double cum_sum_cap_V_start;
};

const double PI = 3.141592653589793238462643383279502884;

// Function to compute V(z) = 10^10 z - sum_cap_V(z)
double compute_V(double z, const vector<Interval>& intervals) {
    // Binary search to find the interval containing z
    int left = 0;
    int right = intervals.size();
    while (left < right) {
        int mid = left + (right - left) / 2;
        if (intervals[mid].z_end <= z) {
            left = mid + 1;
        }
        else {
            right = mid;
        }
    }
    if (left >= intervals.size()) left = intervals.size() -1;
    const Interval& interval = intervals[left];
    double z_start = interval.z_start;
    double a = interval.a;
    double b = interval.b;
    double c = interval.c;
    double d = interval.d;
    double cum_sum_cap_V_start = interval.cum_sum_cap_V_start;
    // Compute f(z) = (a / 4)(z^4 - z_start^4) + (b / 3)(z^3 - z_start^3) + (c / 2)(z^2 - z_start^2) + d*(z - z_start)
    double f_z = (a / 4.0) * (pow(z,4) - pow(z_start,4)) +
                (b / 3.0) * (pow(z,3) - pow(z_start,3)) +
                (c / 2.0) * (pow(z,2) - pow(z_start,2)) +
                d * (z - z_start);
    double sum_cap_V = cum_sum_cap_V_start + f_z;
    double V_z = 1e10 * z - sum_cap_V;
    return V_z;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, s;
    cin >> n >> s;
    vector<Event> events;
    events.reserve(2*n);
    for(int i=0;i<n;i++){
        long long r_in, x, y, z;
        cin >> r_in >> x >> y >> z;
        double r = (double)r_in;
        double zc = (double)z;
        double start_z = zc - r;
        double end_z = zc + r;
        // Ensure start_z and end_z are within [0, 100000]
        start_z = max(start_z, 0.0);
        end_z = min(end_z, 100000.0);
        events.push_back(Event{start_z, 0, zc, r});
        events.push_back(Event{end_z, 1, zc, r});
    }
    // Sort events by z ascending, then 'start' before 'end'
    sort(events.begin(), events.end(), [&](const Event& a, const Event& b) -> bool{
        if(abs(a.z - b.z) > 1e-9) return a.z < b.z;
        return a.type < b.type;
    });
    // Sweep to build intervals
    vector<Interval> intervals;
    double prev_z = 0.0;
    double a = 0.0, b_coef = 0.0, c = 0.0, d = 0.0;
    int idx = 0;
    int total_events = events.size();
    while(idx < total_events){
        double current_z = events[idx].z;
        // If current_z > prev_z, add interval [prev_z, current_z)
        if(current_z > prev_z){
            intervals.push_back(Interval{prev_z, current_z, a, b_coef, c, d, 0.0});
            prev_z = current_z;
        }
        // Process all events at current_z
        while(idx < total_events && abs(events[idx].z - current_z) < 1e-6){
            Event e = events[idx];
            if(e.type == 0){
                // 'start' event
                a += -PI / 3.0;
                b_coef += PI * e.zc;
                c += (-PI * e.zc * e.zc + PI * e.r * e.r);
                d += (PI / 3.0) * (pow(e.zc,3) - 3.0 * pow(e.zc,2) * e.r + 3.0 * e.zc * pow(e.r,2) + 2.0 * pow(e.r,3));
            }
            else{
                // 'end' event
                a += PI / 3.0;
                b_coef -= PI * e.zc;
                c += PI * e.zc * e.zc - PI * e.r * e.r;
                d += (2.0 / 3.0) * PI * pow(e.r,3) - (PI / 3.0) * (pow(e.zc,3) - 3.0 * pow(e.zc,2) * e.r + 3.0 * e.zc * pow(e.r,2) + 2.0 * pow(e.r,3));
            }
            idx++;
        }
    }
    // After all events, add the last interval if needed
    if(prev_z < 100000.0){
        intervals.push_back(Interval{prev_z, 100000.0, a, b_coef, c, d, 0.0});
    }
    // Now compute cumulative sum_cap_V_start
    double cum_sum_cap_V = 0.0;
    for(auto &interval : intervals){
        interval.cum_sum_cap_V_start = cum_sum_cap_V;
        double z_start = interval.z_start;
        double z_end = interval.z_end;
        double a_i = interval.a;
        double b_i = interval.b;
        double c_i = interval.c;
        double d_i = interval.d;
        double cap_integral = (a_i / 4.0) * (pow(z_end,4) - pow(z_start,4)) +
                              (b_i / 3.0) * (pow(z_end,3) - pow(z_start,3)) +
                              (c_i / 2.0) * (pow(z_end,2) - pow(z_start,2)) +
                              d_i * (z_end - z_start);
        cum_sum_cap_V += cap_integral;
    }
    // Now, set cum_sum_cap_V_start for each interval correctly
    cum_sum_cap_V = 0.0;
    for(auto &interval : intervals){
        interval.cum_sum_cap_V_start = cum_sum_cap_V;
        double z_start = interval.z_start;
        double z_end = interval.z_end;
        double a_i = interval.a;
        double b_i = interval.b;
        double c_i = interval.c;
        double d_i = interval.d;
        double cap_integral = (a_i / 4.0) * (pow(z_end,4) - pow(z_start,4)) +
                              (b_i / 3.0) * (pow(z_end,3) - pow(z_start,3)) +
                              (c_i / 2.0) * (pow(z_end,2) - pow(z_start,2)) +
                              d_i * (z_end - z_start);
        cum_sum_cap_V += cap_integral;
    }
    // Now compute total_cheese_volume
    // V_total = 10^10 * 100000 - sum_cap_V(100000)
    double V_total = 1e10 * 100000.0 - cum_sum_cap_V;
    // Compute slice_size
    double slice_size = V_total / (double)s;
    // Now, find z_i for each slice
    vector<double> z_i(s, 0.0);
    for(int i=1;i<=s;i++){
        double target_V = slice_size * i;
        // Binary search on z from 0 to 100000 to find z_i such that V(z_i) = target_V
        double low = 0.0;
        double high = 100000.0;
        for(int iter=0; iter<100; iter++){
            double mid = (low + high) / 2.0;
            // Compute V(mid)
            // Find the interval containing mid
            int left = 0;
            int right = intervals.size();
            while(left < right){
                int m = left + (right - left) / 2;
                if(intervals[m].z_end <= mid){
                    left = m + 1;
                }
                else{
                    right = m;
                }
            }
            if(left >= intervals.size()) left = intervals.size() -1;
            const Interval& interval = intervals[left];
            double z_start = interval.z_start;
            double a_i = interval.a;
            double b_i = interval.b;
            double c_i = interval.c;
            double d_i = interval.d;
            double cum_sum_cap_V_start = interval.cum_sum_cap_V_start;
            double f_z = (a_i / 4.0) * (pow(mid,4) - pow(z_start,4)) +
                        (b_i / 3.0) * (pow(mid,3) - pow(z_start,3)) +
                        (c_i / 2.0) * (pow(mid,2) - pow(z_start,2)) +
                        d_i * (mid - z_start);
            double sum_cap_V = cum_sum_cap_V_start + f_z;
            double V_z = 1e10 * mid - sum_cap_V;
            if(V_z < target_V){
                low = mid;
            }
            else{
                high = mid;
            }
        }
        double z_found = (low + high) / 2.0;
        z_i[i-1] = z_found;
    }
    // Now, compute slice thicknesses
    vector<double> slice_thickness(s, 0.0);
    double prev = 0.0;
    for(int i=0;i<s;i++){
        slice_thickness[i] = (z_i[i] - prev) / 1000.0; // Convert to mm
        prev = z_i[i];
    }
    // Output with 9 decimal digits
    cout << fixed << setprecision(9);
    for(int i=0;i<s;i++){
        cout << slice_thickness[i] << "\n";
    }
}
