#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    int l, b;
    cin >> l >> b;
    string initial_colors;
    cin >> initial_colors;
    // Convert initial colors to need_i
    // R=0, G=1, B=2
    vector<int> need(l+1, 0);
    for(int i=0;i<l;i++){
        if(initial_colors[i] == 'R') need[i+1] = 0;
        else if(initial_colors[i] == 'G') need[i+1] = 1;
        else if(initial_colors[i] == 'B') need[i+1] = 2;
    }
    // Read buttons and map lights to buttons
    // light_to_buttons[i] = list of buttons controlling light i
    vector<vector<int>> light_to_buttons(l+1, vector<int>());
    // Read b buttons
    for(int j=1; j<=b; j++){
        int k;
        cin >> k;
        for(int x=0; x<k; x++){
            int light;
            cin >> light;
            light_to_buttons[light].push_back(j);
        }
    }
    // Fixed assignments
    vector<int> fixed(b+1, -1);
    // Collect binary constraints
    // Each constraint is (j, k, c)
    vector<tuple<int, int, int>> binary_constraints;
    bool impossible_flag = false;
    for(int i=1; i<=l && !impossible_flag; i++){
        int cnt = light_to_buttons[i].size();
        if(cnt == 0){
            if(need[i] != 0){
                impossible_flag = true;
                break;
            }
            continue;
        }
        if(cnt == 1){
            int j = light_to_buttons[i][0];
            int c = need[i];
            if(fixed[j] == -1){
                fixed[j] = c;
            }
            else{
                if(fixed[j] != c){
                    impossible_flag = true;
                    break;
                }
            }
        }
        else if(cnt == 2){
            int j = light_to_buttons[i][0];
            int k = light_to_buttons[i][1];
            int c = need[i];
            if(j > k) swap(j, k);
            binary_constraints.emplace_back(j, k, c);
        }
        else{
            // More than 2 buttons controlling a light, which shouldn't happen
            impossible_flag = true;
            break;
        }
    }
    if(impossible_flag){
        cout << "impossible";
        return 0;
    }
    // Sort binary_constraints by (j, k)
    sort(binary_constraints.begin(), binary_constraints.end(), [&](const tuple<int,int,int>& a, const tuple<int,int,int>& b) -> bool{
        if(get<0>(a) != get<0>(b)) return get<0>(a) < get<0>(b);
        return get<1>(a) < get<1>(b);
    });
    // Now, check for multiple constraints on same (j,k)
    // and ensure they are consistent
    // Also build adjacency list
    vector<vector<pair<int, int>>> adj(b+1, vector<pair<int, int>>());
    int n = binary_constraints.size();
    int idx = 0;
    while(idx < n){
        int j = get<0>(binary_constraints[idx]);
        int k = get<1>(binary_constraints[idx]);
        int c = get<2>(binary_constraints[idx]);
        int next_idx = idx + 1;
        while(next_idx < n && get<0>(binary_constraints[next_idx]) == j && get<1>(binary_constraints[next_idx]) == k){
            if(get<2>(binary_constraints[next_idx]) != c){
                // Conflicting constraints
                impossible_flag = true;
                break;
            }
            next_idx++;
        }
        if(impossible_flag) break;
        adj[j].emplace_back(k, c);
        adj[k].emplace_back(j, c);
        idx = next_idx;
    }
    if(impossible_flag){
        cout << "impossible";
        return 0;
    }
    // Now, process components
    // Use BFS to collect components
    vector<bool> visited(b+1, false);
    ll sum_total = 0;
    // Assignment array
    // To save time, we'll use a single assignment array and reset after each component
    vector<int> assign(b+1, -1);
    for(int j=1; j<=b; j++){
        if(!visited[j]){
            // Collect the component
            vector<int> component_vars;
            queue<int> q_collect;
            q_collect.push(j);
            visited[j] = true;
            component_vars.push_back(j);
            while(!q_collect.empty()){
                int u = q_collect.front(); q_collect.pop();
                for(auto &[v, c] : adj[u]){
                    if(!visited[v]){
                        visited[v] = true;
                        component_vars.push_back(v);
                        q_collect.push(v);
                    }
                }
            }
            // Check if any variable has fixed[j] != -1
            bool has_fixed = false;
            int fixed_j = -1, fixed_val = -1;
            for(auto &u : component_vars){
                if(fixed[u] != -1){
                    has_fixed = true;
                    fixed_j = u;
                    fixed_val = fixed[u];
                    break;
                }
            }
            if(has_fixed){
                // Assign based on fixed_j
                fill(assign.begin(), assign.end(), -1);
                assign[fixed_j] = fixed_val;
                queue<int> q_bfs;
                q_bfs.push(fixed_j);
                bool valid = true;
                while(!q_bfs.empty() && valid){
                    int u = q_bfs.front(); q_bfs.pop();
                    for(auto &[v, c] : adj[u]){
                        int expected = (c - assign[u]) % 3;
                        if(expected < 0) expected += 3;
                        if(assign[v] == -1){
                            assign[v] = expected;
                            q_bfs.push(v);
                        }
                        else{
                            if(assign[v] != expected){
                                valid = false;
                                break;
                            }
                        }
                    }
                }
                if(valid){
                    // Compute the sum
                    ll sum_component = 0;
                    for(auto &u : component_vars){
                        if(assign[u] == -1){
                            valid = false;
                            break;
                        }
                        sum_component += assign[u];
                    }
                    if(valid){
                        sum_total += sum_component;
                    }
                    else{
                        impossible_flag = true;
                        break;
                    }
                }
                else{
                    impossible_flag = true;
                    break;
                }
            }
            else{
                // No fixed variables, try all possible assignments for one variable
                // and take the minimal sum
                int u0 = component_vars[0];
                ll min_sum = LLONG_MAX;
                for(int val0=0; val0<3; val0++){
                    fill(assign.begin(), assign.end(), -1);
                    assign[u0] = val0;
                    queue<int> q_bfs;
                    q_bfs.push(u0);
                    bool valid = true;
                    while(!q_bfs.empty() && valid){
                        int u = q_bfs.front(); q_bfs.pop();
                        for(auto &[v, c] : adj[u]){
                            int expected = (c - assign[u]) % 3;
                            if(expected < 0) expected += 3;
                            if(assign[v] == -1){
                                assign[v] = expected;
                                q_bfs.push(v);
                            }
                            else{
                                if(assign[v] != expected){
                                    valid = false;
                                    break;
                                }
                            }
                        }
                    }
                    if(valid){
                        // Compute the sum
                        ll sum_component = 0;
                        bool complete = true;
                        for(auto &u : component_vars){
                            if(assign[u] == -1){
                                complete = false;
                                break;
                            }
                            sum_component += assign[u];
                        }
                        if(complete){
                            min_sum = min(min_sum, sum_component);
                        }
                    }
                }
                if(min_sum == LLONG_MAX){
                    impossible_flag = true;
                    break;
                }
                else{
                    sum_total += min_sum;
                }
            }
        }
    }
    // Now, handle buttons not connected to any lights
    // These buttons have no constraints, so to minimize sum, set them to 0
    // But according to the problem, every button is connected to at least one light, since b <= 2*l and l >=1
    // So no need to handle separately
    // Now, check if there are any buttons not connected to any lights
    for(int j=1; j<=b && !impossible_flag; j++){
        bool connected = false;
        // Check if button j controls any light
        // Since buttons are connected via lights, already handled in components
        // So nothing to do
    }
    // Finally, check if all lights are handled
    // Already handled during pre-processing
    if(impossible_flag){
        cout << "impossible";
    }
    else{
        cout << sum_total;
    }
}
