#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long ll;

struct Task {
    ll a, b;
};

int main() {
    int n;
    ll t;
    cin >> n >> t;
    vector<Task> tasks(n);
    for(int i = 0; i < n; ++i) {
        cin >> tasks[i].a >> tasks[i].b;
    }
    sort(tasks.begin(), tasks.end(), [](const Task& x, const Task& y) {
        return x.b < y.b;
    });
    ll current_time = 0;
    for(const auto& task : tasks) {
        ll start_time = max(current_time, task.a);
        if(start_time + t > task.b) {
            cout << "no\n";
            return 0;
        }
        current_time = start_time + t;
    }
    cout << "yes\n";
    return 0;
}
