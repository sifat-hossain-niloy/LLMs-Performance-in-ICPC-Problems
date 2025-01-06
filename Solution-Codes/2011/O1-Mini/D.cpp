#include <bits/stdc++.h>
using namespace std;

// Define Edge structure
struct Edge {
    int to;
    int rev;
    int cap;
};

// Dinic's algorithm implementation
class MaxFlow {
public:
    int N;
    vector<vector<Edge>> graph;
    vector<int> level;
    vector<int> ptr;

    MaxFlow(int N_) : N(N_), graph(N, vector<Edge>()), level(N, -1), ptr(N, 0) {}

    void add_edge(int from, int to, int cap){
        Edge a = {to, (int)graph[to].size(), cap};
        Edge b = {from, (int)(graph[from].size()), 0};
        graph[from].push_back(a);
        graph[to].push_back(b);
    }

    bool bfs(int s, int t){
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        q.push(s);
        level[s] = 0;
        while(!q.empty()){
            int v = q.front(); q.pop();
            for(auto &e : graph[v]){
                if(e.cap > 0 && level[e.to] == -1){
                    level[e.to] = level[v] +1;
                    q.push(e.to);
                }
            }
        }
        return level[t] != -1;
    }

    int dfs(int v, int t, int pushed){
        if(v == t) return pushed;
        for(int &cid = ptr[v]; cid < graph[v].size(); cid++){
            Edge &e = graph[v][cid];
            if(e.cap > 0 && level[e.to] == level[v] +1){
                int tr = dfs(e.to, t, min(pushed, e.cap));
                if(tr > 0){
                    e.cap -= tr;
                    graph[e.to][e.rev].cap += tr;
                    return tr;
                }
            }
        }
        return 0;
    }

    int max_flow(int s, int t){
        int flow = 0;
        while(bfs(s, t)){
            fill(ptr.begin(), ptr.end(), 0);
            while(int pushed = dfs(s, t, INT32_MAX)){
                flow += pushed;
            }
        }
        return flow;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int N, A, B;
    int case_num = 1;
    while(cin >> N >> A >> B){
        if(N ==0 && A ==0 && B ==0) break;
        vector<string> grid(N);
        for(int i=0;i<N;i++) cin >> grid[i];
        // Compute C, C_i, C'_j, available cells
        int C =0;
        vector<int> C_i(N, 0);
        vector<int> C_j(N, 0);
        vector<pair<int,int>> available;
        for(int i=0;i<N;i++) {
            for(int j=0;j<N;j++){
                if(grid[i][j] == 'C'){
                    C++;
                    C_i[i]++;
                    C_j[j]++;
                }
                else if(grid[i][j] == '.'){
                    available.emplace_back(i,j);
                }
                // '/' is disabled
            }
        }
        int M = available.size();
        // Binary search on W
        int left = 0, right = M;
        int answer = -1;
        while(left <= right){
            int mid = left + (right - left)/2;
            // Check feasibility for W = mid
            // Compute S, limit
            long long S = (long long)C + mid;
            // Compute floor(A * S / B)
            long long limit = (A * S) / B;
            // Compute D_i, lower_i, upper_i
            bool infeasible = false;
            vector<int> lower_i(N, 0);
            vector<int> upper_i(N, 0);
            for(int i=0;i<N;i++){
                int D = C_j[i] - C_i[i];
                lower_i[i] = max(-D, 0);
                long long tmp1 = limit - (long long)C_j[i];
                long long tmp2 = limit - (long long)C_i[i] - D;
                upper_i[i] = min((long long)tmp1, (long long)tmp2);
                if(upper_i[i] < lower_i[i]){
                    infeasible = true;
                    break;
                }
            }
            if(infeasible){
                // Not feasible, try smaller W
                right = mid -1;
                continue;
            }
            // Compute upper'_j
            vector<int> upper_j(N, 0);
            for(int j=0;j<N;j++){
                long long tmp = limit - (long long)C_j[j];
                if(tmp <0){
                    infeasible = true;
                    break;
                }
                upper_j[j] = min((int)tmp, (int)(limit - (long long)C_j[j]));
            }
            if(infeasible){
                right = mid -1;
                continue;
            }
            // Compute sum_lower and sum_upper
            long long sum_lower =0, sum_upper =0;
            for(int i=0;i<N;i++){
                sum_lower += lower_i[i];
                sum_upper += upper_i[i];
            }
            if(mid < sum_lower || mid > sum_upper){
                infeasible = true;
                right = mid -1;
                continue;
            }
            // Proceed to build flow network
            // Nodes:
            // 0: SS
            // 1: TT
            // 2: S
            // 3: T
            // 4..4+N-1: R1..RN
            // 4+N..4+2N-1: C1..CN
            int total_nodes = 4 + 2*N;
            MaxFlow mf(total_nodes);
            // Add edges from S (2) to Ri (4+i)
            for(int i=0;i<N;i++){
                mf.add_edge(2, 4+i, upper_i[i] - lower_i[i]);
            }
            // Add edges from Ri to Cj
            for(auto &[i,j] : available){
                mf.add_edge(4+i, 4+N+j, 1);
            }
            // Add edges from Cj to T (3)
            for(int j=0;j<N;j++){
                mf.add_edge(4+N+j, 3, upper_j[j]);
            }
            // Add edges from SS (0) to Ri (4+i) with capacity lower_i
            for(int i=0;i<N;i++){
                mf.add_edge(0, 4+i, lower_i[i]);
            }
            // Add edge from S (2) to TT (1) with capacity sum_lower
            mf.add_edge(2, 1, sum_lower);
            // Add edge from T (3) to S (2) with capacity INF
            mf.add_edge(3, 2, 1000000000);
            // Compute max flow from SS (0) to TT (1)
            int flow = mf.max_flow(0,1);
            if(flow >= sum_lower){
                // Feasible, try higher W
                answer = mid;
                left = mid +1;
            }
            else{
                // Not feasible, try lower W
                right = mid -1;
            }
        }
        // After binary search, check if answer is feasible
        if(answer == -1){
            cout << "Case " << case_num++ << ": impossible\n";
        }
        else{
            // To confirm if answer is feasible
            // It should have been checked in binary search
            // But to ensure, we can skip and assume it's feasible
            cout << "Case " << case_num++ << ": " << answer << "\n";
        }
    }
}
