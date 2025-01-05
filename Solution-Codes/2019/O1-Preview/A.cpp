#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

struct Tile {
    int price;
    int height;
    int index;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<Tile> backTiles(n), frontTiles(n);

    for (int i = 0; i < n; ++i) {
        cin >> backTiles[i].price;
        backTiles[i].index = i + 1;
    }
    for (int i = 0; i < n; ++i) {
        cin >> backTiles[i].height;
    }
    for (int i = 0; i < n; ++i) {
        cin >> frontTiles[i].price;
        frontTiles[i].index = i + 1;
    }
    for (int i = 0; i < n; ++i) {
        cin >> frontTiles[i].height;
    }

    // Sort back tiles and front tiles by price
    sort(backTiles.begin(), backTiles.end(), [](const Tile &a, const Tile &b) {
        return a.price < b.price;
    });
    sort(frontTiles.begin(), frontTiles.end(), [](const Tile &a, const Tile &b) {
        return a.price < b.price;
    });

    // Check cumulative counts
    map<int, int> countBack, countFront;
    vector<int> uniquePrices;
    for (const auto &tile : backTiles) {
        countBack[tile.price]++;
    }
    for (const auto &tile : frontTiles) {
        countFront[tile.price]++;
    }
    set<int> pricesSet;
    for (const auto &p : countBack)
        pricesSet.insert(p.first);
    for (const auto &p : countFront)
        pricesSet.insert(p.first);

    vector<int> prices(pricesSet.begin(), pricesSet.end());
    map<int, int> cumCountBack, cumCountFront;
    int cumBack = 0, cumFront = 0;
    for (int p : prices) {
        cumBack += countBack[p];
        cumFront += countFront[p];
        cumCountBack[p] = cumBack;
        cumCountFront[p] = cumFront;
        if (cumFront > cumBack) {
            cout << "impossible\n";
            return 0;
        }
    }

    // Rearrange tiles within price groups
    int i = 0, j = 0;
    while (i < n) {
        int currentPrice = backTiles[i].price;
        int startBack = i;
        while (i < n && backTiles[i].price == currentPrice) i++;
        int endBack = i;

        int startFront = j;
        while (j < n && frontTiles[j].price == currentPrice) j++;
        int endFront = j;

        // Rearrange back tiles in decreasing order of height
        sort(backTiles.begin() + startBack, backTiles.begin() + endBack, [](const Tile &a, const Tile &b) {
            return a.height > b.height;
        });

        // Rearrange front tiles in increasing order of height
        sort(frontTiles.begin() + startFront, frontTiles.begin() + endFront, [](const Tile &a, const Tile &b) {
            return a.height < b.height;
        });
    }

    // Validate heights
    for (int k = 0; k < n; ++k) {
        if (backTiles[k].height <= frontTiles[k].height) {
            cout << "impossible\n";
            return 0;
        }
    }

    // Output the result
    for (const auto &tile : backTiles) {
        cout << tile.index << ' ';
    }
    cout << '\n';
    for (const auto &tile : frontTiles) {
        cout << tile.index << ' ';
    }
    cout << '\n';

    return 0;
}
