#include <iostream>
#include <iomanip>
#include <cmath>

int main() {
    int n;
    std::cin >> n;

    double psunny, pcloudy, prainy, pfrogs = 0.0;
    std::string line;
    std::getline(std::cin, line); // To consume the newline character
    std::getline(std::cin, line);

    // Parse the probabilities from the input line
    std::istringstream iss(line);
    std::vector<double> probs;
    double prob;
    while (iss >> prob) {
        probs.push_back(prob);
    }

    // If only three probabilities are provided, calculate the fourth
    if (probs.size() == 3) {
        double sum = probs[0] + probs[1] + probs[2];
        probs.push_back(1.0 - sum);
    }

    // Calculate the entropy
    double H_single = 0.0;
    for (double p : probs) {
        if (p > 0.0) {
            H_single -= p * std::log2(p);
        }
    }

    double H_total = n * H_single;

    // Output the result with high precision
    std::cout << std::fixed << std::setprecision(6) << H_total << std::endl;

    return 0;
}
