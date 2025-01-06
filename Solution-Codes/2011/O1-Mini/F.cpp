#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

// Machine structure
struct Machine {
    int Di;
    int Pi;
    int Ri;
    int Gi;
    ll net_profit;
};

// Comparator for machines by net_profit descending
bool cmp_profit_desc(const Machine &a, const Machine &b) {
    return a.net_profit > b.net_profit;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int N;
    ll C;
    ll D;
    int case_num =1;
    while(cin >> N >> C >> D){
        if(N ==0 && C ==0 && D ==0) break;
        vector<Machine> machines(N);
        for(int i=0;i<N;i++){
            cin >> machines[i].Di >> machines[i].Pi >> machines[i].Ri >> machines[i].Gi;
            machines[i].net_profit = (ll)machines[i].Ri + (ll)machines[i].Gi * (D - machines[i].Di) - (ll)machines[i].Pi;
        }
        // Sort machines by Di ascending
        sort(machines.begin(), machines.end(), [&](const Machine &a, const Machine &b) -> bool{
            if(a.Di != b.Di) return a.Di < b.Di;
            return a.net_profit > b.net_profit;
        });
        // Group machines by Di
        vector<pair<int, vector<Machine>>> groups;
        int current_Di = -1;
        vector<Machine> current_group;
        for(auto &m : machines){
            if(m.Di != current_Di){
                if(current_Di != -1){
                    groups.emplace_back(make_pair(current_Di, current_group));
                }
                current_Di = m.Di;
                current_group.clear();
            }
            current_group.push_back(m);
        }
        if(current_group.size() >0){
            groups.emplace_back(make_pair(current_Di, current_group));
        }
        // Priority queue: min-heap based on net_profit
        // We will keep the machines with highest net_profit
        // To facilitate replacing, we need to store Pi and Ri
        // Modify the heap to store (net_profit, (Pi, Ri, Gi))
        // For simplicity, store Gi as well
        // But C++ priority_queue does not support updating, so we manage it manually
        // Instead, store net_profit and Gi
        // We'll use a min-heap based on net_profit
        priority_queue<pair<ll, pair<int, int>>, vector<pair<ll, pair<int, int>>>, std::greater<pair<ll, pair<int, int>>>> pq;
        ll current_day =0;
        ll current_cash = C;
        ll total_G =0;
        ll sum_Ri =0;
        // To track Gi and Ri for each machine in heap, use separate structures
        // Use a vector to store Gi and Ri
        // Alternatively, use a struct
        struct HeapMachine {
            ll net_profit;
            int Pi;
            int Ri;
            int Gi;
        };
        auto cmp = [&](const HeapMachine &a, const HeapMachine &b) -> bool{
            return a.net_profit < b.net_profit;
        };
        priority_queue<HeapMachine, vector<HeapMachine>, decltype(cmp)> heap(cmp);
        for(auto &grp : groups){
            int Di = grp.first;
            const vector<Machine> &m_list = grp.second;
            // Accumulate profits
            ll t = Di - current_day;
            current_cash += total_G * t;
            current_day = Di;
            // Sort machines by net_profit descending
            vector<Machine> sorted_machines = m_list;
            sort(sorted_machines.begin(), sorted_machines.end(), cmp_profit_desc);
            for(auto &m : sorted_machines){
                if(m.net_profit <0) continue;
                if(m.Pi <= current_cash){
                    // Buy it
                    current_cash -= m.Pi;
                    total_G += m.Gi;
                    sum_Ri += m.Ri;
                    HeapMachine hm;
                    hm.net_profit = m.net_profit;
                    hm.Pi = m.Pi;
                    hm.Ri = m.Ri;
                    hm.Gi = m.Gi;
                    heap.push(hm);
                }
                else{
                    // Try to sell the machine with the smallest net_profit
                    if(!heap.empty() && heap.top().net_profit < m.net_profit){
                        // Sell the machine with smallest net_profit
                        HeapMachine sold = heap.top();
                        heap.pop();
                        // Update cash, total_G, sum_Ri
                        current_cash += sold.Ri;
                        total_G -= sold.Gi;
                        sum_Ri -= sold.Ri;
                        // Now, try to buy the new machine
                        if(m.Pi <= current_cash){
                            current_cash -= m.Pi;
                            total_G += m.Gi;
                            sum_Ri += m.Ri;
                            HeapMachine hm;
                            hm.net_profit = m.net_profit;
                            hm.Pi = m.Pi;
                            hm.Ri = m.Ri;
                            hm.Gi = m.Gi;
                            heap.push(hm);
                        }
                        else{
                            // Cannot buy even after selling, put back the sold machine
                            current_cash -=0; // Nothing
                            total_G +=0;
                            sum_Ri +=0;
                            heap.push(sold);
                        }
                    }
                }
            }
        }
        // After all groups, accumulate profits up to D
        ll t_final = D - current_day;
        if(t_final >0){
            current_cash += total_G * t_final;
            current_day = D;
        }
        // Sell all machines
        // sum_Ri is already the sum of Ri of owned machines
        current_cash += sum_Ri;
        cout << "Case " << case_num++ << ": " << current_cash << "\n";
    }
}
