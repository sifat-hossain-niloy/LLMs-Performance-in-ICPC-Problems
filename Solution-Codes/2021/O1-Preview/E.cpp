#include <iostream>
#include <iomanip>

int main() {
    int k, m;
    std::cin >> k >> m;
    int n = 0;
    for (int i = 0; i < m; ++i) {
        int count;
        std::cin >> count;
        n += count;
    }

    // Calculate s = k * (k - 1)!
    int s = 1;
    for (int i = 1; i <= k - 1; ++i) {
        s *= i;
    }
    s *= k;

    double P_max = (double)s / (s + 1);

    std::cout << std::setprecision(15) << P_max << std::endl;

    return 0;
}
