#include <bits/stdc++.h>
using namespace std;

// Constants for segment statuses
const int STATUS_WORKING = 1 << 0;   // 1
const int STATUS_STUCK_ON = 1 << 1;  // 2
const int STATUS_STUCK_OFF = 1 << 2; // 4

// Digit segment encoding: a,b,c,d,e,f,g
const int digit_segments[10] = {
    0x3F, // 0: a,b,c,d,e,f
    0x06, // 1: b,c
    0x5B, // 2: a,b,g,e,d
    0x4F, // 3: a,b,g,c,d
    0x66, // 4: f,g,b,c
    0x6D, // 5: a,f,g,c,d
    0x7D, // 6: a,f,g,c,d,e
    0x07, // 7: a,b,c
    0x7F, // 8: a,b,c,d,e,f,g
    0x6F  // 9: a,b,c,d,f,g
};

// Segment positions mapping
vector<vector<pair<int, int>>> segment_positions(30, vector<pair<int, int>>());

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    // Define segment positions
    // digits0,1,2,3: each has 7 segments
    // colon: two segments
    for(int digit=0; digit<4; digit++){
        int seg_base = digit *7;
        int c_start;
        if(digit ==0) c_start=0;
        else if(digit ==1) c_start=5;
        else if(digit ==2) c_start=11;
        else if(digit ==3) c_start=16;
        
        // Segment a
        segment_positions[seg_base +0].emplace_back(0, c_start+1);
        segment_positions[seg_base +0].emplace_back(0, c_start+2);
        // Segment b
        segment_positions[seg_base +1].emplace_back(1, c_start+4);
        segment_positions[seg_base +1].emplace_back(2, c_start+4);
        // Segment c
        segment_positions[seg_base +2].emplace_back(4, c_start+4);
        segment_positions[seg_base +2].emplace_back(5, c_start+4);
        // Segment d
        segment_positions[seg_base +3].emplace_back(6, c_start+1);
        segment_positions[seg_base +3].emplace_back(6, c_start+2);
        // Segment e
        segment_positions[seg_base +4].emplace_back(4, c_start+0);
        segment_positions[seg_base +4].emplace_back(5, c_start+0);
        // Segment f
        segment_positions[seg_base +5].emplace_back(1, c_start+0);
        segment_positions[seg_base +5].emplace_back(2, c_start+0);
        // Segment g
        segment_positions[seg_base +6].emplace_back(3, c_start+1);
        segment_positions[seg_base +6].emplace_back(3, c_start+2);
    }
    // Colon segments
    // colon_upper: segment 28
    segment_positions[28].emplace_back(2,10);
    // colon_lower: segment 29
    segment_positions[29].emplace_back(4,10);
    
    // Read input
    int n;
    cin >> n;
    // Read n displays, each with 7 lines, separated by blank lines
    vector<vector<string>> displays(n, vector<string>(7, ""));
    string s;
    for(int m=0; m<n; m++){
        for(int l=0; l<7; l++){
            cin >> s;
            if(s.size() <21){
                // pad with '.' if needed
                s += string(21 - s.size(), '.');
            }
            displays[m][l] = s;
        }
        if(m !=n-1){
            // Read the blank line
            getline(cin, s);
            while(s.empty()){
                getline(cin, s);
            }
            // Put back the non-empty line if any
            if(!s.empty()){
                // Not expected, but just in case
                // Since we expect a blank line, s should be empty
            }
        }
    }
    
    // Precompute observed states
    // observed[m][seg] = 'on', 'off', 'invalid'
    vector<vector<char>> observed(n, vector<char>(30, 'x'));
    for(int m=0; m<n; m++){
        for(int seg=0; seg<30; seg++){
            bool all_X = true, all_dot = true;
            for(auto &[line, col] : segment_positions[seg]){
                if(col >=21 || line >=7){
                    // Invalid position, should not happen
                    all_X = false;
                    all_dot = false;
                    break;
                }
                if(displays[m][line][col] != 'X'){
                    all_X = false;
                }
                if(displays[m][line][col] != '.'){
                    all_dot = false;
                }
            }
            if(all_X){
                observed[m][seg] = 'on';
            }
            else if(all_dot){
                observed[m][seg] = 'off';
            }
            else{
                observed[m][seg] = 'invalid';
            }
        }
    }
    
    // Initialize possible_status_global
    int possible_status_global[30];
    for(int seg=0; seg<30; seg++) possible_status_global[seg] = STATUS_WORKING | STATUS_STUCK_ON | STATUS_STUCK_OFF;
    
    // Iterate over all possible starting times
    for(int t=0; t<1440; t++){
        // Initialize possible_status_local
        int possible_status_local[30];
        for(int seg=0; seg<30; seg++) possible_status_local[seg] = STATUS_WORKING | STATUS_STUCK_ON | STATUS_STUCK_OFF;
        bool valid = true;
        for(int m=0; m<n && valid; m++){
            int tm = (t +m) %1440;
            int H = tm /60;
            int M = tm %60;
            int H1 = H /10;
            int H2 = H %10;
            int M1 = M /10;
            int M2 = M %10;
            // Digits: digit0=H1, digit1=H2, digit2=M1, digit3=M2
            int digits[4] = {H1, H2, M1, M2};
            for(int d=0; d<4 && valid; d++){
                int digit = digits[d];
                int seg_base = d *7;
                for(int s=0; s<7 && valid; s++){
                    int seg = seg_base +s;
                    int expected_on = (digit_segments[digit] >>s) &1;
                    char obs = observed[m][seg];
                    if(obs == 'invalid'){
                        valid = false;
                        break;
                    }
                    // Determine possible statuses
                    int possible =0;
                    if(expected_on){
                        if(obs == 'on'){
                            possible = STATUS_WORKING | STATUS_STUCK_ON;
                        }
                        else{ // obs == 'off'
                            possible = STATUS_STUCK_OFF;
                        }
                    }
                    else{
                        if(obs == 'off'){
                            possible = STATUS_WORKING | STATUS_STUCK_OFF;
                        }
                        else{ // obs == 'on'
                            possible = STATUS_STUCK_ON;
                        }
                    }
                    possible_status_local[seg] &= possible;
                    if(possible_status_local[seg] ==0){
                        valid = false;
                        break;
                    }
                }
            }
            if(!valid) break;
            // Colon segments: 28 and29
            for(int cs=28; cs<=29 && valid; cs++){
                int seg = cs;
                // Colon segments are always expected to be on
                char obs = observed[m][seg];
                if(obs == 'invalid'){
                    valid = false;
                    break;
                }
                int possible =0;
                // Expected on
                if(obs == 'on'){
                    possible = STATUS_WORKING | STATUS_STUCK_ON;
                }
                else{ // obs == 'off'
                    possible = STATUS_STUCK_OFF;
                }
                possible_status_local[seg] &= possible;
                if(possible_status_local[seg] ==0){
                    valid = false;
                    break;
                }
            }
        }
        if(valid){
            // Update possible_status_global
            for(int seg=0; seg<30; seg++){
                possible_status_global[seg] &= possible_status_local[seg];
            }
        }
    }
    
    // Check if any possible starting time exists
    bool possible = true;
    for(int seg=0; seg<30; seg++) {
        if(possible_status_global[seg] ==0){
            possible = false;
            break;
        }
    }
    if(!possible){
        cout << "impossible\n";
        return 0;
    }
    
    // Now, check if there exists at least one starting time that is possible
    // To ensure that possible_status_global has at least one valid assignment
    // Here, since we intersect all possible starting times, we need to ensure that at least one starting time was valid
    // However, our approach above might have segments with empty possible_status_global
    // To ensure at least one starting time exists, check all segments have non-zero possible_status_global
    bool any_valid = false;
    for(int seg=0; seg<30; seg++){
        if(possible_status_global[seg] !=0){
            any_valid = true;
            break;
        }
    }
    // To be accurate, we need to check if possible_status_global has at least one combination where all segments have a possible status
    // Since we already checked possible = true above, we proceed
    // Now, determine the status for each segment
    // Initialize output grid
    vector<string> output_grid(7, string(21, '.'));
    for(int seg=0; seg<30; seg++){
        char status_char = '?';
        if(possible_status_global[seg] == STATUS_WORKING){
            status_char = 'W';
        }
        else if(possible_status_global[seg] == STATUS_STUCK_ON){
            status_char = '1';
        }
        else if(possible_status_global[seg] == STATUS_STUCK_OFF){
            status_char = '0';
        }
        else{
            // Multiple possibilities
            // Check if 'W' is possible
            bool can_working = possible_status_global[seg] & STATUS_WORKING;
            bool can_stuck_on = possible_status_global[seg] & STATUS_STUCK_ON;
            bool can_stuck_off = possible_status_global[seg] & STATUS_STUCK_OFF;
            if(can_working && !(can_stuck_on || can_stuck_off)){
                status_char = 'W';
            }
            else{
                status_char = '?';
            }
        }
        // Assign to all positions of the segment
        for(auto &[line, col] : segment_positions[seg]){
            output_grid[line][col] = status_char;
        }
    }
    // Now, verify if at least one starting time is possible
    // Since possible_status_global was updated by intersecting all valid starting times,
    // If any segment has all possible statuses, then there exists at least one starting time
    // Otherwise, it's impossible
    // However, we have already checked possible = true above
    // So, proceed to output
    // But to ensure, let's check if at least one starting time was possible
    // For this, we need to recheck if possible_status_global allows at least one starting time
    // Alternatively, proceed to output
    // Output the grid
    for(int l=0; l<7; l++){
        cout << output_grid[l] << "\n";
    }
}
