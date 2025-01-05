#include <bits/stdc++.h>
#include <cmath>
#include <iomanip>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    int case_num =1;
    const double pi = acos(-1.0);
    while(cin>>n){
        // Read coefficients
        vector<double> a(n+1, 0.0);
        for(int i=0;i<=n;i++) cin>>a[i];
        // Read xlow, xhigh, inc
        double xlow, xhigh, inc;
        cin>>xlow>>xhigh>>inc;
        // Compute Qk = [P(x)]^2
        int degree_q = 2*n;
        vector<double> Q(degree_q +1, 0.0);
        for(int i=0;i<=n;i++) {
            for(int j=0;j<=n;j++) {
                if(i+j <= degree_q){
                    Q[i+j] += a[i]*a[j];
                }
            }
        }
        // Compute total volume V
        double V =0.0;
        for(int k=0; k<=degree_q;k++){
            if(k+1 ==0) continue; // k >=0, k+1 >=1
            V += Q[k] * (pow(xhigh, k+1) - pow(xlow, k+1))/(k+1);
        }
        V *= pi;
        // Compute number of markings m
        int m = (int)(V / inc +1e-9);
        m = min(m, 8);
        // Output
        cout<<"Case "<<case_num<<": ";
        cout<<fixed<<setprecision(2)<< V<<"\n";
        if(m ==0){
            cout<<"insufficient volume\n";
        }
        else{
            // Find k=1 to m, find x for C(x)=k *inc
            vector<double> marks;
            for(int k=1; k<=m; k++){
                double target = k * inc;
                // Binary search for x where C(x) = target
                double low =xlow, high =xhigh;
                double mid;
                for(int it=0; it<100; it++){
                    mid=(low + high)/2.0;
                    // Compute C(mid)
                    double Cx=0.0;
                    for(int ki=0; ki<=degree_q; ki++){
                        Cx += Q[ki] * (pow(mid, ki+1) - pow(xlow, ki+1))/(ki+1);
                    }
                    Cx *= pi;
                    if(Cx < target){
                        low = mid;
                    }
                    else{
                        high = mid;
                    }
                }
                // Compute distance up: mid -xlow
                double distance = mid -xlow;
                // Round to two decimal places
                distance = round(distance * 100.0 +1e-8)/100.0;
                marks.push_back(distance);
            }
            // Output marks separated by space
            for(int i=0;i<marks.size();i++){
                if(i>0) cout<<" ";
                cout<<fixed<<setprecision(2)<< marks[i];
            }
            cout<<"\n";
        }
        case_num++;
    }
}
