#include <iostream>

int main() {
    long long m, n;
    std::cin >> m >> n;

    if (m > n) std::swap(m, n);

    long long result = 0;

    if (m <= 2) {
        result = 0;
    } else if (m == 3) {
        if (n >= 3)
            result = 6;
    } else if (m == 4) {
        if (n >= 3)
            result = 8;
    } else if (m >= 5) {
        if (m % 2 == 1)
            result = 4 * n;
        else
            result = 2 * n;
    }

    std::cout << result << std::endl;

    return 0;
}
