#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <cstdint>

// Maximum total exponent sum to consider
const int MAX_T = 20;

// Precomputed factorials up to MAX_T
uint64_t factorial[MAX_T + 1];

// Generate all integer partitions of n into positive integers >= 1
void generate_partitions(int n, int max, std::vector<int>& partition, std::vector<std::vector<int>>& partitions) {
    if (n == 0) {
        partitions.push_back(partition);
    } else {
        for (int i = std::min(max, n); i >= 1; --i) {
            partition.push_back(i);
            generate_partitions(n - i, i, partition, partitions);
            partition.pop_back();
        }
    }
}

int main() {
    // Precompute factorials
    factorial[0] = 1;
    for (int i = 1; i <= MAX_T; ++i) {
        factorial[i] = factorial[i - 1] * i;
    }

    // Precompute minimal k for each possible f(k)
    std::map<uint64_t, uint64_t> n_to_k;

    // List of the first few primes
    std::vector<uint64_t> primes = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 
                                    37, 41, 43, 47, 53, 59, 61, 67, 71};

    for (int T = 1; T <= MAX_T; ++T) {
        std::vector<std::vector<int>> partitions;
        std::vector<int> partition;
        generate_partitions(T, T, partition, partitions);

        for (const auto& e_i : partitions) {
            // Sort exponents in decreasing order for minimal k
            std::vector<int> exponents = e_i;
            std::sort(exponents.begin(), exponents.end(), std::greater<int>());

            // Calculate f(k)
            uint64_t numerator = factorial[T];
            uint64_t denominator = 1;
            for (int e : exponents) {
                denominator *= factorial[e];
            }
            uint64_t f_k = numerator / denominator;

            // Assign primes to exponents
            uint64_t k = 1;
            bool overflow = false;
            for (size_t i = 0; i < exponents.size(); ++i) {
                if (i >= primes.size()) {
                    // Not enough primes to assign
                    overflow = true;
                    break;
                }
                for (int j = 0; j < exponents[i]; ++j) {
                    if (k > UINT64_MAX / primes[i]) {
                        overflow = true;
                        break;
                    }
                    k *= primes[i];
                }
                if (overflow) break;
            }
            if (overflow) continue;

            // Update minimal k for this f(k)
            auto it = n_to_k.find(f_k);
            if (it == n_to_k.end() || k < it->second) {
                n_to_k[f_k] = k;
            }
        }
    }

    // Read test cases
    uint64_t n;
    while (std::cin >> n) {
        auto it = n_to_k.find(n);
        if (it != n_to_k.end()) {
            std::cout << n << " " << it->second << std::endl;
        } else {
            // According to the problem description, this should not happen
            std::cout << n << " " << -1 << std::endl;
        }
    }

    return 0;
}
