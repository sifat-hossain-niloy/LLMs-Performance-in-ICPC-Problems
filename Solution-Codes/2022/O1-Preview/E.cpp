#include <bits/stdc++.h>
using namespace std;

// A small struct for 2D points
struct Pt {
    double x, y;
};

// Returns the squared Euclidian distance in 2D
inline double dist2(const Pt &a, const Pt &b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return dx*dx + dy*dy;
}

// Returns the Euclidian distance in 2D
inline double dist(const Pt &a, const Pt &b) {
    return sqrt(dist2(a,b));
}

// We will store everything for one pyramid in a struct:
struct Pyramid {
    // apex center in plane (cx, cy), height h
    double cx, cy, h;
    // corners of the square base
    Pt corner[4];
};

// Build the Pyramid struct from input (x1, y1, x2, y2, h)
Pyramid buildPyramid(double x1, double y1, double x2, double y2, double h) {
    Pyramid P;
    P.h = h;
    
    // Vector v = (x2 - x1, y2 - y1)
    double vx = x2 - x1;
    double vy = y2 - y1;
    double s = sqrt(vx*vx + vy*vy); // side length
    
    // We'll form a 90-degree-left rotation of (vx, vy) of the same length s.
    //  90-left-rotation of (vx, vy) is (-vy, vx).
    // But we also need it to have length s, so let's check if it already has length s:
    // length of (-vy, vx) is also sqrt(vx^2 + vy^2) = s, so no rescaling needed.
    double wx = -vy;
    double wy =  vx;
    // We want the pyramid to lie to the "left" of v, which this direction should achieve
    // given the problem statement.

    // Now corners:
    // corner[0] = (x1,y1)
    // corner[1] = (x2,y2)
    // corner[2] = corner[1] + (wx, wy)
    // corner[3] = corner[0] + (wx, wy)
    P.corner[0] = { x1,      y1      };
    P.corner[1] = { x2,      y2      };
    P.corner[2] = { x2+wx,   y2+wy   };
    P.corner[3] = { x1+wx,   y1+wy   };
    
    // Center = corner[0] + 0.5*v + 0.5*w
    double cx = x1 + 0.5*vx + 0.5*wx;
    double cy = y1 + 0.5*vy + 0.5*wy;
    
    P.cx = cx;
    P.cy = cy;
    return P;
}

// 3D distance from apex of pyramid P to a point e on the plane (z=0),
// if e is on the correct face's edge.  The apex is at (P.cx, P.cy, P.h).
// The point e is (ex, ey, 0).
inline double apexToEdgeDist(const Pyramid &P, const Pt &e) {
    double dx = P.cx - e.x;
    double dy = P.cy - e.y;
    double dz = P.h; // apex is at z = h, e is at z=0
    return sqrt(dx*dx + dy*dy + dz*dz);
}

// We'll parametrize the i-th edge of pyramid P by t in [0,1]:
//    e_i(t) = corner[i] + t*(corner[i+1] - corner[i])
// Then we define the function
//    D_{ij}(t,u) = dist_on_P1(A1-> e1_i(t))
//                  + dist_2D( e1_i(t), e2_j(u) )
//                  + dist_on_P2(A2-> e2_j(u))
//
// We'll do a small numeric search for each (i,j).

// Evaluate D_{ij}(t,u)
double D_ij(const Pyramid &P1, int i, double t,
            const Pyramid &P2, int j, double u)
{
    // Compute e1_i(t)
    Pt c10 = P1.corner[i];
    Pt c11 = P1.corner[(i+1)%4];
    Pt e1; 
    e1.x = c10.x + t*(c11.x - c10.x);
    e1.y = c10.y + t*(c11.y - c10.y);
    
    // Compute e2_j(u)
    Pt c20 = P2.corner[j];
    Pt c21 = P2.corner[(j+1)%4];
    Pt e2; 
    e2.x = c20.x + u*(c21.x - c20.x);
    e2.y = c20.y + u*(c21.y - c20.y);
    
    // distance A1-> e1 in 3D on pyramid
    double dA1_e1 = apexToEdgeDist(P1, e1);
    // distance e1-> e2 in plane
    double d_e1_e2 = dist(e1, e2);
    // distance A2-> e2 in 3D on pyramid
    double dA2_e2 = apexToEdgeDist(P2, e2);
    
    return dA1_e1 + d_e1_e2 + dA2_e2;
}

// We can do a helper that, given (i,j), does a small 2D search in [0,1]^2.
// We'll do a simple approach: check the 4 corners, plus do a few steps
// of gradient descent from a few starting points.  Alternatively, we
// could do a straightforward "grid + refinement" approach.

static const double EPS = 1e-12;

double minimizeOverUnitSquare(
    function<double(double,double)> f
) {
    // We'll sample a small NxN grid, pick the best cell, then do a small gradient approach
    // or a more thorough approach.  Because performance constraints are mild, we can
    // get away with a small mesh search.  

    const int N = 25;  // sample 25x25 = 625 points
    double bestVal = 1e30, bestT = 0.0, bestU = 0.0;
    for(int a=0; a<=N; a++){
        for(int b=0; b<=N; b++){
            double t = double(a)/N;
            double u = double(b)/N;
            double val = f(t,u);
            if(val < bestVal){
                bestVal = val;
                bestT   = t;
                bestU   = u;
            }
        }
    }
    
    // Now do a small local gradient-descent or coordinate-descent from (bestT, bestU).
    auto grad = [&](double t, double u){
        // partial derivatives numeric approximation
        const double h = 1e-7;
        double f0 = f(t,u);
        double fx = f(t+h,u) - f0;
        double fy = f(t,u+h) - f0;
        return make_pair(fx/h, fy/h);
    };
    
    double tCur = bestT, uCur = bestU;
    double step = 0.1;
    for(int iter=0; iter<60; iter++){
        double val = f(tCur, uCur);
        auto [gx, gy] = grad(tCur,uCur);
        double gLen = sqrt(gx*gx + gy*gy);
        if(gLen < 1e-14) break;
        // take a small step
        double tNext = tCur - step*(gx / (gLen+1e-15));
        double uNext = uCur - step*(gy / (gLen+1e-15));
        // clamp to [0,1]
        tNext = max(0.0, min(1.0, tNext));
        uNext = max(0.0, min(1.0, uNext));
        double valNext = f(tNext,uNext);
        if(valNext < val){
            // accept
            tCur = tNext;
            uCur = uNext;
        } else {
            // reduce step
            step *= 0.5;
        }
        if(step < 1e-10) break;
    }
    
    // Also check corners after the gradient:
    double finalVal = f(tCur,uCur);
    // check boundary corners:
    static double cands[4][2] = {{0,0},{0,1},{1,0},{1,1}};
    for (auto &cand: cands) {
        double tv = cand[0], uv = cand[1];
        double val = f(tv,uv);
        if(val < finalVal){
            finalVal = val;
            tCur = tv; uCur=uv;
        }
    }
    
    return finalVal;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    // Read input
    double x1,y1,x2,y2,h;
    cin >> x1 >> y1 >> x2 >> y2 >> h;
    Pyramid P1 = buildPyramid(x1,y1,x2,y2,h);
    
    cin >> x1 >> y1 >> x2 >> y2 >> h;
    Pyramid P2 = buildPyramid(x1,y1,x2,y2,h);
    
    // We want to find the global minimum of
    //   D_{ij}(t,u)  for i,j in {0,1,2,3}, t,u in [0,1].
    double best = 1e30;
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            // We'll build a lambda f(t,u) = D_ij(P1,i; P2,j; t,u)
            auto f = [&](double t, double u){
                return D_ij(P1,i,t, P2,j,u);
            };
            double localMin = minimizeOverUnitSquare(f);
            if(localMin < best) {
                best = localMin;
            }
        }
    }
    
    cout << fixed << setprecision(9) << best << "\n";
    return 0;
}
