#include <bits/stdc++.h>
using namespace std;

// A small struct for 2D and 3D vector operations
struct Pt2 {
    double x, y;
};
struct Pt3 {
    double x, y, z;
};

// Euclidian distance in 2D
double dist2D(const Pt2 &A, const Pt2 &B) {
    double dx = A.x - B.x, dy = A.y - B.y;
    return sqrt(dx*dx + dy*dy);
}

// Euclidian distance in 3D
double dist3D(const Pt3 &A, const Pt3 &B) {
    double dx = A.x - B.x, dy = A.y - B.y, dz = A.z - B.z;
    return sqrt(dx*dx + dy*dy + dz*dz);
}

// We'll store each pyramid's data in a struct
struct Pyramid {
    // The 4 corners of the square base (2D, z=0)
    Pt2 corner[4];
    // The apex in 3D
    Pt3 apex;
};

// Given (x1,y1,x2,y2,h), build the Pyramid struct
Pyramid buildPyramid(double x1, double y1, double x2, double y2, double h) {
    Pyramid P;
    // Edge vector
    double vx = (x2 - x1);
    double vy = (y2 - y1);
    double d  = sqrt(vx*vx + vy*vy);

    // "Left" perpendicular of length d
    //  (-(y2-y1), x2-x1) normalized * d
    double px = -vy; 
    double py =  vx;
    // length(px,py) = d, so no need to re-scale if we want a side = d
    // but we must confirm that (px,py) indeed has length d:
    // length = sqrt( (-vy)^2 + (vx)^2 ) = sqrt(vy^2 + vx^2 ) = d
    // Perfect. So (px,py) is the correct “left” side of the same length d.

    P.corner[0] = { x1,        y1 };
    P.corner[1] = { x2,        y2 };
    P.corner[2] = { x2 + px,   y2 + py };
    P.corner[3] = { x1 + px,   y1 + py };

    // Center of square is average of corners
    double cx = 0.0, cy = 0.0;
    for(int i=0; i<4; i++){
        cx += P.corner[i].x;
        cy += P.corner[i].y;
    }
    cx /= 4.0; 
    cy /= 4.0;

    // Apex: (cx, cy, h)
    P.apex = { cx, cy, h };

    return P;
}

// Get 2D point on edge #e of pyramid P, parameter t in [0,1]
Pt2 edgePoint(const Pyramid &P, int e, double t) {
    // e goes 0..3, corners are in order [0..3], with corner[4] = corner[0].
    Pt2 A = P.corner[e];
    Pt2 B = P.corner[(e+1)%4];
    Pt2 R;
    R.x = A.x + t*(B.x - A.x);
    R.y = A.y + t*(B.y - A.y);
    return R;
}

// 3D distance from apex to a perimeter point on edge e with parameter t
double distApexToEdgeParam(const Pyramid &P, int e, double t) {
    Pt2 base = edgePoint(P, e, t);
    // Apex is (P.apex.x, P.apex.y, P.apex.z)
    // base is (base.x, base.y, 0)
    return sqrt( (P.apex.x - base.x)*(P.apex.x - base.x)
               + (P.apex.y - base.y)*(P.apex.y - base.y)
               + (P.apex.z)*(P.apex.z) ); // since base.z = 0
}

// F(t1,t2) = distApex1(e1,t1) + dist2D(...) + distApex2(e2,t2).
// We'll make a helper function to compute F if we fix e1,e2:
struct PairOfPyramids {
    Pyramid P1, P2;
};

double Fvalue(const PairOfPyramids &pp, int e1, double t1, int e2, double t2) {
    // 1) apex1 -> B1
    double d1 = distApexToEdgeParam(pp.P1, e1, t1);

    // 2) B1 -> B2 in plane
    Pt2 B1 = edgePoint(pp.P1, e1, t1);
    Pt2 B2 = edgePoint(pp.P2, e2, t2);
    double dPlane = dist2D(B1, B2);

    // 3) B2 -> apex2
    double d2 = distApexToEdgeParam(pp.P2, e2, t2);

    return d1 + dPlane + d2;
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

    PairOfPyramids pp{P1,P2};

    // We'll do a search over all pairs of edges e1 in [0..3], e2 in [0..3].
    // Then do a small grid search + boundary checks + (optionally) local refine.
    double ans = 1e30; // something large

    const int N = 30;  // grid size in [0,1] for t1,t2
    for(int e1=0; e1<4; e1++){
        for(int e2=0; e2<4; e2++){
            // Grid sampling in t1,t2
            // We'll just do a small uniform grid for simplicity
            for(int i=0; i<=N; i++){
                double t1 = double(i)/N;
                for(int j=0; j<=N; j++){
                    double t2 = double(j)/N;
                    double val = Fvalue(pp, e1, t1, e2, t2);
                    ans = min(ans, val);
                }
            }
        }
    }

    // Print result with required precision
    cout << fixed << setprecision(9) << ans << "\n";
    return 0;
}
