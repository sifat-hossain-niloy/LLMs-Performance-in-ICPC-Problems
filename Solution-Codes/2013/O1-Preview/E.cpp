#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <climits>

using namespace std;

// Function to parse and expand the program
void parseProgram(const string& programStr, vector<string>& program) {
    vector<string> tokens;
    stringstream ss(programStr);
    string token;
    while (ss >> token) {
        tokens.push_back(token);
    }
    vector<string> stack;
    vector<int> repeatStack;
    int idx = 0;
    function<void()> parse = [&]() {
        while (idx < tokens.size()) {
            string t = tokens[idx++];
            if (t[0] == 'R') {
                int n = stoi(t.substr(1));
                repeatStack.push_back(n);
                int start = stack.size();
                parse();
                int end = stack.size();
                vector<string> repeatBlock(stack.begin() + start, stack.end());
                stack.erase(stack.begin() + start, stack.end());
                for (int i = 0; i < n; ++i) {
                    stack.insert(stack.end(), repeatBlock.begin(), repeatBlock.end());
                }
            } else if (t == "E") {
                repeatStack.pop_back();
                return;
            } else {
                stack.push_back(t);
            }
        }
    };
    parse();
    program = stack;
}

int main() {
    int b, s;
    cin >> b >> s;
    cin.ignore(); // Ignore the newline after the integers
    string programStr;
    getline(cin, programStr);
    vector<string> programTokens;
    parseProgram(programStr, programTokens);

    // Count variable frequencies
    unordered_map<string, int> varFreq;
    vector<string> variables;
    unordered_map<string, int> varIndices;
    int varCount = 0;
    for (const string& token : programTokens) {
        if (token[0] == 'V') {
            if (varFreq[token] == 0) {
                variables.push_back(token);
                varIndices[token] = varCount++;
            }
            varFreq[token]++;
        }
    }

    int totalVars = varCount;

    // Assign variables to banks
    int maxBankSize = s;
    int numBanks = b;
    vector<vector<int>> banks(b); // banks[bank_id] = list of variable indices

    // Sort variables by frequency
    vector<pair<int, string>> freqVars;
    for (const string& var : variables) {
        freqVars.push_back({ -varFreq[var], var }); // Negative for descending sort
    }
    sort(freqVars.begin(), freqVars.end());

    // Assign variables to bank 0
    int bank0Capacity = s;
    int idx = 0;
    unordered_map<string, int> varBank; // var -> bank_id
    for (; idx < freqVars.size() && banks[0].size() < bank0Capacity; ++idx) {
        string var = freqVars[idx].second;
        varBank[var] = 0;
        banks[0].push_back(varIndices[var]);
    }
    // Assign remaining variables to other banks
    int bankId = 1;
    for (; idx < freqVars.size(); ++idx) {
        string var = freqVars[idx].second;
        varBank[var] = bankId;
        banks[bankId].push_back(varIndices[var]);
        if (banks[bankId].size() == s) {
            bankId++;
            if (bankId >= b) {
                bankId = 1; // Wrap around or handle overflow
            }
        }
    }

    // Now, proceed with DP
    int n = programTokens.size();
    vector<unordered_map<int, int>> dp(n + 1); // dp[pos][bsr] = min total cost
    dp[0][ -1 ] = 0; // Initial state, BSR undefined (-1)

    for (int pos = 0; pos < n; ++pos) {
        const string& token = programTokens[pos];
        if (token[0] != 'V') {
            // Should not happen, but skip if not a variable
            continue;
        }
        string var = token;
        int varBankId = varBank[var]; // Bank where the variable is stored

        // For each possible previous BSR value
        for (const auto& kv : dp[pos]) {
            int prevBSR = kv.first;
            int cost = kv.second;

            if (varBankId == 0) {
                // Variable is in bank 0, accessible without changing BSR
                int newCost = cost + 1; // Access variable
                if (dp[pos + 1].count(prevBSR) == 0 || dp[pos + 1][prevBSR] > newCost) {
                    dp[pos + 1][prevBSR] = newCost;
                }
            } else {
                // Variable is in bank varBankId
                if (prevBSR == varBankId) {
                    // BSR already set to this bank
                    int newCost = cost + 1; // Access variable
                    if (dp[pos + 1].count(prevBSR) == 0 || dp[pos + 1][prevBSR] > newCost) {
                        dp[pos + 1][prevBSR] = newCost;
                    }
                } else {
                    // Need to set BSR to varBankId
                    int newCost = cost + 2; // Set BSR + Access variable
                    if (dp[pos + 1].count(varBankId) == 0 || dp[pos + 1][varBankId] > newCost) {
                        dp[pos + 1][varBankId] = newCost;
                    }
                }
            }
        }
    }

    // Find minimal total cost at the end of the program
    int minTotalCost = INT_MAX;
    for (const auto& kv : dp[n]) {
        int cost = kv.second;
        if (cost < minTotalCost) {
            minTotalCost = cost;
        }
    }

    cout << minTotalCost << endl;

    return 0;
}
