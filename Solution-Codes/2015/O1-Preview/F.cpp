#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <cstring>
#include <climits>

using namespace std;

const int MAX_R = 50;
const int MAX_C = 50;
const int INF = INT_MAX;

int r, c;
char grid[MAX_R][MAX_C];
int key_id[MAX_R][MAX_C];
int dist[40][MAX_R][MAX_C]; // dist[key][i][j]: distance from key to position (i,j)
int move_dist[40][40]; // move_dist[k1][k2]: minimal distance from key k1 to key k2
int start_dist[MAX_R][MAX_C]; // Distance from initial position to all positions
int S[40]; // Minimal strokes from initial position to key k
int key_count;
map<char, int> key_map; // Map from character to key index
vector<pair<int, int>> key_positions[40]; // Positions belonging to each key
int dx[4] = {-1, 0, 1, 0}; // Up, Right, Down, Left
int dy[4] = {0, 1, 0, -1};

int rdx[4] = {0, 1, 0, -1}; // Right, Down, Left, Up
int rdy[4] = {1, 0, -1, 0};

int main() {
    cin >> r >> c;
    for (int i = 0; i < r; ++i) {
        cin >> grid[i];
    }
    // Assign key IDs
    memset(key_id, -1, sizeof(key_id));
    key_count = 0;
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            char ch = grid[i][j];
            if (key_map.find(ch) == key_map.end()) {
                key_map[ch] = key_count++;
            }
            int k = key_map[ch];
            key_id[i][j] = k;
            key_positions[k].push_back({i, j});
        }
    }
    // Precompute movement for each position and direction
    pair<int, int> move[MAX_R][MAX_C][4]; // move[i][j][dir]
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            int k = key_id[i][j];
            for (int dir = 0; dir < 4; ++dir) {
                int ni = i + dx[dir];
                int nj = j + dy[dir];
                while (ni >= 0 && ni < r && nj >= 0 && nj < c && key_id[ni][nj] == k) {
                    ni += dx[dir];
                    nj += dy[dir];
                }
                if (ni >= 0 && ni < r && nj >= 0 && nj < c) {
                    move[i][j][dir] = {ni, nj};
                } else {
                    move[i][j][dir] = {i, j}; // Cannot move
                }
            }
        }
    }
    // Precompute distances between keys
    memset(move_dist, -1, sizeof(move_dist));
    for (int k = 0; k < key_count; ++k) {
        int d[MAX_R][MAX_C];
        for (int i = 0; i < r; ++i) {
            fill(d[i], d[i] + c, INF);
        }
        queue<pair<int, int>> q;
        for (auto p : key_positions[k]) {
            int i = p.first, j = p.second;
            d[i][j] = 0;
            q.push({i, j});
        }
        while (!q.empty()) {
            auto curr = q.front(); q.pop();
            int i = curr.first, j = curr.second;
            int dist_curr = d[i][j];
            int curr_key = key_id[i][j];
            // If we reach a different key, record the minimal distance
            if (curr_key != k) {
                if (move_dist[k][curr_key] == -1 || dist_curr < move_dist[k][curr_key]) {
                    move_dist[k][curr_key] = dist_curr;
                }
                continue; // Do not continue BFS from here
            }
            for (int dir = 0; dir < 4; ++dir) {
                int ni = move[i][j][dir].first;
                int nj = move[i][j][dir].second;
                if (d[ni][nj] == INF) {
                    d[ni][nj] = dist_curr + 1;
                    q.push({ni, nj});
                }
            }
        }
    }
    // Compute distances from initial position
    for (int i = 0; i < r; ++i) {
        fill(start_dist[i], start_dist[i] + c, INF);
    }
    int start_i = 0, start_j = 0;
    start_dist[start_i][start_j] = 0;
    queue<pair<int, int>> q;
    q.push({start_i, start_j});
    while (!q.empty()) {
        auto curr = q.front(); q.pop();
        int i = curr.first, j = curr.second;
        int dist_curr = start_dist[i][j];
        int curr_key = key_id[i][j];
        // If we reach a key, record the minimal distance
        if (S[curr_key] == 0 || dist_curr < S[curr_key]) {
            S[curr_key] = dist_curr;
        }
        for (int dir = 0; dir < 4; ++dir) {
            int ni = move[i][j][dir].first;
            int nj = move[i][j][dir].second;
            if (start_dist[ni][nj] == INF) {
                start_dist[ni][nj] = dist_curr + 1;
                q.push({ni, nj});
            }
        }
    }
    // Read the text to be typed
    string text;
    cin.ignore();
    getline(cin, text);
    // Build the sequence of keys
    vector<int> text_keys;
    for (char ch : text) {
        int k = key_map[ch];
        text_keys.push_back(k);
    }
    int N = text_keys.size();
    int total_strokes = 0;
    // Start with the distance from initial position to first key
    int first_key = text_keys[0];
    total_strokes += S[first_key]; // Exclude select button
    total_strokes += 1; // Select button
    // For each pair of keys in the text
    for (int i = 1; i < N; ++i) {
        int prev_key = text_keys[i - 1];
        int curr_key = text_keys[i];
        total_strokes += move_dist[prev_key][curr_key]; // Exclude select button
        total_strokes += 1; // Select button
    }
    // Move from last key to Enter key
    int last_key = text_keys[N - 1];
    int enter_key = key_map['*'];
    total_strokes += move_dist[last_key][enter_key]; // Exclude select button
    total_strokes += 1; // Select button
    // Output the total strokes
    cout << total_strokes << endl;
    return 0;
}
