#include <bits/stdc++.h>
using namespace std;

struct Task {
    int id;
    int start_time;
    int base_priority;
    int a;
    vector<string> raw_instructions;
    vector<string> instructions;
    int ip; // instruction pointer
    vector<int> ownership_stack;
    int finish_time;
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int t, r;
    cin >> t >> r;
    vector<Task> tasks(t);
    for(int i=0;i<t;i++){
        Task &task = tasks[i];
        task.id = i;
        cin >> task.start_time >> task.base_priority >> task.a;
        task.raw_instructions.reserve(task.a);
        for(int j=0; j<task.a; j++){
            string instr;
            cin >> instr;
            task.raw_instructions.push_back(instr);
        }
        // Expand instructions
        for(auto &instr : task.raw_instructions){
            if(instr[0] == 'C'){
                int num = stoi(instr.substr(1));
                for(int k=0; k<num; k++) task.instructions.push_back("C");
            }
            else{
                task.instructions.push_back(instr);
            }
        }
        task.ip = 0;
        task.finish_time = -1;
    }
    // Compute resource ceilings
    // Resource numbered from 1 to r
    vector<int> ceiling(r+1, 0);
    for(int k=1; k<=r; k++){
        int max_b = 0;
        for(int i=0;i<t;i++){
            for(auto &instr : tasks[i].raw_instructions){
                if(instr[0] == 'L'){
                    int res = stoi(instr.substr(1));
                    if(res == k){
                        max_b = max(max_b, tasks[i].base_priority);
                    }
                }
            }
        }
        ceiling[k] = max_b;
    }
    // Initialize resource ownership
    // owner[k] = task id +1 (1-based), or -1 if not owned
    vector<int> owner(r+1, -1);
    // Initialize clock
    int clock = 0;
    // Number of finished tasks
    int finished = 0;
    while(finished < t){
        // Identify running tasks
        vector<int> running_tasks;
        for(int i=0;i<t;i++) {
            if(tasks[i].start_time <= clock && tasks[i].finish_time == -1){
                running_tasks.push_back(i);
            }
        }
        if(running_tasks.empty()){
            clock++;
            continue;
        }
        // Compute current_priority for running tasks
        // Initialize current_priority to base_priority
        unordered_map<int, int> current_priority;
        for(auto &i : running_tasks){
            current_priority[i] = tasks[i].base_priority;
        }
        bool changed = true;
        while(changed){
            changed = false;
            // Create a copy to hold new priorities
            unordered_map<int, int> new_current_priority = current_priority;
            for(auto &i : running_tasks){
                Task &T = tasks[i];
                if(T.ip >= T.instructions.size()) continue;
                string instr = T.instructions[T.ip];
                if(instr[0] == 'L'){
                    // Determine blocking_set
                    set<int> blocking_set;
                    // Parse k
                    int k = stoi(instr.substr(1));
                    if(owner[k] != -1){
                        blocking_set.insert(owner[k]);
                    }
                    for(int res=1; res<=r; res++){
                        if(owner[res] != -1 && ceiling[res] >= current_priority[i]){
                            blocking_set.insert(owner[res]);
                        }
                    }
                    if(!blocking_set.empty()){
                        // Find max priority among blockers
                        int max_p = 0;
                        for(auto &u : blocking_set){
                            if(current_priority.find(u)!=current_priority.end()){
                                max_p = max(max_p, current_priority[u]);
                            }
                            else{
                                // This blocker is not a running task, so its priority is its base priority
                                // But according to problem, blocking tasks are owning resources, which must be running
                                // because only running tasks can own resources
                                // So this case should not happen
                                // To be safe, assume blocker has base priority
                                // Find the task with id = u
                                int base_p = 0;
                                for(int ti=0; ti<t; ti++) if(ti == u){
                                    base_p = tasks[ti].base_priority;
                                    break;
                                }
                                max_p = max(max_p, base_p);
                            }
                        }
                        if(new_current_priority[i] < max_p){
                            new_current_priority[i] = max_p;
                            changed = true;
                        }
                    }
                }
            }
            // Check if priorities have changed
            for(auto &i : running_tasks){
                if(new_current_priority[i] > current_priority[i]){
                    current_priority[i] = new_current_priority[i];
                    changed = true;
                }
            }
            if(!changed){
                break;
            }
        }
        // Now, find non-blocked tasks
        vector<int> non_blocked_tasks;
        for(auto &i : running_tasks){
            Task &T = tasks[i];
            if(T.ip >= T.instructions.size()) continue;
            string instr = T.instructions[T.ip];
            if(instr[0] == 'L'){
                int k = stoi(instr.substr(1));
                set<int> blocking_set;
                if(owner[k] != -1){
                    blocking_set.insert(owner[k]);
                }
                for(int res=1; res<=r; res++){
                    if(owner[res] != -1 && ceiling[res] >= current_priority[i]){
                        blocking_set.insert(owner[res]);
                    }
                }
                if(blocking_set.empty()){
                    non_blocked_tasks.push_back(i);
                }
            }
            else{
                non_blocked_tasks.push_back(i);
            }
        }
        if(!non_blocked_tasks.empty()){
            // Find the task with highest current_priority
            int selected = non_blocked_tasks[0];
            for(auto &i : non_blocked_tasks){
                if(current_priority[i] > current_priority[selected]){
                    selected = i;
                }
            }
            Task &T = tasks[selected];
            string instr = T.instructions[T.ip];
            if(instr[0] == 'C'){
                // Compute
                T.ip++;
                clock++;
                if(T.ip == T.instructions.size()){
                    T.finish_time = clock;
                    finished++;
                }
            }
            else if(instr[0] == 'L'){
                // Lock
                int k = stoi(instr.substr(1));
                owner[k] = selected;
                T.ownership_stack.push_back(k);
                T.ip++;
                if(T.ip == T.instructions.size()){
                    T.finish_time = clock;
                    finished++;
                }
                // No clock increment
            }
            else if(instr[0] == 'U'){
                // Unlock
                int k = stoi(instr.substr(1));
                if(T.ownership_stack.empty() || T.ownership_stack.back() != k){
                    // Invalid unlock, but problem states input is correct
                }
                T.ownership_stack.pop_back();
                owner[k] = -1;
                T.ip++;
                if(T.ip == T.instructions.size()){
                    T.finish_time = clock;
                    finished++;
                }
                // No clock increment
            }
        }
        else{
            // No task was executed, increment clock
            clock++;
        }
    }
    // Output finish times in input order
    for(int i=0;i<t;i++) cout << tasks[i].finish_time << "\n";
}
