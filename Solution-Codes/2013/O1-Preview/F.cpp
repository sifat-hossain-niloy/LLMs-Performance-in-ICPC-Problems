#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    int n, k;
    std::cin >> n >> k;
    int totalBatteries = 2 * n * k;
    std::vector<int> batteries(totalBatteries);
    for (int i = 0; i < totalBatteries; ++i) {
        std::cin >> batteries[i];
    }

    // Sort the batteries
    std::sort(batteries.begin(), batteries.end());

    // Extract the minimal 2n batteries
    std::vector<int> minimalBatteries(batteries.begin(), batteries.begin() + 2 * n);

    int d = 0;
    // Pair consecutive minimal batteries and compute the maximum difference
    for (int i = 0; i < 2 * n; i += 2) {
        int diff = minimalBatteries[i + 1] - minimalBatteries[i];
        if (diff > d) {
            d = diff;
        }
    }

    // Output the result
    std::cout << d << std::endl;

    return 0;
}
