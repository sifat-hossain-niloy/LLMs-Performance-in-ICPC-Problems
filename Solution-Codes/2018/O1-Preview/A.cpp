#include <bits/stdc++.h>
using namespace std;

struct Bus {
    int departure_station;
    int arrival_station;
    long long departure_time;
    long long arrival_time;
    double probability;
};

int main() {
    int m, n;
    scanf("%d %d", &m, &n);
    long long k;
    scanf("%lld", &k);

    vector<Bus> buses(m);
    set<long long> times_set;
    times_set.insert(0);
    for (int i = 0; i < m; ++i) {
        int a, b;
        long long s, t;
        double p;
        scanf("%d %d %lld %lld %lf", &a, &b, &s, &t, &p);
        buses[i] = {a, b, s, t, p};
        times_set.insert(s);
        times_set.insert(t);
    }

    // Filter buses with departure time <= k
    buses.erase(remove_if(buses.begin(), buses.end(), [&](const Bus &b) {
        return b.departure_time > k;
    }), buses.end());

    // Sort buses in decreasing order of departure time
    sort(buses.begin(), buses.end(), [](const Bus &a, const Bus &b) {
        return a.departure_time > b.departure_time;
    });

    // Initialize V(s) for all stations
    unordered_map<int, double> V;
    V[1] = 1.0; // Probability of being at the destination is 1 at time <= k

    // Process buses in decreasing order of departure time
    for (const Bus &bus : buses) {
        int s = bus.departure_station;
        int s_prime = bus.arrival_station;
        double p_b = bus.probability;
        double candidate_value = p_b * V[s_prime];
        if (V.find(s) == V.end() || candidate_value > V[s]) {
            V[s] = candidate_value;
        }
    }

    // The final answer is V[0] if it exists, else 0
    double answer = V.count(0) ? V[0] : 0.0;
    // Ensure the answer is within absolute error of 1e-6
    printf("%.10f\n", answer);

    return 0;
}
