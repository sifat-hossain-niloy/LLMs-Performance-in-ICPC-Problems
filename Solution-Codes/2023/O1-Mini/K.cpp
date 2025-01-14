#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

// Precompute powers of 6
int pow6_vals[7] = {1, 6, 36, 216, 1296, 7776, 46656};

// Function to count bits set
int count_bits(int mask){
    return __builtin_popcount(mask);
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int d, w;
    cin >> d >> w;
    // Read dice faces
    vector<string> dice(d);
    for(int i=0;i<d;i++) cin >> dice[i];
    // Map symbols to indices per die
    // Assuming symbols are ASCII
    vector< vector<int> > symbol_to_index(d, vector<int>(256, -1));
    for(int i=0;i<d;i++) {
        for(int j=0;j<6;j++) {
            symbol_to_index[i][(unsigned char)dice[i][j]] = j;
        }
    }
    // Enumerate all possible states
    int num_states = 1;
    for(int i=0;i<d;i++) num_states *=6;
    // Function to convert symbols to state id
    auto symbols_to_id = [&](const vector<int>& symbols) -> int {
        int id=0;
        for(int i=0;i<d;i++) id += symbols[i] * pow6_vals[i];
        return id;
    };
    // Function to convert state id to symbols
    auto id_to_symbols = [&](int id, vector<int>& symbols) -> void {
        symbols.resize(d);
        for(int i=0;i<d;i++){
            symbols[i] = (id / pow6_vals[i]) %6;
        }
    };
    // Read words and mark terminal states
    vector<bool> is_terminal(num_states, false);
    for(int i=0;i<w;i++){
        string word;
        cin >> word;
        bool valid=true;
        vector<int> symbols(d);
        for(int j=0;j<d;j++){
            char c = word[j];
            if(symbol_to_index[j][(unsigned char)c]==-1){
                valid=false;
                break;
            }
            symbols[j] = symbol_to_index[j][(unsigned char)c];
        }
        if(valid){
            int s = symbols_to_id(symbols);
            is_terminal[s]=true;
        }
    }
    // Check if any terminal states exist
    bool any_terminal=false;
    for(int i=0;i<num_states;i++) if(is_terminal[i]) {any_terminal=true; break;}
    if(!any_terminal){
        cout << "impossible\n";
        return 0;
    }
    // Precompute masks_present
    // masks_present[mask][key] = true if any word has this mask's symbol combination
    // Since d<=6, mask ranges from 0 to (1<<6)-1 =63
    // For mask=0, size=1, no symbols to match
    // Initialize masks_present
    vector< vector<bool> > masks_present(1<<6, vector<bool>());
    for(int mask=0; mask<(1<<d); mask++){
        int k = count_bits(mask);
        if(k ==0){
            masks_present[mask].resize(1, false);
            continue;
        }
        int size = pow6_vals[k];
        masks_present[mask].resize(size, false);
    }
    // Re-read words to set masks_present
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    // To re-read words, need to skip first d+1 lines
    // Reset cin to after reading the words
    // Not feasible, so instead, reconstruct masks_present from is_terminal
    // Since is_terminal[s] = true if some word maps to s
    // Thus, for each mask, set masks_present[mask][key] = true if any terminal s has key for mask
    for(int s=0; s<num_states; s++) {
        if(!is_terminal[s]) continue;
        // Get symbols
        vector<int> symbols;
        id_to_symbols(s, symbols);
        for(int mask=0; mask<(1<<d); mask++){
            int k = count_bits(mask);
            if(k ==0){
                masks_present[mask][0] = true;
                continue;
            }
            // Extract key
            int key=0;
            for(int i=0;i<d;i++) if(mask & (1<<i)){
                key = key *6 + symbols[i];
            }
            masks_present[mask][key] = true;
        }
    }
    // Precompute list_masks[h] for h=0 to d
    vector< vector<int> > list_masks(d+1, vector<int>());
    for(int mask=0; mask<(1<<d); mask++){
        int k = count_bits(mask);
        if(k <=d){
            list_masks[k].push_back(mask);
        }
    }
    // Precompute masks of size h for h from d downto0
    // Compute max_h[s] for all s
    vector<int> max_h(num_states, 0);
    vector<int> symbols_temp;
    for(int s=0; s<num_states; s++){
        // Find max h for this s
        bool found=false;
        for(int h=d; h>=0; h--){
            for(auto mask: list_masks[h]){
                if(h==0){
                    if(masks_present[mask][0]){
                        max_h[s]=0;
                        found=true;
                        break;
                    }
                }
                else{
                    // Extract key
                    int key=0;
                    for(int i=0;i<d;i++) if(mask & (1<<i)){
                        key = key *6 + ((s / pow6_vals[i]) %6);
                    }
                    if(key < masks_present[mask].size() && masks_present[mask][key]){
                        max_h[s]=h;
                        found=true;
                        break;
                    }
                }
            }
            if(found) break;
        }
    }
    // Initialize E[s]
    vector<double> E(num_states, 1e18);
    for(int s=0; s<num_states; s++) {
        if(is_terminal[s]) E[s]=0.0;
    }
    // Value Iteration
    bool converged=false;
    double epsilon=1e-8;
    int iterations=0;
    while(!converged && iterations < 1000){
        converged=true;
        // Create a copy of E to store new values
        vector<double> E_new = E;
        for(int s=0; s<num_states; s++){
            if(is_terminal[s]) continue;
            int h = max_h[s];
            double min_expected=1e18;
            for(auto mask: list_masks[h]){
                // Check if mask is of size h
                if(count_bits(mask)!=h) continue;
                // Extract key
                int key=0;
                if(h >0){
                    for(int i=0;i<d;i++) if(mask & (1<<i)){
                        key = key *6 + ((s / pow6_vals[i]) %6);
                    }
                }
                if(!masks_present[mask].empty()){
                    if(key >= masks_present[mask].size() || !masks_present[mask][key]) continue;
                }
                // Determine rerolled dice
                vector<int> rerolled_dice;
                for(int i=0;i<d;i++) if( !(mask & (1<<i)) ) rerolled_dice.push_back(i);
                // Compute expected E[s']
                double expected;
                if(rerolled_dice.empty()){
                    expected=0.0;
                }
                else{
                    int k = rerolled_dice.size();
                    double sum=0.0;
                    int total = pow6_vals[k];
                    for(int x=0; x < total; x++){
                        int s_new = s;
                        int tmp =x;
                        for(int i=0;i<k;i++){
                            int face = tmp %6;
                            tmp /=6;
                            int die = rerolled_dice[i];
                            int old_face = (s_new / pow6_vals[die]) %6;
                            s_new = s_new - old_face * pow6_vals[die] + face * pow6_vals[die];
                        }
                        sum += E[s_new];
                    }
                    expected = sum / (double)pow6_vals[k];
                }
                if(expected < min_expected){
                    min_expected = expected;
                }
            }
            double new_E = 1.0 + min_expected;
            if(abs(new_E - E[s]) > epsilon){
                converged=false;
                E_new[s] = new_E;
            }
        }
        E = E_new;
        iterations++;
    }
    // After convergence, check if any E[s] is still large
    bool impossible_flag=false;
    for(int s=0; s<num_states; s++) {
        if(!is_terminal[s] && E[s] >=1e17){
            impossible_flag=true;
            break;
        }
    }
    if(impossible_flag){
        cout << "impossible\n";
        return 0;
    }
    // Compute the average E[s]
    double answer=0.0;
    for(int s=0; s<num_states; s++) answer += E[s];
    answer /= (double)num_states;
    // Check if answer is finite
    if(!isfinite(answer)){
        cout << "impossible\n";
    }
    else{
        cout << fixed << setprecision(9) << answer << "\n";
    }
}
