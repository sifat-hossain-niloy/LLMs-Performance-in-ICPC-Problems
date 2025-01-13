#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <cmath>
#include <queue>

using namespace std;

double solve(int d, int w, vector<string>& dice_faces, vector<string>& words) {
    // Preprocess the dice faces into a vector of sets for quick lookup
    vector<unordered_set<char>> dice(d);
    for (int i = 0; i < d; ++i) {
        for (char face : dice_faces[i]) {
            dice[i].insert(face);
        }
    }

    // Create a set of valid words for quick lookup
    unordered_set<string> word_set(words.begin(), words.end());

    // BFS to calculate the expected number of rolls
    unordered_map<string, double> dp;
    queue<string> q;

    // Initialize
    for (const string& word : words) {
        dp[word] = 1.0;  // If a word is formed, it takes exactly 1 roll
        q.push(word);
    }

    // BFS to compute the probabilities of reaching valid words
    while (!q.empty()) {
        string current = q.front();
        q.pop();

        // Iterate over each possible dice combination
        double prob = 1.0;  // Probability of forming `current` from dice
        for (int i = 0; i < d; ++i) {
            if (dice[i].count(current[i]) == 0) {
                prob = 0.0;  // This combination is impossible
                break;
            }
            prob /= 6.0;  // Each face has equal probability
        }

        // Update expectations
        for (const string& word : words) {
            if (dp.count(word) == 0) {
                dp[word] = dp[current] + 1 / prob;
                q.push(word);
            }
        }
    }

    // Find the minimum expectation across all valid words
    double min_expectation = INFINITY;
    for (const string& word : words) {
        if (dp.count(word)) {
            min_expectation = min(min_expectation, dp[word]);
        }
    }

    return (min_expectation == INFINITY) ? -1 : min_expectation;
}

int main() {
    int d, w;
    cin >> d >> w;
    vector<string> dice_faces(d);
    for (int i = 0; i < d; ++i) {
        cin >> dice_faces[i];
    }

    vector<string> words(w);
    for (int i = 0; i < w; ++i) {
        cin >> words[i];
    }

    double result = solve(d, w, dice_faces, words);
    if (result == -1) {
        cout << "impossible" << endl;
    } else {
        cout.precision(9);
        cout << fixed << result << endl;
    }

    return 0;
}
