#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <cstring>
using namespace std;

int main() {
    int n;
    cin >> n;
    string line;
    getline(cin, line); // consume newline
    getline(cin, line); // our cards
    // Map cards 'A' to 'U' to indices 0 to 20
    map<char, int> cardToIndex;
    char indexToCard[21];
    for (char c = 'A'; c <= 'U'; ++c) {
        int idx = c - 'A';
        cardToIndex[c] = idx;
        indexToCard[idx] = c;
    }
    // Players 0-3 (us to player 4), player 4 is the solution
    const int NUM_PLAYERS = 5;
    const int PLAYER_US = 0;
    const int PLAYER_SOLUTION = 4;
    const int NUM_CARDS = 21;

    int possibleHolders[NUM_CARDS]; // bitmask of possible holders
    memset(possibleHolders, 0, sizeof(possibleHolders));
    // Initialize possible holders
    for (int i = 0; i < NUM_CARDS; ++i) {
        possibleHolders[i] = (1<<1)|(1<<2)|(1<<3)|(1<<4); // players 1-3 and solution
    }
    // Our cards
    set<int> ourCards;
    {
        stringstream ss(line);
        string cardStr;
        while (ss >> cardStr) {
            char c = cardStr[0];
            int idx = cardToIndex[c];
            possibleHolders[idx] = (1<<PLAYER_US); // we have it
            ourCards.insert(idx);
        }
    }
    // Process suggestions
    for (int t = 0; t < n; ++t) {
        getline(cin, line);
        stringstream ss(line);
        string s1, s2, s3;
        ss >> s1 >> s2 >> s3;
        char suggestion[3];
        suggestion[0] = s1[0];
        suggestion[1] = s2[0];
        suggestion[2] = s3[0];
        vector<int> suggestionCards;
        for (int i = 0; i < 3; ++i) {
            int idx = cardToIndex[suggestion[i]];
            suggestionCards.push_back(idx);
        }
        vector<string> responses;
        string resp;
        while (ss >> resp) {
            responses.push_back(resp);
        }
        int suggester = t % 4; // players 0-3
        // Players being asked are (suggester + 1)%4, (suggester + 2)%4, (suggester + 3)%4
        for (int k = 0; k < responses.size(); ++k) {
            int playerIdx = (suggester + 1 + k)%4;
            string response = responses[k];
            if (response == "-") {
                // Player does not have any of the cards
                for (int c : suggestionCards) {
                    possibleHolders[c] &= ~(1<<playerIdx);
                }
            } else {
                if (response != "*") {
                    // We know the specific card
                    int c = cardToIndex[response[0]];
                    possibleHolders[c] = (1<<playerIdx);
                    // Remove player from other cards in suggestion
                    for (int c2 : suggestionCards) {
                        if (c2 != c) {
                            possibleHolders[c2] &= ~(1<<playerIdx);
                        }
                    }
                }
                // Player holds at least one of the cards in the suggestion
                // Remove player from possible holders of cards not in the suggestion
                for (int c = 0; c < NUM_CARDS; ++c) {
                    if (std::find(suggestionCards.begin(), suggestionCards.end(), c) == suggestionCards.end()) {
                        possibleHolders[c] &= ~(1<<playerIdx);
                    }
                }
                // Since evidence was provided, stop asking further players
                break;
            }
        }
    }
    // Apply constraints iteratively
    bool changed = true;
    while (changed) {
        changed = false;
        // For each card, if possible holders reduced to one, remove this player from other cards
        for (int c = 0; c < NUM_CARDS; ++c) {
            int cnt = __builtin_popcount(possibleHolders[c]);
            if (cnt == 1) {
                int playerIdx = __builtin_ctz(possibleHolders[c]); // only bit set
                // Remove this card from possible holders of other players
                for (int p = 0; p < NUM_PLAYERS; ++p) {
                    if (p != playerIdx && (possibleHolders[c] & (1<<p))) {
                        possibleHolders[c] &= ~(1<<p);
                        changed = true;
                    }
                }
                // For this player, remove other cards if they cannot possibly hold them
                for (int c2 = 0; c2 < NUM_CARDS; ++c2) {
                    if (c2 != c && (possibleHolders[c2] & (1<<playerIdx))) {
                        int cnt2 = __builtin_popcount(possibleHolders[c2]);
                        if (cnt2 == 1) continue; // already determined
                        // Check if playerIdx must hold c2
                        // Since playerIdx holds c, and if c is the only card they can hold
                        // But we cannot deduce more without further information
                    }
                }
            }
        }
    }
    // Prepare output
    char result[3];
    // Murderer (Person A-F)
    bool foundPerson = false;
    for (char c = 'A'; c <= 'F'; ++c) {
        int idx = cardToIndex[c];
        if (possibleHolders[idx] == (1<<PLAYER_SOLUTION)) {
            result[0] = c;
            foundPerson = true;
            break;
        }
    }
    if (!foundPerson) result[0] = '?';
    // Weapon (G-L)
    bool foundWeapon = false;
    for (char c = 'G'; c <= 'L'; ++c) {
        int idx = cardToIndex[c];
        if (possibleHolders[idx] == (1<<PLAYER_SOLUTION)) {
            result[1] = c;
            foundWeapon = true;
            break;
        }
    }
    if (!foundWeapon) result[1] = '?';
    // Room (M-U)
    bool foundRoom = false;
    for (char c = 'M'; c <= 'U'; ++c) {
        int idx = cardToIndex[c];
        if (possibleHolders[idx] == (1<<PLAYER_SOLUTION)) {
            result[2] = c;
            foundRoom = true;
            break;
        }
    }
    if (!foundRoom) result[2] = '?';
    // Output result
    cout << result[0] << result[1] << result[2] << endl;
    return 0;
}
