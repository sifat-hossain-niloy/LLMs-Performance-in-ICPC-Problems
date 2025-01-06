#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int rp, cp;
    scanf("%d %d", &rp, &cp);
    vector< pair< pair<int,int>, int > > motif_non_zero_positions;
    for (int i = 0; i < rp; ++i) {
        for (int j = 0; j < cp; ++j) {
            int val;
            scanf("%d", &val);
            if (val != 0) {
                motif_non_zero_positions.push_back({{i, j}, val});
            }
        }
    }
    int rq, cq;
    scanf("%d %d", &rq, &cq);
    vector< vector<int> > mosaic(rq, vector<int>(cq));
    for (int i = 0; i < rq; ++i) {
        for (int j = 0; j < cq; ++j) {
            scanf("%d", &mosaic[i][j]);
        }
    }
    vector< pair<int,int> > result_positions;
    if (motif_non_zero_positions.empty()) {
        // Motif is all empty squares; matches everywhere it fits
        for (int i = 0; i <= rq - rp; ++i) {
            for (int j = 0; j <= cq - cp; ++j) {
                result_positions.push_back({i + 1, j + 1});
            }
        }
    } else {
        for (int i = 0; i <= rq - rp; ++i) {
            for (int j = 0; j <= cq - cp; ++j) {
                bool match = true;
                for (const auto& pos : motif_non_zero_positions) {
                    int dx = pos.first.first;
                    int dy = pos.first.second;
                    int val = pos.second;
                    if (mosaic[i + dx][j + dy] != val) {
                        match = false;
                        break;
                    }
                }
                if (match) {
                    result_positions.push_back({i + 1, j + 1}); // 1-based indexing
                }
            }
        }
    }
    sort(result_positions.begin(), result_positions.end());
    printf("%d\n", (int)result_positions.size());
    for (const auto& pos : result_positions) {
        printf("%d %d\n", pos.first, pos.second);
    }
    return 0;
}
