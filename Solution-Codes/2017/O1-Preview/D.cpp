#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

struct Event {
    int date;
    int type; // 0 = producer, 1 = consumer
    int pi;   // For producer
    int di;   // For producer
    int qj;   // For consumer
};

int main() {
    int m, n;
    scanf("%d %d", &m, &n);
    vector<int> dates;
    vector<Event> events;

    // Read producers
    for (int i = 0; i < m; ++i) {
        int pi, di;
        scanf("%d %d", &pi, &di);
        dates.push_back(di);
        events.push_back({di, 0, pi, di, 0}); // Producer event
    }

    // Read consumers
    for (int i = 0; i < n; ++i) {
        int qj, ej;
        scanf("%d %d", &qj, &ej);
        int e = ej - 1;
        dates.push_back(e);
        events.push_back({e, 1, 0, 0, qj}); // Consumer event
    }

    // Remove duplicate dates and sort
    sort(dates.begin(), dates.end());
    dates.erase(unique(dates.begin(), dates.end()), dates.end());

    // Map original dates to compressed dates
    // Build a map from date to index
    // But since we process dates in order, we can skip coordinate compression

    // Sort events by date, type
    sort(events.begin(), events.end(), [](const Event &a, const Event &b) {
        if (a.date != b.date)
            return a.date < b.date;
        return a.type < b.type; // Producers before consumers on the same date
    });

    int max_profit = 0;
    int minimal_pi = INT_MAX;
    int di_min_pi = -1;

    for (auto &event : events) {
        if (event.type == 0) {
            // Producer event
            if (event.pi < minimal_pi) {
                minimal_pi = event.pi;
                di_min_pi = event.di;
            } else if (event.pi == minimal_pi && event.di > di_min_pi) {
                di_min_pi = event.di;
            }
        } else {
            // Consumer event
            if (minimal_pi == INT_MAX)
                continue; // No producer available yet
            int s = max(di_min_pi, 1);
            int e = event.date;
            int total_days = e - s + 1;
            if (total_days <= 0)
                continue;
            int profit_per_day = event.qj - minimal_pi;
            if (profit_per_day <= 0)
                continue;
            int total_profit = total_days * profit_per_day;
            if (total_profit > max_profit)
                max_profit = total_profit;
        }
    }

    printf("%d\n", max_profit);

    return 0;
}
