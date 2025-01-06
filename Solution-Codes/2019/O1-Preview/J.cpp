#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

typedef long long ll;

struct Player {
    vector<ll> scores;
    vector<ll> cum_scores;
    ll total_score;
};

int main() {
    int p, h;
    cin >> p >> h;

    vector<Player> players(p);
    set<ll> critical_l_set;

    ll l_max = 0;

    // Read scores and collect critical l values
    for (int i = 0; i < p; ++i) {
        players[i].scores.resize(h);
        for (int j = 0; j < h; ++j) {
            cin >> players[i].scores[j];
            critical_l_set.insert(players[i].scores[j]);
            l_max = max(l_max, players[i].scores[j]);
        }
    }

    critical_l_set.insert(l_max + 1);

    vector<ll> critical_l(critical_l_set.begin(), critical_l_set.end());

    // Preprocess players' scores
    for (int i = 0; i < p; ++i) {
        sort(players[i].scores.begin(), players[i].scores.end());
        players[i].cum_scores.resize(h + 1);
        players[i].cum_scores[0] = 0;
        for (int j = 0; j < h; ++j) {
            players[i].cum_scores[j + 1] = players[i].cum_scores[j] + players[i].scores[j];
        }
        players[i].total_score = players[i].cum_scores[h];
    }

    vector<int> min_rank(p, p);

    vector<int> k_indices(p, 0); // Pointers for each player

    // Process each critical l value
    for (size_t l_idx = 0; l_idx < critical_l.size(); ++l_idx) {
        ll l = critical_l[l_idx];

        vector<pair<ll, int>> total_scores(p);

        // Compute adjusted total scores for all players
        for (int i = 0; i < p; ++i) {
            // Update k_indices[i]
            while (k_indices[i] < h && players[i].scores[k_indices[i]] <= l) {
                k_indices[i]++;
            }
            int t_i = k_indices[i];
            ll adjusted_total = players[i].cum_scores[t_i] + (h - t_i) * l;
            total_scores[i] = {adjusted_total, i};
        }

        // Sort adjusted total scores
        sort(total_scores.begin(), total_scores.end());

        // Compute ranks
        ll last_total_score = -1;
        int last_occurrence_index = -1;
        vector<int> ranks(p);
        for (int i = 0; i < p; ++i) {
            ll curr_total_score = total_scores[i].first;
            int player_id = total_scores[i].second;
            if (curr_total_score != last_total_score) {
                last_total_score = curr_total_score;
                last_occurrence_index = i;
            }
            int rank = last_occurrence_index + 1;
            min_rank[player_id] = min(min_rank[player_id], rank);
        }
    }

    // Output the minimal ranks
    for (int i = 0; i < p; ++i) {
        cout << min_rank[i] << '\n';
    }

    return 0;
}
