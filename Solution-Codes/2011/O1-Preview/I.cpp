#include <iostream>
#include <algorithm>
#include <cmath>
#include <climits>

int main() {
    int n, case_num = 1;
    while (std::cin >> n && n != -1) {
        int min_even_distance = INT_MAX;
        for (int i = 0; i < n; ++i) {
            int xm, ym;
            std::cin >> xm >> ym;
            int d0 = std::max(abs(xm), abs(ym));
            if (d0 % 2 == 0) {
                if (d0 < min_even_distance) {
                    min_even_distance = d0;
                }
            }
        }
        std::cout << "Case " << case_num << ": ";
        if (n == 0 || min_even_distance == INT_MAX) {
            std::cout << "never" << std::endl;
        } else {
            std::cout << min_even_distance << std::endl;
        }
        ++case_num;
    }
    return 0;
}
