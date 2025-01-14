#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const int MAXB = 400005;

// Global variables
vector<int> one_var_eq_list[MAXB];
vector<pair<int, int>> two_var_eq_list[MAXB];
int x_assign[MAXB];
bool visited_var[MAXB];

// Helper function to assign and propagate constraints
bool assign_and_propagate(int var, int val, vector<int> &temp_assign, int &sum_presses) {
    if(x_assign[var] != -1){
        if(x_assign[var] != val){
            return false;
        }
        else{
            return true;
        }
    }
    x_assign[var] = val;
    temp_assign.push_back(var);
    sum_presses += val;
    deque<int> queue;
    queue.push_back(var);
    while(!queue.empty()){
        int current = queue.front();
        queue.pop_front();
        // Handle one-variable equations
        for(auto a_i : one_var_eq_list[current]){
            if(x_assign[current] != a_i){
                return false;
            }
        }
        // Handle two-variable equations
        for(auto &[k, a_i] : two_var_eq_list[current]){
            if(x_assign[k] == -1){
                int new_val = (a_i - x_assign[current] + 3) % 3;
                x_assign[k] = new_val;
                temp_assign.push_back(k);
                sum_presses += new_val;
                queue.push_back(k);
            }
            else{
                if( (x_assign[current] + x_assign[k]) % 3 != a_i ){
                    return false;
                }
            }
        }
    }
    return true;
}

// Function to solve a connected component
int solve_component(vector<int> &component_vars, bool has_cycle){
    int min_sum = INT32_MAX;
    if(!has_cycle){
        int root_var = component_vars[0];
        for(int val=0; val<3; val++){
            vector<int> temp_assign;
            int sum_presses =0;
            bool consistent = assign_and_propagate(root_var, val, temp_assign, sum_presses);
            if(consistent){
                min_sum = min(min_sum, sum_presses);
            }
            // Reset assignments
            for(auto var : temp_assign){
                x_assign[var] = -1;
            }
        }
    }
    else{
        int var = component_vars[0];
        for(int val=0; val<3; val++){
            vector<int> temp_assign;
            int sum_presses =0;
            bool consistent = assign_and_propagate(var, val, temp_assign, sum_presses);
            if(consistent){
                min_sum = min(min_sum, sum_presses);
            }
            // Reset assignments
            for(auto var : temp_assign){
                x_assign[var] = -1;
            }
        }
    }
    if(min_sum < INT32_MAX){
        return min_sum;
    }
    else{
        return -1;
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int l, b;
    cin >> l >> b;
    string color_str;
    cin >> color_str;
    // Initialize light_buttons
    vector<vector<int>> light_buttons(l+1, vector<int>());
    for(int j=1; j<=b; j++){
        int k;
        cin >> k;
        for(int t=0; t<k; t++){
            int light;
            cin >> light;
            light_buttons[light].push_back(j);
        }
    }
    // Build equations
    for(int i=1; i<=l; i++){
        char c = color_str[i-1];
        if(light_buttons[i].size() ==0){
            if(c != 'R'){
                cout << "impossible";
                return 0;
            }
        }
        else if(light_buttons[i].size() ==1){
            int j = light_buttons[i][0];
            int a_i;
            if(c == 'R') a_i = 0;
            else if(c == 'G') a_i = 2; // (3 -1) %3 =2
            else a_i = 1; // (3 -2) %3 =1
            one_var_eq_list[j].push_back(a_i);
        }
        else if(light_buttons[i].size() ==2){
            int j = light_buttons[i][0];
            int k = light_buttons[i][1];
            int a_i;
            if(c == 'R') a_i = 0;
            else if(c == 'G') a_i = 2;
            else a_i = 1;
            two_var_eq_list[j].emplace_back(k, a_i);
            two_var_eq_list[k].emplace_back(j, a_i);
        }
        else{
            // More than two buttons controlling a light, which should not happen
            cout << "impossible";
            return 0;
        }
    }
    // Initialize assignments
    for(int j=1; j<=b; j++) x_assign[j] = -1;
    // Process connected components
    long long total_sum =0;
    for(int j=1; j<=b; j++){
        if(x_assign[j] == -1){
            // Perform DFS to collect component_vars and count two-variable equations
            vector<int> component_vars;
            int n_eqs =0;
            stack<int> stk;
            stk.push(j);
            x_assign[j] = -2; // Temporary mark to avoid revisiting
            while(!stk.empty()){
                int current = stk.top();
                stk.pop();
                component_vars.push_back(current);
                for(auto &[k, a_i] : two_var_eq_list[current]){
                    if(current < k){
                        n_eqs +=1;
                    }
                    if(x_assign[k] == -1){
                        stk.push(k);
                        x_assign[k] = -2; // Temporary mark
                    }
                }
            }
            // Reset temporary marks
            for(auto var : component_vars){
                x_assign[var] = -1;
            }
            bool has_cycle = (n_eqs >= (int)component_vars.size());
            int res = solve_component(component_vars, has_cycle);
            if(res == -1){
                cout << "impossible";
                return 0;
            }
            total_sum += res;
        }
    }
    cout << total_sum;
}
