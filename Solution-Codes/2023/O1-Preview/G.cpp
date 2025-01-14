#include <bits/stdc++.h>
using namespace std;

/*
   Turning Red

   We use 0 = R, 1 = G, 2 = B.
   Pressing a button x times adds x mod 3 to the lights it controls.

   Steps:
   1) Read input. Convert initial colors to {0,1,2}.
   2) Lights with 0 controlling buttons => must be color 0 else impossible.
   3) Lights with 1 controlling button => fix that button's value mod 3.
   4) Lights with 2 controlling buttons => build edges representing constraints.
   5) Solve the system by connected components in button-constraint graph.
   6) Output minimal sum or "impossible".
*/

static const int MOD = 3;

// Convert color char -> int
int colorToInt(char c) {
    if (c == 'R') return 0; // R
    if (c == 'G') return 1; // G
    return 2;              // B
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int l, b;
    cin >> l >> b;

    // Read initial colors
    string S;
    cin >> S;
    vector<int> c(l);
    for(int i=0; i<l; i++){
        c[i] = colorToInt(S[i]);
    }

    // We will keep track of which buttons each light is connected to
    // Because each light can appear in at most 2 buttons:
    //    lightConnections[i] = list of button indices controlling light i
    // This list can have size 0,1, or 2
    vector<vector<int>> lightConnections(l);

    // We'll also keep the adjacency for the button graph
    // For each pair of buttons (j1, j2) from a 2-button light,
    // we have an equation x_j1 + x_j2 = w (mod 3).
    // We'll store edges[j1].push_back({j2, w}), edges[j2].push_back({j1, w}).
    vector<vector<pair<int,int>>> edges(b);

    // Keep track of whether a button is "forced" to a certain value mod 3
    // from single-button constraints. -1 means "not yet assigned".
    // If forced multiple ways, we can detect conflict.
    vector<int> buttonValue(b, -1);

    // Read buttons
    // We also store for each button j which lights it controls.
    // (We need that to unify with lightConnections info.)
    vector<vector<int>> buttonLights(b);

    for(int j=0; j<b; j++){
        int k; cin >> k;
        buttonLights[j].resize(k);
        for(int idx=0; idx<k; idx++){
            int lightId; 
            cin >> lightId;
            // Convert 1-based light index to 0-based
            --lightId;
            buttonLights[j][idx] = lightId;
            lightConnections[lightId].push_back(j);
        }
    }

    // Step 1: handle lights with 0,1,2 controlling buttons

    // a) 0 controlling buttons => must already be R
    for(int i=0; i<l; i++){
        if(lightConnections[i].empty()){
            // No buttons => color must be R (0)
            if(c[i] != 0){
                cout << "impossible\n";
                return 0;
            }
        }
    }

    // b) 1 controlling button => fix that button's value
    //    We have c[i] + x_j = 0 (mod 3) => x_j = -c[i] (mod 3) => (3 - c[i]) mod 3
    for(int i=0; i<l; i++){
        if(lightConnections[i].size() == 1u){
            int j = lightConnections[i][0]; // the button controlling light i
            int needed = (MOD - c[i]) % MOD; 
            if(buttonValue[j] == -1){
                buttonValue[j] = needed;
            } else {
                // Check consistency
                if(buttonValue[j] != needed){
                    cout << "impossible\n";
                    return 0;
                }
            }
        }
    }

    // c) 2 controlling buttons => build adjacency constraints
    //    c[i] + x_j1 + x_j2 = 0 (mod 3) => x_j1 + x_j2 = -c[i] (mod 3)
    //    Let w = (3 - c[i]) mod 3
    for(int i=0; i<l; i++){
        if(lightConnections[i].size() == 2u){
            int j1 = lightConnections[i][0];
            int j2 = lightConnections[i][1];
            int w = (MOD - c[i]) % MOD;
            // We'll store edges for j1, j2
            edges[j1].push_back({j2, w});
            edges[j2].push_back({j1, w});
        }
    }

    // Step 2: Solve the system by connected components in the button graph.

    // We'll do a DFS/BFS. For each connected component:
    // - Possibly we have some button(s) already forced from single-lights => that partially fixes the component
    // - If none forced, we try each possible assignment 0,1,2 to an unassigned vertex and see if we get a conflict
    //   among that component; pick the assignment with minimal sum.

    // Mark which component each button belongs to, -1 if not visited
    vector<int> comp(b, -1);
    int ccCount = 0;

    // We'll store adjacency in 'edges' and we have forced values in 'buttonValue'.

    function<void(int)> bfsAssign = [&](int start){
        // BFS/queue
        queue<int>q;
        q.push(start);
        comp[start] = ccCount;
        while(!q.empty()){
            int u = q.front(); q.pop();
            for(auto &nx : edges[u]){
                int v = nx.first;
                int w = nx.second; // means x_v = w - x_u mod 3
                if(comp[v] == -1){
                    comp[v] = ccCount;
                    // if buttonValue[u] != -1, then that forces buttonValue[v]
                    if(buttonValue[u] != -1){
                        int forcedV = ( (w - buttonValue[u]) % MOD + MOD ) % MOD;
                        if(buttonValue[v] == -1){
                            buttonValue[v] = forcedV;
                        } else {
                            // check consistency
                            if(buttonValue[v] != forcedV){
                                // conflict => impossible
                                // We'll mark an impossible state with a sentinel
                                comp[v] = -2; 
                                return;
                            }
                        }
                    }
                    q.push(v);
                } else {
                    // already visited, check consistency if both assigned
                    if(buttonValue[u] != -1 && buttonValue[v] != -1){
                        // we want buttonValue[v] == w - buttonValue[u] (mod 3)
                        int checkV = ( (w - buttonValue[u])%MOD + MOD )%MOD;
                        if(buttonValue[v] != checkV){
                            // conflict
                            comp[v] = -2;
                            return;
                        }
                    }
                }
            }
            if(comp[start] == -2) return; // propagate "impossible" up
        }
    };

    // First pass: simply mark connected components and propagate forced values.
    // If we find a conflict, we are done (impossible).
    for(int j=0; j<b; j++){
        if(comp[j] == -1){
            // BFS from j
            bfsAssign(j);
            if(comp[j] == -2){
                // conflict
                cout << "impossible\n";
                return 0;
            }
            ccCount++;
        }
    }

    // Now we have each button labeled by comp[j], or possibly -2 if conflict => we handled it above.

    // Step 3: For each connected component, we either already have at least one forced button or not.
    // If none of the buttons in that component is forced, we must try all 3 assignments for one reference button
    // and see which yields a consistent solution + minimal sum.

    // We'll gather the buttons of each component
    vector<vector<int>> components(ccCount);
    for(int j=0; j<b; j++){
        if(comp[j] >= 0) {
            components[comp[j]].push_back(j);
        }
    }

    // We also need a function that, given we fix buttonValue[ref] = val,
    // we propagate to see if the assignment is consistent, and if so, compute the sum.

    // We'll store a temporary array for each attempt to not overwrite buttonValue globally.
    vector<int> bestValue(buttonValue); // keep final results here
    long long totalPresses = 0;         // we'll accumulate from components

    // We define a BFS/DFS to propagate values for a single component
    // returning the sum of x_j or -1 if conflict.
    auto propagateComponent = [&](int cidx, int refButton, int refVal, 
                                  const vector<int> &initialVal) -> long long {
        // copy initialVal (which might have some forced values) so we can modify
        vector<int> localVal = initialVal;
        // set refButton's value
        localVal[refButton] = refVal;

        // BFS from refButton to propagate constraints
        queue<int>q;
        q.push(refButton);

        while(!q.empty()){
            int u = q.front(); q.pop();
            for(auto &nx : edges[u]){
                int v = nx.first;
                int w = nx.second; // x_v = w - x_u (mod 3)
                // if same component
                if(comp[v] == cidx){
                    if(localVal[u] == -1) {
                        // we can't propagate from an unassigned u
                        // but that shouldn't happen if BFS goes in the correct order
                        continue;
                    }
                    int forcedV = ( (w - localVal[u])%MOD + MOD ) % MOD;
                    if(localVal[v] == -1){
                        localVal[v] = forcedV;
                        q.push(v);
                    } else {
                        if(localVal[v] != forcedV){
                            return -1; // conflict
                        }
                    }
                }
            }
        }

        // If we reach here, no conflict. But some buttons might still be -1
        // (if the component is not fully connected among themselves—though BFS on edges
        //  in the connected component should catch them all; but let's be safe).
        // Any leftover -1 means that button is unconstrained => we can set it = 0 to minimize cost.
        for(int btn : components[cidx]){
            if(localVal[btn] == -1){
                localVal[btn] = 0; 
            }
        }

        // compute sum of this component
        long long sumC = 0;
        for(int btn : components[cidx]){
            sumC += localVal[btn];
        }
        return sumC;
    };

    // We'll produce final assigned values in "resVal" for each button,
    // starting from the global "buttonValue" that we have, but we might refine them.
    vector<int> resVal(buttonValue);

    // For each component, find if there's at least one forced button
    // If yes, we do a single BFS pass (the forced values are already assigned in buttonValue).
    // Then compute sum. If conflict => impossible.
    // If no forced button, we pick an arbitrary reference (the first in the list),
    // try all 3 possible assignments, pick the best if feasible.
    for(int cidx = 0; cidx < ccCount; cidx++){
        // check if any forced button in this component
        bool hasForced = false;
        for(int btn : components[cidx]){
            if(buttonValue[btn] != -1){
                hasForced = true;
                break;
            }
        }

        if(hasForced){
            // We just run a BFS from all forced nodes to see if there's conflict.
            // The BFS we did earlier (bfsAssign) already partly took care of that,
            // but we must also handle unassigned in the same component => set them to 0
            // to minimize cost.

            // After the earlier BFS, if there's no conflict, each connected node *should*
            // either be assigned or remain -1 if unconstrained. We can safely set -1 => 0.
            long long sumC = 0;
            for(int btn : components[cidx]){
                if(buttonValue[btn] == -1){
                    buttonValue[btn] = 0;
                }
                sumC += buttonValue[btn];
            }
            totalPresses += sumC;
        } else {
            // no forced button => pick a reference
            int refButton = components[cidx][0];
            // We'll try refVal in {0,1,2}, pick best
            long long bestSum = LLONG_MAX;
            int bestRefVal = -1;

            // We'll do a small BFS for each attempt
            // We pass the *current* global buttonValue as "initialVal",
            // but specifically for these buttons in cidx, they're all -1 right now
            // or possibly all unforced. We rely on the function to do a local copy.
            vector<int> snapshot = buttonValue; // so we can revert
            for(int valTry = 0; valTry < 3; valTry++){
                long long sumC = propagateComponent(cidx, refButton, valTry, snapshot);
                if(sumC >= 0 && sumC < bestSum){
                    bestSum = sumC;
                    bestRefVal = valTry;
                }
            }
            if(bestSum == LLONG_MAX){
                // all attempts failed => impossible
                cout << "impossible\n";
                return 0;
            }
            // accept bestRefVal, and also we want to store those values in buttonValue
            // so that future components see the final assignment
            // We'll do one more propagate with bestRefVal
            long long finalSum = propagateComponent(cidx, refButton, bestRefVal, buttonValue);
            // finalSum should be the same as bestSum, no conflict
            if(finalSum < 0){
                cout << "impossible\n";
                return 0;
            }
            totalPresses += finalSum - 0; // add sumC for this component

            // Now we must *retrieve* the assigned values from that propagation.
            // Because `propagateComponent` returns a sum but doesn't automatically store them in `buttonValue`.
            // We'll do one more BFS to actually fill them in:
            // (Or re-implement a version that returns the assigned array.)
            // For simplicity, let's do it carefully:

            // We'll reconstruct the assigned array:
            vector<int> finalVals = buttonValue; 
            // We do the same BFS approach but actually fill in finalVals:
            queue<int>q;
            finalVals[refButton] = bestRefVal;
            q.push(refButton);
            while(!q.empty()){
                int u = q.front(); q.pop();
                for(auto &nx : edges[u]){
                    int v = nx.first;
                    int w = nx.second;
                    if(comp[v] == cidx){
                        if(finalVals[u] != -1){
                            int forcedV = (w - finalVals[u] + MOD) % MOD;
                            if(finalVals[v] == -1){
                                finalVals[v] = forcedV;
                                q.push(v);
                            } 
                            // else if conflict, we already know there's no conflict from above
                        }
                    }
                }
            }
            // any leftover -1 => set to 0
            for(int btn : components[cidx]){
                if(finalVals[btn] == -1) finalVals[btn] = 0;
            }
            // Now store back into buttonValue
            for(int btn : components[cidx]){
                buttonValue[btn] = finalVals[btn];
            }
        }
    }

    // If we got here, we have a consistent assignment in buttonValue for all buttons
    // Summation is stored in totalPresses. But let's verify:
    // The BFS logic for forced components also appended to totalPresses, so let's do a final sum check:

    // Actually, we were accumulating partial sums per component. But be careful:
    // For the components with forced buttons, we added sumC at once. 
    // For the free components, we computed bestSum and added that.

    // The above approach yields the correct totalPresses, but let's confirm by re-summing:
    long long checkSum = 0;
    for(int j=0; j<b; j++){
        if(buttonValue[j] >= 0){
            checkSum += buttonValue[j];
        }
    }

    cout << checkSum << "\n";
    return 0;
}
