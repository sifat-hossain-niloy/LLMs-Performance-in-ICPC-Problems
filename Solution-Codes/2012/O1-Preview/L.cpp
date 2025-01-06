#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

const int MAX_N = 10;
int N, M;
int market_value0[MAX_N], market_value1[MAX_N];
int dp[2][1 << MAX_N][1 << MAX_N];

bool win(int player_turn, int subs0, int subs1) {
    int &res = dp[player_turn][subs0][subs1];
    if (res != -1)
        return res;

    // Check if opponent has no subsidiaries
    if ((player_turn == 0 && subs1 == 0) || (player_turn == 1 && subs0 == 0)) {
        res = 1;
        return res;
    }

    // Check if current player has no subsidiaries
    if ((player_turn == 0 && subs0 == 0) || (player_turn == 1 && subs1 == 0)) {
        res = 0;
        return res;
    }

    int my_subs, opp_subs, my_N, opp_N;
    int *my_market_values, *opp_market_values;

    if (player_turn == 0) {
        my_subs = subs0;
        opp_subs = subs1;
        my_N = N;
        opp_N = M;
        my_market_values = market_value0;
        opp_market_values = market_value1;
    } else {
        my_subs = subs1;
        opp_subs = subs0;
        my_N = M;
        opp_N = N;
        my_market_values = market_value1;
        opp_market_values = market_value0;
    }

    for (int s = 0; s < my_N; ++s) {
        if (!(my_subs & (1 << s)))
            continue;
        for (int t = 0; t < opp_N; ++t) {
            if (!(opp_subs & (1 << t)))
                continue;

            int new_subs0 = subs0, new_subs1 = subs1;
            if (my_market_values[s] > opp_market_values[t]) {
                if (player_turn == 0)
                    new_subs1 = opp_subs & (~(1 << t));
                else
                    new_subs0 = opp_subs & (~(1 << t));
            }

            bool opponent_can_win = win(1 - player_turn, new_subs0, new_subs1);
            if (!opponent_can_win) {
                res = 1;
                return res;
            }
        }
    }

    res = 0;
    return res;
}

int main() {
    int case_num = 0;
    while (cin >> N >> M) {
        case_num++;
        for (int i = 0; i < N; ++i)
            cin >> market_value0[i];
        for (int i = 0; i < M; ++i)
            cin >> market_value1[i];

        memset(dp, -1, sizeof(dp));

        int initial_subs0 = (1 << N) - 1;
        int initial_subs1 = (1 << M) - 1;

        bool takeover_wins = win(0, initial_subs0, initial_subs1);

        cout << "Case " << case_num << ": ";
        if (takeover_wins)
            cout << "Takeover Incorporated" << endl;
        else
            cout << "Buyout Limited" << endl;
    }
    return 0;
}
