#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int n, c;
    cin >> n >> c;
    
    vector<int> times(n);
    for (int i = 0; i < n; ++i) {
        cin >> times[i];
    }
    
    // Sort times to start with the fastest walkers
    sort(times.begin(), times.end());
    
    int total_time = 0;
    int left = 0, right = n - 1;
    
    while (right >= left) {
        // Send the fastest `c` walkers from left to right
        total_time += times[right];
        right--;
        
        if (right >= left) {
            // Send the fastest `c` walkers back from right to left
            total_time += times[left];
            left++;
        }
    }
    
    cout << total_time << endl;
    
    return 0;
}
