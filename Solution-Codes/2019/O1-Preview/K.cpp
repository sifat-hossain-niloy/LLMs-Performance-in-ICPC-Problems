#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>

using namespace std;

const int M = 10000;  // Number of bins per period

struct Light {
    double x;
    double r;
    double g;
    double p;  // Period
    double red_end;  // End time of red phase in one period
};

int main() {
    int n;
    cin >> n;
    vector<Light> lights(n);
    for (int i = 0; i < n; ++i) {
        int x, r, g;
        cin >> x >> r >> g;
        lights[i].x = x;
        lights[i].r = r;
        lights[i].g = g;
        lights[i].p = r + g;
        lights[i].red_end = r;
    }

    // Initialize PDF at light 0 (starting point)
    vector<double> pdf_current(M, 1.0 / M);  // Uniform distribution
    vector<double> pdf_next(M, 0.0);

    vector<double> stop_probs(n, 0.0);

    for (int i = 0; i < n; ++i) {
        Light &light = lights[i];
        double p = light.p;
        double red_end = light.red_end;
        double delta_x = (i == 0) ? light.x : (light.x - lights[i - 1].x);

        // Reset pdf_next
        fill(pdf_next.begin(), pdf_next.end(), 0.0);

        for (int j = 0; j < M; ++j) {
            double phi = (j + 0.5) * p / M;  // Arrival phase at light i
            double prob = pdf_current[j];

            // Arrival phase at light i+1
            double phi_next = fmod(phi + delta_x, p);

            // Map phi_next to bin index
            int k = int(phi_next / p * M);
            if (k >= M) k = M - 1;

            pdf_next[k] += prob;

            // Determine if the car stops at light i
            if (phi_next < red_end) {
                stop_probs[i] += prob;
            }
        }

        // Normalize pdf_next
        double sum_probs = 0.0;
        for (int j = 0; j < M; ++j) {
            sum_probs += pdf_next[j];
        }
        if (sum_probs > 0) {
            for (int j = 0; j < M; ++j) {
                pdf_next[j] /= sum_probs;
            }
        }

        // Prepare for next iteration
        pdf_current.swap(pdf_next);
    }

    // Output probabilities that the car stops at each light
    for (int i = 0; i < n; ++i) {
        printf("%.12f\n", stop_probs[i]);
    }

    // Output probability that the car passes all lights
    double pass_all = 0.0;
    for (int j = 0; j < M; ++j) {
        double phi = (j + 0.5) * lights[n - 1].p / M;
        if (phi >= lights[n - 1].red_end) {
            pass_all += pdf_current[j];
        }
    }
    printf("%.12f\n", pass_all);

    return 0;
}
