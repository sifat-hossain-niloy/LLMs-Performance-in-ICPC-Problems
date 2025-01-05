#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    string s;
    getline(cin, s);
    vector<string> words;
    vector<string> separators;
    int pos = 0;
    int len = s.length();
    while(pos < len){
        // Extract word
        int start = pos;
        while(pos < len && 'a' <= s[pos] && s[pos] <= 'z') pos++;
        words.emplace_back(s.substr(start, pos - start));
        // Extract separator
        if(pos >= len) break;
        if(s[pos] == ' '){
            separators.emplace_back(" ");
            pos += 1;
        }
        else if(s[pos] == ','){
            if(pos +1 < len && s[pos+1] == ' '){
                separators.emplace_back(", ");
                pos += 2;
            }
            else{
                // According to constraints, this should not happen
                // But to handle gracefully, treat as space
                separators.emplace_back(" ");
                pos +=1;
            }
        }
        else if(s[pos] == '.'){
            if(pos +1 < len && s[pos+1] == ' '){
                separators.emplace_back(". ");
                pos += 2;
            }
            else{
                separators.emplace_back(".");
                pos +=1;
            }
        }
        else{
            // According to constraints, this should not happen
            // Treat as space
            separators.emplace_back(" ");
            pos +=1;
        }
    }
    int n = words.size();
    // Assign unique IDs
    vector<string> unique_words = words;
    sort(unique_words.begin(), unique_words.end());
    unique_words.erase(unique(unique_words.begin(), unique_words.end()), unique_words.end());
    int m = unique_words.size();
    // Assign word_ids
    vector<int> word_ids(n);
    for(int i=0;i<n;i++){
        word_ids[i] = lower_bound(unique_words.begin(), unique_words.end(), words[i]) - unique_words.begin();
    }
    // Collect occurrences
    vector<vector<int>> occurrences(m, vector<int>());
    for(int i=0;i<n;i++) occurrences[word_ids[i]].push_back(i);
    // Initialize flags
    vector<bool> has_comma_before(m, false);
    vector<bool> has_comma_after(m, false);
    for(int i=0;i<n;i++){
        if(i > 0 && separators[i-1].find(',') != string::npos){
            has_comma_before[word_ids[i]] = true;
        }
        if(separators[i].find(',') != string::npos){
            has_comma_after[word_ids[i]] = true;
        }
    }
    // Initialize queue
    queue<pair<int, int>> q;
    const int COMMA_BEFORE = 0;
    const int COMMA_AFTER = 1;
    for(int w=0; w<m; w++){
        if(has_comma_before[w]){
            q.emplace(w, COMMA_BEFORE);
        }
        if(has_comma_after[w]){
            q.emplace(w, COMMA_AFTER);
        }
    }
    // Initialize processed flags
    vector<bool> processed_comma_before(m, false);
    vector<bool> processed_comma_after(m, false);
    // Process queue
    while(!q.empty()){
        pair<int, int> current = q.front(); q.pop();
        int w = current.first;
        int flag = current.second;
        if(flag == COMMA_BEFORE){
            if(has_comma_before[w] && !processed_comma_before[w]){
                // Process comma_before for word w
                for(auto i: occurrences[w]){
                    // Check if first word of sentence
                    bool is_first_word = false;
                    if(i == 0){
                        is_first_word = true;
                    }
                    else{
                        if(separators[i-1].size() >=2 && separators[i-1][0] == '.' && separators[i-1][1] == ' '){
                            is_first_word = true;
                        }
                    }
                    if(!is_first_word){
                        // Check if already has comma before
                        bool already_has_comma_before = false;
                        if(separators[i-1].find(',') != string::npos){
                            already_has_comma_before = true;
                        }
                        if(!already_has_comma_before){
                            // Modify separator[i-1] from " " to ", "
                            if(separators[i-1] == " "){
                                separators[i-1] = ", ";
                                // Now, previous word has comma after
                                int prev_w = word_ids[i-1];
                                if(!has_comma_after[prev_w]){
                                    has_comma_after[prev_w] = true;
                                    q.emplace(prev_w, COMMA_AFTER);
                                }
                            }
                            // If separators[i-1] was not " ", do nothing
                        }
                    }
                }
                processed_comma_before[w] = true;
            }
        }
        else if(flag == COMMA_AFTER){
            if(has_comma_after[w] && !processed_comma_after[w]){
                // Process comma_after for word w
                for(auto i: occurrences[w]){
                    // Check if last word of sentence
                    bool is_last_word = false;
                    if(i == n-1){
                        is_last_word = true;
                    }
                    else{
                        if(separators[i].size() >=1 && separators[i][0] == '.'){
                            is_last_word = true;
                        }
                    }
                    if(!is_last_word){
                        // Check if already has comma after
                        bool already_has_comma_after = false;
                        if(separators[i].find(',') != string::npos){
                            already_has_comma_after = true;
                        }
                        if(!already_has_comma_after){
                            // Modify separator[i] from " " to ", "
                            if(separators[i] == " "){
                                separators[i] = ", ";
                                // Now, next word has comma before
                                int next_w = word_ids[i+1];
                                if(!has_comma_before[next_w]){
                                    has_comma_before[next_w] = true;
                                    q.emplace(next_w, COMMA_BEFORE);
                                }
                            }
                            // If separators[i] was not " ", do nothing
                        }
                    }
                }
                processed_comma_after[w] = true;
            }
        }
    }
    // Reconstruct the final text
    string result;
    result.reserve(s.length()+n); // approximate
    for(int i=0;i<n;i++){
        result += unique_words[word_ids[i]];
        result += separators[i];
    }
    cout << result;
}
