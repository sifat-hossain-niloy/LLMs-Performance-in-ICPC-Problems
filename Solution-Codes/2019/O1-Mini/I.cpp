#include <bits/stdc++.h>
using namespace std;

// Define command types
enum CommandType { MOVE, TURN_LEFT, PROC_CALL, IF_CMD, UNTIL_CMD };

// Define Command structure
struct Command {
    CommandType type;
    // For PROC_CALL
    char proc_name;
    // For IF
    char condition;
    vector<Command> then_cmds;
    vector<Command> else_cmds;
    // For UNTIL
    // Reusing condition and cmds
    // No constructor needed
};

// Frame for execution stack
struct Frame {
    const vector<Command>* cmds;
    int pos;
};

// State of Karel
struct StateKarel {
    int i;
    int j;
    char h;
};

// Function to turn left
char turn_left(char h) {
    switch(h) {
        case 'n': return 'w';
        case 'w': return 's';
        case 's': return 'e';
        case 'e': return 'n';
    }
    return h; // should not reach here
}

// Function to get next position based on heading
pair<int, int> next_position(int i, int j, char h) {
    if(h == 'n') return {i-1, j};
    if(h == 's') return {i+1, j};
    if(h == 'e') return {i, j+1};
    if(h == 'w') return {i, j-1};
    return {i, j};
}

// Function to parse a program string into commands
vector<Command> parse_program(const string& s, int& pos) {
    vector<Command> commands;
    while (pos < s.size()) {
        char c = s[pos];
        if(c == 'm') {
            commands.push_back(Command{MOVE, 0, 0, {}, {}});
            pos +=1;
        }
        else if(c == 'l') {
            commands.push_back(Command{TURN_LEFT, 0, 0, {}, {}});
            pos +=1;
        }
        else if(c >= 'A' && c <= 'Z') {
            commands.push_back(Command{PROC_CALL, c, 0, {}, {}});
            pos +=1;
        }
        else if(c == 'i') {
            pos +=1;
            if(pos >= s.size()) break;
            char cond = s[pos];
            pos +=1;
            if(pos >= s.size() || s[pos] != '(') break;
            pos +=1;
            vector<Command> then_cmds = parse_program(s, pos);
            if(pos >= s.size() || s[pos] != ')') break;
            pos +=1;
            if(pos >= s.size() || s[pos] != '(') break;
            pos +=1;
            vector<Command> else_cmds = parse_program(s, pos);
            if(pos >= s.size() || s[pos] != ')') break;
            pos +=1;
            commands.push_back(Command{IF_CMD, 0, cond, then_cmds, else_cmds});
        }
        else if(c == 'u') {
            pos +=1;
            if(pos >= s.size()) break;
            char cond = s[pos];
            pos +=1;
            if(pos >= s.size() || s[pos] != '(') break;
            pos +=1;
            vector<Command> until_cmds = parse_program(s, pos);
            if(pos >= s.size() || s[pos] != ')') break;
            pos +=1;
            commands.push_back(Command{UNTIL_CMD, 0, cond, until_cmds, {}});
        }
        else if(c == ')') {
            break;
        }
        else {
            // Invalid character, skip
            pos +=1;
        }
    }
    return commands;
}

// Function to check condition
bool check_condition(char cond, const StateKarel& state, const vector<string>& grid, int r, int c) {
    if(cond == 'b') {
        pair<int, int> np = next_position(state.i, state.j, state.h);
        int ni = np.first;
        int nj = np.second;
        if(ni < 1 || ni > r || nj <1 || nj > c) return true;
        if(grid[ni-1][nj-1] == '#') return true;
        return false;
    }
    else {
        return state.h == cond;
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int r, c, d, e;
    cin >> r >> c >> d >> e;
    // Read grid
    vector<string> grid(r);
    for(int i=0;i<r;i++) {
        cin >> grid[i];
    }
    // Read procedure definitions
    map<char, vector<Command>> procedures;
    for(int i=0;i<d;i++) {
        string line;
        cin >> line;
        // Split at '='
        int eq = line.find('=');
        char proc_name = line[0];
        string proc_prog = line.substr(eq+1);
        int pos = 0;
        vector<Command> proc_commands = parse_program(proc_prog, pos);
        procedures[proc_name] = proc_commands;
    }
    // Read e program executions
    for(int exec=0; exec<e; exec++) {
        // Read initial state
        int init_i, init_j;
        char init_h;
        cin >> init_i >> init_j >> init_h;
        // Read program string
        string prog_str;
        cin >> prog_str;
        // Parse main program
        int pos = 0;
        vector<Command> main_program = parse_program(prog_str, pos);
        // Initialize Karel's state
        StateKarel state;
        state.i = init_i;
        state.j = init_j;
        state.h = init_h;
        // Initialize execution stack
        vector<Frame> stack;
        stack.push_back(Frame{&main_program, 0});
        // Initialize step count
        long long step_count = 0;
        bool infinite = false;
        while(!stack.empty()) {
            Frame &current = stack.back();
            if(current.pos >= current.cmds->size()) {
                stack.pop_back();
                continue;
            }
            // Get the current command
            Command cmd = (*current.cmds)[current.pos];
            current.pos +=1;
            // Increment step count
            step_count +=1;
            if(step_count > 100000){
                infinite = true;
                break;
            }
            // Execute the command
            if(cmd.type == MOVE){
                // Move forward if possible
                pair<int, int> np = next_position(state.i, state.j, state.h);
                int ni = np.first;
                int nj = np.second;
                if(ni >=1 && ni <=r && nj >=1 && nj <=c && grid[ni-1][nj-1] == '.') {
                    state.i = ni;
                    state.j = nj;
                }
            }
            else if(cmd.type == TURN_LEFT){
                state.h = turn_left(state.h);
            }
            else if(cmd.type == PROC_CALL){
                // Push procedure commands
                if(procedures.find(cmd.proc_name) != procedures.end()){
                    stack.push_back(Frame{&procedures[cmd.proc_name], 0});
                }
                else{
                    // Undefined procedure, skip
                }
            }
            else if(cmd.type == IF_CMD){
                bool cond = check_condition(cmd.condition, state, grid, r, c);
                if(cond){
                    if(!cmd.then_cmds.empty()){
                        stack.push_back(Frame{&cmd.then_cmds, 0});
                    }
                }
                else{
                    if(!cmd.else_cmds.empty()){
                        stack.push_back(Frame{&cmd.else_cmds, 0});
                    }
                }
            }
            else if(cmd.type == UNTIL_CMD){
                bool cond = check_condition(cmd.condition, state, grid, r, c);
                if(!cond){
                    // To execute P then UNTIL C P again
                    if(!cmd.until_cmds.empty()){
                        stack.push_back(Frame{&cmd.until_cmds, 0});
                    }
                    stack.push_back(Frame{&stack.back().cmds->at(current.pos-1).until_cmds, 0});
                }
                // If condition holds, do nothing
            }
        }
        if(infinite){
            cout << "inf\n";
        }
        else{
            cout << state.i << " " << state.j << " " << state.h << "\n";
        }
    }
}
