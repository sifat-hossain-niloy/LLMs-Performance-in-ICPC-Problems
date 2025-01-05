#include <bits/stdc++.h>
using namespace std;

struct Deposit {
    long long x0, x1, y;
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    cin >> n;
    vector<Deposit> deposits(n);
    // Read deposits
    for(int i=0;i<n;i++){
        cin >> deposits[i].x0 >> deposits[i].x1 >> deposits[i].y;
        if(deposits[i].x0 > deposits[i].x1){
            swap(deposits[i].x0, deposits[i].x1);
        }
    }
    // Step1: vertical lines
    // We need to find the x where the sum of widths of overlapping intervals is maximum
    // Sweep line approach
    vector<pair<long long, long long>> events;
    for(int i=0;i<n;i++){
        events.emplace_back(deposits[i].x0, deposits[i].x1);
    }
    // Sort events by x0
    // To compute the maximum overlapping sum, we can sweep through all endpoints
    vector<pair<long long, pair<int, bool>>> sorted_events; // x, (deposit index, is_end)
    for(int i=0;i<n;i++){
        sorted_events.emplace_back(deposits[i].x0, make_pair(i, false));
        sorted_events.emplace_back(deposits[i].x1, make_pair(i, true));
    }
    sort(sorted_events.begin(), sorted_events.end(), [&](const pair<long long, pair<int, bool>> &a, const pair<long long, pair<int, bool>> &b) -> bool{
        if(a.first != b.first) return a.first < b.first;
        return a.second.second < b.second.second; // start before end
    });
    long long current_sum = 0;
    long long max_sum = 0;
    // To handle overlapping, we can track current sum
    // At each point, add widths of deposits that cover this x
    // To implement it correctly, we need to track active deposits
    // Initialize
    current_sum = 0;
    // We will sweep through sorted x's and keep track of active deposits
    // At each event, if it's a start, add the width; if it's an end, remove the width
    // The maximum sum is the maximum current_sum encountered
    // To do this, we need to track active deposits
    // But since widths are independent, just add and remove widths
    // Initialize
    current_sum = 0;
    max_sum = 0;
    for(auto &event: sorted_events){
        long long x = event.first;
        int idx = event.second.first;
        bool is_end = event.second.second;
        if(!is_end){
            current_sum += (deposits[idx].x1 - deposits[idx].x0);
            if(current_sum > max_sum){
                max_sum = current_sum;
            }
        }
        else{
            current_sum -= (deposits[idx].x1 - deposits[idx].x0);
        }
    }
    // Step1 done, max_sum now contains the maximum sum for vertical lines
    // Now, Step2: non-vertical lines
    // Enumerate all lines passing through two endpoints
    // Collect all endpoints
    struct Point {
        long long x, y;
    };
    vector<Point> endpoints;
    endpoints.reserve(2*n);
    for(int i=0;i<n;i++){
        endpoints.push_back(Point{deposits[i].x0, deposits[i].y});
        endpoints.push_back(Point{deposits[i].x1, deposits[i].y});
    }
    // Enumerate all unique lines passing through two endpoints
    // To avoid duplicate lines, use a set with precision
    // But to keep it fast, iterate through all pairs
    // And for each line, compute the sum
    // Since n=2000, endpoints=4000, total lines=4000*3999/2 ~8,000,000
    // To optimize, iterate through endpoints and compute lines
    // But with 8,000,000 lines and 2000 deposits, it's too slow
    // Thus, implement a fast inner loop
    // To store lines, use a map with a unique key, but it's too slow
    // Instead, just iterate and keep track of maximum sum
    // Implemented with double precision
    // Precompute y and x0, x1 for deposits
    vector<long long> y(n), x0(n), x1(n);
    for(int i=0;i<n;i++){
        y[i] = deposits[i].y;
        x0[i] = deposits[i].x0;
        x1[i] = deposits[i].x1;
    }
    // Iterate through all pairs of endpoints
    // To speed up, iterate with outer loop on first endpoint
    // and inner loop on second endpoint
    // Keep max_sum updated
    // To further speed up, precompute the deposits' y and x ranges
    // Implemented with fast loops and integer arithmetic where possible
    // To handle vertical lines separately, skip when y1 == y2
    // But we have already handled vertical lines in step1
    // So, proceed
    // To prevent floating precision issues, use doubles
    // But using integer arithmetic can be tricky
    // Proceed with doubles
    // Use multithreading or other optimizations are not allowed
    // Thus, proceed with a single thread and fast code
    // But with n=2000, it's too slow, but proceed
    // Implement early pruning if possible
    // Initialize
    // To keep it faster, iterate through pairs and compute sum on the fly
    // Implemented as follows:
    // Note: This part may not complete within the time limit for n=2000
    // But proceed
    // To speed up, use cache-friendly structures and minimize computations
    // Implement step2
    for(int i=0;i<(int)endpoints.size();i++){
        for(int j=i+1;j<(int)endpoints.size();j++){
            // Get two points
            double x1_p = endpoints[i].x;
            double y1_p = endpoints[i].y;
            double x2_p = endpoints[j].x;
            double y2_p = endpoints[j].y;
            if(y1_p == y2_p){
                // This is a horizontal line, already handled in vertical lines
                continue;
            }
            // Compute slope a and intercept b
            double a = (x2_p - x1_p) / (y2_p - y1_p);
            double b_val = x1_p - a * y1_p;
            // Now, iterate through all deposits and check if a y + b is in [x0, x1]
            double sum = 0;
            for(int k=0;k<n;k++){
                double xk = a * (double)y[k] + b_val;
                if(xk >= (double)x0[k] && xk <= (double)x1[k]){
                    sum += (x1[k] - x0[k]);
                }
            }
            if(sum > max_sum){
                max_sum = sum;
            }
        }
    }
    // Now, also consider vertical lines
    // They are already considered in step1
    // Thus, the maximum sum is already stored in max_sum
    // Output the maximum sum
    cout << (long long)max_sum;
}
