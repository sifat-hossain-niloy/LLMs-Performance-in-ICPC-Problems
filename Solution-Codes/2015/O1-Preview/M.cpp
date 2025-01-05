#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <queue>
#include <algorithm>

using namespace std;

struct Window {
    int x, y, w, h;
    int id; // order of opening
    Window(int x_, int y_, int w_, int h_, int id_) : x(x_), y(y_), w(w_), h(h_), id(id_) {}
};

int xmax, ymax;
vector<Window> windows;
vector<string> errors;

bool ranges_overlap(int a1, int a2, int b1, int b2) {
    return a1 < b2 && b1 < a2;
}

bool check_overlap(Window &w1, Window &w2) {
    return ranges_overlap(w1.x, w1.x + w1.w, w2.x, w2.x + w2.w) &&
           ranges_overlap(w1.y, w1.y + w1.h, w2.y, w2.y + w2.h);
}

bool window_fits(Window &w, int exclude_id = -1) {
    if (w.x < 0 || w.y < 0 || w.x + w.w > xmax || w.y + w.h > ymax) {
        return false;
    }
    for (auto &other : windows) {
        if (other.id != exclude_id) {
            if (check_overlap(w, other)) {
                return false;
            }
        }
    }
    return true;
}

Window* find_window_at(int x, int y) {
    for (auto &w : windows) {
        if (x >= w.x && x < w.x + w.w && y >= w.y && y < w.y + w.h) {
            return &w;
        }
    }
    return nullptr;
}

void process_open(int cmd_num, int x, int y, int w, int h) {
    Window new_window(x, y, w, h, cmd_num);
    if (!window_fits(new_window)) {
        errors.push_back("Command " + to_string(cmd_num) + ": OPEN - window does not fit");
    } else {
        windows.push_back(new_window);
    }
}

void process_close(int cmd_num, int x, int y) {
    Window* w = find_window_at(x, y);
    if (!w) {
        errors.push_back("Command " + to_string(cmd_num) + ": CLOSE - no window at given position");
    } else {
        // Remove the window
        windows.erase(remove_if(windows.begin(), windows.end(),
                                [w](Window &win) { return win.id == w->id; }),
                      windows.end());
    }
}

void process_resize(int cmd_num, int x, int y, int w_new, int h_new) {
    Window* w = find_window_at(x, y);
    if (!w) {
        errors.push_back("Command " + to_string(cmd_num) + ": RESIZE - no window at given position");
    } else {
        // Temporarily change size
        int w_old = w->w;
        int h_old = w->h;
        w->w = w_new;
        w->h = h_new;
        if (!window_fits(*w, w->id)) {
            // Revert changes
            w->w = w_old;
            w->h = h_old;
            errors.push_back("Command " + to_string(cmd_num) + ": RESIZE - window does not fit");
        }
    }
}

void process_move(int cmd_num, int x, int y, int dx, int dy) {
    Window* w = find_window_at(x, y);
    if (!w) {
        errors.push_back("Command " + to_string(cmd_num) + ": MOVE - no window at given position");
        return;
    }
    int D = abs(dx != 0 ? dx : dy);
    int direction = dx != 0 ? (dx > 0 ? 1 : -1) : (dy > 0 ? 1 : -1);
    char move_dim = dx != 0 ? 'x' : 'y';
    char ortho_dim = dx != 0 ? 'y' : 'x';

    int movement_distance = D;

    vector<Window*> windows_to_move;
    windows_to_move.push_back(w);
    queue<Window*> q;
    q.push(w);

    while (!q.empty()) {
        Window* curr_w = q.front();
        q.pop();

        int movement_allowed;
        if (move_dim == 'x') {
            if (direction > 0) {
                movement_allowed = xmax - (curr_w->x + curr_w->w);
            } else {
                movement_allowed = curr_w->x;
            }
        } else {
            if (direction > 0) {
                movement_allowed = ymax - (curr_w->y + curr_w->h);
            } else {
                movement_allowed = curr_w->y;
            }
        }
        movement_allowed = min(movement_allowed, D);
        movement_distance = min(movement_distance, movement_allowed);

        // Check for collisions
        for (auto &other : windows) {
            if (find(windows_to_move.begin(), windows_to_move.end(), &other) != windows_to_move.end())
                continue;

            // Check if other overlaps in orthogonal dimension
            bool overlap_ortho;
            if (move_dim == 'x') {
                overlap_ortho = ranges_overlap(curr_w->y, curr_w->y + curr_w->h, other.y, other.y + other.h);
            } else {
                overlap_ortho = ranges_overlap(curr_w->x, curr_w->x + curr_w->w, other.x, other.x + other.w);
            }
            if (!overlap_ortho)
                continue;

            // Compute new position of curr_w after movement
            int new_x = curr_w->x;
            int new_y = curr_w->y;
            if (move_dim == 'x') {
                new_x += movement_distance * direction;
            } else {
                new_y += movement_distance * direction;
            }

            // Check for overlap after movement
            bool overlap_after_move = ranges_overlap(new_x, new_x + curr_w->w, other.x, other.x + other.w) &&
                                      ranges_overlap(new_y, new_y + curr_w->h, other.y, other.y + other.h);

            if (overlap_after_move) {
                windows_to_move.push_back(&other);
                q.push(&other);
            }
        }
    }

    // After processing, move all windows
    for (auto w_move : windows_to_move) {
        if (move_dim == 'x') {
            w_move->x += movement_distance * direction;
        } else {
            w_move->y += movement_distance * direction;
        }
    }

    if (movement_distance < D) {
        errors.push_back("Command " + to_string(cmd_num) + ": MOVE - moved " + to_string(movement_distance) + " instead of " + to_string(D));
    }
}

int main() {
    int cmd_num = 0;
    cin >> xmax >> ymax;
    string line;
    getline(cin, line); // consume rest of line
    while (getline(cin, line)) {
        cmd_num++;
        if (line.empty())
            continue;
        stringstream ss(line);
        string cmd;
        ss >> cmd;
        if (cmd == "OPEN") {
            int x, y, w, h;
            ss >> x >> y >> w >> h;
            process_open(cmd_num, x, y, w, h);
        } else if (cmd == "CLOSE") {
            int x, y;
            ss >> x >> y;
            process_close(cmd_num, x, y);
        } else if (cmd == "RESIZE") {
            int x, y, w, h;
            ss >> x >> y >> w >> h;
            process_resize(cmd_num, x, y, w, h);
        } else if (cmd == "MOVE") {
            int x, y, dx, dy;
            ss >> x >> y >> dx >> dy;
            process_move(cmd_num, x, y, dx, dy);
        } else {
            // Invalid command
        }
    }

    // Output errors
    for (auto &e : errors) {
        cout << e << endl;
    }

    // Output remaining windows
    cout << windows.size() << " window(s):" << endl;
    // Sort windows by their id (order they were opened)
    sort(windows.begin(), windows.end(), [](Window &a, Window &b) {
        return a.id < b.id;
    });
    for (auto &w : windows) {
        cout << w.x << " " << w.y << " " << w.w << " " << w.h << endl;
    }
    return 0;
}
