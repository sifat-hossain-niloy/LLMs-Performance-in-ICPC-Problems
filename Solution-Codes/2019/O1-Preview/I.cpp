#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cassert>

using namespace std;

const int MAX_STEPS = 1000000;

int r, c, d, e;
vector<string> grid;
map<char, string> procedures;
vector<pair<int, int>> initial_positions;
vector<int> initial_directions;
vector<string> programs_to_execute;

int dx[4] = {0, 1, 0, -1}; // n, e, s, w
int dy[4] = {-1, 0, 1, 0};

map<char, int> direction_map = {{'n', 0}, {'e',1}, {'s',2}, {'w',3}};
map<int, char> direction_map_inv = {{0,'n'},{1,'e'},{2,'s'},{3,'w'}};

struct State {
    int x, y; // position (column, row)
    int dir;  // direction 0:north, 1:east, 2:south, 3:west
    int steps;
    bool infinite_loop;
};

bool is_barrier(int x, int y) {
    if (x < 0 || x >= c || y < 0 || y >= r) return true;
    return grid[y][x] == '#';
}

bool evaluate_condition(State &state, char cond) {
    if (cond == 'b') {
        int nx = state.x + dx[state.dir];
        int ny = state.y + dy[state.dir];
        return is_barrier(nx, ny);
    } else {
        return state.dir == direction_map[cond];
    }
}

bool execute_program(State &state, const string &program, int &pos);

bool execute_command(State &state, const string &program, int &pos) {
    if (state.steps > MAX_STEPS) {
        state.infinite_loop = true;
        return false;
    }
    if (pos >= program.length()) return true;
    char cmd = program[pos++];
    if (cmd == 'm') {
        state.steps++;
        int nx = state.x + dx[state.dir];
        int ny = state.y + dy[state.dir];
        if (!is_barrier(nx, ny)) {
            state.x = nx;
            state.y = ny;
        }
    } else if (cmd == 'l') {
        state.steps++;
        state.dir = (state.dir + 3) % 4; // turn left
    } else if ('A' <= cmd && cmd <= 'Z') {
        state.steps++;
        string &proc_body = procedures[cmd];
        int proc_pos = 0;
        if (!execute_program(state, proc_body, proc_pos)) {
            return false;
        }
    } else if (cmd == 'i') {
        if (pos >= program.length()) return false;
        char cond = program[pos++];
        if (pos >= program.length() || program[pos++] != '(') return false;
        int start_pos = pos;
        int paren_count = 1;
        while (paren_count > 0 && pos < program.length()) {
            if (program[pos] == '(') paren_count++;
            else if (program[pos] == ')') paren_count--;
            pos++;
        }
        if (paren_count != 0) return false;
        int mid_pos = pos - 1;
        if (pos >= program.length() || program[pos++] != '(') return false;
        int start_pos2 = pos;
        paren_count = 1;
        while (paren_count > 0 && pos < program.length()) {
            if (program[pos] == '(') paren_count++;
            else if (program[pos] == ')') paren_count--;
            pos++;
        }
        if (paren_count != 0) return false;
        int end_pos = pos - 1;
        bool cond_result = evaluate_condition(state, cond);
        int temp_pos = cond_result ? start_pos : start_pos2;
        int temp_end = cond_result ? mid_pos : end_pos;
        while (temp_pos < temp_end) {
            if (!execute_command(state, program, temp_pos)) return false;
        }
    } else if (cmd == 'u') {
        if (pos >= program.length()) return false;
        char cond = program[pos++];
        if (pos >= program.length() || program[pos++] != '(') return false;
        int start_pos = pos;
        int paren_count = 1;
        while (paren_count > 0 && pos < program.length()) {
            if (program[pos] == '(') paren_count++;
            else if (program[pos] == ')') paren_count--;
            pos++;
        }
        if (paren_count != 0) return false;
        int end_pos = pos - 1;
        while (!evaluate_condition(state, cond)) {
            int temp_pos = start_pos;
            while (temp_pos < end_pos) {
                if (!execute_command(state, program, temp_pos)) return false;
            }
            state.steps++;
            if (state.steps > MAX_STEPS) {
                state.infinite_loop = true;
                return false;
            }
        }
    } else {
        // Invalid command
        return false;
    }
    return true;
}

bool execute_program(State &state, const string &program, int &pos) {
    while (pos < program.length()) {
        if (!execute_command(state, program, pos)) {
            return false;
        }
        if (state.infinite_loop) return false;
    }
    return true;
}

int main() {
    cin >> r >> c >> d >> e;
    cin.ignore();
    grid.resize(r);
    for (int i = 0; i < r; i++) {
        getline(cin, grid[i]);
    }
    for (int i = 0; i < d; i++) {
        string line;
        getline(cin, line);
        char proc_name = line[0];
        assert(line[1] == '=');
        string proc_body = line.substr(2);
        procedures[proc_name] = proc_body;
    }
    for (int i = 0; i < e; i++) {
        int row, col;
        char dir_char;
        cin >> row >> col >> dir_char;
        cin.ignore();
        string program_line;
        getline(cin, program_line);
        initial_positions.push_back({col - 1, row - 1}); // convert to 0-based index
        initial_directions.push_back(direction_map[dir_char]);
        programs_to_execute.push_back(program_line);
    }
    for (int i = 0; i < e; i++) {
        State state;
        state.x = initial_positions[i].first;
        state.y = initial_positions[i].second;
        state.dir = initial_directions[i];
        state.steps = 0;
        state.infinite_loop = false;
        int pos = 0;
        execute_program(state, programs_to_execute[i], pos);
        if (state.infinite_loop) {
            cout << "inf" << endl;
        } else {
            cout << state.y + 1 << " " << state.x + 1 << " " << direction_map_inv[state.dir] << endl;
        }
    }
    return 0;
}
