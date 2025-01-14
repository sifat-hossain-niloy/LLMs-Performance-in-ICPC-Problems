#include <bits/stdc++.h>
using namespace std;

/*
  We'll maintain, for each of the 32 cards, a set<int> of possible owners {1,2,3,4},
  or an empty set if it has been definitively removed from the game.

  We process each action one by one.  After reading action i, we update constraints.
  If we detect a "guaranteed cheat," we print "no" and i, then stop.
  If we get through all actions, we print "yes".

  Mapping from sK -> cardIndex:
    index(sK) = 4*(s-1) + offset(K),
    offset(A)=0, offset(B)=1, offset(C)=2, offset(D)=3.
*/

// A small helper: offset for 'A','B','C','D'.
int cardOffset(char c) {
    // c will be one of A,B,C,D
    // We could also use a small switch or lookup table.
    switch(c) {
        case 'A': return 0;
        case 'B': return 1;
        case 'C': return 2;
        case 'D': return 3;
    }
    // Should never happen if input is guaranteed correct format
    return -1;
}

// Convert "sK" to an integer 0..31
int cardIndex(int s, char K) {
    return 4*(s-1) + cardOffset(K);
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    // possibleOwners[c] = set of players who might hold card c.
    // If a card is removed from game (some quartet Q), we set it to empty set.
    vector< set<int> > possibleOwners(32, {1,2,3,4});

    // A helper lambda to remove a player p from card c's possibleOwners
    // Return false if that leads to a contradiction (i.e. empty set for a card still in game).
    auto removeOwner = [&](int c, int p){
        auto &S = possibleOwners[c];
        if(!S.empty()) {
            // If c is not already removed from game
            if(S.count(p)){
                S.erase(p);
                if(S.empty()){
                    // This means card c can no longer be owned by anyone,
                    // but it hasn't been "Q"ed away. Contradiction -> cheat.
                    return false;
                }
            }
        }
        return true;
    };

    // A helper to require that card c is *definitely* owned by player p.
    // i.e. set possibleOwners[c] = {p}.
    // Return false if that is impossible.
    auto forceOwner = [&](int c, int p){
        auto &S = possibleOwners[c];
        if(!S.empty()) {
            // If c is not removed
            // check if p is even in the set
            if(!S.count(p)) {
                // impossible
                return false;
            }
            // reduce to only p
            S.clear();
            S.insert(p);
        }
        // If it's already empty, that means the card is removed from game,
        // but we are trying to "force" an owner -> contradiction
        if(S.empty()){
            // contradiction
            return false;
        }
        return true;
    };

    // A helper to check "does player x possibly own any card at all right now?"
    // If not, that means x is definitely out (0 cards).
    // But if the action says x is taking a turn or is being asked,
    // that can't be correct -> guaranteed cheat.
    auto canPlayerHaveAnyCard = [&](int x){
        // does there exist a card c for which x is in possibleOwners[c] ?
        for(int c=0; c<32; c++){
            if(possibleOwners[c].count(x)) return true;
        }
        return false;
    };

    for(int i = 1; i <= n; i++){
        // parse one action
        // lines can be:
        //  x A y sK yes
        //  x A y sK no
        //  x Q s
        // We'll parse them accordingly.
        int x; 
        cin >> x;
        char t; // 'A' or 'Q'
        cin >> t;

        if(t == 'A'){
            int y;  // the player being asked
            cin >> y;
            string cardStr; // e.g. "3C"
            string ans;     // "yes" or "no"
            cin >> cardStr >> ans;
            // parse the card sK
            // cardStr[0..(maybe1)] => s is 1..8, possibly two digits if s >= 10, but problem states s is 1-digit
            // but in the problem, s is guaranteed 1..8 => single digit
            // so cardStr[0] is digit '1'..'8', cardStr[1] is 'A','B','C','D'
            int s = cardStr[0]-'0';  
            char K = cardStr[1];
            int cIdx = cardIndex(s, K);

            // 1) Check that both x and y might still be in the game
            //    i.e. can x or y have any card at this moment?
            if(!canPlayerHaveAnyCard(x)){
                cout << "no\n" << i << "\n";
                return 0;
            }
            if(!canPlayerHaveAnyCard(y)){
                // If y definitely has 0 cards, then y is out. We can't ask an out player.
                cout << "no\n" << i << "\n";
                return 0;
            }

            // 2) Check the "asker must already hold a different card from set s"
            //    i.e. among the 3 other cards in set s, at least one must possibly belong to x.
            //    If sK is sA => the other 3 are sB,sC,sD. We check if x is in possibleOwners for any.
            // gather the 4 cardIndices of set s
            int base = 4*(s-1); // index of sA
            vector<int> setCards = {base, base+1, base+2, base+3};
            bool hasSome = false;
            for(int c : setCards){
                if(c == cIdx) continue; // skip the asked card
                if(possibleOwners[c].count(x)) {
                    hasSome = true;
                    break;
                }
            }
            if(!hasSome){
                // guaranteed cheat: x asked for a card from set s but cannot possibly have
                // any other card from that set.
                cout << "no\n" << i << "\n";
                return 0;
            }

            // Now differentiate "yes" vs "no"
            if(ans == "yes"){
                // => y definitely had cIdx at that moment -> must forceOwner(cIdx, y)
                // Then after the transfer, cIdx moves to x.
                // In *our* final "state after action i," the card belongs to x.
                // So we do: first check if y can have it, then we force cIdx -> x
                // But we must also check if it's consistent that y can have it up to now.

                // If y was not in possibleOwners[cIdx], it's a contradiction:
                if(!possibleOwners[cIdx].count(y)){
                    cout << "no\n" << i << "\n";
                    return 0;
                }
                // Force cIdx => y *before* handing it over
                // But we only store a single "current state" per action, so we do:
                //   step 1: y must be able to have had it (we checked above).
                //   step 2: now, *after* the action, x has it, so we do forceOwner(cIdx, x).
                if(!forceOwner(cIdx, x)){
                    // If that fails, contradiction
                    cout << "no\n" << i << "\n";
                    return 0;
                }
            }
            else {
                // ans == "no"
                // => y must NOT actually have cIdx, or else they'd be cheating.
                // So if possibleOwners[cIdx] == {y}, i.e. y is the *only* possible owner,
                // that is a contradiction.
                auto &S = possibleOwners[cIdx];
                if(S.size() == 1 && S.count(y) == 1){
                    // contradiction
                    cout << "no\n" << i << "\n";
                    return 0;
                }
                // Otherwise we remove y from possibleOwners[cIdx].
                if(!removeOwner(cIdx, y)){
                    // became empty => contradiction
                    cout << "no\n" << i << "\n";
                    return 0;
                }
            }
        }
        else {
            // t == 'Q'
            // format: "x Q s"
            // => x claims a full set s, i.e. x has sA,sB,sC,sD
            int s;
            cin >> s;
            // The 4 card indices:
            int base = 4*(s-1);
            vector<int> setCards = {base, base+1, base+2, base+3};

            // x must be able to have all 4 at this moment
            // if for any c in that set, x not in possibleOwners[c], contradiction
            for(int c : setCards){
                if(!possibleOwners[c].count(x)){
                    cout << "no\n" << i << "\n";
                    return 0;
                }
            }
            // If consistent, we remove them from the game => set possibleOwners[c] = {}
            for(int c : setCards){
                possibleOwners[c].clear(); // removed from game
            }
        }

        // After applying this action’s constraints, we still must ensure
        // that no "in-game" card ended up with an empty possibleOwner by accident.
        // But we only consider it a contradiction if the card wasn't removed on purpose.
        for(int c=0; c<32; c++){
            // if it's not empty, fine.
            // if it's empty, it's either because it's removed by a Q action or forced remove (contradiction).
            // We have already tested forcedRemove contradictions above (removeOwner check).
            // So if we find an empty set here for a card that wasn't explicitly set empty by Q, it’s a cheat.
            // Actually we have already caught that in removeOwner(...).
            // But let's be extra safe and do a final check:
            // The only time we are okay with empty is if a Q removed it.
            // We'll treat that as done if it is indeed from the set that was just removed or from a previous Q.
            // Because we do that forcibly: possibleOwners[c].clear() => it is obviously "removed from the game."
            // So we do not treat that as a contradiction at this step.
            // => No extra code needed, because removeOwner() checks the contradiction itself.
        }

        // If we reach here, no contradiction. Move on to next action.
    }

    // If we read all n actions without contradiction, print "yes".
    cout << "yes\n";
    return 0;
}
