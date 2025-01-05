#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    int r, c;
    cin >> r >> c;
    cin.ignore(); // Ignore the remaining newline after reading c

    long long n = (long long)r * c;
    vector<uint64_t> keys;
    keys.reserve((long long)r *c *6); // Reserve enough space

    for(int l=1; l<=2*r-1; l++){
        string line;
        getline(cin, line);
        // If line is empty and l is odd, assume 'x's are present
        if(l%2==1){
            // Vertex line
            int i = (l-1)/2;
            int pos_offset = (l%4 ==1) ? 0 : 2;
            // Read horizontal edges
            for(int j=0; j <c-1; j++){
                int pos_edge = pos_offset + 4*j +1;
                if(pos_edge +2 < line.size()){
                    if(line[pos_edge] == '-' && line[pos_edge+1] == '-' && line[pos_edge+2] == '-'){
                        long long u = (long long)i *c +j;
                        long long v = (long long)i *c +j+1;
                        if(u < v){
                            keys.emplace_back(((uint64_t)u <<32) | v);
                        }
                        else{
                            keys.emplace_back(((uint64_t)v <<32) | u);
                        }
                    }
                }
            }
        }
        else{
            // Diagonal line
            int i = (l-2)/2;
            int pos_offset = (l%4 ==2) ? 0 : 2;
            for(int j=0; j <c; j++){
                int pos = pos_offset +4*j;
                if(pos < line.size()){
                    char ch = line[pos];
                    if(ch == '/'){
                        if(j >0){
                            long long u = (long long)i *c +j;
                            long long v = (long long)(i+1)*c +j-1;
                            if(u < v){
                                keys.emplace_back(((uint64_t)u <<32) | v);
                            }
                            else{
                                keys.emplace_back(((uint64_t)v <<32) | u);
                            }
                        }
                    }
                    else if(ch == '\\'){
                        long long u = (long long)i *c +j;
                        long long v = (long long)(i+1)*c +j;
                        if(u < v){
                            keys.emplace_back(((uint64_t)u <<32) | v);
                        }
                        else{
                            keys.emplace_back(((uint64_t)v <<32) | u);
                        }
                    }
                }
            }
        }
    }

    // Sort the keys
    sort(keys.begin(), keys.end());

    // Build ptr
    vector<int> ptr(n+1, 0);
    int current_k =0;
    for(long long u=0; u <n; u++){
        ptr[u+1] = ptr[u];
        while(current_k < keys.size() && ((keys[current_k] >>32) == u)){
            ptr[u+1]++;
            current_k++;
        }
    }

    // Build adj_edges
    vector<int> adj_edges(keys.size(), 0);
    for(int k=0; k < keys.size(); k++){
        adj_edges[k] = (int)(keys[k] & 0xFFFFFFFF);
    }

    // Triangle counting
    long long count =0;
    for(long long u=0; u <n; u++){
        for(int k=ptr[u]; k <ptr[u+1]; k++){
            int v = adj_edges[k];
            if((long long)v > u){
                int p = ptr[u];
                int q = ptr[v];
                int end_u = ptr[u+1];
                int end_v = ptr[v+1];
                long long tmp_count =0;
                while(p < end_u && q < end_v){
                    int w = adj_edges[p];
                    int x = adj_edges[q];
                    if(w <= v){
                        p++;
                    }
                    else if(x <= v){
                        q++;
                    }
                    else{
                        if(w == x){
                            tmp_count++;
                            p++;
                            q++;
                        }
                        else if(w < x){
                            p++;
                        }
                        else{
                            q++;
                        }
                    }
                }
                count += tmp_count;
            }
        }
    }

    cout << count;
}
