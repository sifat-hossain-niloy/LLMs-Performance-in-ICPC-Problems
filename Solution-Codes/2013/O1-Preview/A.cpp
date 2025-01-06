#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

int main() {
    int n;
    std::cin >> n;
    std::vector<std::string> molecules(n);
    std::string temp;
    std::getline(std::cin, temp); // consume the newline character after n

    // Map to track if we've found a molecule with both '+' and '-' connectors for a letter
    bool unbounded = false;
    std::unordered_map<char, bool> letterHasBothSigns;

    for (int i = 0; i < n; ++i) {
        std::getline(std::cin, molecules[i]);
    }

    // Since molecules might be separated by spaces, split them correctly
    std::string allMolecules;
    for (const auto& molecule : molecules) {
        allMolecules += molecule + " ";
    }

    size_t pos = 0;
    std::string token;
    while ((pos = allMolecules.find(' ')) != std::string::npos) {
        token = allMolecules.substr(0, pos);
        allMolecules.erase(0, pos + 1);
        // Process each molecule
        std::string connector[4];
        for (int i = 0; i < 4; ++i) {
            connector[i] = token.substr(i*2, 2);
        }

        std::unordered_map<char, int> letterSigns; // 1: '+', 2: '-', 3: both
        for (int i = 0; i < 4; ++i) {
            std::string label = connector[i];
            if (label == "00") continue;
            char letter = label[0];
            char sign = label[1];
            if (letterSigns.find(letter) == letterSigns.end()) {
                letterSigns[letter] = (sign == '+') ? 1 : 2;
            } else {
                if ((letterSigns[letter] == 1 && sign == '-') ||
                    (letterSigns[letter] == 2 && sign == '+')) {
                    letterSigns[letter] = 3;
                }
            }
        }
        // Check if any letter has both '+' and '-' in this molecule
        for (const auto& pair : letterSigns) {
            if (pair.second == 3) {
                letterHasBothSigns[pair.first] = true;
                unbounded = true;
            }
        }
    }

    if (unbounded) {
        std::cout << "unbounded" << std::endl;
    } else {
        std::cout << "bounded" << std::endl;
    }

    return 0;
}
