#include <iostream>
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

struct Point {
    double x, y;
};

struct Segment {
    double t_start, t_end;
    Point p_start, p_end;
    Point v; // velocity
};

double dot(Point a, Point b) {
    return a.x*b.x + a.y*b.y;
}

double norm(Point a) {
    return sqrt(dot(a,a));
}

Point operator-(Point a, Point b) {
    return {a.x - b.x, a.y - b.y};
}

Point operator+(Point a, Point b) {
    return {a.x + b.x, a.y + b.y};
}

Point operator*(double k, Point a) {
    return {k*a.x, k*a.y};
}

int main() {
    int n_misha;
    scanf("%d", &n_misha);
    vector<Point> misha_points(n_misha);
    for (int i = 0; i < n_misha; ++i) {
        scanf("%lf %lf", &misha_points[i].x, &misha_points[i].y);
    }
    int n_nadia;
    scanf("%d", &n_nadia);
    vector<Point> nadia_points(n_nadia);
    for (int i = 0; i < n_nadia; ++i) {
        scanf("%lf %lf", &nadia_points[i].x, &nadia_points[i].y);
    }

    // Precompute Misha's segments
    vector<Segment> misha_segs;
    double t = 0.0;
    for (int i = 0; i < n_misha -1; ++i) {
        Point p0 = misha_points[i];
        Point p1 = misha_points[i+1];
        double dist = norm(p1 - p0);
        Segment seg;
        seg.t_start = t;
        seg.t_end = t + dist;
        seg.p_start = p0;
        seg.p_end = p1;
        seg.v = (1.0 / dist)*(p1 - p0);
        misha_segs.push_back(seg);
        t += dist;
    }
    double Tm = t;

    // Precompute Nadia's segments
    vector<Segment> nadia_segs;
    t = 0.0;
    for (int i = 0; i < n_nadia -1; ++i) {
        Point p0 = nadia_points[i];
        Point p1 = nadia_points[i+1];
        double dist = norm(p1 - p0);
        Segment seg;
        seg.t_start = t;
        seg.t_end = t + dist;
        seg.p_start = p0;
        seg.p_end = p1;
        seg.v = (1.0 / dist)*(p1 - p0);
        nadia_segs.push_back(seg);
        t += dist;
    }
    double Tn = t;

    double D_low = 0.0, D_high = Tn + Tm; // upper limit
    double epsilon = 1e-7;
    bool possible = false;
    double result = -1.0;

    for (int iter = 0; iter < 100; ++iter) {
        double D_mid = (D_low + D_high) / 2.0;
        bool delivery_possible = false;

        // For each of Misha's segments
        int idx_nadia = 0;
        for (auto &m_seg : misha_segs) {
            double t1a = m_seg.t_start;
            double t1b = m_seg.t_end;

            double t2a = t1a + D_mid;
            double t2b = t1b + D_mid;

            if (t2a > Tn) continue;
            t2a = max(t2a, 0.0);
            t2b = min(t2b, Tn);
            if (t2a > t2b) continue;

            // Move idx_nadia to the first segment where t2a <= nadia_segs[idx_nadia].t_end
            while (idx_nadia < nadia_segs.size() && nadia_segs[idx_nadia].t_end < t2a) idx_nadia++;
            int idx_nadia_curr = idx_nadia;

            while (idx_nadia_curr < nadia_segs.size() && nadia_segs[idx_nadia_curr].t_start <= t2b) {
                auto &n_seg = nadia_segs[idx_nadia_curr];

                double s2a = n_seg.t_start;
                double s2b = n_seg.t_end;

                // Overlapping t2 interval is [max(t2a, s2a), min(t2b, s2b)]
                double t2_start = max(t2a, s2a);
                double t2_end = min(t2b, s2b);
                if (t2_start > t2_end) {
                    idx_nadia_curr++;
                    continue;
                }

                // Corresponding t1 interval is [t1_start, t1_end] where t2 = t1 + D_mid
                double t1_start = t2_start - D_mid;
                double t1_end = t2_end - D_mid;

                t1_start = max(t1_start, t1a);
                t1_end = min(t1_end, t1b);
                if (t1_start > t1_end) {
                    idx_nadia_curr++;
                    continue;
                }

                // Now compute minimal F(t1) over [t1_start, t1_end]
                // M(t1) = m_seg.p_start + vM*(t1 - m_seg.t_start)
                // N(t2) = n_seg.p_start + vN*(t2 - n_seg.t_start)
                // t2 = t1 + D_mid
                // So N(t1 + D_mid)

                Point s0 = m_seg.p_start - n_seg.p_start - n_seg.v * (D_mid);
                Point v_rel = m_seg.v - n_seg.v;

                // s(t1) = s0 + v_rel*(t1 - m_seg.t_start)
                // Distance D(t1) = ||s(t1)||

                // To find where derivative is zero:
                // s(t1) ⋅ v_rel = 0
                double denom = dot(v_rel, v_rel);
                double t1_crit;
                if (denom == 0.0) {
                    t1_crit = t1_start; // v_rel is zero, so derivative is zero everywhere
                } else {
                    t1_crit = m_seg.t_start - dot(s0, v_rel) / denom;
                }

                // Evaluate F(t1) at t1_crit, t1_start, t1_end
                vector<double> t1_candidates = {t1_start, t1_end};
                if (t1_crit >= t1_start && t1_crit <= t1_end) {
                    t1_candidates.push_back(t1_crit);
                }

                for (double t1_candidate : t1_candidates) {
                    Point m_pos = m_seg.p_start + m_seg.v * (t1_candidate - m_seg.t_start);
                    double t2_candidate = t1_candidate + D_mid;
                    Point n_pos = n_seg.p_start + n_seg.v * (t2_candidate - n_seg.t_start);
                    double F_t1 = norm(m_pos - n_pos) - D_mid;
                    if (F_t1 <= 0.0) {
                        delivery_possible = true;
                        break;
                    }
                }
                if (delivery_possible) break;
                idx_nadia_curr++;
            }
            if (delivery_possible) break;
        }

        if (delivery_possible) {
            possible = true;
            result = D_mid;
            D_high = D_mid;
        } else {
            D_low = D_mid;
        }
    }

    if (possible) {
        printf("%.10lf\n", result);
    } else {
        printf("impossible\n");
    }

    return 0;
}
