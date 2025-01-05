#include <bits/stdc++.h>
using namespace std;

struct Event {
    double m; // marginal tastiness per unit weight
    double weight; // total weight available at this m
    int type; // 0 for discrete, 1 for continuous start, -1 for continuous end
};

bool operator<(const Event &a, const Event &b) {
    if (abs(a.m - b.m) > 1e-8)
        return a.m > b.m;
    return a.type < b.type; // process start before end
}

int main() {
    int d, w;
    cin >> d >> w;
    vector<Event> events;
    double total_weight_zero_decay = 0.0;
    double total_tastiness_zero_decay = 0.0;

    vector<double> ti_list;
    vector<double> delta_ti_list;

    for (int i = 0; i < d; ++i) {
        string dish_type;
        cin >> dish_type;
        if (dish_type == "D") {
            int wi;
            double ti, delta_ti;
            cin >> wi >> ti >> delta_ti;
            if (delta_ti == 0.0) {
                // Infinite marginal tastiness per unit weight
                int N_i_max = w / wi;
                double total_wi = N_i_max * wi;
                double total_tastiness = N_i_max * ti;
                total_weight_zero_decay += total_wi;
                total_tastiness_zero_decay += total_tastiness;
            } else {
                int N_i_max = min(int(ti / delta_ti) + 1, w / wi);
                for (int n = 1; n <= N_i_max; ++n) {
                    double m = (ti - (n - 1) * delta_ti) / wi;
                    double weight = wi;
                    events.push_back({m, weight, 0});
                }
            }
        } else if (dish_type == "C") {
            double ti, delta_ti;
            cin >> ti >> delta_ti;
            if (delta_ti == 0.0) {
                // Infinite marginal tastiness per unit weight
                double weight_available = w;
                double total_tastiness = ti * weight_available;
                total_weight_zero_decay += weight_available;
                total_tastiness_zero_decay += total_tastiness;
            } else {
                // Events at m = ti and m = 0
                events.push_back({ti, 0.0, 1}); // start of continuous dish
                events.push_back({0.0, 0.0, -1}); // end of continuous dish
                ti_list.push_back(ti);
                delta_ti_list.push_back(delta_ti);
            }
        }
    }

    double remaining_weight = w - total_weight_zero_decay;
    double total_tastiness = total_tastiness_zero_decay;

    if (remaining_weight < -1e-6) {
        cout << "impossible" << endl;
        return 0;
    } else if (abs(remaining_weight) <= 1e-6) {
        printf("%.9f\n", total_tastiness);
        return 0;
    }

    sort(events.begin(), events.end());

    int idx = 0;
    int num_continuous = ti_list.size();
    vector<double> derivative_changes;
    vector<double> m_points;
    map<double, double> derivative_map;

    for (int i = 0; i < num_continuous; ++i) {
        double ti = ti_list[i];
        double delta_ti = delta_ti_list[i];
        double m_start = ti;
        double m_end = 0.0;

        derivative_map[m_start] -= 1.0 / delta_ti;
    }

    double current_m = 1e9;
    double dWdm = 0.0; // derivative of W_continuous(m) with respect to m
    double W_continuous = 0.0;

    for (const auto &event : events) {
        double m = event.m;

        // Update W_continuous and dWdm between current_m and m
        if (current_m > m) {
            W_continuous += dWdm * (current_m - m);
            current_m = m;
        }

        // Update derivative at this m
        if (event.type == 1) { // start of continuous dish
            // Add derivative change
            double delta_ti = delta_ti_list[idx];
            dWdm += -1.0 / delta_ti;
            ++idx;
        } else if (event.type == -1) {
            // End of continuous dish, handled implicitly since m reaches 0
        } else {
            // Discrete dish
            // W_discrete at this m is event.weight
            double weight_available = event.weight;
            double total_weight_available = W_continuous + weight_available;
            if (total_weight_available >= remaining_weight) {
                total_tastiness += remaining_weight * m;
                remaining_weight = 0.0;
                break;
            } else {
                total_tastiness += total_weight_available * m;
                remaining_weight -= total_weight_available;
                W_continuous = 0.0;
            }
        }
    }

    // After processing all events
    if (remaining_weight > 1e-6) {
        cout << "impossible" << endl;
    } else {
        printf("%.9f\n", total_tastiness);
    }

    return 0;
}
