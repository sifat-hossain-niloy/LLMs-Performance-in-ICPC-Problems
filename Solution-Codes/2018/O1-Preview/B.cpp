#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string text;
    getline(cin, text);

    vector<string> words;
    vector<string> separators;

    size_t pos = 0;
    size_t N = text.size();

    // Parsing the text into words and separators
    while (pos < N) {
        // Read a word
        size_t word_start = pos;
        while (pos < N && islower(text[pos])) {
            pos++;
        }
        words.push_back(text.substr(word_start, pos - word_start));

        // Read the separator after the word
        string sep;
        if (pos < N) {
            if (text[pos] == ',') {
                sep += ',';
                pos++;
                if (pos < N && text[pos] == ' ') {
                    sep += ' ';
                    pos++;
                }
            } else if (text[pos] == '.') {
                sep += '.';
                pos++;
                if (pos < N && text[pos] == ' ') {
                    sep += ' ';
                    pos++;
                }
            } else if (text[pos] == ' ') {
                sep += ' ';
                pos++;
            }
        }
        separators.push_back(sep);
    }

    size_t num_words = words.size();
    vector<bool> Has_comma_before(num_words, false);
    vector<bool> Has_comma_after(num_words, false);
    vector<bool> Is_first_in_sentence(num_words, false);
    vector<bool> Is_last_in_sentence(num_words, false);

    // Determine attributes for each word
    for (size_t i = 0; i < num_words; ++i) {
        // Has_comma_before
        if (i == 0) {
            Has_comma_before[i] = false;
            Is_first_in_sentence[i] = true;
        } else {
            Has_comma_before[i] = separators[i - 1].find(',') != string::npos;
            string prev_sep = separators[i - 1];
            if (prev_sep == ". " || prev_sep == ".") {
                Is_first_in_sentence[i] = true;
            } else {
                Is_first_in_sentence[i] = false;
            }
        }
        // Has_comma_after
        Has_comma_after[i] = separators[i].find(',') != string::npos;
        string curr_sep = separators[i];
        if (curr_sep == ". " || curr_sep == ".") {
            Is_last_in_sentence[i] = true;
        } else {
            Is_last_in_sentence[i] = false;
        }
    }

    // Build word to indices map
    unordered_map<string, vector<size_t>> word_to_indices;
    for (size_t i = 0; i < num_words; ++i) {
        word_to_indices[words[i]].push_back(i);
    }

    // Initialize sets of words with commas before or after
    unordered_set<string> comma_before_words;
    unordered_set<string> comma_after_words;
    for (size_t i = 0; i < num_words; ++i) {
        if (Has_comma_before[i]) {
            comma_before_words.insert(words[i]);
        }
        if (Has_comma_after[i]) {
            comma_after_words.insert(words[i]);
        }
    }

    // Apply Dr. Sprinkler's rules
    while (!comma_before_words.empty() || !comma_after_words.empty()) {
        unordered_set<string> next_comma_before_words;
        unordered_set<string> next_comma_after_words;

        // Process words for comma before
        for (const string& word : comma_before_words) {
            for (size_t idx : word_to_indices[word]) {
                if (!Is_first_in_sentence[idx] && !Has_comma_before[idx]) {
                    Has_comma_before[idx] = true;
                    next_comma_before_words.insert(word);
                }
            }
        }

        // Process words for comma after
        for (const string& word : comma_after_words) {
            for (size_t idx : word_to_indices[word]) {
                if (!Is_last_in_sentence[idx] && !Has_comma_after[idx]) {
                    Has_comma_after[idx] = true;
                    next_comma_after_words.insert(word);
                }
            }
        }

        comma_before_words = move(next_comma_before_words);
        comma_after_words = move(next_comma_after_words);
    }

    // Modify separators according to the updated comma positions
    for (size_t i = 0; i < num_words; ++i) {
        // Modify separator before the word
        if (i > 0 && Has_comma_before[i] && !Is_first_in_sentence[i]) {
            if (separators[i - 1] == " ") {
                separators[i - 1] = ", ";
            }
        }
        // Modify separator after the word
        if (Has_comma_after[i] && !Is_last_in_sentence[i]) {
            if (separators[i] == " ") {
                separators[i] = ", ";
            }
        }
    }

    // Reconstruct the text
    string result;
    for (size_t i = 0; i < num_words; ++i) {
        if (i > 0) {
            result += separators[i - 1];
        }
        result += words[i];
    }
    // Append the last separator after the last word
    result += separators[num_words - 1];

    cout << result << endl;

    return 0;
}
