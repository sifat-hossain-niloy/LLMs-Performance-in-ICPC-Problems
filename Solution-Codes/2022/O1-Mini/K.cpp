#include <bits/stdc++.h>
#include <complex>
using namespace std;

typedef complex<double> cd;
const double PI = acos(-1.0);

// Function to compute s dot v mod 3
int dot_product_mod3(const vector<int> &s, const vector<int> &v) {
    int res = 0;
    for(int i=0;i<s.size();i++) {
        res += s[i] * v[i];
    }
    return res % 3;
}

// Function to generate all group elements
vector<vector<int>> generate_group(int k) {
    int m = pow(3, k);
    vector<vector<int>> group;
    group.reserve(m);
    for(int num=0; num < m; num++) {
        vector<int> vec(k, 0);
        int tmp = num;
        for(int i=0;i<k;i++) {
            vec[k-1-i] = tmp % 3;
            tmp /=3;
        }
        group.push_back(vec);
    }
    return group;
}

// Function to map vector to index
int vector_to_index(const vector<vector<int>> &group, const vector<int> &vec) {
    for(int i=0;i<group.size();i++) {
        if(group[i] == vec) return i;
    }
    return -1;
}

// Function to convert index to string
string index_to_string(const vector<int> &vec) {
    string s = "";
    for(auto x: vec) {
        if(x ==0) s += '0';
        else if(x==1) s += '+';
        else s += '-';
    }
    return s;
}

// Function to perform FFT-like character computation
cd compute_chi(int s_dot_v) {
    if(s_dot_v ==0) return 1.0;
    else if(s_dot_v ==1) return polar(1.0, 2.0 * PI /3.0);
    else return polar(1.0, 4.0 * PI /3.0);
}

// Gaussian elimination for complex matrices
bool gaussian_elimination(vector<vector<cd>> &A, vector<cd> &b, vector<cd> &x) {
    int n = A.size();
    int m_cols = A[0].size();
    for(int i=0;i<n;i++) {
        // Find pivot
        int pivot = -1;
        for(int j=i;j<n;j++) {
            if(abs(A[j][i]) > 1e-8) {
                pivot = j;
                break;
            }
        }
        if(pivot == -1) return false; // No unique solution
        if(pivot !=i) {
            swap(A[i], A[pivot]);
            swap(b[i], b[pivot]);
        }
        // Normalize pivot row
        cd factor = A[i][i];
        for(int j=i; j<m_cols; j++) {
            A[i][j] /= factor;
        }
        b[i] /= factor;
        // Eliminate below
        for(int j=0; j<n; j++) {
            if(j !=i && abs(A[j][i]) >1e-8) {
                cd factor2 = A[j][i];
                for(int l=i; l<m_cols; l++) {
                    A[j][l] -= factor2 * A[i][l];
                }
                b[j] -= factor2 * b[i];
            }
        }
    }
    // Now, A is identity
    x.resize(m_cols, 0);
    for(int i=0;i<m_cols;i++) {
        x[i] = b[i];
    }
    return true;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, k, t;
    cin >> n >> k >> t;
    // Generate group
    vector<vector<int>> group = generate_group(k);
    int m = group.size();
    // Map configurations to indices
    // 'A' =0, 'E'=1, 'I'=2
    // Initialize F
    vector<long long> F(m, 0);
    for(int i=0;i<t;i++) {
        string s;
        long long f;
        cin >> s >> f;
        vector<int> vec(k,0);
        for(int j=0;j<k;j++) {
            if(s[j] == 'A') vec[j] =0;
            else if(s[j] == 'E') vec[j] =1;
            else vec[j] =2;
        }
        int idx = vector_to_index(group, vec);
        if(idx == -1){
            // Should not happen
            cerr << "Invalid configuration" << endl;
            return 1;
        }
        F[idx] += f;
    }
    // Enumerate all s in G
    // Precompute chi_s(v)
    // Compute F_hat[s}
    // Compute A[s][v} and b[s}
    // Prepare
    // Generate all s in G
    vector<vector<int>> S = group;
    // Precompute chi_s(v}
    // chi[s][v}
    vector<vector<cd>> chi(m, vector<cd>(m, 0));
    for(int s_idx=0;s_idx<m;s_idx++) {
        for(int v_idx=0;v_idx<m;v_idx++) {
            int dp = dot_product_mod3(S[s_idx], group[v_idx]);
            chi[s_idx][v_idx] = compute_chi(dp);
        }
    }
    // Compute F_hat[s}
    vector<cd> F_hat(m, 0);
    for(int s_idx=0;s_idx<m;s_idx++) {
        for(int g_idx=0;g_idx<m;g_idx++) {
            F_hat[s_idx] += F[g_idx] * chi[s_idx][g_idx];
        }
    }
    // Compute log(F_hat[s})
    vector<cd> b_vec(m, 0);
    for(int s_idx=0;s_idx<m;s_idx++) {
        if(abs(F_hat[s_idx]) <1e-8){
            // Log undefined
            // But according to problem statement, input is valid
            // So this should not happen
            b_vec[s_idx] = 0;
            continue;
        }
        b_vec[s_idx] = log(F_hat[s_idx]);
    }
    // Compute A[s][v} = log(1 + chi_s(v)}
    vector<vector<cd>> A_mat(m, vector<cd>(m, 0));
    for(int s_idx=0;s_idx<m;s_idx++) {
        for(int v_idx=0;v_idx<m;v_idx++) {
            A_mat[s_idx][v_idx] = log(1.0 + chi[s_idx][v_idx]);
        }
    }
    // Solve A * m = b
    vector<cd> solution;
    bool solvable = gaussian_elimination(A_mat, b_vec, solution);
    if(!solvable){
        // According to problem statement, always solvable
        cerr << "No solution" << endl;
        return 1;
    }
    // Round the solution to nearest integer
    vector<long long> m_v(m, 0);
    for(int v_idx=0; v_idx<m; v_idx++) {
        double real_part = round(solution[v_idx].real());
        // Handle small imaginary parts due to numerical errors
        m_v[v_idx] = round(solution[v_idx].real());
    }
    // Due to numerical errors, adjust m_v's to be non-negative and sum to n
    // First, set negatives to 0
    for(int v_idx=0; v_idx<m; v_idx++) {
        if(m_v[v_idx] <0) m_v[v_idx] =0;
    }
    // Then, compute the total
    long long total =0;
    for(int v_idx=0; v_idx<m; v_idx++) total += m_v[v_idx];
    // Adjust to make total ==n
    // If total <n, find vectors to increment
    // If total >n, find vectors to decrement
    if(total != n){
        // Find the difference
        long long diff = n - total;
        if(diff >0){
            // Need to add diff levers
            // Choose any vectors, preferably those with non-zero contributions
            for(int v_idx=0; v_idx<m && diff >0; v_idx++) {
                m_v[v_idx]++;
                diff--;
            }
        }
        else{
            // Need to remove -diff levers
            diff = -diff;
            for(int v_idx=0; v_idx<m && diff >0; v_idx++) {
                if(m_v[v_idx] >0){
                    m_v[v_idx]--;
                    diff--;
                }
            }
        }
    }
    // Now, sum m_v should be equal to n
    // Now, reconstruct the lever types
    vector<string> levers;
    levers.reserve(n);
    for(int v_idx=0; v_idx<m; v_idx++) {
        while(m_v[v_idx] >0){
            levers.push_back(index_to_string(group[v_idx]));
            m_v[v_idx]--;
        }
    }
    // If levers.size() <n, pad with '0's
    while(levers.size() < n){
        string s = "";
        for(int j=0; j<k; j++) s += '0';
        levers.push_back(s);
    }
    // Output the first n levers
    for(int i=0;i<n;i++) {
        cout << levers[i] << "\n";
    }
}
