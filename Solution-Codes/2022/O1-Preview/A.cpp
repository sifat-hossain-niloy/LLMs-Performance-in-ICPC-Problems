#include <bits/stdc++.h>
using namespace std;

/*
  We will implement the plan described in the analysis.
*/

static const int MOD = 3;

// Read a line of ints into a vector<int>.
vector<int> read_ints() {
    string line;
    getline(cin, line);
    if (!cin.good()) return {};
    stringstream ss(line);
    vector<int> vals;
    int x;
    while (ss >> x) vals.push_back(x);
    return vals;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 1) Read input: l, b
    int l, b;
    cin >> l >> b;
    string colors;
    cin >> colors;

    // Convert colors to {0,1,2} for {R,G,B}
    // R->0, G->1, B->2
    vector<int> c(l);
    for(int i = 0; i < l; i++){
        if (colors[i] == 'R') c[i] = 0;
        else if (colors[i] == 'G') c[i] = 1;
        else /* B */            c[i] = 2;
    }

    // We know each button can control multiple lights, but each light appears at most twice across all buttons.
    // We'll read the button info, but store for each light the list of controlling buttons.
    // Then we'll build the graph of button-nodes.

    // For convenience, store which lights each button controls (not strictly necessary, but let's do it cleanly).
    // Actually we only need, for each light, up to two controlling buttons.
    // We'll read the input for each button, but then reorganize it into "light->buttons".
    vector<vector<int>> lightsControlledByButton(b);
    for(int btn = 0; btn < b; btn++){
        int k;
        cin >> k;
        lightsControlledByButton[btn].resize(k);
        for(int i = 0; i < k; i++){
            cin >> lightsControlledByButton[btn][i];
            // convert 1-based light index to 0-based
            lightsControlledByButton[btn][i]--;
        }
    }

    // We'll build an array of vector<int> size l that holds which buttons control that light.
    vector<vector<int>> buttonOfLight(l);
    for(int btn = 0; btn < b; btn++){
        for(int lt : lightsControlledByButton[btn]){
            buttonOfLight[lt].push_back(btn);
        }
    }

    // 2) For each light, define t_i = (3 - c_i) mod 3
    vector<int> t(l);
    for(int i = 0; i < l; i++){
        t[i] = (3 - c[i]) % 3; 
    }

    // 3) Build the button graph:
    //    - adjacency list: for each button u, store pairs (v, w)
    //    - nodeConstraint[u] = -1 if no single-node constraint, else in {0,1,2}
    vector<vector<pair<int,int>>> adj(b);
    vector<int> nodeConstraint(b, -1); // -1 => none

    // For each light i:
    //   if 0 controllers => if c_i != 0 => impossible
    //   if 1 controller => x_u = t_i => nodeConstraint[u] must be t_i or conflict
    //   if 2 controllers => x_u + x_v = t_i => add edge(u,v) with weight t_i
    for(int i = 0; i < l; i++){
        auto &controllers = buttonOfLight[i];
        if(controllers.size() == 0){
            // no buttons => must already be R => c[i] must be 0
            if(c[i] != 0){
                cout << "impossible\n";
                return 0;
            }
        } else if(controllers.size() == 1){
            int u = controllers[0];
            int need = t[i];
            if(nodeConstraint[u] == -1){
                nodeConstraint[u] = need;
            } else {
                // check for conflict
                if(nodeConstraint[u] != need){
                    cout << "impossible\n";
                    return 0;
                }
            }
        } else if(controllers.size() == 2){
            int u = controllers[0];
            int v = controllers[1];
            int w = t[i]; 
            // We'll store adjacency in both directions
            adj[u].push_back({v, w});
            adj[v].push_back({u, w});
        } else {
            // problem statement says each light can appear at most twice, so shouldn't happen
            cout << "impossible\n";
            return 0;
        }
    }

    // 4) Solve connected components
    // We'll store x[u] in {0,1,2} or -1 if not assigned
    vector<int> x(b, -1);

    // A function to do BFS/DFS given a root with x[root] assigned, and fill out the entire component.
    // Returns true if consistent, false if conflict found.
    // "assignVal[u]" will be assigned to x[u] if unvisited, or checked for conflict otherwise.
    auto propagate = [&](int start, vector<int> &assignVal) {
        queue<int>q;
        q.push(start);
        while(!q.empty()){
            int u = q.front(); 
            q.pop();
            int xu = assignVal[u];
            for(auto &edge : adj[u]){
                int v = edge.first, w = edge.second;
                int want = (w - xu) % MOD;
                if(want < 0) want += MOD;
                if(assignVal[v] == -1){
                    assignVal[v] = want;
                    q.push(v);
                } else {
                    // check consistency
                    if(assignVal[v] != want){
                        return false; // conflict
                    }
                }
            }
        }
        return true;
    };

    // We'll keep track of which nodes have been visited in the big outer loop
    vector<bool> visited(b, false);

    long long totalPresses = 0; // We'll accumulate the sum of x[j]

    // A helper to explore an entire connected component from a node "start",
    // collecting the nodes in that component.
    function<vector<int>(int)> get_component = [&](int start){
        vector<int> comp;
        stack<int> st;
        st.push(start);
        visited[start] = true;
        comp.push_back(start);
        while(!st.empty()){
            int u = st.top(); 
            st.pop();
            for(auto &edge : adj[u]){
                int v = edge.first;
                if(!visited[v]){
                    visited[v] = true;
                    st.push(v);
                    comp.push_back(v);
                }
            }
        }
        return comp;
    };

    for(int u = 0; u < b; u++){
        if(!visited[u]){
            // get all nodes in this connected component
            auto comp = get_component(u);

            // find if there's any nodeConstraint in this component
            // also check if there are multiple constraints that might conflict
            vector<int> constrainedNodes;
            for(int node : comp){
                if(nodeConstraint[node] != -1){
                    constrainedNodes.push_back(node);
                }
            }

            if(constrainedNodes.empty()){
                // Case B: no node constraints
                // We must do up to 3 BFS attempts: root=0,1,2
                // Then pick the assignment with minimal sum
                long long bestSum = LLONG_MAX;
                vector<int> bestAssign; // store the best x array for these nodes

                for(int rootVal = 0; rootVal < 3; rootVal++){
                    // local assignment for nodes in this component
                    vector<int> assignVal(b, -1);
                    // copy current global x for others (just not to confuse ourselves).
                    // Actually we only need to store values for these comp-nodes,
                    // the rest won't matter for now. But let's keep it consistent:

                    // We'll set the entire component's assigned values to -1, except pick one node's value
                    for(int node : comp){
                        assignVal[node] = -1;
                    }

                    // pick comp[0] = rootVal
                    assignVal[comp[0]] = rootVal;

                    // do BFS
                    bool ok = propagate(comp[0], assignVal);
                    if(!ok){
                        // conflict => rootVal is invalid
                        continue;
                    }

                    // if BFS succeeded, we might have unvisited nodes in the same comp if the graph isn't fully connected?
                    // Actually get_component ensures they're all in the same connected component.  
                    // But the edges might not be enough to propagate from comp[0] to all. So let's do a BFS from each node that is still -1:
                    // That basically means the component could be multiple sub-components *within* the adjacency if there are no edges connecting them.
                    // In that scenario, each isolated node can independently be 0,1,2.  
                    // But we said there's no single-node constraint.  Minimizing sum => we set them all to 0 if possible.
                    // Indeed, for any node v that has not been assigned, it means it has no edges from the assigned region, so we can safely pick 0 for it to minimize sum.
                    for(int node : comp){
                        if(assignVal[node] == -1){
                            // no edges forced it => set x[node]=0 to minimize cost
                            assignVal[node] = 0;
                            // then propagate from there just to be safe
                            if(!propagate(node, assignVal)){
                                // If even picking 0 leads to conflict, then no solution
                                ok = false;
                                break;
                            }
                        }
                        if(!ok) break;
                    }
                    if(!ok) continue;

                    // Now compute sum for these nodes
                    long long thisSum = 0;
                    for(int node : comp){
                        // each node's x[node] in {0,1,2}
                        thisSum += assignVal[node];
                    }
                    // check if better
                    if(thisSum < bestSum){
                        bestSum = thisSum;
                        bestAssign = assignVal;
                    }
                }

                if(bestSum == LLONG_MAX){
                    // no valid assignment => impossible
                    cout << "impossible\n";
                    return 0;
                }

                // accept bestAssign into global x
                // and add bestSum to totalPresses
                for(int node : comp){
                    x[node] = bestAssign[node];
                }
                totalPresses += bestSum;
            }
            else {
                // Case A: at least one node has a constraint
                // We must check if there's a conflict among them.  
                // We'll pick the first constrainedNode as root
                // but first we must verify all constraints are consistent.
                // Approach: if there's more than one constrained node, 
                //  we can do BFS from that node to see if the assignment it imposes conflicts with the constraint in the other node(s).
                // A simpler approach: we only need to pick ANY constrained node as the BFS root.  If at the end, all constraints are respected, good; else impossible.

                // We'll pick the first constrained node in "constrainedNodes".
                int root = constrainedNodes[0];
                int rootVal = nodeConstraint[root];

                // local assignment array
                vector<int> assignVal(b, -1);
                assignVal[root] = rootVal;

                // propagate
                bool ok = propagate(root, assignVal);
                if(!ok){
                    cout << "impossible\n";
                    return 0;
                }

                // For any other node in comp that is constrained, check consistency
                for(int node : constrainedNodes){
                    int need = nodeConstraint[node];
                    if(assignVal[node] == -1){
                        // not assigned => assign and propagate
                        assignVal[node] = need;
                        if(!propagate(node, assignVal)){
                            ok = false;
                            break;
                        }
                    } else {
                        // check conflict
                        if(assignVal[node] != need){
                            ok = false;
                            break;
                        }
                    }
                    if(!ok) break;
                }
                if(!ok){
                    cout << "impossible\n";
                    return 0;
                }

                // There may still be unvisited nodes in comp (isolated or separate subcomponent),
                // set them to 0 to minimize cost, then propagate
                for(int node : comp){
                    if(assignVal[node] == -1){
                        assignVal[node] = 0;
                        if(!propagate(node, assignVal)){
                            ok = false;
                            break;
                        }
                    }
                }
                if(!ok){
                    cout << "impossible\n";
                    return 0;
                }

                // Now we have a unique assignment for all nodes in this component
                // Sum them up
                long long compSum = 0;
                for(int node : comp){
                    compSum += assignVal[node];
                }

                // Write them back to x and add compSum to totalPresses
                for(int node : comp){
                    x[node] = assignVal[node];
                }
                totalPresses += compSum;
            }
        }
    }

    // If we got here, we have a valid assignment for every button.
    cout << totalPresses << "\n";
    return 0;
}
