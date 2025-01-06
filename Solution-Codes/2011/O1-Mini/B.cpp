#include <bits/stdc++.h>
using namespace std;

struct Point {
    int x;
    int y;
};

struct Transformation {
    int rotation; // 0, 90, 180, 270
    int Sx;
    int Sy;
    int Tx;
    int Ty;

    bool operator<(const Transformation &other) const{
        if (rotation != other.rotation)
            return rotation < other.rotation;
        if (Sx != other.Sx)
            return Sx < other.Sx;
        if (Sy != other.Sy)
            return Sy < other.Sy;
        if (Tx != other.Tx)
            return Tx < other.Tx;
        return Ty < other.Ty;
    }

    bool operator==(const Transformation &other) const{
        return rotation == other.rotation && Sx == other.Sx && Sy == other.Sy && Tx == other.Tx && Ty == other.Ty;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int x1,y1,x2,y2,x3,y3;
    int x4,y4,x5,y5,x6,y6;
    int case_num=1;
    while(cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3){
        cin >> x4 >> y4 >> x5 >> y5 >> x6 >> y6;
        // Check for termination
        if(x1==0 && y1==0 && x2==0 && y2==0 && x3==0 && y3==0 &&
           x4==0 && y4==0 && x5==0 && y5==0 && x6==0 && y6==0){
               break;
        }
        // Initial points
        Point A = {x1, y1};
        Point B = {x2, y2};
        Point C = {x3, y3};
        // Final points
        Point D = {x4, y4};
        Point E = {x5, y5};
        Point F = {x6, y6};
        // All final points
        vector<Point> finals = {D, E, F};
        // All initial points
        vector<Point> initials = {A, B, C};
        // Generate all permutations of finals
        vector<vector<Point>> permutes;
        // Generate all 6 permutations
        vector<int> indices = {0,1,2};
        sort(indices.begin(), indices.end());
        do{
            vector<Point> current;
            current.push_back(finals[indices[0]]);
            current.push_back(finals[indices[1]]);
            current.push_back(finals[indices[2]]);
            permutes.push_back(current);
        } while(next_permutation(indices.begin(), indices.end()));
        // Define rotations
        vector<int> rotations = {0,90,180,270};
        // Collect all valid transformations
        vector<Transformation> valid_transforms;
        // Iterate over all permutations
        for(auto &perm : permutes){
            // Iterate over all rotations
            for(auto R : rotations){
                // Iterate over Sx and Sy
                for(int Sx = -20; Sx <=20; Sx++){
                    if(Sx ==0) continue;
                    for(int Sy = -20; Sy <=20; Sy++){
                        if(Sy ==0) continue;
                        // Apply scaling and rotation to initials
                        vector<pair<int, int>> transformed;
                        bool valid = true;
                        for(auto &p : initials){
                            // Scale
                            long long scaled_x = (long long)Sx * p.x;
                            long long scaled_y = (long long)Sy * p.y;
                            // Rotate
                            long long rot_x, rot_y;
                            if(R ==0){
                                rot_x = scaled_x;
                                rot_y = scaled_y;
                            }
                            else if(R ==90){
                                rot_x = -scaled_y;
                                rot_y = scaled_x;
                            }
                            else if(R ==180){
                                rot_x = -scaled_x;
                                rot_y = -scaled_y;
                            }
                            else if(R ==270){
                                rot_x = scaled_y;
                                rot_y = -scaled_x;
                            }
                            // Snapping is not needed as coordinates are integers
                            transformed.emplace_back(make_pair((int)rot_x, (int)rot_y));
                        }
                        // Now, compute Tx and Ty based on first point
                        int Tx = perm[0].x - transformed[0].first;
                        int Ty = perm[0].y - transformed[0].second;
                        // Check if other points match after translation
                        bool match = true;
                        for(int i=1;i<3;i++){
                            if(transformed[i].first + Tx != perm[i].x || transformed[i].second + Ty != perm[i].y){
                                match = false;
                                break;
                            }
                        }
                        if(match){
                            // Store this transformation
                            Transformation tr;
                            tr.rotation = R;
                            tr.Sx = Sx;
                            tr.Sy = Sy;
                            tr.Tx = Tx;
                            tr.Ty = Ty;
                            valid_transforms.push_back(tr);
                        }
                    }
                }
            }
        }
        // Now, analyze the valid transformations
        if(valid_transforms.empty()){
            cout << "Case " << case_num++ << ": no solution\n";
        }
        else{
            // Check if all transformations are the same
            bool all_same = true;
            Transformation first = valid_transforms[0];
            for(auto &tr : valid_transforms){
                if(!(tr == first)){
                    all_same = false;
                    break;
                }
            }
            if(all_same){
                cout << "Case " << case_num++ << ": equivalent solutions\n";
            }
            else{
                cout << "Case " << case_num++ << ": inconsistent solutions\n";
            }
        }
    }
}
