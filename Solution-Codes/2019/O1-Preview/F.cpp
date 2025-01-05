#include <bits/stdc++.h>
using namespace std;

const int INF = INT_MAX;

struct SegmentTree {
    int size;
    vector<int> mins, lazy;

    void init(int n) {
        size = 1;
        while (size < n) size *= 2;
        mins.assign(2 * size, INF);
        lazy.assign(2 * size, INF);
    }

    void push(int x, int lx, int rx) {
        if (lazy[x] != INF) {
            mins[x] = min(mins[x], lazy[x]);
            if (rx - lx > 1) {
                lazy[2 * x + 1] = min(lazy[2 * x + 1], lazy[x]);
                lazy[2 * x + 2] = min(lazy[2 * x + 2], lazy[x]);
            }
            lazy[x] = INF;
        }
    }

    void range_update(int l, int r, int v, int x, int lx, int rx) {
        push(x, lx, rx);
        if (lx >= r || rx <= l) return;
        if (lx >= l && rx <= r) {
            lazy[x] = min(lazy[x], v);
            push(x, lx, rx);
            return;
        }
        int m = (lx + rx) / 2;
        range_update(l, r, v, 2 * x + 1, lx, m);
        range_update(l, r, v, 2 * x + 2, m, rx);
        mins[x] = min(mins[2 * x + 1], mins[2 * x + 2]);
    }

    int point_query(int idx, int x, int lx, int rx) {
        push(x, lx, rx);
        if (rx - lx == 1) {
            return mins[x];
        }
        int m = (lx + rx) / 2;
        int res;
        if (idx < m) {
            res = point_query(idx, 2 * x + 1, lx, m);
        } else {
            res = point_query(idx, 2 * x + 2, m, rx);
        }
        mins[x] = min(mins[2 * x + 1], mins[2 * x + 2]);
        return res;
    }

    void range_update(int l, int r, int v) {
        range_update(l, r, v, 0, 0, size);
    }

    int point_query(int idx) {
        return point_query(idx, 0, 0, size);
    }
};

struct Tarp {
    int x_l, x_h;
    int idx_l, idx_h;
    int y_l, y_h;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int l, r, n;
    cin >> l >> r >> n;
    vector<int> xs = {l, r};
    vector<Tarp> tarps(n);

    for (int i = 0; i < n; ++i) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        xs.push_back(x1);
        xs.push_back(x2);
        tarps[i] = {x1, x2, 0, 0, y1, y2};
    }

    // Coordinate compression
    sort(xs.begin(), xs.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());
    unordered_map<int, int> x_idx;
    for (int i = 0; i < xs.size(); ++i) {
        x_idx[xs[i]] = i;
    }
    int m = xs.size();
    SegmentTree st;
    st.init(m);

    // Initialize dp[x_i] = INF
    for (int i = 0; i < m; ++i) {
        st.mins[st.size - 1 + i] = INF;
        st.lazy[st.size - 1 + i] = INF;
    }
    // For x_i in [l, r], dp[x_i] = 0
    int idx_l = x_idx[l];
    int idx_r = x_idx[r];
    if (idx_l > idx_r) swap(idx_l, idx_r);
    st.range_update(idx_l, idx_r + 1, 0);

    // Prepare tarps sorted by decreasing y_h
    vector<pair<int, int>> events; // (y_h, tarp_index)
    for (int i = 0; i < n; ++i) {
        tarps[i].idx_l = x_idx[tarps[i].x_l];
        tarps[i].idx_h = x_idx[tarps[i].x_h];
        if (tarps[i].y_h < tarps[i].y_l) {
            swap(tarps[i].x_l, tarps[i].x_h);
            swap(tarps[i].idx_l, tarps[i].idx_h);
            swap(tarps[i].y_l, tarps[i].y_h);
        }
        events.emplace_back(tarps[i].y_h, i);
    }
    sort(events.rbegin(), events.rend()); // Decreasing y

    for (auto &e : events) {
        int idx = e.second;
        Tarp &t = tarps[idx];
        int dp_l = st.point_query(t.idx_l);
        int idx_min = min(t.idx_l, t.idx_h);
        int idx_max = max(t.idx_l, t.idx_h);
        st.range_update(idx_min, idx_max + 1, dp_l);
    }

    // Find minimal dp[x_i] for x_i ∈ [l, r]
    int min_punctures = INF;
    for (int i = idx_l; i <= idx_r; ++i) {
        int dp_i = st.point_query(i);
        if (dp_i < min_punctures) {
            min_punctures = dp_i;
        }
    }

    cout << min_punctures << '\n';
    return 0;
}
