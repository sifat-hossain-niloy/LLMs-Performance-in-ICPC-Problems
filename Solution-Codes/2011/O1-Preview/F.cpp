#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

struct Machine {
    long long Di, Pi, Ri, Gi;
    Machine(long long Di, long long Pi, long long Ri, long long Gi)
        : Di(Di), Pi(Pi), Ri(Ri), Gi(Gi) {}
};

struct OwnedMachine {
    long long Gi, owning_start_day, Pi, Ri;
    OwnedMachine(long long Gi, long long owning_start_day, long long Pi, long long Ri)
        : Gi(Gi), owning_start_day(owning_start_day), Pi(Pi), Ri(Ri) {}
    bool operator<(const OwnedMachine& other) const {
        return Gi > other.Gi; // Min-heap based on Gi
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int case_num = 1;
    while (true) {
        long long N, C, D;
        cin >> N >> C >> D;
        if (N == 0 && C == 0 && D == 0) break;

        vector<Machine> machines;
        vector<long long> event_days;

        for (long long i = 0; i < N; ++i) {
            long long Di, Pi, Ri, Gi;
            cin >> Di >> Pi >> Ri >> Gi;
            machines.emplace_back(Di, Pi, Ri, Gi);
            event_days.push_back(Di);
        }

        // Sort machines based on availability day
        sort(machines.begin(), machines.end(), [](const Machine& a, const Machine& b) {
            return a.Di < b.Di;
        });

        // Remove duplicate event days
        sort(event_days.begin(), event_days.end());
        event_days.erase(unique(event_days.begin(), event_days.end()), event_days.end());

        long long cash = C;
        long long total_profit = 0;
        priority_queue<OwnedMachine> owned_machines; // Min-heap based on Gi
        size_t machine_idx = 0;

        for (long long day : event_days) {
            // Process machines available on this day
            while (machine_idx < machines.size() && machines[machine_idx].Di == day) {
                Machine& mi = machines[machine_idx];
                // Try to buy machine mi
                while (cash < mi.Pi) {
                    if (owned_machines.empty()) break;
                    OwnedMachine om = owned_machines.top();
                    if (om.Gi >= mi.Gi) break;
                    owned_machines.pop();
                    // Compute profit from operating the machine
                    long long days_operated = day - om.owning_start_day;
                    if (days_operated > 0) {
                        total_profit += om.Gi * days_operated;
                    }
                    cash += om.Ri; // Sell the machine
                }
                if (cash >= mi.Pi) {
                    // Buy the machine
                    cash -= mi.Pi;
                    owned_machines.emplace(mi.Gi, day + 1, mi.Pi, mi.Ri);
                }
                // Move to next machine
                ++machine_idx;
            }
        }

        // Sell all remaining machines at the end
        while (!owned_machines.empty()) {
            OwnedMachine om = owned_machines.top();
            owned_machines.pop();
            // Compute profit from operating the machine
            long long days_operated = D + 1 - om.owning_start_day;
            if (days_operated > 0) {
                total_profit += om.Gi * days_operated;
            }
            cash += om.Ri; // Sell the machine
        }

        // Final cash is current cash plus total profit
        long long final_cash = cash + total_profit;
        cout << "Case " << case_num << ": " << final_cash << "\n";
        ++case_num;
    }

    return 0;
}
