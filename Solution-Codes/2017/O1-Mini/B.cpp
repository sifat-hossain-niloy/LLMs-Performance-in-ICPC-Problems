#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    cin >> n;
    // Map letters to indices 0-20
    auto char_to_idx = [&](char c) -> int {
        return c - 'A';
    };
    // Categories
    vector<int> persons, weapons, rooms;
    for(int i=0;i<6;i++) persons.push_back(i); // A-F
    for(int i=6;i<12;i++) weapons.push_back(i); // G-L
    for(int i=12;i<21;i++) rooms.push_back(i); // M-U
    // Initialize possible owners
    // Owners: player1=1, player2=2, player3=4, player4=8, solution=16
    // possible[21] holds bitmask for each card
    int num_cards = 21;
    vector<int> possible(num_cards, 31); // 1+2+4+8+16=31
    // Read player1's cards
    for(int i=0;i<5;i++){
        char c;
        cin >> c;
        int idx = char_to_idx(c);
        possible[idx] = 1; // player1
    }
    // Read n suggestions
    struct Suggestion {
        int suggester;
        int p, w, r;
        vector<string> responses;
    };
    vector<Suggestion> suggestions(n);
    for(int m=0; m<n; m++){
        string p_str, w_str, r_str;
        cin >> p_str >> w_str >> r_str;
        int p = char_to_idx(p_str[0]);
        int w = char_to_idx(w_str[0]);
        int r = char_to_idx(r_str[0]);
        // Determine suggester
        int s = (m % 4) +1;
        // Read remaining tokens in the line as responses
        string token;
        vector<string> responses;
        // To read the rest of the line, use getline
        // First, ignore the current buffer's newline
        // But since we read with cin, it's already at the next token
        // So read up to three tokens
        for(int i=0;i<3;i++){
            if(cin.peek() == '\n' || cin.eof()) break;
            cin >> token;
            responses.push_back(token);
        }
        suggestions[m] = Suggestion{ s, p, w, r, responses };
    }
    // Assign constants for owners
    const int PLAYER1 = 1;
    const int PLAYER2 = 2;
    const int PLAYER3 = 4;
    const int PLAYER4 = 8;
    const int SOLUTION = 16;
    // Function to count bits set
    auto count_bits_set = [&](int x) -> int {
        return __builtin_popcount(x);
    };
    // Function to get the first bit set
    auto get_first_bit = [&](int x) -> int {
        if(x & PLAYER1) return PLAYER1;
        if(x & PLAYER2) return PLAYER2;
        if(x & PLAYER3) return PLAYER3;
        if(x & PLAYER4) return PLAYER4;
        if(x & SOLUTION) return SOLUTION;
        return 0;
    };
    // Function to assign a card to an owner
    auto assign_card = [&](int card, int owner, vector<int> &possible_ref) -> bool {
        if(possible_ref[card] != owner){
            possible_ref[card] = owner;
            return true;
        }
        return false;
    };
    // Function to eliminate an owner from a card's possible owners
    auto eliminate_owner = [&](int card, int owner, vector<int> &possible_ref) -> bool {
        if(possible_ref[card] & owner){
            possible_ref[card] &= ~owner;
            return true;
        }
        return false;
    };
    bool changed = true;
    while(changed){
        changed = false;
        // Assign cards with only one possible owner
        for(int card=0; card<num_cards; card++){
            int owners = possible[card];
            int cnt = count_bits_set(owners);
            if(cnt ==1){
                int owner = get_first_bit(owners);
                // Assign card to owner
                // Remove this card from other owners' possibilities
                // Not needed since we have exactly one owner
                // But ensure that the owner is correctly set
                // No action needed
                // Continue
                // Nothing to do
                // So no action, as possible[card] is already set
                // But we can mark it as assigned
                // So nothing to do here
            }
        }
        // Assign if a category has only one possible card for S
        // Persons
        for(auto &card: persons){
            if(possible[card] & SOLUTION){
                // Count how many can be in solution
            }
        }
        // For each category, check if only one card can be in solution
        // Persons
        auto check_and_assign = [&](const vector<int> &category) -> bool {
            int cnt =0;
            int last = -1;
            for(auto &card: category){
                if(possible[card] & SOLUTION){
                    cnt++;
                    last = card;
                }
            }
            if(cnt ==1){
                // Assign last to SOLUTION
                if(assign_card(last, SOLUTION, possible)){
                    changed = true;
                }
            }
            return false;
        };
        if(check_and_assign(persons)) changed = true;
        if(check_and_assign(weapons)) changed = true;
        if(check_and_assign(rooms)) changed = true;
        // Assign cards with only one possible owner again after assignments
        for(int card=0; card<num_cards; card++){
            int owners = possible[card];
            int cnt = count_bits_set(owners);
            if(cnt ==1){
                // Already assigned
                continue;
            }
        }
        // Process suggestions
        for(int m=0; m<n; m++){
            Suggestion sug = suggestions[m];
            int s = sug.suggester;
            // Determine responders
            vector<int> responders;
            responders.push_back( ((s)%4) +1 );
            responders.push_back( ((s+1)%4) +1 );
            responders.push_back( ((s+2)%4) +1 );
            // Get the responses
            int num_responses = sug.responses.size();
            if(num_responses ==0){
                continue;
            }
            string last_response = sug.responses[num_responses-1];
            if(last_response == "-"){
                // Remove p,w,r from all responders
                for(int i=0; i<num_responses; i++){
                    int responder = responders[i];
                    // Remove p,w,r from responder's possible holdings
                    // p
                    if(eliminate_owner(sug.p, 1<< (responder-1), possible)){
                        changed = true;
                    }
                    // w
                    if(eliminate_owner(sug.w, 1<< (responder-1), possible)){
                        changed = true;
                    }
                    // r
                    if(eliminate_owner(sug.r, 1<< (responder-1), possible)){
                        changed = true;
                    }
                }
                // If suggester is player1, assign p,w,r to SOLUTION if you don't have them
                if(s ==1){
                    // Check player1's holdings
                    // Player1 is owner 1
                    auto has_card = [&](int c) -> bool {
                        return (possible[c] & PLAYER1);
                    };
                    for(auto &c : vector<int>{sug.p, sug.w, sug.r}){
                        if(!has_card(c)){
                            // Assign to SOLUTION
                            if(eliminate_owner(c, PLAYER2 | PLAYER3 | PLAYER4, possible)){
                                changed = true;
                            }
                            if(assign_card(c, SOLUTION, possible)){
                                changed = true;
                            }
                        }
                    }
                }
            }
            else{
                if(last_response != "*"){
                    // Assign the specific card to the last responder
                    // Find which responder gave the last response
                    int responder = responders[num_responses-1];
                    int c = char_to_idx(last_response[0]);
                    if(assign_card(c, 1 << (responder-1), possible)){
                        changed = true;
                    }
                }
                else{
                    // last_response == "*"
                    // The last responder has at least one of p,w,r
                    int responder = responders[num_responses-1];
                    // Count how many of p,w,r are possible for responder
                    vector<int> candidates;
                    for(auto &c : vector<int>{sug.p, sug.w, sug.r}){
                        if(possible[c] & (1 << (responder-1)) || (possible[c] & SOLUTION)){
                            candidates.push_back(c);
                        }
                    }
                    // If only one candidate, assign it
                    int valid =0;
                    int last_c = -1;
                    for(auto &c : candidates){
                        // If c can be with responder
                        if(possible[c] & (1 << (responder-1))){
                            valid++;
                            last_c = c;
                        }
                        else{
                            // If c can be in SOLUTION, do nothing
                        }
                    }
                    if(valid ==1 && last_c != -1){
                        if(assign_card(last_c, 1 << (responder-1), possible)){
                            changed = true;
                        }
                    }
                }
            }
        }
        // Additional pass for '*' responses to enforce at least one
        for(int m=0; m<n; m++){
            Suggestion sug = suggestions[m];
            int s = sug.suggester;
            // Determine responders
            vector<int> responders;
            responders.push_back( ((s)%4) +1 );
            responders.push_back( ((s+1)%4) +1 );
            responders.push_back( ((s+2)%4) +1 );
            // Get the responses
            int num_responses = sug.responses.size();
            if(num_responses ==0){
                continue;
            }
            string last_response = sug.responses[num_responses-1];
            if(last_response == "*"){
                int responder = responders[num_responses-1];
                // The responder must have at least one of p,w,r
                // Check how many of p,w,r are possible for responder
                vector<int> possible_cards;
                for(auto &c : vector<int>{sug.p, sug.w, sug.r}){
                    if(possible[c] & (1 << (responder-1))){
                        possible_cards.push_back(c);
                    }
                }
                if(possible_cards.size() ==1){
                    // Assign this card to responder
                    if(assign_card(possible_cards[0], 1 << (responder-1), possible)){
                        changed = true;
                    }
                }
            }
        }
    }
    // After constraint propagation, determine the solution
    // For each category, find which card is in SOLUTION
    string murderer = "?", weapon = "?", room = "?";
    // Persons
    int cnt =0;
    char m_char;
    for(auto &c : persons){
        if(possible[c] & SOLUTION){
            cnt++;
            m_char = 'A' + c;
        }
    }
    if(cnt ==1) murderer = string(1, m_char);
    // Weapons
    cnt =0;
    char w_char;
    for(auto &c : weapons){
        if(possible[c] & SOLUTION){
            cnt++;
            w_char = 'A' + c;
        }
    }
    if(cnt ==1) weapon = string(1, w_char);
    // Rooms
    cnt =0;
    char r_char;
    for(auto &c : rooms){
        if(possible[c] & SOLUTION){
            cnt++;
            r_char = 'A' + c;
        }
    }
    if(cnt ==1) room = string(1, r_char);
    // Output
    cout << murderer << weapon << room;
}
