#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    cin >> n;
    // Map card to index: 1A to 8D => 0 to 31
    auto card_to_index = [&](string sk) -> int {
        int s = sk[0]-'0';
        char k = sk[1];
        return (s-1)*4 + (k - 'A');
    };
    // Map set s to its 4 card indices
    auto set_to_cards = [&](int s) -> vector<int> {
        vector<int> v;
        for(int i=0;i<4;i++) v.push_back((s-1)*4 + i);
        return v;
    };
    // Initialize possible owners: 1 to 4 as bits 0 to 3
    // 0: player1, 1: player2, 2: player3, 3: player4, 4: removed
    // Use bitmask 1 to 4
    vector<int> possible_owners(32, 0b1111);
    // Read actions
    bool cheated = false;
    int cheat_action = -1;
    for(int action_num=1; action_num<=n; action_num++){
        string first;
        cin >> first;
        if(first.find('A') != string::npos || first.find('Q') != string::npos){
            // It's a 'Q' action or 'A' action, but according to the input format, actions start with x
            // So adjust
        }
        // Read the full line
        string rest_of_line;
        getline(cin, rest_of_line);
        // Combine first and rest
        string line = first + rest_of_line;
        // Parse the action
        // Possible formats:
        // x A y sk yes
        // x A y sk no
        // x Q s
        // Tokenize
        vector<string> tokens;
        string token;
        stringstream ss(line);
        while(ss >> token){
            tokens.push_back(token);
        }
        if(tokens.size() < 2){
            // Invalid action, skip
            continue;
        }
        if(tokens[1] == "A"){
            // "x A y sk yes/no"
            if(tokens.size() != 5){
                // Invalid action
                // Treat as cheating
                cheated = true;
                cheat_action = action_num;
                break;
            }
            int x = stoi(tokens[0]);
            int y = stoi(tokens[2]);
            string sk = tokens[3];
            string response = tokens[4];
            int c = card_to_index(sk);
            if(response == "yes"){
                // y must have c
                if( !(possible_owners[c] & (1 << (y-1))) ){
                    // y does not have c, cheating
                    cheated = true;
                    cheat_action = action_num;
                    break;
                }
                // Assign c to x
                possible_owners[c] = (1 << (x-1));
            }
            else{
                // y does not have c
                possible_owners[c] &= ~(1 << (y-1));
            }
            // After immediate constraints, check for any card with no possible owners
            bool conflict = false;
            for(int i=0;i<32;i++) if(possible_owners[i]==0){
                conflict = true;
                break;
            }
            if(conflict){
                cheated = true;
                cheat_action = action_num;
                break;
            }
            // If it's an 'A' action, check that x has at least one card from set s
            // s is the set of sk
            int s = sk[0]-'0';
            vector<int> set_cards = set_to_cards(s);
            bool has_possible = false;
            for(auto &card:set_cards){
                if(possible_owners[card] & (1 << (x-1))){
                    has_possible = true;
                    break;
                }
            }
            if(!has_possible){
                cheated = true;
                cheat_action = action_num;
                break;
            }
        }
        else if(tokens[1] == "Q"){
            // "x Q s"
            if(tokens.size() != 3){
                // Invalid action
                cheated = true;
                cheat_action = action_num;
                break;
            }
            int x = stoi(tokens[0]);
            int s = stoi(tokens[2]);
            vector<int> set_cards = set_to_cards(s);
            // x must have all four cards from set s
            bool conflict = false;
            for(auto &c:set_cards){
                if( !(possible_owners[c] & (1 << (x-1))) ){
                    conflict = true;
                    break;
                }
            }
            if(conflict){
                cheated = true;
                cheat_action = action_num;
                break;
            }
            // Assign all four cards to removed
            for(auto &c:set_cards){
                possible_owners[c] = 0; // Removed
                // According to the problem, removed is a state, but not tracked in possible_owners
                // So, to represent removed, we set possible_owners[c] to 0
                // But this would conflict with our earlier logic
                // Instead, represent removed as the 5th bit (bit 4)
                // So, adjust:
                // Initially, possible_owners[c] = 1 to 4
                // To assign removed, set possible_owners[c] = 16 (1 <<4)
                possible_owners[c] = (1 <<4);
            }
            // After assigning removed, check for any card with no possible owners
            bool conflict2 = false;
            for(int i=0;i<32;i++) if(possible_owners[i]==0){
                conflict2 = true;
                break;
            }
            if(conflict2){
                cheated = true;
                cheat_action = action_num;
                break;
            }
        }
        else{
            // Invalid action type
            cheated = true;
            cheat_action = action_num;
            break;
        }
    }
    if(cheated){
        cout << "no\n" << cheat_action;
    }
    else{
        cout << "yes";
    }
}
