#include <bits/stdc++.h>
using namespace std;

struct State {
    long long c;
    long long d;
    string program;
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int a, m, p, q, r, s;
    int case_num =1;
    while(cin >> a >> m >> p >> q >> r >> s){
        if(a ==0 && m ==0 && p ==0 && q ==0 && r ==0 && s ==0){
            break;
        }
        // Handle m ==1 separately
        if(m ==1){
            // Check if initial state satisfies
            long long c =1;
            long long d =0;
            bool empty_ok = (c * p + d >= r) && (c * q + d <=s);
            if(empty_ok){
                cout << "Case " << case_num++ << ": empty\n";
                continue;
            }
            // Find d_min and d_max
            long long d_min = max((long long)(r - (c * p)), (long long)0);
            long long d_max = (long long)(s - (c * q));
            if(d_min > d_max){
                cout << "Case " << case_num++ << ": impossible\n";
                continue;
            }
            // Find minimal k such that k *a >=d_min and k *a <=d_max
            long long k = (d_min + a -1)/a;
            if(k * a > d_max){
                cout << "Case " << case_num++ << ": impossible\n";
                continue;
            }
            // Output kA
            string output = to_string(k) + "A";
            cout << "Case " << case_num++ << ": " << output << "\n";
            continue;
        }
        // m >1
        // Check if initial state satisfies
        long long c_init =1;
        long long d_init =0;
        bool empty_ok = (c_init * p + d_init >= r) && (c_init * q + d_init <=s);
        if(empty_ok){
            cout << "Case " << case_num++ << ": empty\n";
            continue;
        }
        // BFS
        queue<State> q_bfs;
        State start;
        start.c = c_init;
        start.d = d_init;
        start.program = "";
        q_bfs.push(start);
        // Visited map
        // Since c is built by multiplying m, and m>1, c is limited
        // Use map from c to set of d's
        unordered_map<long long, unordered_set<long long>> visited;
        visited[c_init].insert(d_init);
        bool found = false;
        string found_program = "";
        while(!q_bfs.empty()){
            int sz = q_bfs.size();
            for(int i=0;i<sz;i++){
                State current = q_bfs.front(); q_bfs.pop();
                // Apply A
                long long c_a = current.c;
                long long d_a = current.d + a;
                // Check constraints
                long long d_min_a = max((long long)(r - c_a * p), (long long)0);
                long long d_max_a = (long long)(s - c_a * q);
                if(d_a >= d_min_a && d_a <= d_max_a){
                    // Found a solution
                    string prog = current.program + "A";
                    // Now, need to group and output
                    // But since BFS, this is the minimal length and lex smallest
                    // So, set found and break
                    // But we need to reconstruct the program
                    // However, we will collect it here
                    // To group operations later
                    // Thus, set found_program and break all loops
                    // But since we need to find the first in BFS order, return it
                    // So, collect and output
                    // To ensure lex order, BFS explores A first
                    // Thus, this is the desired program
                    // So, set found_program and break
                    found_program = prog;
                    found = true;
                    break;
                }
                // Else, enqueue A if possible
                if(d_a >= d_min_a && d_a <= d_max_a){
                    if(visited.find(c_a) == visited.end() || visited[c_a].find(d_a) == visited[c_a].end()){
                        State next_a;
                        next_a.c = c_a;
                        next_a.d = d_a;
                        if(current.program.empty()) next_a.program = "A";
                        else next_a.program = current.program + "A";
                        q_bfs.push(next_a);
                        visited[c_a].insert(d_a);
                    }
                }
                // Apply M
                long long c_m = current.c * (long long)m;
                if(m !=0 && c_m >0){
                    // Check c_m <= floor(s /p)
                    if(p !=0){
                        if(c_m > (long long)(s / p)){
                            // Cannot apply M
                        }
                        else{
                            long long d_m = current.d * (long long)m;
                            // Check constraints
                            long long d_min_m = max((long long)(r - c_m * p), (long long)0);
                            long long d_max_m = (long long)(s - c_m * q);
                            if(d_m >= d_min_m && d_m <= d_max_m){
                                // Found a solution
                                string prog = current.program + "M";
                                found_program = prog;
                                found = true;
                                break;
                            }
                            // Else, enqueue M if possible
                            if(d_m >= d_min_m && d_m <= d_max_m){
                                if(visited.find(c_m) == visited.end() || visited[c_m].find(d_m) == visited[c_m].end()){
                                    State next_m;
                                    next_m.c = c_m;
                                    next_m.d = d_m;
                                    if(current.program.empty()) next_m.program = "M";
                                    else next_m.program = current.program + "M";
                                    q_bfs.push(next_m);
                                    visited[c_m].insert(d_m);
                                }
                            }
                        }
                    }
                }
            }
            if(found){
                break;
            }
        }
        if(found){
            // Group the found_program
            string program = found_program;
            // Now, check if program is empty
            if(program.empty()){
                cout << "Case " << case_num++ << ": empty\n";
                continue;
            }
            // Group the operations
            string grouped = "";
            char current_op = program[0];
            int count =1;
            for(int i=1;i<program.size();i++){
                if(program[i] == current_op){
                    count++;
                }
                else{
                    grouped += to_string(count) + current_op + " ";
                    current_op = program[i];
                    count =1;
                }
            }
            grouped += to_string(count) + current_op;
            cout << "Case " << case_num++ << ": " << grouped << "\n";
            continue;
        }
        // If not found yet, continue BFS with proper constraints
        // Restart BFS
        // Implement BFS correctly
        // Reset queue and visited
        // Implement the BFS correctly
        // Re-initialize
        // Restart BFS
        // Implement properly
        // Here, let's implement a correct BFS
        // Restart
        // Clear queue and visited
        while(!q_bfs.empty()) q_bfs.pop();
        q_bfs.push(start);
        visited.clear();
        visited[c_init].insert(d_init);
        found = false;
        found_program = "";
        while(!q_bfs.empty()){
            int sz = q_bfs.size();
            for(int i=0;i<sz;i++){
                State current = q_bfs.front(); q_bfs.pop();
                // Check if current satisfies
                if((current.c * p + current.d >= r) && (current.c * q + current.d <=s)){
                    // Found a solution
                    found_program = current.program;
                    found = true;
                    break;
                }
                // Apply A
                long long c_a_new = current.c;
                long long d_a_new = current.d + a;
                // Check constraints
                long long d_min_a_new = max((long long)(r - c_a_new * p), (long long)0);
                long long d_max_a_new = (long long)(s - c_a_new * q);
                if(d_a_new >= d_min_a_new && d_a_new <= d_max_a_new){
                    if(visited.find(c_a_new) == visited.end() || visited[c_a_new].find(d_a_new) == visited[c_a_new].end()){
                        State next_a;
                        next_a.c = c_a_new;
                        next_a.d = d_a_new;
                        if(current.program.empty()) next_a.program = "A";
                        else next_a.program = current.program + "A";
                        q_bfs.push(next_a);
                        visited[c_a_new].insert(d_a_new);
                        // Check if this new state satisfies
                        if((next_a.c * p + next_a.d >= r) && (next_a.c * q + next_a.d <=s)){
                            found_program = next_a.program;
                            found = true;
                            break;
                        }
                    }
                }
                // Apply M
                long long c_m_new = current.c * (long long)m;
                if(p !=0 && c_m_new > (long long)(s / p)){
                    // Cannot apply M, skip
                }
                else{
                    long long d_m_new = current.d * (long long)m;
                    // Check constraints
                    long long d_min_m_new = max((long long)(r - c_m_new * p), (long long)0);
                    long long d_max_m_new = (long long)(s - c_m_new * q);
                    if(d_m_new >= d_min_m_new && d_m_new <= d_max_m_new){
                        if(visited.find(c_m_new) == visited.end() || visited[c_m_new].find(d_m_new) == visited[c_m_new].end()){
                            State next_m;
                            next_m.c = c_m_new;
                            next_m.d = d_m_new;
                            if(current.program.empty()) next_m.program = "M";
                            else next_m.program = current.program + "M";
                            q_bfs.push(next_m);
                            visited[c_m_new].insert(d_m_new);
                            // Check if this new state satisfies
                            if((next_m.c * p + next_m.d >= r) && (next_m.c * q + next_m.d <=s)){
                                found_program = next_m.program;
                                found = true;
                                break;
                            }
                        }
                    }
                }
            }
            if(found){
                break;
            }
        }
        if(found){
            // Group the found_program
            string program = found_program;
            if(program.empty()){
                cout << "Case " << case_num++ << ": empty\n";
                continue;
            }
            string grouped = "";
            char current_op = program[0];
            int count =1;
            for(int i=1;i<program.size();i++){
                if(program[i] == current_op){
                    count++;
                }
                else{
                    grouped += to_string(count) + current_op + " ";
                    current_op = program[i];
                    count =1;
                }
            }
            grouped += to_string(count) + current_op;
            cout << "Case " << case_num++ << ": " << grouped << "\n";
            continue;
        }
        // If not found, output impossible
        cout << "Case " << case_num++ << ": impossible\n";
    }
}
