#include <bits/stdc++.h>
using namespace std;

typedef unsigned long long ull;

// Precompute factorials from 0! to 20!
ull factorials[21];
void compute_factorials() {
    factorials[0] = 1;
    for(int i=1;i<=20;i++) {
        factorials[i] = factorials[i-1] * i;
    }
}

// List of first 15 primes
const int NUM_PRIMES = 15;
const int primes_list[15] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47};

// Map from n to minimal k
unordered_map<ull, ull> n_to_k;

// Function to compute p^a with overflow check
bool pow_prime_ull(ull base, int exp, ull &result) {
    result = 1;
    for(int i=0;i<exp;i++) {
        if (result > (ULLONG_MAX / base)) return false;
        result *= base;
    }
    return true;
}

// Recursive function to generate partitions
void generate_partitions(int s, int max_part, vector<int> &current_partition) {
    if(s ==0){
        // Process current partition
        // Compute n=(sum ai)! / product ai!
        int sum_ai = 0;
        for(auto a: current_partition) sum_ai +=a;
        if(sum_ai >20) return; // sum ai <=20
        ull n = factorials[sum_ai];
        for(auto a: current_partition) {
            n /= factorials[a];
        }
        // Compute k= product p_i^ai
        ull k =1;
        bool overflow = false;
        for(int i=0;i<current_partition.size();i++) {
            ull term;
            if(!pow_prime_ull(primes_list[i], current_partition[i], term)){
                overflow = true;
                break;
            }
            if(k > ULLONG_MAX / term){
                overflow = true;
                break;
            }
            k *= term;
        }
        if(!overflow){
            // Insert into map
            auto it = n_to_k.find(n);
            if(it == n_to_k.end() || k < it->second){
                n_to_k[n] = k;
            }
        }
        return;
    }
    for(int part = min(s, max_part); part >=1; part--){
        current_partition.push_back(part);
        generate_partitions(s - part, part, current_partition);
        current_partition.pop_back();
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    compute_factorials();
    // Generate all partitions for sum s=1 to 20
    vector<int> current_partition;
    for(int s=1;s<=20;s++){
        generate_partitions(s, s, current_partition);
    }
    // Read test cases
    string line;
    while(getline(cin,line)){
        if(line.empty()) continue;
        ull n;
        // Parse n
        size_t pos =0;
        while(pos < line.size() && isspace(line[pos])) pos++;
        if(pos >= line.size()) continue;
        size_t end = pos;
        while(end < line.size() && isdigit(line[end])) end++;
        if(pos < end){
            string num_str = line.substr(pos, end - pos);
            n = stoull(num_str);
            auto it = n_to_k.find(n);
            if(it != n_to_k.end()){
                cout << n << " " << it->second << "\n";
            }
            else{
                // According to the problem statement, there always exists at least one k for given n
                // So this should not happen
                cout << n << " " << "-1" << "\n";
            }
        }
    }
}
