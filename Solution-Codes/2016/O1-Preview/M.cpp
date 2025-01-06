#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <map>
#include <set>

using namespace std;

struct Instruction {
    char type; // 'C', 'L', 'U'
    int value; // For 'C', the remaining compute time; for 'L' and 'U', the resource id
};

struct Task {
    int id; // Task id (0-based)
    int start_time;
    int base_priority;
    vector<Instruction> instructions;
    int ip; // Instruction pointer
    int compute_remaining; // Remaining compute time for current 'C' instruction
    int current_priority;
    vector<int> owned_resources; // Stack of owned resources
    bool completed;
    int completion_time;
};

int main() {
    int t, r;
    cin >> t >> r;
    vector<Task> tasks(t);
    map<int, int> base_priorities; // Map from task id to base priority
    for (int i = 0; i < t; ++i) {
        Task &task = tasks[i];
        task.id = i;
        int s, b, a;
        cin >> s >> b >> a;
        task.start_time = s;
        task.base_priority = b;
        base_priorities[i] = b;
        task.ip = 0;
        task.compute_remaining = 0;
        task.current_priority = b;
        task.completed = false;
        task.completion_time = -1;
        for (int j = 0; j < a; ++j) {
            string instr;
            cin >> instr;
            if (instr[0] == 'C') {
                int n = stoi(instr.substr(1));
                task.instructions.push_back({'C', n});
            } else if (instr[0] == 'L') {
                int k = stoi(instr.substr(1));
                task.instructions.push_back({'L', k});
            } else if (instr[0] == 'U') {
                int k = stoi(instr.substr(1));
                task.instructions.push_back({'U', k});
            }
        }
    }

    // Compute priority ceilings for resources
    vector<int> resource_ceiling(r + 1, 0); // 1-based indexing for resources
    for (int k = 1; k <= r; ++k) {
        int ceiling = 0;
        for (Task &task : tasks) {
            for (Instruction &instr : task.instructions) {
                if (instr.type == 'L' && instr.value == k) {
                    ceiling = max(ceiling, task.base_priority);
                }
            }
        }
        resource_ceiling[k] = ceiling;
    }

    int time = 0;
    map<int, int> resource_owner; // Map from resource id to task id
    while (true) {
        // Step 1: Identify running tasks
        vector<int> running_tasks;
        bool all_tasks_completed = true;
        for (Task &task : tasks) {
            if (task.start_time <= time && !task.completed) {
                running_tasks.push_back(task.id);
            }
            if (!task.completed) {
                all_tasks_completed = false;
            }
        }
        if (all_tasks_completed) break;

        // Step 2: Determine current priorities and blocking
        vector<int> old_priorities(t);
        vector<int> new_priorities(t);
        vector<bool> blocked(t, false);
        vector<set<int>> blocking_tasks(t);
        for (int i = 0; i < t; ++i) {
            old_priorities[i] = tasks[i].current_priority;
            new_priorities[i] = tasks[i].base_priority;
        }
        bool changed;
        do {
            changed = false;
            // For each task, determine if it's blocked and update priorities
            for (int tid : running_tasks) {
                Task &task = tasks[tid];
                if (task.ip >= task.instructions.size()) continue; // Task completed
                Instruction &instr = task.instructions[task.ip];
                blocked[tid] = false;
                blocking_tasks[tid].clear();
                if (instr.type == 'L') {
                    int k = instr.value;
                    // Check if resource k is owned
                    if (resource_owner.count(k)) {
                        blocked[tid] = true;
                        int owner = resource_owner[k];
                        blocking_tasks[tid].insert(owner);
                    }
                    // Check for resources owned by others with ceiling >= current priority
                    for (auto &entry : resource_owner) {
                        int res_id = entry.first;
                        int owner = entry.second;
                        if (owner == tid) continue;
                        int ceiling = resource_ceiling[res_id];
                        if (ceiling >= old_priorities[tid]) {
                            blocked[tid] = true;
                            blocking_tasks[tid].insert(owner);
                        }
                    }
                }
                // Update current priority
                int max_priority = tasks[tid].base_priority;
                for (int blocked_tid : blocking_tasks[tid]) {
                    max_priority = max(max_priority, old_priorities[blocked_tid]);
                }
                if (new_priorities[tid] != max_priority) {
                    new_priorities[tid] = max_priority;
                    changed = true;
                }
            }
            old_priorities = new_priorities;
        } while (changed);

        // Update tasks' current priorities
        for (int i = 0; i < t; ++i) {
            tasks[i].current_priority = new_priorities[i];
        }

        // Step 3: Execute next instruction
        int highest_priority = -1;
        int selected_task_id = -1;
        for (int tid : running_tasks) {
            if (blocked[tid]) continue;
            if (tasks[tid].ip >= tasks[tid].instructions.size()) continue; // Task completed
            if (tasks[tid].current_priority > highest_priority) {
                highest_priority = tasks[tid].current_priority;
                selected_task_id = tid;
            }
        }

        bool increment_time = true;
        if (selected_task_id == -1) {
            // No task can proceed; increment time
            time++;
        } else {
            Task &task = tasks[selected_task_id];
            Instruction &instr = task.instructions[task.ip];
            if (instr.type == 'C') {
                if (task.compute_remaining == 0) {
                    task.compute_remaining = instr.value;
                }
                // Execute one unit of compute
                task.compute_remaining--;
                if (task.compute_remaining == 0) {
                    task.ip++; // Move to next instruction
                } else {
                    // Remain on the same instruction
                }
                time++;
            } else if (instr.type == 'L') {
                int k = instr.value;
                // Lock resource k
                resource_owner[k] = selected_task_id;
                task.owned_resources.push_back(k);
                task.ip++;
                increment_time = false;
            } else if (instr.type == 'U') {
                // Unlock the most recently locked resource
                if (!task.owned_resources.empty()) {
                    int k = task.owned_resources.back();
                    task.owned_resources.pop_back();
                    resource_owner.erase(k);
                }
                task.ip++;
                increment_time = false;
            }
            // Check if task has completed
            if (task.ip >= task.instructions.size()) {
                task.completed = true;
                task.completion_time = time;
            }
        }
    }

    // Output the completion times
    for (Task &task : tasks) {
        cout << task.completion_time << endl;
    }

    return 0;
}
