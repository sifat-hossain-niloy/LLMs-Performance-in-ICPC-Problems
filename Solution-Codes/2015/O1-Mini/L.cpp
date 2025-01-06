#include <bits/stdc++.h>
using namespace std;

int main(){
    int n;
    cin >> n;
    double p1, p2, p3, p4;
    cin >> p1 >> p2 >> p3 >> p4;

    // Enumerate all (k1,k2,k3,k4) with k1+k2+k3+k4=n
    // and compute p_seq = p1^k1 * p2^k2 * p3^k3 * p4^k4
    // Insert all p_seq into a min heap
    priority_queue<double, vector<double>, std::greater<double>> pq;

    for(int k1=0; k1<=n; ++k1){
        for(int k2=0; k2<=n -k1; ++k2){
            for(int k3=0; k3<=n -k1 -k2; ++k3){
                int k4 = n -k1 -k2 -k3;
                // Compute p_seq = p1^k1 * p2^k2 * p3^k3 * p4^k4
                double p_seq = 1.0;
                if(k1 >0){
                    p_seq *= pow(p1, k1);
                }
                if(k2 >0){
                    p_seq *= pow(p2, k2);
                }
                if(k3 >0){
                    p_seq *= pow(p3, k3);
                }
                if(k4 >0){
                    p_seq *= pow(p4, k4);
                }
                pq.push(p_seq);
            }
        }
    }

    // Implement Huffman sum
    double sum =0.0;
    while(pq.size() >1){
        double p1 = pq.top(); pq.pop();
        double p2 = pq.top(); pq.pop();
        sum += p1 + p2;
        pq.push(p1 + p2);
    }

    // Output the sum with precision up to 6 decimal places
    cout << fixed << setprecision(6) << sum << "\n";
}
